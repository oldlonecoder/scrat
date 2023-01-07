#include <scrat/ui/elements/label.h>




namespace scrat::ui
{

_object_name(label)


label::label(widget *parent_, const std::string &txt_):widget(parent_, parent_ ? WClass::Child:WClass::TopLevel),
_text(txt_)
{
    _attr.set_color(colors::db::data["default"]["label"][State::Normal]);

}

label::~label()
{
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
    try{
        auto & p = *begin_draw({_xy, _wh});
        p.home();
        p << _text;
        end_draw(p);
    }
    catch(rem e)
    {
        rem::push_info() < rem::endl < "[" < e.cc() < "]";
    }
}


}
