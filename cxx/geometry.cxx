

#include <scrat/geometry.h>
#include <scrat/stracc>



namespace scrat
{


void winbuffer::clear()
{
}

void winbuffer::release()
{
}

std::string winbuffer::details()
{
	return std::string();
}




winbuffer& winbuffer::gotoxy(int x, int y)
{
	return *this;
}
winbuffer& scrat::winbuffer::operator<<(point xy)
{
	return *this;
}

void winbuffer::set_geometry(int w, int h)
{
	;
}

winbuffer& winbuffer::operator++()
{
	return *this;
}

winbuffer& winbuffer::operator++(int)
{
	return *this;
}

winbuffer& winbuffer::operator--()
{
	return *this;
}

winbuffer& winbuffer::operator--(int)
{
	return *this;
}

winbuffer& winbuffer::put(const std::string& txt)
{
	return *this;
}


rect::operator std::string()
{
	stracc str = "[{%d,%d} {%d,%d}]:{%d*%d}[%d]";
	str << a.x , a.y, b.x, b.y, sz.w, sz.h, sz.w * sz.h;
	return str();
	// return std::format("[{{{},{}}} {{{},{}}}]:{{{}*{}}}[{}]", a.x, a.y, b.x, b.y, sz.w, sz.h, sz.w * sz.h);
}

std::string rect::to_string()
{
	return this->operator std::string();
}

}
