#pragma once


#include <scrat/textattr>
#include <scrat/stracc>
#include <scrat/result>
#include <scrat/Icons.h>
#include <scrat/accents.fr.h>

namespace scrat
{
    //    "─", //    "━", //    "│", //    "┃", //    "┌", //    "┍", //    "┎", //    "┏", //    "┐", //    "┑", //    "┒", //    "┓", //    "└", //    "┕", //    "┖", //    "┗", //    "┘", //    "┙", //    "┚", //    "┛", //    "├", //    "┝", //    "┞", //    "┟", //    "┠", //    "┡", //    "┢", //    "┣", //    "┤", //    "┥", //    "┦", //    "┧", //    "┨", //    "┩", //    "┪", //    "┫", //    "┬", //    "┭", //    "┮", //    "┯", //    "┰", //    "┱", //    "┲", //    "┳", //    "┴", //    "┵", //    "┶", //    "┷", //    "┸", //    "┹", //    "┺", //    "┻", //    "┼", //    "┽", //    "┾", //    "┿", //    "╀", //    "╁", //    "╂", //    "╃", //    "╄", //    "╅", //    "╆", //    "╇", //    "╈", //    "╉", //    "╊", //    "╋", //    "═", //    "║", //    "╒", //    "╓", //    "╔", //    "╕", //    "╖", //    "╗", //    "╘", //    "╙", //    "╚", //    "╛", //    "╜", //    "╝", //    "╞", //    "╟", //    "╠", //    "╡", //    "╢", //    "╣", //    "╤", //    "╥", //    "╦", //    "╧", //    "╨", //    "╩", //    "╪", //    "╫", //    "╬", //    "╭", //    "╮", //    "╯", //    "╰", //    "╴", //    "╵", //    "╶", //    "╷", //    "╸", //    "╹", //    "╺", //    "╻", //    "╼", //    "╽", //    "╾", //    "╿",

    /*!
        * @todo write docs
        *
        <p>
            Test Sample:
        </p>
        @code result<> Test::TestText()
        {
            text txt;
            txt = " <FB: Reset; F:Yellow; Icon:Home;> Salut <FB:Reset White;  f:Yellow;> &agrave; <FB:Reset;>Toi!";
            txt.Compile();

            return Message::Code::Ok;
        }
        @endcode


    */

using std::string;
using std::string_view;

class SCRAT_API text_tokenizer
{
    std::string_view _d;

    [[maybe_unused]] static string_view _default_token_separators;
    using iterator = string_view::iterator;
    using list     = std::vector<string_view>;
public:
    text_tokenizer() = default;
    explicit text_tokenizer(string_view);
    explicit text_tokenizer(char* aStr);
    explicit text_tokenizer(string_view, string_view Delim, bool KeepDelim);
    explicit text_tokenizer(char* aStr, string_view Delim, bool KeepDelim);
    explicit text_tokenizer(const std::string& aStr, string_view Delim, bool KeepDelim);

    text_tokenizer(const char* aStr);
    explicit text_tokenizer(std::string aStr);
    //text_tokenizer(string&&);
    ~text_tokenizer() = default;

    struct SCRAT_API token_t
    {
        string_view::iterator mStart;
        string_view::iterator mEnd;
        string_view::iterator mSE;

        string operator()() const;
        string operator*() const;

        using list = std::vector<text_tokenizer::token_t>;
        using iterator = text_tokenizer::token_t::list::iterator;
        [[maybe_unused]] [[nodiscard]] string Mark() const;

        int         mLine     = 1;
        int         mCol      = 1;
        std::size_t mPosition = 0;

        std::string location();
    };

    struct SCRAT_API config_data
    {
        text_tokenizer::token_t::list tokens;
        string_view delimiters;
        bool keep;
        size_t z;
    };


    std::size_t operator()(text_tokenizer::token_t::list &Collection, string_view aDelimiters = "", bool KeepAsWord = true) const;
    //text_tokenizer& operator>>(text_tokenizer::Token::list& Collection);

