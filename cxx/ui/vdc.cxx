#include <scrat/ui/vdc.h>
#include <scrat/ui/console.h>



namespace scrat::ui
{

_object_name(vdc)


vdc::vdc(object* parent_, scrat::dim dimension_):
	_owner(parent_),
	wh(dimension_)
{
    _geo = rect({}, wh);
}

vdc::~vdc()
{
	delete[] mem;
}

result<vdc::type> vdc::alloc(const dim& dim_)
{
	if (dim_)
	{
		wh = dim_;
		delete[] mem;
		mem = nullptr;
	}
	if (mem)
		delete[] mem;

	mem = new vdc::cell::type[wh.w * wh.h + 1];
	return mem;
}

result<vdc::type> vdc::realloc(const dim& dim_)
{
	if (!dim_)
		return rem::push_error() < rem::null_ptr < " cannot re-allocate undefined or invalide dimension.";

	wh = dim_;
    _geo.assign({},wh);
	if (mem)
		delete[] mem;
	mem = mem = new vdc::cell::type[wh.w * wh.h + 1];
	return mem;
}

void vdc::dealloc()
{
	wh = {};
    _geo = {};
	delete[] mem;
}

rect vdc::geometry()
{
	return _geo;
}

int vdc::width()
{
	return wh.w;
}

int vdc::height()
{
	return wh.h;
}

point vdc::crs_pos()
{
	return pos;
}

result<> vdc::set_position(const point& pt_)
{
	pos = pt_;
	if (!_geo.in(pt_))
		return rem::push_error() < rem::rejected < " vdc::set_position : out of bounds :[" < (std::string)pt_ < ']';
	return rem::ok;
}

vdc::type vdc::peek(const point& pt_)
{
	auto* p = mem + (pt_.x + pt_.y * wh.w);
	if (!_geo.in(pt_))
		return nullptr;

	return p;
}

std::string vdc::cell::details()
{
   stracc str = "Cell Details%s[Char:'%s%d%s'/Fg:'%s%s%s'/Bg:'%s%s%s']";
    str << color::White << color::Yellow << (int)ascii() << color::White
        << color::Yellow << textattr::name(fg()) << color::White
        << color::Yellow << textattr::name(bg()) << color::White;

    return str();
}

result<> vdc::update_rect(const rect &r_)
{
    // rect r = r_ & geometry();
    // if(!r)
    //     return rem::rejected;

    rem::push_debug(source_fnl) < color::OrangeRed1 < class_name() < " Exposed geometry: " < color::Yellow < r_ < color::Reset;
    console::update(this, location(), r_);

    return rem::accepted;
}
result<> vdc::clear()
{
   if (!mem)
        throw rem::push_fatal(source_fl) << " Attempt to clear a null VDC";

    //Rem::Debug(SourceLocation) << " Clear " << Color::Yellow << sizeof(_Block) << "bytes;";
    auto* C = mem;
    auto blk = wh.w * wh.h;
    for (int x = 0; x < blk; x++) *C++ = _cell;
    return rem::accepted;
}

result<> vdc::clear(const rect& r_)
{
    rect b;
    if (r_)
        b = rect({0,0},wh) & r_;
    else
        return clear();

    if(!mem)
        throw rem::push_fatal(source_fl) < " Attempt to clear a null VDC";


    if (!b)
        return rem::rejected;

    for (int y = b.a.y; y <= b.b.y; y++)
    {
        cell::type* C = peek({b.a.x,y});
        for (int x = 0; x < b.width(); x++) *C++ = _cell;
    }
    return rem::accepted;
}

result<> vdc::clear(vdc::cell::type a_, const rect& r)
{
	auto a = _cell;
    _cell = a_;
    clear(r);
    _cell = a;

	return rem::accepted;
}




//------------------------- vdc::cell : -------------------

vdc::cell::cell(cell::type c_): mem(c_){}
vdc::cell::cell(vdc::type c_): mem(*c_){}





uint16_t vdc::cell::attributes()
{
    return (mem & AttrMask) >> ATShift;
}


std::string vdc::cell::render_colors()
{
    using Ansi = attr<textattr::format::ansi256>;
    std::string str;
    str += Ansi::bg(bg()) += Ansi::fg(fg());
    return str;
}

vdc::cell &scrat::ui::vdc::cell::set_fg(color::type fg_)
{
	mem = (mem & ~FGMask) | (static_cast<cell::type>(fg_) << FGShift);
	return *this;
}

vdc::cell &vdc::cell::set_bg(color::type bg_)
{
	mem = (mem & ~BGMask) | (static_cast<cell::type>(bg_) << BGShift);
    return *this;
}
vdc::cell &vdc::cell::set_attribute(vdc::cell::type a_)
{
	 mem = (mem & ~AttrMask) | a_;
    return *this;
}
vdc::cell &vdc::cell::set_color(textattr::pair &&c_)
{
	return set_fg(c_.fg).set_bg(c_.bg);
}

vdc::cell &vdc::cell::set_color(const textattr::pair &c_)
{
    return set_fg(c_.fg).set_bg(c_.bg);
}

vdc::cell &vdc::cell::operator=(vdc::cell::type d_)
{
	mem = d_;
    return *this;
}

vdc::cell &vdc::cell::operator=(vdc::type d_)
{
    mem = *d_;
    return *this;
}

vdc::cell &vdc::cell::operator=(char d_)
{
    mem = mem & (~CharMask) | d_;
    return *this;
}

color::type vdc::cell::fg()
{
    return static_cast<color::type>((mem & FGMask) >> FGShift);
}

color::type vdc::cell::bg()
{
    return static_cast<color::type>((mem & BGMask) >> BGShift);
}

textattr::pair vdc::cell::colors()
{
    return {fg(),bg()};
}

uint8_t vdc::cell::ascii()
{
    return mem & CharMask;

}
vdc::cell& vdc::cell::operator << (Icon::Type i_)
{
	mem = (mem & ~(cell::AttrMask | cell::CharMask)) | cell::UGlyph | i_;
	return *this;
}

vdc::cell& vdc::cell::operator<< (Accent::Type accent_)
{
    mem = (mem & ~(cell::UGlyph | cell::CharMask)) | cell::Accent | accent_;
    return *this;
}


vdc::cell& vdc::cell::reset_attributes(vdc::cell::type bits_)
{
    mem = (mem & (CharMask | UGlyph)) | bits_;
    return *this;
}

Icon::Type vdc::cell::icon_id()
{
    if (!(mem & cell::UGlyph)) return Icon::NullPtr;
    auto Ic = mem & cell::CharMask;
    if (Ic > 59) Ic = Icon::Bug;
    return  Ic;
}


Accent::Type vdc::cell::accent_id()
{
    auto AID = mem & cell::CharMask;
    if (AID > Accent::Ucirc) return Accent::Agrave;
    return static_cast<Accent::Type>(AID);
}


}
