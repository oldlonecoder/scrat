#include <scrat/ui/elements/label.h>
#include "label.h"



namespace scrat::ui
{

label::label(widget *parent_, const std::string &txt_):widget(parent_, parent_ ? WClass::Child),
_text(txt_)
{
    _attr.set_color(colors::db::data["default"]["label"][State::Normal]);
}

label::~label()
{
    _text.clear();
}

}