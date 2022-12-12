#pragma once

#include <scrat/encodings/textattr.h>
#include <scrat/geometry.h>





template<> struct std::formatter<scrat::color::type>
{
    constexpr auto parse(std::format_parse_context& ctx) {
        auto pos = ctx.begin();
        while (pos != ctx.end() && *pos != '}') ++pos;
        return pos;
    }

    auto format(scrat::color::type c_, std::format_context& ctx)
    {
        return std::format_to(ctx.out(), "{}", scrat::attr<scrat::textattr::format::ansi256>::fg(c_));
    }
};


//template<typename T> struct std::formatter<scrat::point<T>>
//{
//    constexpr auto parse(std::format_parse_context& ctx) {
//        auto pos = ctx.begin();
//        while (pos != ctx.end() && *pos != '}') ++pos;
//        return pos;
//    }
//
//    auto format(scrat::point<T> c_, std::format_context& ctx)
//    {
//        return std::format_to(ctx.out(), "{},{}", c_.x, c_.y);
//    }
//};
//
//template<typename T> struct std::formatter<scrat::dim<T>>
//{
//    constexpr auto parse(std::format_parse_context& ctx) {
//        auto pos = ctx.begin();
//        while (pos != ctx.end() && *pos != '}') ++pos;
//        return pos;
//    }
//
//    auto format(scrat::dim<T> d, std::format_context& ctx)
//    {
//        return std::format_to(ctx.out(), "{} * {}={}", d.w, d.h, d.w*d.h);
//    }
//};




