#include <scrat/ui/painter.h>
#include <scrat/ui/console.h>


namespace scrat::ui
{


_object_name(painter)

painter::painter(vdc* parent_, vdc::type def_attr_, rect r_):
    _dc(parent_), _def_attr(def_attr_), _r(r_)
{
    _cell = *_def_attr;
    rem::push_debug(source_fl) << color::Fuchsia <<  " Receiving Geometry: " << color::Yellow << _r.to_string();
    setup_geometry();
    _cell << ' ';
    //...
}


// ------------------------------- Check Boundaries : ----------------------------------------------------------
painter& painter::operator<<(color::type name_)
{
    _cell.set_fg(name_);
    return *this;
}


/**
 * @brief Assigns the IconAttr to the cell's attr bitsfield and the the Icon Code ID to the ascci bitsfield
 *
 * @param ic_id_
 * @return painter& ( ref to self )
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
painter& painter::operator<<(Icon::Type ic_id_)
{
    vdc::cell c = _cell;
    *_cursor = (c << ic_id_).mem;
    ++_cursor;
    return *this;
}

/**
 * @brief Assigns the AccentAttr to the cell's attr bitsfield and the Accent Code ID to the ascii bitsfield
 *
 * @param aAcc  Accent Code
 * @return painter&
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
painter& painter::operator << (Accent::Type aAcc)
{
    vdc::cell c = _cell;
    *_cursor = (c << aAcc).mem;
    ++_cursor;
    return *this;
}

/**
 * @brief Writes string \a aStr into the painter's backbuffer vdc
 *
 * @param aStr
 * @return painter& (ref to self )
 *
 * @note This painter version does not handle justify flags, nor checks the logical line and columns boundaries - Only the linear-bloc limit.
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
painter& painter::operator<<(const std::string& aStr)
{
    point pt = cpos();
    auto mem = aStr.begin();
    rem::push_debug() << "painter <<  \"" << aStr << "\" @" << (std::string)pt << " -> in " << _r.to_string();
    while (mem != aStr.end() && (pt.x <= _r.b.x))
    {
        *_cursor = (_cell << *mem++).mem;
        ++_cursor;
        pt.x++;
    }

    return *this;
}

/**
 * @brief Writes string \a aStr into the painter's backbuffer vdc
 *
 * @param aStr
 * @return painter& (ref to self )
 *
 * @note Invokes the std::string overload operator
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
painter& painter::operator<<(const char* aStr)
{
        return (* this) << std::string(aStr);
}

/**
 * @brief Writes string \a aStr into the painter's backbuffer vdc
 *
 * @param aStr
 * @return painter& (ref to self )
 *
 * @note Invokes the std::string overload operator
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
painter& painter::operator<<(const stracc& aStr)
{
    return (* this) << aStr();
}


/**
 * @brief operator << overload acting as painter::gotoxy
 *
 * @param XY coordinate point into the painter's subregion.
 * @return painter& ( ref to self )
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
painter& painter::operator<<(const point& XY)
{
    point Pt = XY + _r.a; // + {0,0} by default;
    if (!_r.in(Pt))
        throw rem::push_exception(source_fl) < rem::endl < " : " < (std::string)XY < " is out of range in " < _r.to_string();

    _cursor = _dc->peek(Pt);
    return *this;
}


// ------------------------------- Check Boundaries - end----------------------------------------------------------

/**
 * @brief This the "set cursor position" function.
 *
 * @param pt_
 * @return painter&  ( ref to self )
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
painter& painter::gotoxy(const point& pt_)
{
    //rem::push_debug(SourceLocation) << " @(" << XY.to_string() << "):";
    point pt;
    pt += pt_ + _r.a; // + {0,0} by default;
    if (!_r.in(pt))
        throw rem::push_exception(source_fl) < rem::endl < " : " < (std::string)pt_ < " is out of range in " < _r.to_string();

    rem::push_debug(source_fnl) < color::Yellow < pt < rem::end;
    _cursor = _dc->peek(pt);
    return *this;
}


painter& painter::set_colors(const textattr::pair& aSet)
{

    _cell.set_fg(aSet.fg);
    _cell.set_bg(aSet.bg);
    *_cursor = (* _cursor & ~vdc::cell::CMask) | (_cell.mem & vdc::cell::CMask);
    return *this;
}

result<> painter::setup_geometry()
{
    rem::push_debug(source_ffl) < " painter _dc's '" < color::Chartreuse6 <  "Geometry: " <  _dc->geometry();
    if(!_r)
        _r = _dc->geometry();
    else
        _r = _dc->geometry()  & _r;

    if (!_r)
        throw rem::push_exception(source_ffl) < ": " < rem::endl < " - Attempt to < Contruct > a painter object on invalid Geometry : " < _r;

    rem::push_debug(source_ffl) < " Configured Geometry:" < color::Yellow < _r < color::Reset < " :";
    _cursor = _dc->peek(_r.a);
    //rem::push_debug(SourceLocation) <<  VDC::Cell{ *_cursor }.Details();
    return rem::ok;
}

painter& painter::clear()
{
    auto* CC = _cursor;
    _cursor = _dc->peek(_r.a);
    _cell << ' ';
    rem::push_debug(source_fl) < _cell.details();
    rem::push_output() < "Clearing subrect:" < _r;
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

/*!
    @brief Computes coordinates within the painter's bloc from the linear index (cursor) location.

    @param none
    @return computed point.
    @note none
    @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
*/
point painter::cpos()
{
    auto* B = _dc->mem;
    int dx = static_cast<int>((_cursor - B));
    point Pt = { dx % _dc->width(), dx / _dc->width() };
    rem::push_debug(source_fl) < " _cursor @" < color::Yellow < (std::string)Pt < color::Reset;
    return Pt;
}


/*!
    @brief set the justify bits.

    @param bits.
    @return ref to self.
    @note Justification bit values are not implemented yet ...
    @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
*/
painter& painter::set_bits(Justify::Type aBits)
{
    j_bits = aBits;
    return *this;
}

/*!
 *  @brief set the justify bits.
 *
 *  @param bits.
 *  @return ref to self.
 *  @note Justification bit values are not implemented yet ...
 *  @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
 */
painter& painter::set_hcenter(bool S)
{
    j_bits = S ? j_bits | Justify::HCenter : j_bits & ~Justify::HCenter;
    return *this;
}

/*!
 *  @brief set the justify bits.
 *
 *  @param bits.
 *  @return ref to self.
 *  @note Justification bit values are not implemented yet ...
 *  @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
 */
painter& painter::set_vcenter(bool S)
{
    j_bits = S ? j_bits | Justify::VCenter : j_bits & ~Justify::VCenter;
    return  *this;
}

/*!
 *  @brief set the justify bits.
 *
 *  @param bits.
 *  @return ref to self.
 *  @note Justification bit values are not implemented yet ...
 *  @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
 */
painter& painter::set_center(bool S)
{
    j_bits = S ? j_bits | Justify::HCenter | Justify::VCenter : j_bits & ~(Justify::HCenter | Justify::VCenter);
    return  *this;
}

/*!
 *  @brief set the word_wrap flag
 *
 *  @param S ( true or false)
 *  @return ref to self.
 *  @note Word-Wrapping is not implemented yet ...
 *  @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
 */
painter& painter::set_word_wrap(bool S)
{
    return  *this;
}

/*!
 *  @brief Set the current background color
 *
 *  @param color type code.
 *  @return ref to self.
 *
 *  @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
 */
painter& painter::set_bg(color::type aBg)
{
    _cell.set_bg(aBg);
    return *this;
}



/*!
 *  @brief Set the current foreground color
 *
 *  @param color type code.
 *  @return ref to self.
 *
 *  @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
 */
painter& painter::set_fg(color::type aFg)
{
    _cell.set_fg(aFg);
    return *this;
}


}
