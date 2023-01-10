// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <scrat/ui/elements/icon.h>



namespace scrat::ui
{


_object_name(icon)

icon::icon() : widget()
{
    assign_attributes_components(colors::db::data["default"]["icon"]);
    _attr.set_color(colors::db::data["default"]["icon"][State::Focus]);
}


icon::icon(widget* parent_, Icon::Type ic_): widget(parent_),
_ic(ic_)
{
    assign_attributes_components(colors::db::data["default"]["icon"]);
    _attr.set_color(colors::db::data["default"]["icon"][State::Focus]);
    rem::push_debug(source_pffl) < _attr.details();
    set_geometry({0,0, 2,1});
}


icon::~icon(){}

void icon::draw()
{
    widget::draw();
    painter& p = *begin_draw();
    p.set_bg(_attr.bg());
    p << _ic;
    end_draw(p);
}


}
