
#include <scrat/text.h>
#include <map>



namespace scrat
{
[[maybe_unused]] string_view text_tokenizer::_default_token_separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";


std::string text_tokenizer::token_t::operator()() const
{
    std::string Str;
    if(mStart == mEnd)
        Str.insert(Str.begin(), mStart, mEnd + 1);
    else
        Str.insert(Str.begin(), mStart, mEnd + 1);

    return Str;
}

std::string text_tokenizer::token_t::operator*() const
{
    std::string Str;
    if(mStart == mEnd)
        Str.insert(Str.begin(), mStart, mEnd + 1);
    else
        Str.insert(Str.begin(), mStart, mEnd + 1);

    return Str;

}

std::string text_tokenizer::token_t::Mark() const
{
    std::string Str;
    string_view::iterator CStart = mStart - mPosition;

    //int                   l  = 1;
    string_view::iterator cc = CStart;
    // localiser le debut de la ligne;
    while(*cc && (cc > CStart) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if(cc >= CStart)
    {
        if((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while((cc != mSE) && (*cc != '\n') && (*cc != '\r'))
            Str += *cc++;
    }

    Str += '\n'; ///@todo Encode new line.
    for(int x = 1; x < mCol; x++)
        Str += ' ';
    Str += '^';
    return Str;
}

std::string text_tokenizer::token_t::location()
{
    std::ostringstream Str;
    Str << '(' << mLine << ',' << mCol << ')';
    return Str.str();
}


// ------------------------ text_tokenizer::s_p_s --------------------------------


text_tokenizer::s_p_s::s_p_s(std::string_view aStr):
    _begin(aStr.begin()),
    _pos(aStr.begin()),
    _end(aStr.begin())
{

}

/*!
    @note Enleveer le code de trop (suivi du num de ligne et de la col).
*/
bool text_tokenizer::s_p_s::skip_ws()
{
     if(end())
        return false;

    while(isspace(*_pos))
    {
        switch(*_pos)
        {
            case 10:
            {
                if((++_pos) >= _end)
                    return false;
                if(*_pos == 13)
                    ++_pos;
                ++_line;
                _col = 1;
            }
                break;
            case 13:
            {
                if((++_pos) >= _end)
                    return false;
                if(*_pos == 10)
                    ++_pos;
                ++_line;
                _col = 1;
            }
                break;
            case '\t':++_pos;
                ++_col;
                break;
            default:++_pos;
                ++_col;
                break;
        }
    }
    return _pos < _end;

}


bool text_tokenizer::s_p_s::end() const
{
    return _pos >= _end;
}


bool text_tokenizer::s_p_s::operator++()
{
    if(_pos >= _end)
        return false;
    ++_pos;
    ++_col;
    if(_pos >= _end)
        return false;
    return skip_ws();
}
bool text_tokenizer::s_p_s::operator++(int)
{
    if(_pos >= _end)
        return false;
    ++_pos;
    ++_col;
    if(_pos >= _end)
        return false;
    return skip_ws();
}

text_tokenizer::s_p_s &text_tokenizer::s_p_s::operator>>(text_tokenizer::token_t &aWord)
{
    aWord.mStart    = _pos;
    aWord.mLine     = _line;
    aWord.mCol      = _col;
    aWord.mPosition = _index = (uint64_t) (_pos - _begin);
    return *this;
}

text_tokenizer::iterator text_tokenizer::scan_to(text_tokenizer::iterator aStart, char c) const
{
    text_tokenizer::iterator p = aStart;
    ++p;
    while((p != _d.end()) && (*p != c))
        ++p;
    return p;
}


/*!
    * @brief break/split/tokenize,etc... the content of a string into pieces.
    * @param text_tokenizer::token_t::list  OUTPUT reference to the 'Tokens array' containter, filled by this method.
    * @param aDelimiters Separators in the form of a std::sting_view of ascii-8 characters.
    * @param keep_as_word if true (or non-zero), the Separators will be put into the collection as they appear
    * @return number of "Words/tokens" contained into the collection.
    * @notice : After several years of experience and experimentations, We have determined that
    * white-spaces/ctrl or spacing characters are silent and implicit delimiters, in addition to the ones supplied by \c aDelimiters.
    */

std::size_t text_tokenizer::operator()(text_tokenizer::token_t::list &Collection, string_view aDelimiters, bool KeepAsWord) const
{

    auto Crs = text_tokenizer::s_p_s(_d);
    if(_d.empty())
    {
        std::cout << " --> Contents is Empty!";
        return (std::size_t) 0;
    }
    Crs.reset(_d);
    string_view token_separators = aDelimiters.empty() ? text_tokenizer::_default_token_separators : aDelimiters;
    if(!Crs.skip_ws())
    {
        //std::cout << " --> Contents Skip is false? (internal?)...\n";
        return (std::size_t) 0;
    }
    token_t w;
    Crs >> w;

    while(!Crs.end())
    {
        //if (!wcollection.empty());
        text_tokenizer::iterator cc = Crs._pos;
        if(token_separators.find(*Crs._pos) != string_view::npos)
        {
            cc = Crs._pos;
            if(cc > w.mStart)
            {
                --cc;
                Collection.push_back({w.mStart, cc, Crs._end, w.mLine, w.mCol, w.mPosition});
                Crs >> w;
                cc = Crs._pos;
            }

            // '//' as one token_t instead of having two consecutive '/'
            if((*Crs._pos == '/') && (*(Crs._pos + 1) == '/'))
                ++Crs;

            if(KeepAsWord)
            {
                Collection.push_back({w.mStart, Crs._pos, Crs._end, w.mLine, w.mCol, w.mPosition});
            }
            ++Crs;
            //std::cout << "        Iterator eos: " << _Cursor.end() << "\n";
            if(!Crs.end())
                Crs >> w;
            else
            {
                return Collection.size();
            }

        }
        else if((*Crs._pos == '\'') || (*Crs._pos == '"'))
        { // Quoted litteral string...
            Crs >> w;
            if(KeepAsWord)
            {
                // Create the three parts of the quoted string: (") + (litteral) + (") ( or ' )
                // So, we save the token_t coords anyway.
                Collection.push_back({w.mStart, w.mStart, Crs._end, w.mLine, w.mCol, w.mPosition});
            }

            text_tokenizer::iterator p = scan_to(w.mStart + (KeepAsWord ? 0 : 1), *Crs._pos); // w.B is the starting position, _Cursor.m is the quote delim.
            while(Crs._pos < p)
                ++Crs; // compute white spaces!!!

            if(KeepAsWord)
            {
                // then push the litteral that is inside the quotes.
                Collection.push_back({w.mStart + 1, p - 1, Crs._end, w.mLine, w.mCol, w.mPosition});
                //++_Cursor; // _Cursor now on the closing quote
                Crs >> w; // Litteral is done, update w.
                Collection.push_back({w.mStart, p, Crs._end, w.mLine, w.mCol, w.mPosition});
            }
            else
            {
                // Push the entire quote delims surrounding the litteral as the token_t.
                Collection.push_back({w.mStart, Crs._pos, Crs._end, w.mLine, w.mCol, w.mPosition});
            }
            if(++Crs)
                Crs >> w;
            else
                return Collection.size();

        }
        else
        {
            cc = Crs._pos;
            ++cc;
            if(cc == Crs._end)
            {
                ++Crs._pos;
                break;
            }
            if(isspace(*cc))
            {
                if(w.mStart < cc)
                {
                    Collection.push_back({w.mStart, cc - 1, Crs._end, w.mLine, w.mCol, w.mPosition});
                    ++Crs;
                }

                if(Crs.skip_ws())
                {
                    Crs >> w;
                    continue;
                }
                return Collection.size();
            }
            if(!Crs.end())
                ++Crs; // advance offset to the next separator/white space.
        }
    }
    if(Crs._pos > w.mStart)
        Collection.push_back({w.mStart, Crs._pos - 1, Crs._end, w.mLine, w.mCol, w.mPosition});

    return Collection.size();
}

//text_tokenizer& text_tokenizer::operator>>(text_tokenizer::token_t::List& Collection)
//{
//    mConfig.z = Tokenize(mConfig.Collection, mConfig.Delim, mConfig.Keep);
//    return *this;
//}

text_tokenizer::text_tokenizer(string_view aStr):
    _d(aStr)
{

}
text_tokenizer &text_tokenizer::operator=(string_view aStr)
{
    _d = aStr;
    return *this;
}
text_tokenizer &text_tokenizer::operator=(const char *aStr)
{
    _d = aStr;
    return *this;
}
text_tokenizer::text_tokenizer(char *aStr): _d(aStr)
{

}
text_tokenizer::text_tokenizer(string_view aStr, string_view Delim, bool KeepDelim):
    _d(aStr)
{
    _cfg.delimiters = Delim;
    _cfg.keep = KeepDelim;
}


text_tokenizer::text_tokenizer(char* aStr, string_view Delim, bool KeepDelim) :
    _d(aStr)
{
    _cfg.delimiters = Delim;
    _cfg.keep = KeepDelim;
}




text_tokenizer::text_tokenizer(const std::string& aStr, string_view Delim, bool KeepDelim) :
    _d(aStr)
{
    _cfg.delimiters = Delim;
    _cfg.keep = KeepDelim;
}



text_tokenizer::text_tokenizer(const char* aStr) : _d(aStr)
{}


text_tokenizer::text_tokenizer(std::string aStr): _d(aStr.c_str())
{

}






//--------------------------------------------------------------------------------------------------------------------------------------------


    // Referentiel des tokens du processeur :
    text::token_data::list text::token_data::Referential =
    {
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::OpenTag,     text::token_data::OpenSeq, 0},
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::ClosingTag,  text::token_data::CloseSeq,0},
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::ArgSeq,      text::token_data::ArgSeq,0},
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::ArgSep,      text::token_data::ArgSep,0},
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::Eos,         text::token_data::EoSt,0},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Color,       text::token_data::ColorPair,2},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Fg,          text::token_data::Fg,1},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Bg,          text::token_data::Bg,1},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Ic,          text::token_data::Icon,1},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Br,          text::token_data::LineBreak,0},
        {text::token_data::type::Accent,         text::token_data::mnemonic::AccentSeq,   text::token_data::Accent,1}
        //...
    };

    // Parsers:




    text::text()
    {

    }

    text::text(const text& other)
    {
        _d = other._d;
    }

    text::text(const std::string& Str) :_d(Str) {}

    text::text(std::string&& Str) noexcept : _d(std::move(Str)) {}

    text::text(const char* aStr) : _d(aStr) {}

    text::~text()
    {

    }



    text::token_data text::token_data::scan(const char* Start)
    {
        //Rem::Debug(SourceLocation) << " Entering with Start :[" << *Start << "] -> '" << Start << "':";
        if (!*Start)
            return {};

        for (auto TokenRef : text::token_data::Referential)
        {
            TokenRef._location.s_begin = Start;
            while (isspace(*Start)) ++Start;

            auto crs = Start;
            const char* rtxt = TokenRef.L;
            auto sz = std::strlen(rtxt);

            if (toupper(*crs) != toupper(*rtxt)) continue;

            while (*rtxt && *crs && (toupper(*crs) == toupper(*rtxt))) { ++crs; ++rtxt; }
            rem::push_debug(source_pfnl) < "Token.L = [" < color::Yellow < TokenRef.L < color::Reset < "]:";
            if (!*rtxt)
            {   // fin de Token.L :
                if (TokenRef.T == token_data::type::AttrCmd)
                {
                    // Si le mot continu alors ce n'est pas un Token:
                    if (*crs)
                    {   // *crs n'est pas un type d'espace:
                        if (isalnum(*crs))
                            continue;
                    }
                } // Il reste les autres non-espace comme les ponctuations, symboles...
                --crs; // Replacer crs sur le dernier caractere du Token.
                rem::push_debug(source_fl) < " Scanned to :'" < color::Yellow < *crs < color::Reset < '\'';
                TokenRef._location.begin = Start;
                TokenRef._location.end = crs; // Fin du Token

                return TokenRef; // On le retourne.... doh!
            }
        }
        rem::push_debug(source_pfnl) < " '" < *Start < "' not a TokenRef";
        return {}; // crs ( ou Start )  n'est pas sur un token du referentiel.
    }


    text::token_data::token_data(text::token_data::type aType, text::token_data::mnemonic aMnenomic, text::token_data::Lexem aLexem, int nParams) :
        T(aType), M(aMnenomic), L(aLexem),NParam(nParams)
    {

    }

    text::token_data::token_data(text::token_data&& r) noexcept :
        T(r.T), M(r.M), L(r.L)
    {

        _location = r._location;
        //Rem::Debug(SourceLocation) << " ->[" << (*this)() << "]";
    }

    std::string text::token_data::mark(const char* Stream)
    {
        if (!*this)
            return " - Can't mark an invalid token...";
        const char* B = _location.begin;
        const char* E = _location.end;
        while ((B >= Stream) && (*B != '\n')) --B;
        ++B;
        while (*E && (*E != '\n')) ++E;
        --E;

        std::string MStr = std::string(_location.begin - Stream, ' ');

        MStr += Icon::CArrowUp;
        stracc Str;
        Str << color::Grey78 << std::string(B, E - B) << '\n' << color::Yellow << MStr << color::Reset;
        return Str();
    }

    result<> text::compile()
    {
        text::compiler Parse{ *this };
        auto A = Parse.execute();

        rem::push_debug(source_fl) < " Number of compiled Attribute(s) :" < _attributes.size();
        for (auto const& A : _attributes)
        {
            rem::push_info() < A();
            rem::push_output() < color::White < "Fg:" < color::Yellow < static_cast<int>(A._fg) < color::White < ", Bg:" < color::Yellow < static_cast<int>(A._bg);
            rem::push_output() < color::White < "Icon:" < color::Yellow < static_cast<int>(A._icn) < color::Reset;
        }
        return rem::accepted;
    }

    result<> text::operator>>(std::string& Out)
    {

        const char* r = _d.c_str();
        auto l = _d.length();
        const char* b = r;

        std::string AttrStr;

        for (auto const& A : _attributes)
        {

            while (r != A._begin)
            {
                Out += *r;
                r++;
            }
            r += A().length();
            if(A._assign._for && A._assign._bac)
            {
                textattr::pair{A._fg, A._bg} >> AttrStr;
                Out += AttrStr;
            }
            else
            {

                if (A._assign._bac)
                {
                    AttrStr = _f == textattr::format::ansi256 ? attr<textattr::format::ansi256>::bg(A._bg) : attr<textattr::format::html>::bg(A._bg);
                    Out += AttrStr;
                }
                else
                if (A._assign._for)
                {
                    AttrStr = _f == textattr::format::ansi256 ? attr<textattr::format::ansi256>::bg(A._fg) : attr<textattr::format::html>::bg(A._fg);
                    Out += AttrStr;
                }
            }

            if (A._assign._acc)
            {
                AttrStr = scrat::Accent::Data[A._accnt];
                Out += AttrStr;
            }
            else
            {
                if (A._assign._icn)
                {
                    AttrStr = scrat::Icon::Data[A._icn];
                    Out += AttrStr;
                }
                //...
            }
        }
        while (static_cast<size_t>(r - b) < l)
        {
            Out += *r++;
        }
        return rem::ok;
    }

    void text::push_attribute(text::attribute Attr)
    {
        _attributes.push_back(Attr);
    }

    text::token_data text::scan(text::token_data::mnemonic M)
    {
        for (auto iM : text::token_data::Referential)
        {
            if (iM.M == M) return iM;
        }
        return {};
    }


    //     text::Attribute::Attribute(const char* Start)
    //     {
    //         Begin = C = End = Start;
    //     }

    std::string text::token_data::operator()()
    {
        if (L == nullptr)
            return { "null" };

        return { _location.begin, (std::size_t)(_location.end - _location.begin) + 1 };
    }

    text::compiler::compiler(text& aTextInstance) :
        B(aTextInstance._d.c_str()), C(aTextInstance._d.c_str()), E(aTextInstance._d.c_str()), TextRef(aTextInstance)
    {
        E = B + aTextInstance._d.length() - 1;
    }

    bool text::compiler::operator ++()
    {
        return false;
    }

    bool text::compiler::operator ++(int)
    {
        return false;
    }

    //result<> text::Compiler::Skip()
    //{
    //    //Rem::Debug() << " text::TextParser::Skip(" << color::Yellow << *C << color::Reset << "):";
    //    while (C <= E)
    //    {
    //        ++C;
    //        if (!isspace(*C))
    //            return Rem::Code::Accepted;
    //    }
    //    return Rem::Code::Eof;
    //}



    void text::compiler::close_token(token_data& Info)
    {
        Info._location.index = static_cast<int>(Info._location.end - B);
        auto sz = Info._location.end - Info._location.begin;
        C += sz + 1;

        rem::push_debug() < " Token :\n" < Info.mark(B);

    }

    result<text::token_data> text::compiler::scan()
    {
        auto Token = text::token_data::scan(C);
        if (!Token)
        {
            auto R = scan_identifier();
            if (!R)
                return rem(rem::syntax) < Token.mark(TextRef._d.c_str());
        }
        return Token;
    }


    /*!
        @brief Compile le texte pour creer les attributs :

        @code
            Compile :
            loop:
                SkipToAttr
                CompileAttribute
            loop
        @endcode
    */
    result<> text::compiler::execute()
    {
        // Build tokens stream:
        while (!eof())
        {

            auto Token = skip_to_attribute();
            text::attribute Attr;
            result<text::attribute> A;
            if (!Token) return rem::eof;

            rem::push_debug(source_aaa) < rem::endl < Token.mark(B);


            if (Token.M == text::token_data::mnemonic::AccentSeq)
            {
                Attr._begin = Token._location.begin;
                A = compile_accent(Attr);
            }
            else if (Token.M == text::token_data::mnemonic::OpenTag)
            {
                Attr._begin = Token._location.begin;
                A = compile_attribute(Attr);
            }

            if (!A)
            {
                rem::push_debug(source_aaa) < " No attribute. Skipping to next \"OpenTag | AccentSeq\"...";
                //Skip();
                continue;
            }
            TextRef.push_attribute(Attr);

        }
        return rem::accepted;
    }

    /*!
        @brief Compiler une sequence d'attribut(s).

        Ici c'est moins &eacute;vident... On d&eacute;coupe ce qui est entre '<' et '>' en commandes d'attribut.
        //...
        // '<' 'Fg' ':' 'ColorID' ';'...'Color' : 'ColorID(Fg)' ',' 'ColorID(Bg)' ';' 'Reset' ';' '>'

        @note  "Lexing And Parsing... tout &ccedile;a en m&ecirc;me temps!".
    */
    result<text::attribute> text::compiler::compile_attribute(text::attribute& Attr)
    {

        text::token_data Token;
        // On tokenize les elements d'attribut: ( Stop: ClosingTag('>') Token )
        std::map<text::token_data::mnemonic, text::compiler::parser_fnptr_t> Parsers =
        {
            {text::token_data::mnemonic::Fg, &text::compiler::parse_fg},
            {text::token_data::mnemonic::Bg, &text::compiler::ParseBg},
            {text::token_data::mnemonic::Color, &text::compiler::parse_color},
            {text::token_data::mnemonic::Ic, &text::compiler::parse_icon},
            //... Plus tard, on aura les Grilles et Fenetres
            {text::token_data::mnemonic::ClosingTag, &text::compiler::close_attribute},
        };
        while (!eof())
        {
            // on passe '<'
            //Skip();
            result<> ER;
            // Expecting text::token_data::mnemonic:
            Token = text::token_data::scan(C);
            if (!Token)
                return rem::push_syntax(source_pfnl) < " Expected ACM (Attribute Command mnemonic) Token" < rem::endl < mark();

            // Ici c'est obligatoire de faire une boucle qui teste explicitement les mnemonics sp&eacute;cifiques
            // parce qu'on ne prend en charge tous les mnemonics.
            for (auto [M, Fn] : Parsers)
            {
                if (M != Token.M) continue;
                eat_token(Token);
                ER = (this->*Fn)(Attr);
                if (!ER)
                    return ER();
                break;
            }
            if (Token.M == text::token_data::mnemonic::ClosingTag)
            {
                eat_token(Token);
                Attr._end = Token._location.end;
                return Attr; // Peut &ecirc;tre vide si on es sur "<>"
            }
        }
        return rem::push_syntax(source_fl) < " Unexpected end of stream in Attribute parsing";
    }



    /*!
        @brief Cr&eacute;&eacute; un Attribute de texte Accent.

        @note Encoder un Accent est le plus facile, donc pas besoin d'accumuler les tokens ici: juste "parser" '&' + Id + ';' c'est tout!
    */
    result<text::attribute> text::compiler::compile_accent(text::attribute& Attr)
    {

        text::token_data Token;
        //Skip();
        if(eof())
            return rem(rem::syntax) < " Expected identifier.";

        Token = scan_identifier();
        if (!Token)
        {
            rem(rem::syntax) < " Expected identifier." < rem::endl < mark();
        }

        rem::push_debug(source_aaa) < " Identifier: " < Token() < " :";

        Accent::Type T = Accent::Code(Token());
        if (T == Accent::Err)
            return rem::push_syntax(source_fl) < " Unknown Code token(identifier) " < rem::endl < Token.mark(B);

        Attr = text::attribute(Token);
        Attr._accnt = T;
        Attr._assign._acc = 1;
        //Mandatory expect ';'
        (void)eat_token(Token);
        if(check_eos(Attr)) return Attr;
        return rem::push_syntax(source_fl) < " Expected Eos ';' (End Of Statement token)." < rem::endl < mark();
    }

    /*!
        @brief Tokenize jusqu'&agrave; '>' ou ';', puis compiler l'attribut.
    */

    text::token_data text::compiler::skip_to_attribute()
    {
        text::token_data Token;

        while (!eof())
        {
            auto c = toupper(*C);
            if (c != token_data::OpenSeq[0])
            {
                if (c != token_data::Accent[0])
                {
                    ++C;
                    continue;
                }
                Token.T = text::token_data::type::Accent;
                Token.L = text::token_data::Accent;
                Token.M = text::token_data::mnemonic::AccentSeq;
            }
            else
            {
                Token.T = text::token_data::type::AttrCmd;
                Token.L = text::token_data::OpenSeq;
                Token.M = text::token_data::mnemonic::OpenTag;
            }
            Token._location.begin = Token._location.end = C;
            Token._location.index = static_cast<int>(C - TextRef._d.c_str());
            eat_token(Token);
            return Token;
        }
        return {};
    }

    bool text::compiler::eof() { return C > E; }

   result<> text::compiler::parse_icon(text::attribute& A)
   {
       auto Token = text::token_data::scan(C);
       if (Token.T != text::token_data::type::Punctuation)
           return rem::push_syntax(source_fl) < " Expected Punctuation token ':'" < mark();
       eat_token(Token);
       Token = scan_identifier();
       rem::push_debug(source_aaa) < " Icon ID : '" < Token() < "' :";
        auto R = icon_id(Token);
        if (!R)
        {
            rem::push_syntax(source_fl) < rem::notexist < " Icon ID:\n" < Token.mark(B);
            return {};
        }
        A._icn = *R;
        A._assign._icn = 1;
        return check_eos(A);
    }



    result<> text::compiler::parse_fg(text::attribute& A)
    {
        // C sur 'Fg'; ( Consomm&eacute; )
        // Attendus :  ':' , 'ColorID', '; | >';

        auto Token = text::token_data::scan(C);
        if ((Token.T != token_data::type::Punctuation) || (Token.L != text::token_data::ArgSeq))
            return rem::push_syntax(source_fl) < " Expected token ':' " < rem::endl < mark();

        eat_token(Token);
        Token = scan_identifier();
        if (!Token)
            return rem::push_syntax(source_fl) < " Expected Identifier token." < rem::endl < mark();

        auto R = color_id(Token);
        if (!R) return R() < " in ParseFg";
        A._fg = *R;
        A._assign._for = 1;
        rem::push_output() < " Compiler::ParseFg - Token:" < rem::endl < Token.mark(B);
        return check_eos(A);
    }

    result<> text::compiler::ParseBg(text::attribute& A)
    {
        // C sur 'Fg'; ( Consomm&eacute; )
        // Attendus :  ':' , 'ColorID', '; | >';
        auto Token = text::token_data::scan(C);
        if ((Token.T != token_data::type::Punctuation) || (Token.L != text::token_data::ArgSeq))
            return rem::push_syntax(source_fl) < " Expected token ':'\n" < mark();

        eat_token(Token);

        Token = scan_identifier();
        if (!Token)
            return rem::push_syntax(source_fl) < " Expected Identifier token." < rem::endl < mark();

        auto R = color_id(Token);
        if (!R) return R() < " in ParseFg";
        A._bg = *R;
        A._assign._bac = 1;
        rem::push_output() < " Compiler::ParseFg - Token:" < rem::endl < Token.mark(B);

        return check_eos(A);
    }

   std::string text::compiler::mark()
   {
       const char* Left = C;
       const char* Right = C;
       while ((Left >= B) && (*Left != '\n')) --Left;
       ++Left;
       while (Right && (*Right != '\n')) ++Right;
       --Right;

       std::string MStr = std::string(Right - Left, ' ');

       MStr += Icon::CArrowUp;
       stracc Str;
       Str << color::Grey78 << std::string(Left, Right - Left) << '\n' << color::Yellow << MStr << color::Reset;
       return Str();
   }






    result<> text::compiler::parse_color(text::attribute& A)
    {

        auto Token = text::token_data::scan(C);

        if (Token.M != text::token_data::mnemonic::ArgSeq)
            return rem::push_syntax(source_fl) < rem::unexpected < rem::endl < Token.mark(B);

        eat_token(Token);

        Token = scan_identifier();
        if(!Token)
            return rem::push_syntax(source_fl) < " Expected Identifier token." < rem::endl < mark();

        auto R = color_id(Token);
        if (!R) return R();

        A._fg = *R;
        A._assign._for = 1;
        // Ici on doit verfifer si on a une virgule ou eos ou closing tag;
        // result "," | ';' | '>'.

        Token = text::token_data::scan(C);
        if ((Token.M != text::token_data::mnemonic::ArgSep) && (Token.M != text::token_data::mnemonic::Eos) && (Token.M != text::token_data::mnemonic::ClosingTag))
            return rem::push_syntax(source_fl) < "Expected ',' (arg separator) or eos (';') or closing tag ('>') " < rem::endl < Token.mark(B);

        if ((Token.M == text::token_data::mnemonic::Eos) || (Token.M == text::token_data::mnemonic::ClosingTag))
        {
            if (A._fg == color::Reset)
            {
                A._bg = A._fg;
                if(Token.M == text::token_data::mnemonic::Eos)
                    eat_token(Token);

                return rem::accepted;
            }

        }
        // Ici on a obligatoirement argsep:

        eat_token(Token);

        Token = scan_identifier();
        //...
        R = color_id(Token);
        if (!R) return R();
        A._bg = *R;
        A._assign._bac = 1;
        return check_eos(A);
    }



    result<> text::compiler::parse_br(text::attribute& A)
    {

        return rem::notimplemented;
    }

    result<> text::compiler::close_attribute(text::attribute& A)
    {
        //TextRef.PushAttribute(A);
        return rem::accepted;
    }

    result<> text::compiler::check_eos(text::attribute& A)
    {
        //Skip();
        auto Token = text::token_data::scan(C);
        if ( (!Token) || ((Token.M != text::token_data::mnemonic::Eos) && (Token.M != text::token_data::mnemonic::ClosingTag)))
            return rem::push_syntax(source_fl) < " Expected ';'" < rem::endl < Token.mark(B);

        if (Token.M == text::token_data::mnemonic::ClosingTag)
        {

            return rem::accepted;
        }
        eat_token(Token);
        A._end = Token._location.end;
        return rem::accepted;
    }


    text::token_data text::compiler::scan_identifier()
    {
        const char* Sc = C;
        while (isspace(*Sc)) ++Sc;
        C = Sc;
        if (!isalpha(*Sc) && (*Sc != '_'))
        {
            rem::push_syntax(source_fl) < rem::expected < " Identifier, got " < *Sc < " instead.";
            return {};
        }

        text::token_data Token;
        while (*Sc && (isalnum(*Sc) || (*Sc == '_'))) ++Sc;
        --Sc;
        Token._location.begin = C;
        Token._location.end = Sc;
        Token.L = C;
        Token.T = token_data::type::Identifier;
        return Token;
    }

    result<> text::compiler::eat_token(text::token_data& Token)
    {
        C = Token._location.end;
        C++;
        return rem::accepted;
    }


    result<color::type> text::compiler::color_id(token_data& Token)
    {
        auto Str = Token();
        color::type Colr = textattr::scan(Str);
        if (Colr == color::Reset)
        {
            if (Str != "Reset")
                return rem::push_error() < " Expected color::type name (strict case match). Got '" < color::Yellow < Str < color::White < "' instead:" < rem::endl
                < Token.mark(B);
        }
        eat_token(Token);
        return Colr;
    }



    result<Icon::Type> text::compiler::icon_id(token_data& Token)
    {
        auto Str = Token();
        Icon::Type IconId = Icon::Scan(Str);
        if (IconId == Icon::NullPtr)
            return rem::push_error() < " Expected Icon::type name, got '" < color::Yellow < Str < color::White < "' instead:" < rem::endl < Token.mark(B);
        eat_token(Token);
        return IconId;
    }


    text::attribute::attribute(token_data& aInfo) : _begin(aInfo._location.begin-1), _end(aInfo._location.end), _crs(nullptr) {}

    std::string text::attribute::operator()() const
    {
        if (_end)
        {
            std::string Str{ _begin, size_t(_end - _begin) + 1 };
            return Str;
        }
        else
        {
            std::string Str{ _begin };
            return Str;
        }
    }

    std::string text::attribute::informations()
    {
        return "implement";
    }



}
