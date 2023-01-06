#include <scrat/io/listener.h>





namespace scrat::io
{

listener_base::listener_base(object *parent_)
{
    _ifds.push_back({});
}

listener_base::~listener_base()
{
    //...
    _ifds.clear();
    //...
}

result<> scrat::io::listener_base::listen()
{
    on_read_ready(_ifds.back());
    on_write_ready(_ifds.back());
    return rem::notimplemented;
}




}