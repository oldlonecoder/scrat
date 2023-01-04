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

}

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
    return setup_backbuffer();
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


/*!
    @brief Generates a painter object for drawing(writting) contents onto this widget

    @param subregion, default is the entire widget's geometry into the vdc bloc.
    @return pointer to the painter instance.

    @author &copy; 2018,2023; Serge Lussier;oldlonecoder (lussier.serge@gmail.com)
*/
result<painter*> widget::begin_draw(const rect& r_)
{
    painter* p = new painter(_bloc, r_ ? r_ : geometry());
    return p;
}

result<> end_draw(painter* painter_)
{
    delete painter_;
    //...
    return rem::accepted;
}

}

