#pragma once

#include <cstdint>
#include <scrat/dlconfig.h>
#include <string>



namespace scrat
{
namespace script
{

namespace type
{
using T = uint64_t;

constexpr T        null_t         = 0x1000000000000000;
constexpr T        number_t       = 0x000000000001;
constexpr T        text_t         = 0x000000000002;
constexpr T        void_ptr       = 0x000000000004;
constexpr T        any_t          = 0x000000000008;
constexpr T        leaf_t         = 0x000000000010;
constexpr T        sign_t         = 0x000000000020;
constexpr T        i8_t           = 0x000000000040;
constexpr T        i16_t          = 0x000000000080; // 0x7F41
constexpr T        i32_t          = 0x000000000100;
constexpr T        i64_t          = 0x000000000200;
constexpr T        u8_t           = 0x000000000400;
constexpr T        u16_t          = 0x000000000800;
constexpr T        u32_t          = 0x000000001000;
constexpr T        u64_t          = 0x000000002000;
constexpr T        float_t        = 0x000000004000; // real: f32, f64, f128... ^^
constexpr T        var_t          = 0x000000008000; // Ne pas oublier d'ajouter ce type a toutes "variables"
constexpr T        bloc_t         = 0x000000010000;
constexpr T        pipe_t         = 0x000000020000;
constexpr T        keyword_t      = 0x000000040000;
constexpr T        unary_t        = 0x000000080000;
constexpr T        prefix_t       = 0x000000100000;
constexpr T        postfix_t      = 0x000000200000;
constexpr T        operator_t     = 0x000000400000;
constexpr T        binary_t       = 0x000000800000;
constexpr T        function_t     = 0x000001000000;
constexpr T        function_ptr   = 0x000002000000;
constexpr T        object_t       = 0x000004000000;
constexpr T        pointer_t      = 0x000008000000;
constexpr T        type_t         = 0x000010000000;
constexpr T        id_t           = 0x000020000000;
constexpr T        ref_t          = 0x000040000000;
constexpr T        punc_t         = 0x000080000000;
constexpr T        assign_t       = 0x000100000000;
constexpr T        bool_t         = 0x000200000000;
constexpr T        hex_t          = 0x000400000000;
constexpr T        open_pair_t    = 0x000800000000;
constexpr T        close_pair_t   = 0x001000000000;
constexpr T        const_t        = 0x002000000000;
constexpr T        static_t       = 0x008000000000; ///< Flag set at the parser phase when applicable.
constexpr T        f32_t          = 0x010000000000;
constexpr T        f64_t          = 0x020000000000;
constexpr T        f128_t         = 0x040000000000;
constexpr T        octal_format_t = 0x080000000000;
constexpr T        bin_format_t   = 0x100000000000;


std::string SCRAT_API name(T T_);
T    SCRAT_API from_str(std::string);
}

enum class mnemonic : uint16_t
{
    k_null = 0,
    k_left_shift,
    k_radical,
    k_exponent,
    k_right_shift,
    k_decr,
    k_incr,
    k_assign_add,
    k_assign_sub,
    k_assign_mul,
    k_assign_div,
    k_assign_mod,
    k_assign_and,
    k_assign_or,
    k_assign_xor,
    k_assign_c1,
    k_assign_left_shift,
    k_assign_right_shift,
    k_deref,
    k_less_eq,
    k_greater_eq,
    k_equal,
    k_not_equal,  // != <>
    k_add,
    k_sub,
    k_mul,
    k_indirection,
    k_comment_cpp,
    k_modulo,
    k_less_than,
    k_greater_than,
    k_assign,
    k_binary_and,
    k_binary_or,
    k_binary_xor,
    k_c1,           // complement a 1
    k_c2,
    k_binary_not,          //
    k_logical_and,
    k_logical_or,
    k_open_abs_op,
    k_close_abs_op,
    k_open_par,
    k_close_par,
    Openindex,
    Closeindex,
    Openbrace,
    Closebrace,
    k_begin_comment,
    k_end_comment,
    k_div,
    k_comma,
    k_scope,
    k_semicolon,
    k_colon,
    k_range,
    k_factorial,
    k_positive,
    k_negative,
    k_squote, // '
    k_dquote, // "
    k_ternary, // ? :
    k_hash, // #
    k_eos,
    k_dot,
    k_return,
    k_if,
    k_pi,
    k_number,
    k_u8,
    k_u16,
    k_u32,
    k_u64,
    k_i8,
    k_i16,
    k_i32,
    k_i64,
    k_float,
    k_string,
    k_then,
    k_else,
    k_const,
    k_include,
    k_module,
    k_at,
    k_prime,
    k_do,
    k_while,
    k_for,
    k_until,
    k_repeat,
    k_switch,
    k_case,
    k_type,
    k_low_hex,
    k_up_hex,
    k_cosinus,
    k_arc_cosinus,
    k_tangent,
    k_arc_tangent,
    k_sinus,
    k_arc_sinus,
    k_object,
    k_static,
    k_this,
    k_unshadow,
    k_catch,
    k_throw,
    noop_
};

namespace type::distance
{
using T = uint8_t;
constexpr T scope      = 0;
constexpr T assign_rhs = 1;
constexpr T deref      = 2;
constexpr T prefix     = 3;
constexpr T unary      = 3;
constexpr T paranthese = 5;
constexpr T exponent   = 6;
constexpr T bits       = 7;
constexpr T product    = 8;
constexpr T addition   = 9;
constexpr T shift      = 10;
constexpr T equality   = 11;
constexpr T comp       = 12;
constexpr T bool_and   = 13;
constexpr T bool_xor   = 14;
constexpr T bool_or    = 15;
constexpr T boolean    = 16;
constexpr T ternary    = 17;
constexpr T comma      = 18;
constexpr T identifier = 19;
constexpr T assign     = 20; // a + b * m = 0 - 1 ( assign is right hand side association so the it has lowest priority)
constexpr T noop_      = 21;
}

namespace lexem
{

using T = const char *;
constexpr T k_null          = "null";
constexpr T k_left_shift    = "<<";
constexpr T k_radical       = "^/";
constexpr T k_exponent      = "^";
constexpr T k_right_shift   = ">>";
constexpr T k_decr          = "--";
constexpr T k_incr          = "++";
constexpr T k_assign_add    = "+=";
constexpr T k_assign_sub    = "-=";
constexpr T k_assign_mul    = "*=";
constexpr T k_assign_div    = "/=";
constexpr T k_assign_mod    = "%=";
constexpr T k_assign_and    = "&=";
constexpr T k_assign_or     = "|=";
constexpr T k_assign_xor    = "><=";
constexpr T k_assign_c1     = "`=";
constexpr T k_assign_lshift = "<<=";
constexpr T k_assign_rshift = ">>=";
constexpr T k_deref         = "->";
constexpr T k_less_equal    = "<=";
constexpr T k_greater_equal = ">=";
constexpr T k_equal         = "==";
constexpr T k_not_equal     = "!=";  // != <>
constexpr T k_addition      = "+";
constexpr T k_sub           = "-";
constexpr T k_multiply      = "*";
constexpr T k_indirection   = "*";
constexpr T k_comment_cpp   = "//";
constexpr T k_modulo        = "%";
constexpr T k_xor           = "><";
constexpr T k_less_than     = "<";
constexpr T k_greater_than  = ">";
constexpr T k_assign        = "=";
constexpr T k_binary_and    = "&";
constexpr T k_binary_or     = "|";
constexpr T k_c1            = "`"; ///< compl&eacute;ment &agrave; 1
constexpr T k_c2            = "``";///< compl&eacute;ment &agrave; 2
constexpr T k_not           = "!";
constexpr T k_bool_and      = "&&";
constexpr T k_bool_or       = "||";
constexpr T k_abs_begin     = "|<";// |< expr >|
constexpr T k_abs_end       = ">|";
constexpr T k_open_par      = "(";
constexpr T k_close_par     = ")";
constexpr T k_open_index    = "[";
constexpr T k_close_index   = "]";
constexpr T k_brace_begin   = "{";
constexpr T k_brace_end     = "}";
constexpr T k_comment_begin = "/*";
constexpr T k_comment_end   = "*/";
constexpr T k_division      = "/";
constexpr T k_comma         = ",";
constexpr T k_scope         = "::";
constexpr T k_semicolon     = ";";
constexpr T k_colon         = ":";
constexpr T k_range         = "..";
constexpr T k_factorial     = "!";
constexpr T k_positive      = "+";
constexpr T k_negative      = "-";
constexpr T k_squote        = "'";  // '
constexpr T k_dquote        = "\""; // "
constexpr T k_ternary       = "?";  // ? :
constexpr T k_hash          = "#";  // #
constexpr T k_eos           = "$";
constexpr T k_dot           = ".";
constexpr T k_return        = "return";
constexpr T k_if            = "if";
constexpr T k_pi            = "pi";
constexpr T k_number        = "number";
constexpr T k_u8            = "u8";
constexpr T k_u16           = "u16";
constexpr T k_u32           = "u32";
constexpr T k_u64           = "u64";
constexpr T k_i8            = "i8";
constexpr T k_i16           = "i16";
constexpr T k_i32           = "i32";
constexpr T k_i64           = "i64";
constexpr T k_float         = "real";
constexpr T k_string        = "string";
constexpr T k_then          = "then";
constexpr T k_else          = "else";
constexpr T k_const         = "const";
constexpr T k_include       = "include";
constexpr T k_module        = "module";
constexpr T k_at            = "@";
constexpr T k_prime         = "`"; ///@todo change...
constexpr T k_do            = "do";
constexpr T k_while         = "while";
constexpr T k_for           = "for";
constexpr T k_until         = "until";
constexpr T k_repeat        = "repeat";
constexpr T k_switch        = "switch";
constexpr T k_case          = "case";
constexpr T k_type          = "type";
constexpr T k_low_hex       = "0x";
constexpr T k_up_hex        = "0X";
constexpr T k_cosinus       = "cos";
constexpr T k_arc_cosinus   = "acos";
constexpr T k_tangent       = "tan";// tan(4*a*m)  - sin(4ac) sina
constexpr T k_arc_tangent   = "atan";
constexpr T k_sinus         = "sin";
constexpr T k_arc_sinus     = "asin";
constexpr T k_object        = "object";
constexpr T k_static        = "static";
constexpr T k_this          = "me";
constexpr T k_unshadow      = ".::";
constexpr T k_catch         = "catch";
constexpr T k_throw         = "throw";

mnemonic SCRAT_API from_str(const std::string &M_);

}

std::string SCRAT_API mnemonic_name(mnemonic);

}
}
