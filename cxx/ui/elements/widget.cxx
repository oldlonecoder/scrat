// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <scrat/ui/elements/widget.h>
#include <scrat/ui/console.h>



namespace scrat::ui
{
_object_name(widget)



widget::widget():object(){}

widget::widget(object* parent_, WClass::Type f_): object(parent_),
_widget_class_bits(f_)
{

}


widget::~widget()
{

}

result<> widget::update(const rect& r_)
{
    if(!r_)
    {
        // update the entire widget geometry
        auto r = _bloc->geometry() & geometry();
        if(!r)
            return rem::rejected;

        console::update(_bloc, r);
    }
    return rem::notimplemented;
}


rect widget::geometry()
{
    return {_xy,_wh};
}


result<> widget::set_geometry(const dim& wh_)
{
    _wh = wh_;
    return rem::accepted;
}

void widget::set_location(const point& xy_)
{
    _xy = xy_; // ...
}

result<> widget::setup_backbuffer()
{
    if(!_wh)
        return rem::rejected;

    if(_widget_class_bits & WClass::TopLevel)
    {
        if(_bloc)
            _bloc->realloc(_wh);
        else
        {
            _bloc = new vdc(this,_wh);
            _bloc->alloc();
        }
    }
    else
    {
        ;
    }
    return rem::accepted;
}



}

