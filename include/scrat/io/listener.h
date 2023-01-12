#pragma once

#include <scrat/io/ifd.h>
#include <scrat/result>
#include <scrat/object>

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <csignal>
#include <sys/epoll.h>

#include <fcntl.h>
// #include <ifd.h>
// #include <ethreads.h>


namespace scrat::io
{


class SCRAT_API listener :public object
{

    ifd::list   _ifds;
    int         _maxifd = 3;
    epoll_event _epoll_event;
    int         _epollfd = -1;
    int         _epollnumfd = -1;
    bool        _terminate = false;
    scrat::signal<>  _idle_call;
    int msec = -1; ///< default to infinite.
public:

    explicit listener(object* parent_ = nullptr, int msec_=-1);
    ~listener() override;

    result<> run();
    result<> add_ifd(int fd_, uint32_t opt_);
    result<> remove_ifd(int fd_);
    result<> pause_ifd(int fd_);
    result<> init();
    result<> shutdown();

    ifd::iterator query_fd(int fd_);
    result<> start();

    result<> epoll_data_in(ifd& i);
    result<> epoll_data_out(ifd& i);


private:

};

}