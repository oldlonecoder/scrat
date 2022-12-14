#include <scrat/lexer/lexer_color.h>
#include <scrat/lexer/lexer.h>


namespace scrat::script
{

    //Normal Types:
    std::map<script::type::T, color::type> lexer_color::Types =
    {
        {type::number_t, color::Chartreuse6},
        {type::keyword_t, color::Pink3},
        {type::operator_t, color::OrangeRed1},
        {type::binary_t, color::CadetBlue2},
        {type::text_t, color::DarkSlateGray3},
        {type::id_t, color::Aquamarine3},
        {type::punc_t, color::Yellow},

        //...
    };


    ////Affined particular Lexem:
    //std::map<mnemonic, color::Type> LexerColor::Mnemonics =
    //{

    //};


lexer_color::lexer_color()
{
}


lexer_color::~lexer_color()
{
    //...
    _product_data.clear();
    tokens.clear();
    //...
}

result<> lexer_color::operator<<(const std::string& aSource)
{
    lexer lex;
    _product_data = aSource;
    std::string _color;
    size_t Spacing = 0, // Current color String::Length();
        Offset = 0; // Cummulative ( Offset += Spacing )


    lex.config() = { _product_data.c_str(),  & tokens};
    rem::code E = lex();
    if (E != rem::accepted) return E;
    

    //...
    for (auto const& Token : tokens)
    {
        _color.clear();
        for (auto [T, C] : lexer_color::Types)
        {
            if (T & Token.s)
            {
                _color = attr<textattr::format::ansi256>::fg(C);
                Spacing = _color.length();
                break;
            }
        }
        if (!_color.empty())
        {
            _product_data.insert(Token.mLoc.offset + Offset, _color);
            Offset += Spacing;
        }
    }
    return rem::ok;
}




}
