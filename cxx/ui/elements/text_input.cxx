#include <scrat/ui/elements/text_input.h>


namespace scrat::ui
{

_object_name(text_input)
_object_name(input_field);


text_input::text_input(): widget()
{
    assign_attributes_components(colors::db::data["default"]["input-area"]);
    set_state(State::Active);
}

text_input::text_input(widget *parent_, const std::string &ph_): widget(parent_),
_place_holder(ph_)
{
    assign_attributes_components(colors::db::data["default"]["input-area"]);
    set_state(State::Active);
}


void text_input::draw() {
    widget::draw();
    painter& paint = *begin_draw();
    paint.gotoxy({0, 0});
    paint << _place_holder;
    end_draw(paint);
}

text_input::~text_input()
{
    _place_holder.clear();
    _text.clear();
}

void text_input::set_text(const std::string &txt_)
{
    _text = txt_;
}


// ------------------------ input field widget ---------------------------------------------------

input_field::input_field(): widget()
{
    assign_attributes_components(colors::db::data["default"]["input-field"]);
    set_state(State::Active);
}


input_field::input_field(widget *pr_, input_field::opt &&opt_):widget(pr_),
_options(std::move(opt_))
{
    assign_attributes_components(colors::db::data["default"]["input-field"]);
    set_state(State::Active);
}

input_field::~input_field()
{
    delete _prefix_icon;
    delete _suffix_icon;
    delete _input;
}

result<> input_field::set_geometry(const dim &wh_)
{
    widget::set_geometry(wh_);
    if(_options.prefix)
    {
        _prefix_icon = new icon(this, _options.prefix);
        _prefix_icon->set_location({0,0});
    }
    if(_options.suffix)
    {
        _suffix_icon = new icon(this, _options.suffix);
        _suffix_icon->set_location({geometry().b.x-1,0});
    }

    _input = new text_input(this, _options.placeholder);

    _input->set_geometry(dim{{},{},_wh.w-4, _wh.h});
    _input->set_location({2,0});

    return rem::accepted;
}


}