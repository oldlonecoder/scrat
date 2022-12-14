//
// Created by slussier on 21-08-08.
//

#pragma once

#include <scrat/result.h>
#include <scrat/lexer/tokendata.h>

namespace scrat
{

namespace script
{

/*!
 * @brief Lexical Scanner(s)
 *
 * @author &copy; 2020, Serge Lussier(Bretzelus; bretzel; lonesomecoder; LordVorace...),  lussier.serge@gmail.com;
 *
 * @note Re-write of my lexical scanners; offset shall dissociate lexer from Lsc::String and only use std::string.
 */
class SCRAT_API lexer
{
    struct internal_cursor
    {
        const char *B  = nullptr; ///> Absolute Beginning of the source text.
        const char *E  = nullptr; ///> Absolute end of the soure text.
        const char *C  = nullptr;


        bool operator++();
        bool operator++(int);
        [[maybe_unused]] bool skip_ws();
        bool end_of_file(const char *P = nullptr) const;
        [[nodiscard]] std::ptrdiff_t Index() const;
        int        L   = -1;
        int        Col = -1;
        void sync();
        std::string scan_to_eol();
        [[nodiscard]]std::string line_num() const;
        [[nodiscard]]std::string mark() const;
        [[nodiscard]]std::string location() const;
        bool       _F  = false; ///< Used as "state machine" for math factor notation syntax style
        rem::code scan_to(const char *SubStr_);
        std::string scan_string();

        internal_cursor() = default;
        explicit internal_cursor(const char *Source_);
    } src_cursor;

    struct num_scanner
    {
        const char *B   = nullptr;
        const char *E   = nullptr;
        const char *C   = nullptr;
        const char *Eos = nullptr;
        bool Real = false;

        bool Literal = true; ///< scan literal digits - do not validate hexadecimal...

        enum NumBase
        {
            None,
            Bin,
            Oct,
            Dec,
            Hex,/* ..., */ //  FUCK!!!
        }Num = None;

        num_scanner() = default;
        num_scanner(const char *_c, const char *_eos);

        ~num_scanner() = default;

        //bool operator++();
        bool operator++(int);

        explicit operator bool() const;
        script::type::T operator()() const;
    };

public:
    struct SCRAT_API config_data
    {
        const char *Source  = nullptr;
        token_data::collection *Tokens = nullptr;
        operator bool() const { return Source && Tokens; }
    };


    lexer() = default;
    lexer(const lexer&) = default;
    lexer(lexer&&)      = default;

    ~lexer()            = default;

    lexer::config_data& config() { return mConfig;}

    rem::code  Exec();
    rem::code  operator()();

    [[maybe_unused]] [[nodiscard]] bool Empty() const { return mConfig.Tokens == nullptr || mConfig.Tokens->empty(); }
    void dump_tokens(std::function<void(const token_data&)> callback_);


    rem::code step_begin();
    token_data* step();

private:

    config_data mConfig;

    rem::code Push(token_data& Token_) ;


    #pragma region Scanners

    using ScannerFn = rem::code(lexer::*)(token_data&);
    using Input = std::pair<script::type::T, lexer::ScannerFn>;
    using ScanTable = std::vector<lexer::Input>;
    using Scanner = lexer::ScannerFn;
    static lexer::ScanTable scan_table;

    static Scanner get_scanner(token_data& token);

    rem::code input_binary_operator(token_data&);
    rem::code input_default(token_data&);
    rem::code input_unary_operator(token_data&);
    rem::code input_punctuation(token_data&);
    rem::code input_keyword(token_data&);
    rem::code input_hex(token_data&);
    rem::code input_text(token_data&);
    rem::code scan_number(token_data&);
    rem::code scan_identifier(token_data&);
    //rem::code ScanFactorNotation(token_data&);
    rem::code scan_sign_prefix(token_data&);
    rem::code scan_prefix(token_data&);
    rem::code scan_postfix(token_data&);
    rem::code skip_cpp_comment();

    #pragma endregion Scanners

    void insert_multiply(token_data&);
};

}
}

