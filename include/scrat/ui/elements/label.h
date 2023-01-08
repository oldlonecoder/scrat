// SPDX-FileCopyrightText: 2023 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
//#include <scrat/ui/elements/widget.h>
#include <scrat/ui/painter.h>
#include <scrat/ui/elements/icon.h>

namespace scrat::ui
{

class SCRAT_API label : public widget
{
    _decl_objname
    std::string _text;
    Justify::Type _j = Justify::Prefix;
    icon* _prefix_icon = nullptr;
    icon* _suffix_icon = nullptr;
    char  _shortkey = 0; ///< atl+$_shortkey; - surely conflicting with DE/WM/system ... :(

public:


    label():widget(){
        _attr.set_color(colors::db::data["default"]["label"][State::Normal]);
    }

    label(widget* parent_, const std::string& txt_);


    ~label() override;
    void draw() override;
    result<> set_prefix_icon(Icon::Type c_);
    result<> set_suffix_icon(Icon::Type c_);
    result<> set_shortkey(char c_);

};


}
