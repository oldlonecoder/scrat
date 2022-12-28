#include <scrat/ui/vdc.h>



namespace scrat::ui
{




vdc::vdc(object* parent_, scrat::dim dimension_):
	_owner(parent_),
	wh(dimension_)
{
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
	if (mem)
		delete[] mem;
	mem = mem = new vdc::cell::type[wh.w * wh.h + 1];
	return mem;
}

void vdc::dealloc()
{
	wh = {};
	delete[] mem;
}

rect vdc::geometry()
{
	return { {0,0}, wh };
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
	if (!rect{ {0, 0}, wh}.in(pt_))
		return rem::push_error() < rem::rejected < " vdc::set_position : out of bounds :[" < (std::string)pt_ < ']';
	return rem::ok;
}

vdc::type vdc::peek(const point& pt_)
{
	rect r = { {0,0}, wh };
	auto* p = mem + (pt_.x + pt_.y * wh.w);
	if (!r.in(pt_))
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

result<> vdc::clear()
{
	return rem::notimplemented;
}

result<> vdc::clear(const rect& r_)
{
	return rem::notimplemented;
}

result<> vdc::clear(vdc::cell::type a_, const rect& r)
{
	return rem::notimplemented;
}


vdc::cell & vdc::cell::operator<<(Icon::Type i_)
{
    return *this;
}

}
