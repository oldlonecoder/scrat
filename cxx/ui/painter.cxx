#include <scrat/ui/painter.h>
#include <scrat/ui/console.h>


namespace scrat::ui
{


_object_name(painter)

painter::painter(vdc* parent_, rect r_):
    _dc(parent_), _r(r_)
{
    rem::push_debug(source_fl) << color::Fuchsia <<  " Receiving Geometry: " << color::Yellow << _r.to_string();
    setup_geometry();
    //...
}


// ------------------------------- Check Boundaries : ----------------------------------------------------------
painter& painter::operator<<(color::type name_)
{
    _cell.set_fg(name_);
    return *this;
}

painter& painter::operator<<(Icon::Type ic_id_)
{
    vdc::type mem = _cursor;
    (*_cell) << ic_id_;
    ++_cursor;
    return *this;
}



painter& painter::operator << (Accent::Type aAcc)
{
    vdc::cell* mem = (vdc::cell*)_cursor;
    (*mem) << aAcc;
    ++_cursor;
    return *this;
}


painter& painter::operator<<(const std::string& aStr)
{
    point pt = cpos();
    auto mem = aStr.begin();
    rem::push_debug() << "painter <<  \"" << aStr << "\" @" << (std::string)pt << " -> in " << _r.to_string();
    while (mem != aStr.end() && (pt.x <= _r.b.x))
    {
        *_cursor = (
            _cell
            <<
            *mem++
            ).mem;
        //rem::push_debug() << VDC::Cell{ *_cursor }.Details() << " @{" << Color::Yellow << cpos().to_string() << Color::White << "}";
        ++_cursor;
        pt.x++;
    }

    return *this;
}

painter& painter::operator<<(const stracc& aStr)
{
    return (* this) << aStr();
}

painter& painter::operator<<(const point& XY)
{
    point Pt = XY + _r.a; // + {0,0} by default;
    if (!_r.in(Pt))
        throw rem::push_exception(source_fl) < rem::endl < " : " < (std::string)XY < " is out of range in " < _r.to_string();

    _cursor = (vdc::cell::type*)_dc->peek(Pt);
    return *this;
}

painter& painter::operator<<(const char* aStr)
{
    point pt = cpos();
    const char* mem = aStr;
    rem::push_debug() << "painter <<  \"" << aStr << "\" @" << (std::string)pt << " -> in " << _r.to_string();
    while (*mem && (pt.x <= _r.b.x))
    {
        *_cursor = (_cell << *mem++).mem;
        ++_cursor;
        pt.x++;
    }

    return *this;
}


// ------------------------------- Check Boundaries - end----------------------------------------------------------


painter& painter::gotoxy(const point& pt_)
{
    //rem::push_debug(SourceLocation) << " @(" << XY.to_string() << "):";
    point pt;
    pt += pt_ + _r.a; // + {0,0} by default;
    if (!_r.in(pt))
        throw rem::push_exception(source_fl) < rem::endl < " : " < (std::string)pt_ < " is out of range in " < _r.to_string();

    _cursor = _dc->peek(pt);
    return *this;
}


painter& painter::set_colors(const textattr::pair& aSet)
{
    _cell.set_fg(aSet.fg);
    _cell.set_bg(aSet.bg);
    *_cursor = (* _cursor & ~vdc::cell::CMask) | _cell.mem & vdc::cell::CMask;
    return *this;
}

result<> painter::setup_geometry()
{
    rem::push_debug(source_fl) << " painter _dc's '" < color::Chartreuse6 <  "Geometry: " <  _dc->geometry().to_string();

    _r = _r ? rect({ 0,0 }, {_dc->width(), _dc->height()}) & _r : rect({ 0,0 }, {_dc->width(), _dc->height()});
    if (!_r)
        throw rem::push_exception(source_fl) < ": " < rem::endl < " - Attempt to < Contruct > a painter object on invalid Geometry : " < _r.to_string();
    rem::push_debug(source_fl) < " Configured Geometry:" < color::Yellow < _r.to_string() < color::Reset < " :";
    _cursor = _dc->peek(_r.a);
    //rem::push_debug(SourceLocation) <<  VDC::Cell{ *_cursor }.Details();
    return rem::ok;
}

painter& painter::clear()
{
    auto* CC = _cursor;
    _cursor = _dc->peek(_r.a);
    _cell << ' ';
    rem::push_debug(source_fl) << _cell.details();
    rem::push_output() << "Clearing subrect:" << _r.to_string();
    //int Area = _r.Area();
//    Rem::Output() << "-> " << Color::Yellow << Area << Color::White << " cells in block";
    for(int y = 0; y< _r.height(); y++)
    {
        gotoxy({ 0,y });
        for (int X = 0; X < _r.width(); X++) *_cursor++ = _cell.mem;
    }

    _cursor = CC;
    return *this;
}

//void painter::Update()
//{
//    Console::RenderDC(_dc,_r);
//}


point painter::cpos()
{
    auto* B = _dc->mem;
    int dx = static_cast<int>((_cursor - B));
    point Pt = { dx % _dc->width(), dx / _dc->width() };
    rem::push_debug(source_fl) < " _cursor @" < color::Yellow < (std::string)Pt < color::Reset;
    return Pt;
}



painter& painter::set_bits(Justify::Type aBits)
{
    j_bits = aBits;
    return *this;
}

painter& painter::set_hcenter(bool S)
{
    j_bits = S ? j_bits | Justify::HCenter : j_bits & ~Justify::HCenter;
    return *this;
}

painter& painter::set_vcenter(bool S)
{
    j_bits = S ? j_bits | Justify::VCenter : j_bits & ~Justify::VCenter;
    return  *this;
}

painter& painter::set_center(bool S)
{
    j_bits = S ? j_bits | Justify::HCenter | Justify::VCenter : j_bits & ~(Justify::HCenter | Justify::VCenter);
    return  *this;
}

painter& painter::set_word_wrap(bool S)
{
    return  *this;
}

painter& painter::set_bg(color::type aBg)
{
    _cell.set_bg(aBg);
    return *this;
}

painter& painter::set_fg(color::type aFg)
{
    _cell.set_fg(aFg);
    return *this;
}


}
