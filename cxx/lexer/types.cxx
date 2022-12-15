//
// Created by slussier on 21-08-08.
//

#include <scrat/lexer/types.h>
#include <scrat/text.h>

#include <map>


namespace scrat
{
namespace script
{

namespace type
{

std::map<T, std::string> T_STR = {{null_t,         "null"},
                                  {number_t,       "number"},
                                  {text_t,         "text"},
                                  {void_ptr,       "void_ptr"},
                                  {any_t,          "any"},
                                  {leaf_t,         "leaf_t"},
                                  {sign_t,         "signed"},
                                  {i8_t,           "i8_t"},
                                  {i16_t,          "i16_t"},
                                  {i32_t,          "i32_t"},
                                  {i64_t,          "i64_t"},
                                  {u8_t,           "u8_t"},
                                  {u16_t,          "u16_t"},
                                  {u32_t,          "u32_t"},
                                  {u64_t,          "u64_t"},
                                  {float_t,        "float_t"},
                                  {var_t,          "var_t"},
                                  {bloc_t,         "bloc_t"},
                                  {pipe_t,         "pipe_t"},
                                  {keyword_t,      "keyword_t"},
                                  {unary_t,        "unary_t"},
                                  {prefix_t,       "prefix_t"},
                                  {postfix_t,      "postfix_t"},
                                  {operator_t,     "operator_t"},
                                  {binary_t,       "binary_t"},
                                  {function_t,     "function_t"},
                                  {function_ptr,   "function_ptr"},
                                  {object_t,       "object_t"},
                                  {pointer_t,      "pointer_t"},
                                  {type_t,         "type_t"},
                                  {id_t,           "id_t"},
                                  {ref_t,          "ref_t"},
                                  {punc_t,         "punc_t"},
                                  {assign_t,       "assign_t"},
                                  {bool_t,         "bool_t"},
                                  {hex_t,          "hex_t"},
                                  {bin_format_t,   "bin_format_t"},
                                  {octal_format_t, "octal_format_t"},
                                  {f32_t,          "f32_t"},
                                  {f64_t,          "f64_t"},
                                  {f128_t,         "f128_t"},
                                  {open_pair_t,    "open_pair_t"},
                                  {close_pair_t,   "close_pair_t"},
                                  {static_t,       "static_t"},
                                  {const_t,       "const_t"},
                                  };

std::map<std::string, T> STR_T = {{"null_t",       null_t},
                                  {"number_t",     number_t},
                                  {"text_t",       text_t},
                                  {"string_t",     text_t},
                                  {"void_ptr",     void_ptr},
                                  {"void_pointer", void_ptr},
                                  {"any_t",        any_t},
                                  {"leaf_t",       leaf_t},
                                  {"signed",       sign_t},
                                  {"i8_t",         i8_t},
                                  {"i16_t",        i16_t},
                                  {"i32_t",        i32_t},
                                  {"i64_t",        i64_t},
                                  {"u8_t",         u8_t},
                                  {"u16_t",        u16_t},
                                  {"u32_t",        u32_t},
                                  {"u64_t",        u64_t},
                                  {"float_t",      float_t},
                                  {"var_t",        var_t},
                                  {"bloc_t",       bloc_t},
                                  {"pipe_t",       pipe_t},
                                  {"stack_t",      pipe_t},
                                  {"keyword_t",    keyword_t},
                                  {"unary",        unary_t},
                                  {"prefix_t",     prefix_t},
                                  {"postfix_t",    postfix_t},
                                  {"operator_t",   operator_t},
                                  {"binary_t",     binary_t},
                                  {"function_t",   function_t},
                                  {"function_ptr", function_ptr},
                                  {"object_t",     object_t},
                                  {"pointer_t",    pointer_t},
                                  {"type_t",       type_t},
                                  {"id_t",         id_t},
                                  {"identifier",   id_t},
                                  {"ref_t",        ref_t},
                                  {"reference",    ref_t},
                                  {"punc_t",       punc_t},
                                  {"punctuation",  punc_t},
                                  {"assign_t",     assign_t},
                                  {"bool_t",       bool_t},
                                  {"hex_t",        hex_t},
                                  {"open_pair_t",  open_pair_t},
                                  {"close_pair_t", close_pair_t},
                                  {"const_t",       const_t},};

std::string name(T T_)
{
    std::string Str_;

    if(T_ >= script::type::bin_format_t)
    {
        Str_ += "*.*";
        return Str_;
    }

    script::type::T tbm = T_;
    //int                a, c, b;

    for(const auto &t: script::type::T_STR)
    {
        if(!tbm)
            break;
        if(t.first & tbm)
        {
            Str_ += t.second;
            tbm &= ~t.first;  // Remove the Bit from the "Sem" field.
            if(tbm)
                Str_ += '/';
        }
    }
    if(Str_.empty())
        Str_ += "null";
    return Str_;
}


T SCRAT_API from_str(const std::string& Str )
{
    T types = 0;
    text_tokenizer CStr = Str.c_str();
    text_tokenizer::token_t::list W;
    (void) CStr(W, "/", false);

    for(auto I: W) types |= STR_T[I()];

    return types;
}

}

}

namespace script
{

std::string mnemonic_name(mnemonic M)
{
    std::map<mnemonic, std::string_view> _ = {{    mnemonic::k_null,               "k_null"},
                                                  {mnemonic::k_left_shift,         "k_left_shift"},
                                                  {mnemonic::k_radical,            "k_radical"},
                                                  {mnemonic::k_exponent,           "k_exponent"},
                                                  {mnemonic::k_right_shift,        "k_right_shift"},
                                                  {mnemonic::k_decr,               "k_decr"},
                                                  {mnemonic::k_incr,               "k_incr"},
                                                  {mnemonic::k_assign_add,         "k_assign_add"},
                                                  {mnemonic::k_assign_sub,         "k_assign_sub"},
                                                  {mnemonic::k_assign_mul,         "k_assign_mul"},
                                                  {mnemonic::k_assign_div,         "k_assign_div"},
                                                  {mnemonic::k_assign_mod,         "k_assign_mod"},
                                                  {mnemonic::k_assign_and,         "k_assign_and"},
                                                  {mnemonic::k_assign_or,          "k_assign_or"},
                                                  {mnemonic::k_assign_xor,         "k_assign_xor"},
                                                  {mnemonic::k_assign_c1,          "k_assign_c1"},
                                                  {mnemonic::k_assign_left_shift,  "k_assign_left_shift"},
                                                  {mnemonic::k_assign_right_shift, "k_assign_right_shift"},
                                                  {mnemonic::k_deref,              "k_deref"},
                                                  {mnemonic::k_less_eq,            "k_less_eq"},
                                                  {mnemonic::k_greater_eq,         "k_greater_eq"},
                                                  {mnemonic::k_equal,              "k_equal"},
                                                  {mnemonic::k_not_equal,          "k_not_equal"},
                                                  {mnemonic::k_add,                "k_add"},
                                                  {mnemonic::k_sub,                "k_sub"},
                                                  {mnemonic::k_mul,                "k_mul"},
                                                  {mnemonic::k_indirection,        "k_indirection"},
                                                  {mnemonic::k_comment_cpp,        "k_comment_cpp"},
                                                  {mnemonic::k_modulo,             "k_modulo"},
                                                  {mnemonic::k_less_than,          "k_less_than"},
                                                  {mnemonic::k_greater_than,       "k_greater_than"},
                                                  {mnemonic::k_assign,             "k_assign"},
                                                  {mnemonic::k_binary_and,         "k_binary_and"},
                                                  {mnemonic::k_binary_or,          "k_binary_or"},
                                                  {mnemonic::k_binary_xor,         "k_binary_xor"},
                                                  {mnemonic::k_c1,                 "k_c1"},// complement a 1
                                                  {mnemonic::k_c2,                 "k_c2"},
                                                  {mnemonic::k_binary_not,         "k_binary_not"},   //
                                                  {mnemonic::k_logical_and,        "k_logical_and"},
                                                  {mnemonic::k_logical_or,         "k_logical_or"},
                                                  {mnemonic::k_open_abs_op,        "k_open_abs_op"},
                                                  {mnemonic::k_close_abs_op,       "k_close_abs_op"},
                                                  {mnemonic::k_open_par,           "k_open_par"},
                                                  {mnemonic::k_close_par,          "k_close_par"},
                                                  {mnemonic::Openindex,            "k_open_index"},
                                                  {mnemonic::Closeindex,           "k_close_index"},
                                                  {mnemonic::Openbrace,            "k_brace_begin"},
                                                  {mnemonic::Closebrace,           "k_brace_end"},
                                                  {mnemonic::k_begin_comment,      "k_begin_comment"},
                                                  {mnemonic::k_end_comment,        "k_end_comment"},
                                                  {mnemonic::k_div,                "k_div"},
                                                  {mnemonic::k_comma,              "k_comma"},
                                                  {mnemonic::k_scope,              "k_scope"},
                                                  {mnemonic::k_semicolon,          "k_semicolon"},
                                                  {mnemonic::k_colon,              "k_colon"},
                                                  {mnemonic::k_range,              "k_range"},
                                                  {mnemonic::k_factorial,          "factorial"},
                                                  {mnemonic::k_positive,           "k_positive"},
                                                  {mnemonic::k_negative,           "k_negative"},
                                                  {mnemonic::k_squote,             "k_squote"},
                                                  {mnemonic::k_dquote,             "k_dquote"},
                                                  {mnemonic::k_ternary,            "k_ternary"},
                                                  {mnemonic::k_hash,               "k_hash"},
                                                  {mnemonic::k_eos,                "k_eos"},
                                                  {mnemonic::k_dot,                "k_dot"},
                                                  {mnemonic::k_return,             "k_return"},
                                                  {mnemonic::k_if,                 "k_if"},
                                                  {mnemonic::k_pi,                 "k_pi"},
                                                  {mnemonic::k_number,             "k_number"},
                                                  {mnemonic::k_u8,                 "k_u8"},
                                                  {mnemonic::k_u16,                "k_u16"},
                                                  {mnemonic::k_u32,                "k_u32"},
                                                  {mnemonic::k_u64,                "k_u64"},
                                                  {mnemonic::k_i8,                 "k_i8"},
                                                  {mnemonic::k_i16,                "k_i16"},
                                                  {mnemonic::k_i32,                "k_i32"},
                                                  {mnemonic::k_i64,                "k_i64"},
                                                  {mnemonic::k_float,              "k_float"},
                                                  {mnemonic::k_string,             "k_string"},
                                                  {mnemonic::k_then,               "k_then"},
                                                  {mnemonic::k_else,               "k_else"},
                                                  {mnemonic::k_const,              "k_const"},
                                                  {mnemonic::k_include,            "k_include"},
                                                  {mnemonic::k_module,             "k_module"},
                                                  {mnemonic::k_at,                 "k_at"},
                                                  {mnemonic::k_prime,              "k_prime"},
                                                  {mnemonic::k_do,                 "k_do"},
                                                  {mnemonic::k_while,              "k_while"},
                                                  {mnemonic::k_for,                "k_for"},
                                                  {mnemonic::k_until,              "k_until"},
                                                  {mnemonic::k_repeat,             "k_repeat"},
                                                  {mnemonic::k_switch,             "k_switch"},
                                                  {mnemonic::k_case,               "k_case"},
                                                  {mnemonic::k_type,               "k_type"},
                                                  {mnemonic::k_low_hex,            "k_low_hex"},
                                                  {mnemonic::k_up_hex,             "k_up_hex"},
                                                  {mnemonic::k_cosinus,            "k_cosinus"},
                                                  {mnemonic::k_arc_cosinus,        "k_arc_cosinus"},
                                                  {mnemonic::k_tangent,            "k_tangent"},
                                                  {mnemonic::k_arc_tangent,        "k_arc_tangent"},
                                                  {mnemonic::k_sinus,              "k_sinus"},
                                                  {mnemonic::k_arc_sinus,          "k_arc_sinus"},
                                                  {mnemonic::k_object,             "k_object"},
                                                  {mnemonic::k_static,             "k_static"},
                                                  {mnemonic::k_this,               "k_this"},
                                                  {mnemonic::k_unshadow,           ".::"},
                                                  {mnemonic::noop_,                "noop_"}};
    return std::string(_[M]);
}

}

namespace script
{
mnemonic SCRAT_API  lexem::from_str(const std::string &M_)
{
    std::map<mnemonic, std::string> _ = {{mnemonic::k_null,               "k_null"},
                                             {mnemonic::k_left_shift,         "k_left_shift,"},
                                             {mnemonic::k_radical,            "radical,"},
                                             {mnemonic::k_exponent,           "k_exponent,"},
                                             {mnemonic::k_right_shift,        "k_right_shift,"},
                                             {mnemonic::k_decr,               "k_decr"},
                                             {mnemonic::k_incr,               "k_incr"},
                                             {mnemonic::k_assign_add,         "k_assign_add"},
                                             {mnemonic::k_assign_sub,         "k_assign_sub"},
                                             {mnemonic::k_assign_mul,         "k_assign_mul"},
                                             {mnemonic::k_assign_div,         "k_assign_div"},
                                             {mnemonic::k_assign_mod,         "k_assign_mod"},
                                             {mnemonic::k_assign_and,         "k_assign_and"},
                                             {mnemonic::k_assign_or,          "k_assign_or"},
                                             {mnemonic::k_assign_xor,         "k_assign_xor"},
                                             {mnemonic::k_assign_c1,          "k_assign_c1"},
                                             {mnemonic::k_assign_left_shift,  "k_assign_left_shift"},
                                             {mnemonic::k_assign_right_shift, "k_assign_right_shift"},
                                             {mnemonic::k_less_eq,            "k_less_eq"},
                                             {mnemonic::k_greater_eq,         "k_greater_eq"},
                                             {mnemonic::k_equal,              "k_equal"},
                                             {mnemonic::k_not_equal,          "NEq"},
                                             {mnemonic::k_add,                "k_add"},
                                             {mnemonic::k_sub,                "k_sub"},
                                             {mnemonic::k_mul,                "k_mul"},
                                             {mnemonic::k_indirection,        "k_indirection"},
                                             {mnemonic::k_comment_cpp,        "k_comment_cpp"},
                                             {mnemonic::k_modulo,             "k_modulo"},
                                             {mnemonic::k_less_than,          "k_less_than"},
                                             {mnemonic::k_greater_than,       "k_greater_than"},
                                             {mnemonic::k_assign,             "assign_t"},
                                             {mnemonic::k_binary_and,         "k_binary_and"},
                                             {mnemonic::k_binary_or,          "k_binary_or"},
                                             {mnemonic::k_binary_xor,         "k_binary_xor"},
                                             {mnemonic::k_c1,                 "k_c1"},// complement a 1
                                             {mnemonic::k_c2,                 "k_c2"},
                                             {mnemonic::k_binary_not,         "k_binary_not"},   //
                                             {mnemonic::k_logical_and,        "k_logical_and"},
                                             {mnemonic::k_logical_or,         "k_logical_or"},
                                             {mnemonic::k_open_abs_op,        "k_open_abs_op"},
                                             {mnemonic::k_close_abs_op,       "k_close_abs_op"},
                                             {mnemonic::k_open_par,           "k_open_par"},
                                             {mnemonic::k_close_par,          "k_close_par"},
                                             {mnemonic::Openindex,            "k_open_index"},
                                             {mnemonic::Closeindex,           "k_close_index"},
                                             {mnemonic::Openbrace,            "k_brace_begin"},
                                             {mnemonic::Closebrace,           "k_brace_end"},
                                             {mnemonic::k_begin_comment,      "k_begin_comment"},
                                             {mnemonic::k_end_comment,        "k_end_comment"},
                                             {mnemonic::k_div,                "k_div"},
                                             {mnemonic::k_comma,              "k_comma"},
                                             {mnemonic::k_scope,              "k_scope"},
                                             {mnemonic::k_semicolon,          "k_semicolon"},
                                             {mnemonic::k_colon,              "k_colon"},
                                             {mnemonic::k_range,              "k_range"},
                                             {mnemonic::k_factorial,          "k_factorial"},
                                             {mnemonic::k_positive,           "k_positive"},
                                             {mnemonic::k_negative,           "k_negative"},
                                             {mnemonic::k_squote,             "k_squote"},
                                             {mnemonic::k_dquote,             "k_dquote"},
                                             {mnemonic::k_ternary,            "k_ternary"},
                                             {mnemonic::k_hash,               "k_hash"},
                                             {mnemonic::k_eos,                "k_eos"},
                                             {mnemonic::k_dot,                "k_dot"},
                                             {mnemonic::k_return,             "k_return"},
                                             {mnemonic::k_if,                 "k_if"},
                                             {mnemonic::k_pi,                 "k_pi"},
                                             {mnemonic::k_number,             "k_number"},
                                             {mnemonic::k_u8,                 "k_u8"},
                                             {mnemonic::k_u16,                "k_u16"},
                                             {mnemonic::k_u32,                "k_u32"},
                                             {mnemonic::k_u64,                "k_u64"},
                                             {mnemonic::k_i8,                 "k_i8"},
                                             {mnemonic::k_i16,                "k_i16"},
                                             {mnemonic::k_i32,                "k_i32"},
                                             {mnemonic::k_i64,                "k_i64"},
                                             {mnemonic::k_float,              "k_float"},
                                             {mnemonic::k_string,             "k_string"},
                                             {mnemonic::k_then,               "k_then"},
                                             {mnemonic::k_else,               "k_else"},
                                             {mnemonic::k_const,              "k_const"},
                                             {mnemonic::k_include,            "k_include"},
                                             {mnemonic::k_module,             "k_module"},
                                             {mnemonic::k_at,                 "k_at"},
                                             {mnemonic::k_prime,              "k_prime"},
                                             {mnemonic::k_do,                 "k_do"},
                                             {mnemonic::k_while,              "k_while"},
                                             {mnemonic::k_for,                "k_for"},
                                             {mnemonic::k_until,              "k_until"},
                                             {mnemonic::k_repeat,             "k_repeat"},
                                             {mnemonic::k_switch,             "k_switch"},
                                             {mnemonic::k_case,               "k_case"},
                                             {mnemonic::k_type,               "k_type"},
                                             {mnemonic::k_low_hex,            "k_low_hex"},
                                             {mnemonic::k_up_hex,             "k_up_hex"},
                                             {mnemonic::k_cosinus,            "k_cosinus"},
                                             {mnemonic::k_arc_cosinus,        "k_arc_cosinus"},
                                             {mnemonic::k_tangent,            "k_tangent"},
                                             {mnemonic::k_arc_tangent,        "k_arc_tangent"},
                                             {mnemonic::k_sinus,              "k_sinus"},
                                             {mnemonic::k_arc_sinus,          "k_arc_sinus"},
                                             {mnemonic::k_object,             "k_object"},
                                             {mnemonic::k_static,             "k_static"},
                                             {mnemonic::k_this,               "k_this"},
                                             {mnemonic::k_unshadow,           "k_unshadow"},
                                             {mnemonic::noop_,                "noop_"}};
    for(const auto &P: _)
    {
        if(P.second == M_)
            return P.first;
    }
    return mnemonic::noop_;
}
}


}
