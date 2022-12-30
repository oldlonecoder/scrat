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
    vdc::type c = _cursor;
    (*_cell) << ic_id_;
    ++_cursor;
    return *this;
}



painter& painter::operator << (Accent::Type aAcc)
{
    vdc::cell* c = (vdc::cell*)_cursor;
    (*C) << aAcc;
    ++Cursor;
    return *this;
}


painter& painter::operator<<(const std::string& aStr)
{
    Point P = CPos();
    auto c = aStr.begin();
    rem::push_debug() << "painter <<  \"" << aStr << "\" @" << P.ToString() << " -> in " << _R.ToString();
    while (c != aStr.end() && (P.X <= _R.B.X))
    {
        *Cursor = (
            _Cell
            <<
            *c++
            ).C;
        //rem::push_debug() << VDC::Cell{ *Cursor }.Details() << " @{" << Color::Yellow << CPos().ToString() << Color::White << "}";
        ++Cursor;
        P.X++;
    }

    return *this;
}

painter& painter::operator<<(const String& aStr)
{
    return (* this) << aStr();
}

painter& painter::operator<<(const Point& XY)
{
    Point Pt = XY + _R.A; // + {0,0} by default;
    if (!_R.Contains(Pt))
        throw Rem::Exception(SourceLocation) << Rem::Code::End << " : " << XY.ToString() << " is out of range in " << _R.ToString();

    Cursor = (VDC::Cell::Type*)DC->At(Pt);
    return *this;
}

painter& painter::operator<<(const char* aStr)
{
    Point P = CPos();
    const char* c = aStr;
    rem::push_debug() << "painter <<  \"" << aStr << "\" @" << P.ToString() << " -> in " << _R.ToString();
    while (*c && (P.X <= _R.B.X))
    {
        *Cursor = (_Cell << *c++).C;
        ++Cursor;
        P.X++;
    }

    return *this;
}


// ------------------------------- Check Boundaries - end----------------------------------------------------------


painter& painter::GotoXY(const Point& XY)
{
    //rem::push_debug(SourceLocation) << " @(" << XY.ToString() << "):";
    Point Pt = XY + _R.A; // + {0,0} by default;
    if (!_R.Contains(Pt))
        throw Rem::Exception(SourceLocation) << Rem::Code::End << " : " << XY.ToString() << " is out of range in " << _R.ToString();

    Cursor = (VDC::Cell::Type*)DC->At(Pt);
    //rem::push_debug(SourceLocation) << " :Real coords: " << CPos().ToString() << " <-> Pt{" << Pt.ToString() << "}";
    return *this;
}


painter& painter::ColorSet(const Color::Set& aSet)
{
    _Cell.SetFg(aSet.Fg);
    _Cell.SetBg(aSet.Bg);
    *Cursor = (* Cursor & ~VDC::Cell::CMask) | _Cell.C & VDC::Cell::CMask;
    return *this;
}

Expect<> painter::SetupGeometry()
{
    rem::push_debug(SourceName) << " painter DC's '" << Color::Chartreuse6 <<  "Geometry: " <<  DC->Geometry().ToString();

    _R = _R ? Rectangle({ 0,0 }, DC->Width(), DC->Height()) & _R : Rectangle({ 0,0 }, DC->Width(), DC->Height());
    if (!_R)
        throw Rem::Exception(SourceName) << ": " << Rem::Code::Endl << " - Attempt to < Contruct > a painter object on invalid Geometry : " << _R.ToString();
    rem::push_debug(SourceName) << " Configured Geometry:" << Color::Yellow << _R.ToString() << Color::Reset << " :";
    Cursor = DC->At(_R.A);
    //rem::push_debug(SourceLocation) <<  VDC::Cell{ *Cursor }.Details();
    return Rem::Code::Ok;
}

painter& painter::Clear()
{
    auto* CC = Cursor;
    Cursor = DC->At(_R.A);
    _Cell << ' ';
    rem::push_debug(SourceName) << _Cell.Details();
    Rem::Output() << "Clearing subrect:" << _R.ToString();
    //int Area = _R.Area();
//    Rem::Output() << "-> " << Color::Yellow << Area << Color::White << " cells in block";
    for(int y = 0; y< _R.Height(); y++)
    {
        GotoXY({ 0,y });
        for (int X = 0; X < _R.Width(); X++) *Cursor++ = _Cell.C;
    }

    Cursor = CC;
    return *this;
}

//void painter::Update()
//{
//    Console::RenderDC(DC,_R);
//}


Point painter::CPos()
{
    auto* B = DC->Mem();
    int dx = static_cast<int>((Cursor - B));
    Point Pt = { dx % DC->Width(), dx / DC->Width() };
    rem::push_debug(SourceName) << " Cursor @" << Color::Yellow << Pt.ToString() << Color::Reset;
    return Pt;
}



painter& painter::SetBits(Justify::Type aBits)
{
    JBits = aBits;
    return *this;
}

painter& painter::SetHCenter(bool S)
{
    JBits = S ? JBits | Justify::HCenter : JBits & ~Justify::HCenter;
    return *this;
}

painter& painter::SetVCenter(bool S)
{
    JBits = S ? JBits | Justify::VCenter : JBits & ~Justify::VCenter;
    return  *this;
}

painter& painter::SetCenter(bool S)
{
    JBits = S ? JBits | Justify::HCenter | Justify::VCenter : JBits & ~(Justify::HCenter | Justify::VCenter);
    return  *this;
}

painter& painter::SetWordWrap(bool S)
{
    return  *this;
}

painter& painter::SetBg(Color::Type aBg)
{
    _Cell.SetBg(aBg);
    return *this;
}

painter& painter::SetFg(Color::Type aFg)
{
    _Cell.SetFg(aFg);
    return *this;
}


}
