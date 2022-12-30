#pragma once

#include <scrat/ui/vdc.h>
#include <scrat/ui/uidefs.h>

namespace scrat::ui
{

class SCRAT_API painter
{
    vdc* _dc = nullptr;
    rect _r;   ///< Geometrie interne. (Celui du _dc ou un sous-rectangle de _dc)
    vdc::cell _cell;
    vdc::type _cursor = nullptr;
    bool _wrap = false;

    point cpos();
    Justify::Type j_bits = 0;
    _decl_objname

public:

    painter() = default;
    ~painter() = default;

    painter(vdc* dc_, rect r_ = {});
    painter(painter&&) noexcept = default;
    painter(const painter&) = default;

    painter& operator = (painter&&) noexcept = default;
    painter& operator = (const painter&) = default;



    painter& operator << (color::type aColorID);
    painter& operator << (Icon::Type aIconID);
    painter& operator << (const std::string& aStr);
    painter& operator << (const stracc& aStr);
    painter& operator << (const point& XY);
    painter& operator << (const char* aStr);
    painter& operator << (Accent::Type aAcc);


    painter& GotoXY(const point& XY);
    painter& Home() { return GotoXY({}); }

    template<typename T> painter& operator << (const T& aStr)
    {
        std::ostringstream Out;
        Out << aStr;
        return (*this) << Out.str();
    }

    painter& set_colors(const textattr::pair& pair_);

    result<> setup_geometry();
    painter& clear();
   // void Update();
    Justify::Type& justify_bits() { return j_bits; }
    painter& set_bits(Justify::Type aBits);// { Bits = aBits; return *this; }
    painter& set_hcenter(bool S);// { Bits = S ? (Bits & ~HCenter) | HCenter : (Bits & ~HCenter); return *this; }
    painter& set_vcenter(bool S);
    painter& set_center(bool S);
    painter& set_word_wrap(bool S);
    painter& set_bg(color::type aBg);
    painter& set_fg(color::type aFg);
};
}
