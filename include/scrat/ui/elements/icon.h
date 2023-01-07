// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <scrat/ui/elements/widget.h>
#include <scrat/Icons.h>

namespace scrat::ui
{

/**
 * @todo write docs
 */
class SCRAT_API icon : public  scrat::ui::widget
{
    _decl_objname
    Icon::Type _ic = Icon::NullPtr;

public:

    icon();
    icon(widget* parent_, Icon::Type ic_);

    ~icon() override;
    void draw() override;

};

}

