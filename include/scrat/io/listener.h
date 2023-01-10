#pragma once

#include <scrat/io/ifd.h>
#include <scrat/result>
#include <scrat/object>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/epoll.h>

#include <fcntl.h>
// #include <ifd.h>
// #include <ethreads.h>


namespace scrat::io
{


class SCRAT_API listener_base: public object
{

    ifd::list   _ifds;
    int         _maxifd = -1;
    epoll_event _epoll_event;
    int         _epollfd = -1;
    int         _epollnumfd = -1;
    bool        _terminate_set = false;

public:

    listener_base(object* parent_ = nullptr);
    virtual ~listener_base();

    virtual result<> listen();
    result<> add_ifd(int fd_, ifd::opt&& opt_);
    result<> remove_ifd(int fd_);
    result<> pause_ifd(int fd_);
    result<> init();
    result<> shutdown();


private:
    //listener_base(object* parent_);
    virtual result<> on_read_ready(ifd&) = 0;
    virtual result<> on_write_ready(ifd&) = 0;
    virtual result<> on_idle(ifd&) = 0;

};

template<typename T> class SCRAT_API listener : public listener_base
{
    T* _obj = nullptr;
    //...

public:

    using io_delegate = result<> (T::*)(ifd&);
    listener(T* parent_): listener_base(parent_){_obj = parent_;}

    ~listener() override
    {}

    result<> set_read_delegate(listener::io_delegate d_ )
    {
        _read_delegate = d_;
        return rem::ok;
    }

    result<> set_write_delegate(listener::io_delegate d_ )
    {
        _write_delegate = d_;
        return rem::ok;
    }
    result<> set_idle_delegate(listener::io_delegate d_ )
    {
        _write_delegate = d_;
        return rem::ok;
    }


private:

    result<> on_read_ready(ifd& if_) override
    {
        if(_read_delegate)
        {
            if(_obj) return (_obj->*_read_delegate)(if_);
            return rem::null_ptr;
        }
        return rem::notimplemented;
    }

    result<> on_idle(ifd& if_) override
    {
        if(_idle_delegate)
        {
            if(_obj) return (_obj->*_idle_delegate)(if_);
            return rem::null_ptr;
        }
        return rem::notimplemented;
    }


    result<> on_write_ready(ifd& if_) override
    {
        if(_write_delegate)
        {
            if(_obj) return (_obj->*_write_delegate)(if_);
            return rem::null_ptr;
        }
        return rem::notimplemented;

    }

    typename listener::io_delegate _write_delegate = nullptr;
    typename listener::io_delegate _read_delegate = nullptr;
    typename listener::io_delegate _idle_delegate = nullptr;
};


}