    text_tokenizer::iterator scan_to(text_tokenizer::iterator start_, char c_) const;
private:
    struct s_p_s
    {
        string_view::iterator _begin;
        string_view::iterator _pos;
        string_view::iterator _end; /// ...

        int      _line  = 1;
        int      _col   = 1;
        uint64_t _index = 0;

        s_p_s() = default;
        ~s_p_s() = default;

        explicit s_p_s(string_view str_);
        bool skip_ws();
        [[nodiscard]] bool end() const;
        bool operator++();
        bool operator++(int);
        void reset(string_view str_)
        {
            _pos   = _begin = str_.begin();
            _line  = _col   = 1;
            _index = 0;
            _end  = str_.end();
        }
        s_p_s &operator>>(text_tokenizer::token_t &word_);
    } _cursor;
public:
    text_tokenizer& operator=(string_view str_);
    text_tokenizer& operator=(const char* str_);
    string_view operator()(){ return _d.data(); }
private:
    config_data _cfg;



};
class SCRAT_API text
{
    std::string _d;
    textattr::format _f = textattr::format::ansi256;
public:

        /**
            * Default constructor
            */
        text();

        /**
            * Copy constructor
            *
            * @param other TODO
        */
        text(const text& other);
        text(const std::string& aStr);
        text(std::string&& aStr) noexcept;
        text(const char* aStr);
        /**
            * Destructor
            */
        ~text();
        result<> compile();
        result<> operator >> (std::string&);
        struct SCRAT_API token_data
        {
            enum class type : uint8_t
            {
                Accent,
                AttrCmd,
                Identifier,
                Punctuation,
                Justify,
                Unset,
                Eof
            };

            enum class mnemonic : uint16_t
            {
                Fg,
                Bg,
                Color,
                Ic,
                ArgSeq,
                ArgSep,
                Identifier,
                OpenTag,
                ClosingTag,
                Br,
                Eos,
                AccentSeq,
                JustifySeq, ///< " ... |- "
                Left,
                Top,
                Right,
                Bottom,
                Center,
                HVCenter,
                Null
                // ...
            };

            struct location_data
            {
                const char* begin = nullptr;
                const char* end = nullptr;
                const char* s_begin = nullptr;
                int index = -1;
            } _location;

            using Lexem                       = const char*;
            static constexpr Lexem Fg        = "FG";
            static constexpr Lexem Bg        = "BG";
            static constexpr Lexem ColorPair  = "COLOR";
            static constexpr Lexem Icon      = "ICON";
            static constexpr Lexem OpenSeq    = "<";
            static constexpr Lexem CloseSeq   = ">";
            static constexpr Lexem ArgSeq     = ":";
            static constexpr Lexem ArgSep     = ",";
            static constexpr Lexem EoSt       = ";";
            static constexpr Lexem LineBreak  = "BR";
            static constexpr Lexem Accent     = "&";
            static constexpr Lexem Left       = "[";
            static constexpr Lexem Right      = "]";
            static constexpr Lexem Top        = "^";
            static constexpr Lexem Bottom     = "_";
            static constexpr Lexem Center     = "-"; // ex.: "<^-..." Top | Center
            static constexpr Lexem HVCenter   = "+";
            static constexpr Lexem Null       = "Unset";

            token_data::type T = token_data::type::Unset;
            token_data::mnemonic M = token_data::mnemonic::Null;
            token_data::Lexem L = nullptr;

            std::string operator()();

            static token_data scan(const char* Start);

            using list = std::vector <token_data>;
            static list Referential;

            token_data() = default;

            token_data(text::token_data::type aType, text::token_data::mnemonic aMenomic, text::token_data::Lexem aLexem,int nParams);
            token_data(token_data&&)noexcept;
            token_data(const token_data&) = default;

            token_data& operator=(text::token_data&& r) noexcept
            {
                T = r.T;
                M = r.M;
                L = r.L;
                _location = std::move(r._location);
                return *this;
            }
            token_data& operator=(const token_data& r)
            {
                T = r.T;
                M = r.M;
                L = r.L;
                _location = r._location;
                return *this;
            }

