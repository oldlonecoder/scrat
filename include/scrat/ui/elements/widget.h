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
    WClass::Type _widget_class_bits = 0;
    vdc* _bloc = nullptr; ///< point to the instance of the widget's vdc. Whoever is the owner (WClass::Type flags)

    result<> setup_backbuffer(); ///< sous reserve
    dim   _wh; ///< dimensions
    point _xy; ///< relative position into the parent's geometry...
    vdc::cell _attr = ' ';
public:
    widget();
    widget(object* parent_, WClass::Type class_ = 0);
    ~widget() override;

    virtual result<> set_geometry(const dim& wh_);
    virtual void set_location(const point &xy_);

    virtual result<> update(const rect& r_ = {});
    virtual rect geometry();
    virtual result<painter&> begin_draw(const rect& r_ = {});
    virtual result<> end_draw(painter& painter_);

};

}


