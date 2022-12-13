#include <scrat/stracc.h>
//#include "stracc.h"



namespace scrat
{

_object_name(stracc);

std::string::size_type stracc::_scan_arg()
{
    _arg_pos = _d.find("%", 0);
    return _arg_pos;
}

stracc::stracc()
{
}


stracc::~stracc()
{
    _d.clear();
}

stracc& stracc::operator<<(color::type arg_)
{
    if(_arg_pos != std::string::npos) return _format(attr<textattr::format::ansi256>::fg(arg_));
    //_d = _format(_d, attr<textattr::format::ansi256>::fg(arg_).c_str());
    _d += attr<textattr::format::ansi256>::fg(arg_);
    return *this;
}



void stracc::clear()
{
    _d.clear();
}


}
