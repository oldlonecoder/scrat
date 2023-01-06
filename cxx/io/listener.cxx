#include <scrat/io/listener.h>




namespace scrat::io
{

listener_base::listener_base(object *parent_)
{

}

listener_base::~listener_base()
{
    //...
    _ifds.clear();
    //...
}
}