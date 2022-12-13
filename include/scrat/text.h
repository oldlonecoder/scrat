#pragma once


#include <scrat/textattr>
#include <scrat/stracc>
#include <scrat/result>

namespace scrat
{
    //    "─", //    "━", //    "│", //    "┃", //    "┌", //    "┍", //    "┎", //    "┏", //    "┐", //    "┑", //    "┒", //    "┓", //    "└", //    "┕", //    "┖", //    "┗", //    "┘", //    "┙", //    "┚", //    "┛", //    "├", //    "┝", //    "┞", //    "┟", //    "┠", //    "┡", //    "┢", //    "┣", //    "┤", //    "┥", //    "┦", //    "┧", //    "┨", //    "┩", //    "┪", //    "┫", //    "┬", //    "┭", //    "┮", //    "┯", //    "┰", //    "┱", //    "┲", //    "┳", //    "┴", //    "┵", //    "┶", //    "┷", //    "┸", //    "┹", //    "┺", //    "┻", //    "┼", //    "┽", //    "┾", //    "┿", //    "╀", //    "╁", //    "╂", //    "╃", //    "╄", //    "╅", //    "╆", //    "╇", //    "╈", //    "╉", //    "╊", //    "╋", //    "═", //    "║", //    "╒", //    "╓", //    "╔", //    "╕", //    "╖", //    "╗", //    "╘", //    "╙", //    "╚", //    "╛", //    "╜", //    "╝", //    "╞", //    "╟", //    "╠", //    "╡", //    "╢", //    "╣", //    "╤", //    "╥", //    "╦", //    "╧", //    "╨", //    "╩", //    "╪", //    "╫", //    "╬", //    "╭", //    "╮", //    "╯", //    "╰", //    "╴", //    "╵", //    "╶", //    "╷", //    "╸", //    "╹", //    "╺", //    "╻", //    "╼", //    "╽", //    "╾", //    "╿",

    /*!
        * @todo write docs
        *
        <p>
            Test Sample:
        </p>
        @code Expect<> Test::TestText()
        {
            Text txt;
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
        [[maybe_unused]] [[nodiscard]] string mark() const;

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
    //text_tokenizer& operator>>(text_tokenizer::Token::List& Collection);

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

};
}
