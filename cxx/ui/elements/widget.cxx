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

    auto rr = expose(r_);
    rect r = *rr;

    rem::push_debug(source_fnl) < color::OrangeRed1 < class_name() < color::Reset < ":Geometry: " < color::Yellow < r < color::Reset;
    if(!r)
        return rem::rejected;
    rem::push_debug(source_fnl) < color::OrangeRed1 < class_name() < color::Reset < " Exposed geometry: " < color::Yellow < r < color::Reset;
    console::update(_dc,{},r);

    return rem::accepted;
}


result<> widget::update()
{

    return update(geometry());
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
       D{3,0}

       C->B {16,2}
       C->A {21,3}
       C->Terminal{25,7}

       +========================================================+
       |                            A                           |
       |     +====================================+             |
       |     |                 B                  |             |
       |     |                                    |             |
       |     |                +===================|             |
       |     |                |   D     C         |             |
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

    rect r = geometry();
    if(is_toplevel() || is_floating())
    {
        if(r_) r = r & r_;
        if(!r)
            throw rem(rem::message, source_pffl) < " skipping " < class_name() < " update: " < geometry();


    }
    rem::push_debug(source_ffl) < " draw Geometry : " < r < ":";
    if(r_)
        r = r & r_;

    if(!r)
    {
        _state &= ~(State::Visible);
        throw rem(rem::message, source_pffl) < " skipping " < class_name() < " update: " < geometry();
    }
    painter* p = new painter(_dc, &_attr.mem, r);
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
    rem::push_debug(source_pffl) < " class '" < color::Orange3 < this->class_name() < color::Reset <"' geometry:[" < color::Yellow < geometry() < color::Reset < "] :";

    for(auto* c : _children)
    {
        widget* w = c->to<widget>();
        if(w) w->draw();
    }
}

result<rect> widget::expose(const rect &local_sub_r)
{

    auto r = geometry();
    if(local_sub_r)
    {
        r = r & local_sub_r;
        if(!r) return r;
    }

    r += _xy;

    if(is_toplevel() || is_floating() )
    {
        //final offset compute here:
        r =  console::geometry() & r;
        if(!r)
            return r;

        return r;
    }
    else
    {
        auto * par = parent<widget>();
        if(par) return par->expose(r);
    }

    // no expose; (no parent, this widget is not toplevel nor floating -- Logistic Bug ...)
    return {};
}


bool widget::is_toplevel()
{
    return _widget_class_bits & WClass::TopLevel;
}

bool widget::is_child()
{
    return  _widget_class_bits & WClass::Child;
}

bool widget::is_floating()
{
    return  _widget_class_bits & WClass::Floating;
}
}
