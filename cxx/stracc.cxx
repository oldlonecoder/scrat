#include <scrat/stracc.h>
#include <scrat/encodings/fmtio.h>


namespace scrat 
{

    _object_name(stracc);


    stracc::stracc()
    {
    }


    stracc::~stracc()
    {
        _d.clear();
    }

    stracc& stracc::operator<<(color::type arg_)
    {
        
        //_d = _format(_d, attr<textattr::format::ansi256>::fg(arg_).c_str());
        _d += std::format("{}", arg_);
        return *this;
    }

   

    void stracc::clear()
    {
        _d.clear();
    }


    

}
