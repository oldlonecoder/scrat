#pragma once

#include <scrat/io/ifd.h>
#include <scrat/result>
#include <scrat/object>

namespace scrat::io
{


class SCRAT_API listener_base: public object
{

    ifd::list _ifds;

public:

    listener_base(object* parent_ = nullptr);
    virtual ~listener_base();


private:
    //listener_base(object* parent_);
    virtual result<> on_ready_read(ifd&) = 0;
    virtual result<> on_write_read(ifd&) = 0;

};

template<typename T> class SCRAT_API listener : public listener_base
{
    T* _obj = nullptr;
    //...

public:

    using io_delegate = result<> (T::*)(ifd&);
    listener(T* parent_);

    ~listener() override;

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


private:

    result<> on_ready_read(ifd&) override;
    result<> on_write_read(ifd&) override;


    typename listener::io_delegate _write_delegate = nullptr;
    typename listener::io_delegate _read_delegate = nullptr;

};


}