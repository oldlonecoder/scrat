// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <scrat/object>
#include <scrat/ui/vdc.h>
#include <scrat/ui/uidefs.h>
#include <scrat/ui/painter.h>

namespace scrat::ui {

/**
 * @todo write docs
 */
class widget : public object
{
    _decl_objname
protected:
    wclass::type _widget_class_bits = 0;
    vdc* _dc = nullptr; ///< point to the instance of the widget's vdc. Whoever is the owner (wclass::type flags)

    result<> setup_backbuffer(); ///< sous reserve
    dim   _wh; ///< dimensions
    point _xy; ///< relative position into the parent's geometry...
    vdc::cell _attr     = ' ';
    State::Type _state  = State::Normal|State::Visible;
    const colors::db::components* _components_attr;

    //painter _paint;

public:
    widget();
    widget(object* parent_, wclass::type class_ = 0);
    ~widget() override;

    virtual result<> set_geometry(const dim& wh_);
    virtual result<> set_geometry(const rect& r_);
    virtual void set_location(const point &xy_);

    virtual result<> update(const rect& r_);
    virtual result<> update();
    virtual rect geometry();
    virtual result<painter&> begin_draw(const rect& r_ = {});
    virtual result<> end_draw(painter& painter_);
    virtual void set_state(State::Type state_);
    int width() { return _wh.w; }
    int height() { return _wh.h; }
    virtual void draw();
    result<> assign_attributes_components(const colors::db::components& _comp)
    {
        _components_attr = &_comp;
        return rem::notimplemented;
    }


    result<rect> expose(const rect& local_sub_r = {});

    bool is_toplevel();
    bool is_child();
    bool is_floating();

};

}


