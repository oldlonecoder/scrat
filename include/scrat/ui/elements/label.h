// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <scrat/ui/elements/widget.h>
#include <scrat/ui/painter.h>


namespace scrat::ui
{

class SCRAT_API label : public widget
{
    _decl_objname
    std::string _text;
    Justify::Type _j = Justify::Prefix;

public:
    label():widget(){
        _attr.set_color(colors::db::data["default"]["label"][State::Normal]);
    }

    label(widget* parent_, const std::string& txt_);

    ~label() override;
    void draw() override;
};


}
