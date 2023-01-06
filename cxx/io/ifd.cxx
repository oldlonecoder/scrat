#include <scrat/io/ifd.h>


namespace scrat::io
{

    ifd::ifd(int fd_)
    {
        _fd = fd_;
    }

    ifd::~ifd()
    {
    }
}