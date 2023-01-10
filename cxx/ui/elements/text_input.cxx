#include <scrat/ui/elements/text_input.h>


namespace scrat::ui
{

text_input::text_input():widget()
{}

text_input::text_input(widget *parent_, const std::string &ph_): widget(parent_),
_place_holder(ph_)
{
    _prefix_icon = new icon(this, Icon::PencilDr);
    _prefix_icon->set_location({0,0});
    _attr.set_color(colors::db::data["default"]["text-input"][State::Focus]);

    //_suffix_icon = new icon(this, Icon::Warning)
}

text_input::~text_input()
{
    delete _prefix_icon;
    delete _suffix_icon;
    _place_holder.clear();
}

void text_input::draw() {
    widget::draw();

}

result<> text_input::set_shortkey(char c_) {
    return rem::notimplemented;
}


}