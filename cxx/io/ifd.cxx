#include <scrat/io/ifd.h>

#include <sys/ioctl.h>


namespace scrat::io
{

ifd::ifd(int fd_)
{
    _fd = fd_;
}

ifd::~ifd()
{
    buffer.clear();
}

uint32_t scrat::io::ifd::to_read()
{
    ioctl(_fd,FIONREAD,&seglength);
    return seglength;
}

}