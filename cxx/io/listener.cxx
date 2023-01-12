#include <scrat/io/listener.h>
#include <sys/socket.h>
#include <errno.h>
#include <error.h>

#include <scrat/ui/console.h>



namespace scrat::io
{


listener::listener(object *parent_, int msec_) : object(parent_), msec(msec_)
{

}

listener::~listener()
{
    //shutdown();

    _ifds.clear();
}

result<> listener::run()
{
    rem::push_debug(source_ffl) < " _epoll_event.events:" < color::Yellow < _epoll_event.events < color::Reset < ":";
    if(!_epoll_event.events)
        return rem::push_info(source_ffl) < "events poll empty - dismissing this listener";

    auto num = _ifds.size();
    epoll_event events[_maxifd];
    int ev_count=0;
    auto  i_ifd = query_fd(0);
    ifd::iterator i;
    rem::push_info(source_ffl)  < "[ id() < ] entering into the loop with " < color::Green5 < _ifds.size() < " descriptors";
    do{
        rem::push_debug(source_ffl) < " epoll_wait:";
        ev_count = epoll_wait(_epollfd,events,num,msec);
        rem::push_debug(source_ffl) < " epoll_wait:[" < color::Yellow < ev_count < color::Reset < "]:";
        // lock(); not here
        if(!ev_count)
        {
            ui::console::me() << " listern::run()::_idle_call():\n\r";
            _idle_call();
            continue;
        }
        for(int e=0; e< ev_count; e++)
        {
            uint32_t ev = events[e].events;
            int fd = events[e].data.fd;
            rem::push_info(source_ffl) < rem::stamp <  " event on fd " < color::Red4 < fd;
            auto i = query_fd(fd);
            if(i==_ifds.end())
            {
                rem::push_error(source_ffl) < " event triggered on descriptor not in this listener...";
                break;
            }

            if(ev & EPOLLOUT)
            {
                i->write_signal(*i);
                continue;
            }
            if(ev & EPOLLIN ){
                auto R = epoll_data_in(*i);
                if(!R)
                {
                    rem::push_error(source_ffl) < "error on read ifd: breaking the loop!";
                    shutdown();
                    break;
                }

                if(*R != rem::accepted)
                {
                    _terminate = true;
                    break;
                }
            }

            if(ev & EPOLLERR ){
                rem::push_error(source_ffl) < " EPOLLERR on ifd::fd=" < i->fd ;
                ///@todo VERY IMPORTANT!: handle the error on the ifs::fd...
                // Here we are only removing the ifd from the list, error occured anyway ...
                remove_ifd(i->fd);
                continue;
            } // EPOLLERR
            if(ev & EPOLLHUP )// EPOLLHUP
                throw rem::push_exception(source_ffl) < " Unattended Hangup on file descriptor #" < i->fd < "; listener aborted";

            throw rem::push_exception(source_ffl) < " unhandled epoll event.";
        }// epoll events iteration
    }while(!_terminate);
    rem::push_info(source_ffl) < color::Red4 < " exited from the main loop of the listener: ";
    return rem::ok;
}

result<> listener::add_ifd(int fd_, uint32_t opt_)
{
    rem::push_debug(source_ffl) < " fd = " < color::Yellow < fd_;
    auto i = query_fd(fd_);
    if(i != _ifds.end())
        return rem::push_error(source_ffl) < " file descriptor" < fd_ < " already in the epoll set ";

    _ifds.emplace_back(fd_, opt_);

    epoll_event ev;
    ev.events = _epoll_event.events;
    auto &fd = _ifds.back();
    fd.state.active = true;
    ev.data.fd = fd.fd;
    epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd.fd, &ev );
    rem::push_info(source_ffl) < " added ifd[fd=" < fd.fd < "]";
    return rem::ok;
}

result<> listener::remove_ifd(int fd_)
{
    auto i = query_fd(fd_);
    if(i==_ifds.end())
        return rem::push_error(source_ffl) < " fd " < fd_ < " not in this epoll set";


    rem::push_info() < " removing ifd from the epoll set" < rem::endl < " fd:" < i->fd;

    auto fdi = i->fd;
    epoll_event ev;// prend pas de chance pour EPOLL_CTL_DEL - selon la doc, ev doit etre non-null dans la version 2.6.9- du kernel....
    // completement ignor&eacute; dans 2.6.9+
    ev.events = _epoll_event.events;
    ev.data.fd = i->fd;
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, i->fd, &ev );
    _ifds.erase(i);
    rem::push_info(source_ffl) < " removed fd[" < fdi < "] from the epoll set, and destroyed.";
    return rem::ok;
}

result<> listener::pause_ifd(int fd_)
{
    auto i = query_fd(fd_);
    if(i==_ifds.end())
        return rem::push_error(source_ffl) < " fd " < fd_ < " not in this epoll set";
    epoll_event ev;// prend pas de chance pour EPOLL_CTL_DEL - selon la doc, ev doit etre non-null dans la version 2.6.9- du kernel....
    // completement ignor&eacute; dans 2.6.9+
    ev.events = _epoll_event.events;
    ev.data.fd = i->fd;
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, i->fd, &ev );
    rem::push_info(source_ffl) < " fd[" < fd_ <"] is paused";
    return rem::ok;
}

result<> listener::init()
{
    rem::push_debug(source_pffl) < ":";
    _terminate = false;
    _epollfd = epoll_create(_maxifd);
    _epoll_event.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
    return rem::ok;
}

result<> listener::shutdown()
{
    _terminate = true;
    //close/shutdown all ifd's
    for(auto &f : _ifds)
    {
        if(f.fd > 2) // NEVER-EVER shutdown STDIN, STDOUT, or STDERR !!! LOL
            ::shutdown(f.fd, SHUT_RDWR);
    }
    close(_epollfd);
    return rem::accepted;
}

ifd::iterator listener::query_fd(int fd_)
{
    for(auto it = _ifds.begin(); it != _ifds.end(); it++)
        if(it->fd == fd_) return it;
    return _ifds.end();
}

result<> listener::start() {
//    if(_epollfd >=0 ){
//        rem::push_warning(source_ffl) < "this listener has already been initialized and is probably running as of now! - review this listener management!";
//        return false;
//    }
    return this->run();
}

result<> listener::epoll_data_in(ifd &i)
{
    //log_debugfn << log_end;
    i.state.readable = true;
    i.state.writeable = false;
    result<> E;
    if(i.state.active){
        rem::push_debug(source_ffl) < " reading on fd " < i.fd;
        int rsz = i.data_in();
        if(!rsz){
            rem::push_error(source_ffl) < " read error on fd : " < i.fd < rem::endl < " -- removing this ifd from the epoll set.";
            remove_ifd(i.fd);
            return rem::end;
        }
    }
    epoll_event e;
    e.data.fd = i.fd;
    e.events = EPOLLIN | EPOLLOUT |EPOLLERR | EPOLLHUP;
    epoll_ctl(_epollfd, EPOLL_CTL_MOD, i.fd,&e);
    return E;
}

result<> listener::epoll_data_out(ifd &i)
{
    i.state.readable = false;
    i.state.writeable = true;
    result<> E;
    if(i.state.active){
        rem::push_debug(source_ffl) < " writting on fd " < i.fd;
        E = i.write_signal(i);
    }
    epoll_event e;
    e.data.fd = i.fd;
    e.events = EPOLLIN | EPOLLOUT |EPOLLERR | EPOLLHUP;
    epoll_ctl(_epollfd, EPOLL_CTL_MOD, i.fd,&e);
    return E;
}

}