#pragma once

#include <scrat/ui/elements/widget.h>
#include <scrat/ui/elements/icon.h>
#include <scrat/ui/painter.h>

namespace scrat::ui
{


class SCRAT_API text_input : public widget
{
    int _input_length = 0;
    int _cursor = 0;
    State::Type _state;
    _decl_objname
    std::string _place_holder;
    std::string _text;
public:
    text_input();
    text_input(widget* parent_, const std::string& ph_);

    ~text_input() override;

    void draw() override;
    void set_text(const std::string& txt_);
    std::string text() { return _text; }

};

/*!
 * @brief Composite text input widget.
 * @note Make visible the prefix icon only when in State::Focus.
 *
 * @author &copy; 2023, Serge Lussier; lussier.serge@gmail.com; (oldlonecoder)
 */
class SCRAT_API input_field : public widget
{
    _decl_objname
    icon* _prefix_icon = nullptr;
    icon* _suffix_icon = nullptr;
    text_input* _input = nullptr;
public:
    struct opt
    {
        Icon::Type prefix=0;
        Icon::Type suffix=0;
        char       key=0;
        std::string placeholder;
    };

    input_field() ;
    input_field(widget* pr_, input_field::opt&& opt_);
    ~input_field() override;

    result<> set_geometry(const dim& wh_) override;

private:
    input_field::opt _options;
    text_input* input_widget() { return _input; }

};



}