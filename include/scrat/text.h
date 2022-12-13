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
    std::string_view _mData;

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
        string_view::iterator mStart;
        string_view::iterator mPos;
        string_view::iterator mStop; /// ...

        int      mLine  = 1;
        int      mCol   = 1;
        uint64_t mIndex = 0;

        s_p_s() = default;
        ~s_p_s() = default;

        explicit s_p_s(string_view str_);
        bool Skip();
        [[nodiscard]] bool End() const;
        bool operator++();
        bool operator++(int);
        void Reset(string_view str_)
        {
            mPos   = mStart = str_.begin();
            mLine  = mCol   = 1;
            mIndex = 0;
            mStop  = str_.end();
        }
        s_p_s &operator>>(text_tokenizer::token_t &word_);
    } _mCursor;
public:
    text_tokenizer& operator=(string_view str_);
    text_tokenizer& operator=(const char* str_);
    string_view operator()(){ return _mData.data(); }
private:
    config_data mConfig;



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
        result<> Compile();
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
                const char* Begin = nullptr;
                const char* End = nullptr;
                const char* SBegin = nullptr;
                int Index = -1;
            }Loc;

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

            static token_data Scan(const char* Start);

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
                Loc = std::move(r.Loc);
                return *this;
            }
            token_data& operator=(const token_data& r)
            {
                T = r.T;
                M = r.M;
                L = r.L;
                Loc = r.Loc;
                return *this;
            }

            operator bool() { return (L != nullptr) || (T != token_data::type::Unset); }

            std::string Mark(const char*);
            int NParam = 0;

        };

        struct SCRAT_API Attribute
        {
            // ============ Indiquer c'est quoi qu'il faut lire!!!!!! ======================
            color::type Fg  = color::Black;
            color::type Bg  = color::Black;
            Icon::Type Ic   = Icon::NullPtr;
            Accent::Type Ac = Accent::Err;
            // =============================================================================
            const char* Begin = nullptr;
            const char* End   = nullptr;
            const char* C     = nullptr;

            struct Assigned
            {
                int8_t For : 1;
                int8_t Bac : 1;
                int8_t Ic : 1;
                int8_t Acc : 1;
            }Assign = {0};


            Attribute() = default;
            Attribute(token_data& Info);
            Attribute(Attribute&&) noexcept = default;
            Attribute(const Attribute&) = default;
            ~Attribute() = default;
            std::string Infos();
            std::string operator()() const;

            Attribute& operator = (Attribute&&) noexcept = default;
            Attribute& operator = (const Attribute&) = default;
            using list = std::vector<text::Attribute>;
        };

        void PushAttribute(text::Attribute Attr);
    private:
        friend class Compiler;
        friend class Attribute;
        text::Attribute::list Attributes;

        struct Compiler
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

            void CloseToken(token_data& Info);
            result<> Compile();
            result<text::Attribute> CompileAttribute(text::Attribute& Attr);
            result<text::Attribute> CompileAccent(text::Attribute& Attr);
            result<text::token_data> Scan();

            text::token_data SkipToAttr();

            result<color::type> ColorID(token_data& Token);
            result<Icon::Type> IconID(token_data& Token);
            text::token_data ScanIdentifier();
            result<> Eat(text::token_data& Token);
            Compiler() = delete;
            Compiler(text& aTextInstance);
            ~Compiler() = default;
            bool Eof();

            using ParserFnPtr = result<>(text::Compiler::*)(text::Attribute&);
            using ParsersTable = std::unordered_map<token_data::mnemonic, text::Compiler::ParserFnPtr>;

            result<> ParseIcon(text::Attribute& A); // Parser pour ':' + 'IconID' + ';'
            result<> ParseFg(text::Attribute& A); // Parser pour ':' + 'ColorID' + ';'
            result<> ParseBg(text::Attribute& A); // Parser pour ':' + 'ColorID' + ';'
            result<> ParseColor(text::Attribute& A); // Parser pour ':' + 'ColorID' + ',' + 'ColorID' + ';'
            result<> ParseLineBreak(text::Attribute& A); // 'BR'; ( <br; .. > ou <br>)
            result<> CloseAttribute(text::Attribute& A); // 'BR'; ( <br; .. > ou <br>)
            result<> CheckEos(text::Attribute& A);
            std::string Mark();
        };

        static text::token_data Scan(text::token_data::mnemonic M);
    };
}
