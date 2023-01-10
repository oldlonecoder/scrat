#include <scrat/ui/elements/label.h>

namespace scrat::ui
{

_object_name(label)


label::label(widget *parent_, const std::string &txt_):widget(parent_, parent_ ? wclass::Child:wclass::TopLevel),
_text(txt_)
{
    assign_attributes_components(colors::db::data["default"]["label"]);
    set_state(State::Active);
}

label::~label()
{
    delete _prefix_icon;
    delete _suffix_icon;
    _text.clear();
}

/*!
    @brief draw the content of this widget

    @param none
    @return void
    @note This version does not handle or process justification bits, nor the scrat markup text, yet...
    @author &copy; (1965, first breath in this mad world) 1987(first autodidact c++ learning),2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
*/
void label::draw()
{
    widget::draw();
    auto & p = *begin_draw();
    p.gotoxy({3,0}); ///< abritrary positionning for not erasing the icon child widget;
    p << _text;
    end_draw(p);
}

result<> label::set_prefix_icon(Icon::Type c_)
{
    return  rem::notimplemented;
}

result<> label::set_suffix_icon(Icon::Type c_)
{
    return  rem::notimplemented;
}

result<> label::set_shortkey(char c_)
{
    return  rem::notimplemented;
}
}
