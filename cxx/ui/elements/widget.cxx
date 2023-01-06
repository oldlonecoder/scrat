// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <scrat/ui/elements/widget.h>
#include <scrat/ui/console.h>



namespace scrat::ui
{
_object_name(widget)



widget::widget():object()
{
    // no flags, no parent. Thus this is set to toplevel widget by default.
    _attr.set_color(colors::db::data["default"]["widget"][State::Active]);
    _attr = ' ';
    rem::push_debug(source_fnl) < " widget initial cell attribute:" < rem::endl < _attr.details();
}

widget::widget(object* parent_, WClass::Type f_): object(parent_),
_widget_class_bits(f_)
{
    _attr.set_color(colors::db::data["default"]["widget"][State::Active]);
    _attr = ' ';
    rem::push_debug(source_fnl) < " widget initial cell attribute:" < rem::endl < _attr.details();
}


widget::~widget()
{
    if(_widget_class_bits & WClass::TopLevel) delete _dc;
    //...
}

result<> widget::update(const rect& r_)
{
    rect r = geometry();
    rem::push_debug(source_fnl) < color::OrangeRed1 < class_name() < ":Geometry: " < color::Yellow < r < color::Reset;
    if(!r_)
    {
        // update the entire widget geometry
        r = _dc->geometry() & r;
        if(!r)
            return rem::rejected;
    }
    else
        r = r & r_;

    rem::push_debug(source_fnl) < color::OrangeRed1 < class_name() < " Exposed geometry: " < color::Yellow < r < color::Reset;
    _dc->update_rect(r);// -- Disabled
    //console::me().render_vdc(_bloc);
    return rem::accepted;
}


rect widget::geometry()
{
    return {{},_wh};
}


result<> widget::set_geometry(const dim& wh_)
{
    _wh = wh_;
    return setup_backbuffer();
}

void widget::set_location(const point& xy_)
{
    _xy = xy_;
    if(_widget_class_bits& WClass::TopLevel)
        _dc->set_location(_xy);
     // ...
}

result<> widget::setup_backbuffer()
{
    if(!_wh)
        return rem::rejected;

    if(_widget_class_bits & WClass::TopLevel)
    {
        if(_dc)
            _dc->realloc(_wh);
        else
        {
            _dc = new vdc(this,_wh);
            _dc->alloc();
        }
    }
    else
    {
        ;
    }
    return rem::accepted;
}


/*!
    @brief Generates a painter object (on the heap, using new) for drawing(writting) contents onto this widget

    @param subregion, default is the entire widget's geometry into the vdc bloc.
    @return Reference to the painter instance.

    @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
*/
result<painter&> widget::begin_draw(const rect& r_)
{
    painter* p = new painter(_dc, &_attr.mem, r_ ? r_ : rect());
    p->set_colors(_attr.colors());
    p->home();
    return *p;
}


result<> widget::end_draw(painter& painter_)
{
    delete (&painter_);
    //...
    return rem::accepted;
}

}

