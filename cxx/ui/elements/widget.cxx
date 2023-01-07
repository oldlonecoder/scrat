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

    if(parent_)
    {
        _widget_class_bits |= WClass::Child;
        _dc = parent<widget>()->_dc; ///@note Check nullptr Alert!!
    }
    else
        _widget_class_bits |= WClass::TopLevel;

    rem::push_debug(source_fnl) < " widget initial cell attribute:" < rem::endl < _attr.details();
}


widget::~widget()
{
    if(_widget_class_bits & WClass::TopLevel) delete _dc;
    //...
}

result<> widget::update(const rect& r_)
{
    if(!_dc)
        throw rem::push_exception(source_pffl) < " this widget '" < class_name() < "' has no vdc!!!";

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
result<> widget::update()
{
    if(!_dc)
        throw rem::push_exception(source_pffl) < " this widget '" < class_name() < "' has no vdc!!!";

    rect r = geometry();
    rem::push_debug(source_fnl) < color::OrangeRed1 < class_name() < ":Geometry: " < color::Yellow < r < color::Reset;
        // update the entire widget geometry

    if(!(_dc->geometry() & r))
        return rem::rejected;

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

result<> widget::set_geometry(const rect& r_)
{
    _wh = r_.sz;
    _xy = r_.a;

    return setup_backbuffer();
}


void widget::set_location(const point& xy_)
{
    if(!_dc)
        throw rem::push_exception(source_pffl) < " this widget '" < class_name() < "' has no vdc!!!";
    _xy = xy_;
    if(_widget_class_bits& (WClass::TopLevel | WClass::Floating))
        _dc->set_location(_xy);
     // ...
}

result<> widget::setup_backbuffer()
{
    if(!_wh)
        return rem::rejected;

    if(_widget_class_bits & (WClass::TopLevel | WClass::Floating))
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
        if(!(_widget_class_bits & WClass::Child))
            return rem::push_error() < " Can't setup widget's back buffer : neither Toplevel|Floating nor child.";
        auto* p = parent<widget>();
        if(!p)
            return rem::push_error() < " Can't setup widget's back buffer : Parent object is not a widget.";
        if(!p->_dc)
            return rem::push_error() < " Can't setup widget's back buffer : Parent widget has not setup it's back buffer.";
        _dc = p->_dc;

    }

    return rem::accepted;
}


/*!
    @brief Generates a painter object (on the heap, using new) for drawing(writting) contents onto this widget

    @param subregion, default is the entire widget's geometry into the vdc bloc.
    @return Reference to the painter instance.

    @code

       A{2,2}
       B{5,1}
       C{16,2}

       C->B {16,2}
       C->A {21,3}
       C->Terminal{25,7}
       +========================================================+
       |                            A                           |
       |     +====================================+             |
       |     |                 B                  |             |
       |     |                                    |             |
       |     |                +===================|             |
       |     |                |         C         |             |
       |     |                +===================|             |
       |     +====================================+             |
       |                                                        |
       +========================================================+

    @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
*/
result<painter&> widget::begin_draw(const rect& r_)
{
    if(!(_state & State::Visible))
            throw rem(rem::message, source_pffl) < " skipping " < class_name() < " update: " < geometry();

    if(!_dc)
        throw rem::push_exception(source_pffl) < " this widget '" < class_name() < "' has no vdc!!!";

    // _xy is actually relative to immediate parent's location into the back buffer;
    // iterate parent chaining until the owner of the _dc so we adjust the offset of this child widget:

    auto r = (geometry() + _xy) & _dc->geometry();
    if(!r)
    {
        _state &= ~(State::Visible);
        throw rem(rem::message, source_pffl) < " skipping " < class_name() < " update: " < geometry();
    }
    painter* p = new painter(_dc, &_attr.mem, r_ ? r_ : rect());
    p->set_colors(_attr.colors());
    p->home();
    return *p;
}


result<> widget::end_draw(painter& painter_)
{
    if(!_dc)
        throw rem::push_exception(source_pffl) < " this widget '" < class_name() < "' has no vdc!!!";
    delete (&painter_);
    //...
    return rem::accepted;
}

void widget::draw()
{
    if(!_dc)
        throw rem::push_exception(source_pffl) < " this widget '" < class_name() < "' has no vdc!!!";
    auto& paint = *begin_draw();
    paint.clear();
    end_draw(paint);
}

}

