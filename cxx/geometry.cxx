

#include <scrat/geometry.h>
#include <scrat/stracc>



namespace scrat
{


//- -------------------------------- winbuffer ------------------------------------------------------------



rect::rect(rect&& r) noexcept:
a(std::move(r.a)),
b(std::move(r.b)),
sz(std::move(r.sz))
{

}
rect::rect(const rect& r) :
a(r.a),
b(r.b),
sz(r.sz)
{

}


rect& rect::operator=(rect&& r) noexcept
{
    a = std::move(r.a);
    b = std::move(r.b);
    sz = std::move(r.sz);
    return *this;
}
rect& rect::operator=(const rect& r)
{
    a = r.a;
    b = r.b;
    sz = r.sz;
    return *this;

}




winbuffer &winbuffer::gotoxy(int x, int y)
{
    cxy = {x, y};
    return *this;
}

void winbuffer::set_geometry(int w, int h)
{
    r = rect(point(0,0),{{0,0},w,h});

    clear();
}

winbuffer &winbuffer::operator++()
{
    if(cxy.x >= r.sz.w)
    {
        if(cxy.y <= r.sz.h)
        {
            cxy.y++;
            cxy.x = 0;
        }
    }
    else
        cxy.x++;

    return *this;
}

winbuffer &winbuffer::operator++(int)
{
    if(cxy.x >= r.sz.w)
    {
        if(cxy.y <= r.sz.h)
        {
            cxy.y++;
            cxy.x = 0;
        }
    }
    else
        cxy.x++;
    return *this;
}

winbuffer &winbuffer::operator--()
{

    return *this;
}

winbuffer &winbuffer::operator--(int)
{
    return *this;
}

/// <summary>
/// Serge Lussier
/// p = 3
/// 10-13 + 3;
///        *   Serge L*
/// </summary>
/// <param name="txt"></param>
/// <returns></returns>
winbuffer &winbuffer::put(const std::string &txt)
{
    int line_width = r.width() - cxy.x;
    auto ln         = txt.length();

    auto dx = line_width <= ln ? line_width : ln;

    std::string::iterator crs = win->str().begin() + cxy.y * r.width() + cxy.x;
    auto                  p   = txt.begin();
    for(int               x   = 0; x < dx; x++)
        *crs++ = *p++;

    cxy.x += line_width;

    return *this;
}

void winbuffer::clear()
{
    if(!win)
        win = new stracc;
    else
        win->clear();

    *win = std::string(r.width() * r.height(), ' ');
    //std::cerr << __PRETTY_FUNCTION__ << "contents: '" << win->_d << "'\n";
}

void winbuffer::release()
{
    delete win;
}

std::string winbuffer::details()
{
    stracc str = "winbuffer details:\n";
    str << r.to_string() << " cursor: " << std::string(cxy);
    return str();
}

winbuffer::operator std::string()
{
    std::string str;
    if(!win)
        return "oops! there is no such defined and initialised area!";
    str += '\n';
    for(int l = 0; l < r.height(); l++)
    {
        for(int c = 0; c < r.width(); c++)
            str += *(win->str().begin() + (l * r.width() + c));
        str += '\n';
    }
    return str;
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

winbuffer & winbuffer::operator<<(point xy)
{
    gotoxy(xy.x, xy.y);
    return *this;
}


}