            operator bool() { return (L != nullptr) || (T != token_data::type::Unset); }

            std::string mark(const char*);
            int NParam = 0;

        };

        struct SCRAT_API attribute
        {
            // ============ Indiquer c'est quoi qu'il faut lire!!!!!! ======================
            color::type _fg  = color::Black;
            color::type _bg  = color::Black;
            Icon::Type _icn   = Icon::NullPtr;
            Accent::Type _accnt = Accent::Err;
            // =============================================================================
            const char* _begin = nullptr;
            const char* _end   = nullptr;
            const char* _crs     = nullptr;

            struct assigned
            {
                int8_t _for : 1;
                int8_t _bac : 1;
                int8_t _icn : 1;
                int8_t _acc : 1;
            } _assign = {0};


            attribute() = default;
            attribute(token_data& Info);
            attribute(attribute&&) noexcept = default;
            attribute(const attribute&) = default;
            ~attribute() = default;
            std::string informations();
            std::string operator()() const;

            attribute& operator = (attribute&&) noexcept = default;
            attribute& operator = (const attribute&) = default;
            using list = std::vector<text::attribute>;
        };

        void push_attribute(text::attribute Attr);
    private:
        friend class compiler;
        friend struct attribute;
        text::attribute::list _attributes;

        struct compiler
        {
            enum class State : uint8_t {
                Out,        ///< In outside context state ( normal text ) - Begin Seq is Expected
                StartAttr,  ///< Enter Attributes context '<' - Entering into Seq state (inside) context - Requires Out current state
                EndAttr,    ///< EndSeq '<' Requires IntAttr current state; Switch to Out
                InAttr,     ///< in Attr Seq, requires BeginAttr current state.
                AttrSeq,    ///< Command Token requires InAttr current state.
                AttrArg,    ///< Command
                ArgSeq,     ///< Argument Seq, requires CmdToken current state.
                StartUTF,   ///< UTF-8 accent '&' requires Out current state.
                EndUTF,     ///< Set right after ParseAccentMnemonic.
                EndSeq
            };

           // State St = State::Out; unused
            const char* B = nullptr;
            const char* C = nullptr;
            const char* E = nullptr;

            token_data::list Tokens; ///< Sous r&eacute;serve...Pas certain d'en avoir besoin.

            bool operator ++(); ///< Prefix
            bool operator ++(int); ///< Postfix
            //result<> Skip();
            text& TextRef;

            void close_token(token_data& Info);
            result<> execute();
            result<text::attribute> compile_attribute(text::attribute& Attr);
            result<text::attribute> compile_accent(text::attribute& Attr);
            result<text::token_data> scan();

            text::token_data skip_to_attribute();

            result<color::type> color_id(token_data& Token);
            result<Icon::Type> icon_id(token_data& Token);
            text::token_data scan_identifier();
            result<> eat_token(text::token_data& Token);
            compiler() = delete;
            compiler(text& aTextInstance);
            ~compiler() = default;
            bool eof();

            using parser_fnptr_t = result<>(text::compiler::*)(text::attribute&);
            using parsers_table_t = std::unordered_map<token_data::mnemonic, text::compiler::parser_fnptr_t>;

            result<> parse_icon(text::attribute& A); // Parser pour ':' + 'IconID' + ';'
            result<> parse_fg(text::attribute& A); // Parser pour ':' + 'ColorID' + ';'
            result<> ParseBg(text::attribute& A); // Parser pour ':' + 'ColorID' + ';'
            result<> parse_color(text::attribute& A); // Parser pour ':' + 'ColorID' + ',' + 'ColorID' + ';'
            result<> parse_br(text::attribute& A); // 'BR'; ( <br; .. > ou <br>)
            result<> close_attribute(text::attribute& A); // 'BR'; ( <br; .. > ou <br>)
            result<> check_eos(text::attribute& A);
            std::string mark();
        };

        static text::token_data scan(text::token_data::mnemonic M);
    };
}
