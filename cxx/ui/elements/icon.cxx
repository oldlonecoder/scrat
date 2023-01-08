// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <scrat/ui/elements/icon.h>



namespace scrat::ui
{


_object_name(icon)

icon::icon() : widget()
{
    _attr.set_color(colors::db::data["default"]["icon"][State::Focus]);
}

icon::icon(widget* parent_, Icon::Type ic_): widget(parent_),
_ic(ic_)
{
    _attr.set_color(colors::db::data["default"]["icon"][State::Focus]);
    set_geometry({0,0, 1,1});
}


icon::~icon(){}

void icon::draw()
{
    widget::draw();
    painter& p = *begin_draw();
    p << _ic;
    end_draw(p);
}


}
