#pragma once

#include <scrat/ui/elements/widget.h>
#include <scrat/ui/elements/icon.h>
#include <scrat/ui/painter.h>

namespace scrat::ui
{

class SCRAT_API text_input : public widget
{
    icon* _prefix_icon = nullptr;
    icon* _suffix_icon = nullptr;
    std::string _data;
    std::string _place_holder;
public:
    text_input();
    text_input(widget* parent_, const std::string& ph_);

    ~text_input() override;

    void draw() override;
    result<> set_shortkey(char c_);
};

}