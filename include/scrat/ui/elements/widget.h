// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <scrat/object>
#include <scrat/ui/vdc.h>
#include <scrat/ui/uidefs.h>

namespace scrat::ui {

/**
 * @todo write docs
 */
class widget : public object
{
    _decl_objname
protected:
    WClass::Type _widget_class_bits = 0;
    vdc* _mem = nullptr;
public:
    widget();
    widget(object* parent_, WClass::Type class_ = 0);
    ~widget() override;



};

}


