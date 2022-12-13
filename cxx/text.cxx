
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
mStart(aStr.begin()),
mPos(aStr.begin()),
mStop(aStr.begin())
{

}

/*!
    @note Enleveer le code de trop (suivi du num de ligne et de la col).
*/
bool text_tokenizer::s_p_s::Skip()
{
     if(End())
        return false;

    while(isspace(*mPos))
    {
        switch(*mPos)
        {
            case 10:
            {
                if((++mPos) >= mStop)
                    return false;
                if(*mPos == 13)
                    ++mPos;
                ++mLine;
                mCol = 1;
            }
                break;
            case 13:
            {
                if((++mPos) >= mStop)
                    return false;
                if(*mPos == 10)
                    ++mPos;
                ++mLine;
                mCol = 1;
            }
                break;
            case '\t':++mPos;
                ++mCol;
                break;
            default:++mPos;
                ++mCol;
                break;
        }
    }
    return mPos < mStop;

}


bool text_tokenizer::s_p_s::End() const
{
    return mPos >= mStop;
}


bool text_tokenizer::s_p_s::operator++()
{
    if(mPos >= mStop)
        return false;
    ++mPos;
    ++mCol;
    if(mPos >= mStop)
        return false;
    return Skip();
}
bool text_tokenizer::s_p_s::operator++(int)
{
    if(mPos >= mStop)
        return false;
    ++mPos;
    ++mCol;
    if(mPos >= mStop)
        return false;
    return Skip();
}

text_tokenizer::s_p_s &text_tokenizer::s_p_s::operator>>(text_tokenizer::token_t &aWord)
{
    aWord.mStart    = mPos;
    aWord.mLine     = mLine;
    aWord.mCol      = mCol;
    aWord.mPosition = mIndex = (uint64_t) (mPos - mStart);
    return *this;
}

text_tokenizer::iterator text_tokenizer::scan_to(text_tokenizer::iterator aStart, char c) const
{
    text_tokenizer::iterator p = aStart;
    ++p;
    while((p != _mData.end()) && (*p != c))
        ++p;
    return p;
}


/*!
    * @brief break/split/tokenize,etc... the content of this stracc into pieces.
    * @param text_tokenizer::token_t::List  OUTPUT reference to the 'Tokens array' containter, filled by this method.
    * @param aDelimiters Separators in the form of a stracc of ascii-8 characters.
    * @param keep_as_word if true (or non-zero), the Separators will be put into the collection as they appear
    * @return number of "Words/tokens" contained into the wcollection.
    * @notice : After several years of experience and experimentations, offset have determined that
    * white-spaces/ctrl or spacing characters are silent and implicit delimiters, in addition to the ones supplied by \c aDelimiters.
    */

std::size_t text_tokenizer::operator()(text_tokenizer::token_t::list &Collection, string_view aDelimiters, bool KeepAsWord) const
{

    auto Crs = text_tokenizer::s_p_s(_mData);
    if(_mData.empty())
    {
        std::cout << " --> Contents is Empty!";
        return (std::size_t) 0;
    }
    Crs.Reset(_mData);
    string_view token_separators = aDelimiters.empty() ? text_tokenizer::_default_token_separators : aDelimiters;
    if(!Crs.Skip())
    {
        //std::cout << " --> Contents Skip is false? (internal?)...\n";
        return (std::size_t) 0;
    }
    token_t w;
    Crs >> w;

    while(!Crs.End())
    {
        //if (!wcollection.empty());
        text_tokenizer::iterator cc = Crs.mPos;
        if(token_separators.find(*Crs.mPos) != string_view::npos)
        {
            cc = Crs.mPos;
            if(cc > w.mStart)
            {
                --cc;
                Collection.push_back({w.mStart, cc, Crs.mStop, w.mLine, w.mCol, w.mPosition});
                Crs >> w;
                cc = Crs.mPos;
            }

            // '//' as one token_t instead of having two consecutive '/'
            if((*Crs.mPos == '/') && (*(Crs.mPos + 1) == '/'))
                ++Crs;

            if(KeepAsWord)
            {
                Collection.push_back({w.mStart, Crs.mPos, Crs.mStop, w.mLine, w.mCol, w.mPosition});
            }
            ++Crs;
            //std::cout << "        Iterator eos: " << _Cursor.end() << "\n";
            if(!Crs.End())
                Crs >> w;
            else
            {
                return Collection.size();
            }

        }
        else if((*Crs.mPos == '\'') || (*Crs.mPos == '"'))
        { // Quoted litteral stracc...
            Crs >> w;
            if(KeepAsWord)
            {
                // Create the three parts of the quoted stracc: (") + (litteral) + (") ( or ' )
                // So, we save the token_t coords anyway.
                Collection.push_back({w.mStart, w.mStart, Crs.mStop, w.mLine, w.mCol, w.mPosition});
            }

            text_tokenizer::iterator p = scan_to(w.mStart + (KeepAsWord ? 0 : 1), *Crs.mPos); // w.B is the starting position, _Cursor.m is the quote delim.
            while(Crs.mPos < p)
                ++Crs; // compute white spaces!!!

            if(KeepAsWord)
            {
                // then push the litteral that is inside the quotes.
                Collection.push_back({w.mStart + 1, p - 1, Crs.mStop, w.mLine, w.mCol, w.mPosition});
                //++_Cursor; // _Cursor now on the closing quote
                Crs >> w; // Litteral is done, update w.
                Collection.push_back({w.mStart, p, Crs.mStop, w.mLine, w.mCol, w.mPosition});
            }
            else
            {
                // Push the entire quote delims surrounding the litteral as the token_t.
                Collection.push_back({w.mStart, Crs.mPos, Crs.mStop, w.mLine, w.mCol, w.mPosition});
            }
            if(++Crs)
                Crs >> w;
            else
                return Collection.size();

        }
        else
        {
            cc = Crs.mPos;
            ++cc;
            if(cc == Crs.mStop)
            {
                ++Crs.mPos;
                break;
            }
            if(isspace(*cc))
            {
                if(w.mStart < cc)
                {
                    Collection.push_back({w.mStart, cc - 1, Crs.mStop, w.mLine, w.mCol, w.mPosition});
                    ++Crs;
                }

                if(Crs.Skip())
                {
                    Crs >> w;
                    continue;
                }
                return Collection.size();
            }
            if(!Crs.End())
                ++Crs; // advance offset to the next separator/white space.
        }
    }
    if(Crs.mPos > w.mStart)
        Collection.push_back({w.mStart, Crs.mPos - 1, Crs.mStop, w.mLine, w.mCol, w.mPosition});

    return Collection.size();
}

//text_tokenizer& text_tokenizer::operator>>(text_tokenizer::token_t::List& Collection)
//{
//    mConfig.z = Tokenize(mConfig.Collection, mConfig.Delim, mConfig.Keep);
//    return *this;
//}

text_tokenizer::text_tokenizer(string_view aStr):
_mData(aStr)
{

}
text_tokenizer &text_tokenizer::operator=(string_view aStr)
{
    _mData = aStr;
    return *this;
}
text_tokenizer &text_tokenizer::operator=(const char *aStr)
{
    _mData = aStr;
    return *this;
}
text_tokenizer::text_tokenizer(char *aStr): _mData(aStr)
{

}
text_tokenizer::text_tokenizer(string_view aStr, string_view Delim, bool KeepDelim):
    _mData(aStr)
{
    mConfig.delimiters = Delim;
    mConfig.keep = KeepDelim;
}


text_tokenizer::text_tokenizer(char* aStr, string_view Delim, bool KeepDelim) :
    _mData(aStr)
{
    mConfig.delimiters = Delim;
    mConfig.keep = KeepDelim;
}




text_tokenizer::text_tokenizer(const std::string& aStr, string_view Delim, bool KeepDelim) :
    _mData(aStr)
{
    mConfig.delimiters = Delim;
    mConfig.keep = KeepDelim;
}



text_tokenizer::text_tokenizer(const char* aStr) : _mData(aStr)
{}


text_tokenizer::text_tokenizer(std::string aStr): _mData(aStr.c_str())
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



    text::token_data text::token_data::Scan(const char* Start)
    {
        //Rem::Debug(SourceLocation) << " Entering with Start :[" << *Start << "] -> '" << Start << "':";
        if (!*Start)
            return {};

        for (auto TokenRef : text::token_data::Referential)
        {
            TokenRef.Loc.SBegin = Start;
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
                rem::push_debug(source_fl) < " Scanned to :" < color::Yellow < *crs;
                TokenRef.Loc.Begin = Start;
                TokenRef.Loc.End = crs; // Fin du Token

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

        Loc = r.Loc;
        //Rem::Debug(SourceLocation) << " ->[" << (*this)() << "]";
    }

    std::string text::token_data::Mark(const char* Stream)
    {
        if (!*this)
            return " - Can't mark an invalid token...";
        const char* B = Loc.Begin;
        const char* E = Loc.End;
        while ((B >= Stream) && (*B != '\n')) --B;
        ++B;
        while (*E && (*E != '\n')) ++E;
        --E;

        std::string MStr = std::string(Loc.Begin - Stream, ' ');

        MStr += Icon::CArrowUp;
        stracc Str;
        Str << color::Grey78 << std::string(B, E - B) << '\n' << color::Yellow << MStr << color::Reset;
        return Str();
    }

    result<> text::Compile()
    {
        text::Compiler Parse{ *this };
        auto A = Parse.Compile();

        rem::push_debug(source_fl) < " Number of compiled Attribute(s) :" < Attributes.size();
        for (auto const& A : Attributes)
        {
            rem::push_info() < A();
            rem::push_output() < color::White < "Fg:" < color::Yellow < static_cast<int>(A.Fg) < color::White < ", Bg:" < color::Yellow < static_cast<int>(A.Bg);
            rem::push_output() < color::White < "Icon:" < color::Yellow < static_cast<int>(A.Ic) < color::Reset;
        }
        return rem::accepted;
    }

    result<> text::operator>>(std::string& Out)
    {

        const char* r = _d.c_str();
        auto l = _d.length();
        const char* b = r;

        std::string AttrStr;

        for (auto const& A : Attributes)
        {

            while (r != A.Begin)
            {
                Out += *r;
                r++;
            }
            r += A().length();
            if(A.Assign.For && A.Assign.Bac)
            {
                textattr::pair{A.Fg, A.Bg} >> AttrStr;
                Out += AttrStr;
            }
            else
            {

                if (A.Assign.Bac)
                {
                    AttrStr = _f == textattr::format::ansi256 ? attr<textattr::format::ansi256>::bg(A.Bg) : attr<textattr::format::html>::bg(A.Bg);
                    Out += AttrStr;
                }
                else
                if (A.Assign.For)
                {
                    AttrStr = _f == textattr::format::ansi256 ? attr<textattr::format::ansi256>::bg(A.Fg) : attr<textattr::format::html>::bg(A.Fg);
                    Out += AttrStr;
                }
            }

            if (A.Assign.Acc)
            {
                AttrStr = scrat::Accent::Data[A.Ac];
                Out += AttrStr;
            }
            else
            {
                if (A.Assign.Ic)
                {
                    AttrStr = scrat::Icon::Data[A.Ic];
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

    void text::PushAttribute(text::Attribute Attr)
    {
        Attributes.push_back(Attr);
    }

    text::token_data text::Scan(text::token_data::mnemonic M)
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

        return { Loc.Begin, (std::size_t)(Loc.End - Loc.Begin) + 1 };
    }

    text::Compiler::Compiler(text& aTextInstance) :
        B(aTextInstance._d.c_str()), C(aTextInstance._d.c_str()), E(aTextInstance._d.c_str()), TextRef(aTextInstance)
    {
        E = B + aTextInstance._d.length() - 1;
    }

    bool text::Compiler::operator ++()
    {
        return false;
    }

    bool text::Compiler::operator ++(int)
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



    void text::Compiler::CloseToken(token_data& Info)
    {
        Info.Loc.Index = static_cast<int>(Info.Loc.End - B);
        auto sz = Info.Loc.End - Info.Loc.Begin;
        C += sz + 1;

        rem::push_debug() < " Token :\n" < Info.Mark(B);

    }

    result<text::token_data> text::Compiler::Scan()
    {
        auto Token = text::token_data::Scan(C);
        if (!Token)
        {
            auto R = ScanIdentifier();
            if (!R)
                return rem(rem::syntax) < Token.Mark(TextRef._d.c_str());
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
    result<> text::Compiler::Compile()
    {
        // Build tokens stream:
        while (!Eof())
        {

            auto Token = SkipToAttr();
            text::Attribute Attr;
            result<text::Attribute> A;
            if (!Token) return rem::eof;

            rem::push_debug(source_aaa) < rem::endl < Token.Mark(B);


            if (Token.M == text::token_data::mnemonic::AccentSeq)
            {
                Attr.Begin = Token.Loc.Begin;
                A = CompileAccent(Attr);
            }
            else if (Token.M == text::token_data::mnemonic::OpenTag)
            {
                Attr.Begin = Token.Loc.Begin;
                A = CompileAttribute(Attr);
            }

            if (!A)
            {
                rem::push_debug(source_aaa) < " No attribute. Skipping to next \"OpenTag | AccentSeq\"...";
                //Skip();
                continue;
            }
            TextRef.PushAttribute(Attr);

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
    result<text::Attribute> text::Compiler::CompileAttribute(text::Attribute& Attr)
    {

        text::token_data Token;
        // On tokenize les elements d'attribut: ( Stop: ClosingTag('>') Token )
        std::map<text::token_data::mnemonic, text::Compiler::ParserFnPtr> Parsers =
        {
            {text::token_data::mnemonic::Fg, &text::Compiler::ParseFg},
            {text::token_data::mnemonic::Bg, &text::Compiler::ParseBg},
            {text::token_data::mnemonic::Color, &text::Compiler::ParseColor},
            {text::token_data::mnemonic::Ic, &text::Compiler::ParseIcon},
            //... Plus tard, on aura les Grilles et Fenetres
            {text::token_data::mnemonic::ClosingTag, &text::Compiler::CloseAttribute},
        };
        while (!Eof())
        {
            // on passe '<'
            //Skip();
            result<> ER;
            // Expecting text::token_data::mnemonic:
            Token = text::token_data::Scan(C);
            if (!Token)
                return rem::push_syntax(source_pfnl) < " Expected ACM (Attribute Command mnemonic) Token" < rem::endl < Mark();

            // Ici c'est obligatoire de faire une boucle qui teste explicitement les mnemonics sp&eacute;cifiques
            // parce qu'on ne prend en charge tous les mnemonics.
            for (auto [M, Fn] : Parsers)
            {
                if (M != Token.M) continue;
                Eat(Token);
                ER = (this->*Fn)(Attr);
                if (!ER)
                    return ER();
                break;
            }
            if (Token.M == text::token_data::mnemonic::ClosingTag)
            {
                Eat(Token);
                Attr.End = Token.Loc.End;
                return Attr; // Peut &ecirc;tre vide si on es sur "<>"
            }
        }
        return rem::push_syntax(source_fl) < " Unexpected end of stream in Attribute parsing";
    }



    /*!
        @brief Cr&eacute;&eacute; un Attribute de texte Accent.

        @note Encoder un Accent est le plus facile, donc pas besoin d'accumuler les tokens ici: juste "parser" '&' + Id + ';' c'est tout!
    */
    result<text::Attribute> text::Compiler::CompileAccent(text::Attribute& Attr)
    {

        text::token_data Token;
        //Skip();
        if(Eof())
            return rem(rem::syntax) < " Expected identifier.";

        Token = ScanIdentifier();
        if (!Token)
        {
            rem(rem::syntax) < " Expected identifier." < rem::endl < Mark();
        }

        rem::push_debug(source_aaa) < " Identifier: " < Token() < " :";

        Accent::Type T = Accent::Code(Token());
        if (T == Accent::Err)
            return rem::push_syntax(source_fl) < " Unknown Code token(identifier) " < rem::endl < Token.Mark(B);

        Attr = text::Attribute(Token);
        Attr.Ac = T;
        Attr.Assign.Acc = 1;
        //Mandatory expect ';'
        (void)Eat(Token);
        if(CheckEos(Attr)) return Attr;
        return rem::push_syntax(source_fl) < " Expected Eos ';' (End Of Statement token)." < rem::endl < Mark();
    }

    /*!
        @brief Tokenize jusqu'&agrave; '>' ou ';', puis compiler l'attribut.
    */

    text::token_data text::Compiler::SkipToAttr()
    {
        text::token_data Token;

        while (!Eof())
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
            Token.Loc.Begin = Token.Loc.End = C;
            Token.Loc.Index = static_cast<int>(C - TextRef._d.c_str());
            Eat(Token);
            return Token;
        }
        return {};
    }

    bool text::Compiler::Eof() { return C > E; }

   result<> text::Compiler::ParseIcon(text::Attribute& A)
   {
       auto Token = text::token_data::Scan(C);
       if (Token.T != text::token_data::type::Punctuation)
           return rem::push_syntax(source_fl) < " Expected Punctuation token ':'" < Mark();
       Eat(Token);
       Token = ScanIdentifier();
       rem::push_debug(source_aaa) < " Icon ID : '" < Token() < "' :";
        auto R = IconID(Token);
        if (!R)
        {
            rem::push_syntax(source_fl) < rem::notexist < " Icon ID:\n" < Token.Mark(B);
            return {};
        }
        A.Ic = *R;
        A.Assign.Ic = 1;
        return CheckEos(A);
    }



    result<> text::Compiler::ParseFg(text::Attribute& A)
    {
        // C sur 'Fg'; ( Consomm&eacute; )
        // Attendus :  ':' , 'ColorID', '; | >';

        auto Token = text::token_data::Scan(C);
        if ((Token.T != token_data::type::Punctuation) || (Token.L != text::token_data::ArgSeq))
            return rem::push_syntax(source_fl) < " Expected token ':' " < rem::endl < Mark();

        Eat(Token);
        Token = ScanIdentifier();
        if (!Token)
            return rem::push_syntax(source_fl) < " Expected Identifier token." < rem::endl < Mark();

        auto R = ColorID(Token);
        if (!R) return R() < " in ParseFg";
        A.Fg = *R;
        A.Assign.For = 1;
        rem::push_output() < " Compiler::ParseFg - Token:" < rem::endl < Token.Mark(B);
        return CheckEos(A);
    }

    result<> text::Compiler::ParseBg(text::Attribute& A)
    {
        // C sur 'Fg'; ( Consomm&eacute; )
        // Attendus :  ':' , 'ColorID', '; | >';
        auto Token = text::token_data::Scan(C);
        if ((Token.T != token_data::type::Punctuation) || (Token.L != text::token_data::ArgSeq))
            return rem::push_syntax(source_fl) < " Expected token ':'\n" < Mark();

        Eat(Token);

        Token = ScanIdentifier();
        if (!Token)
            return rem::push_syntax(source_fl) < " Expected Identifier token." < rem::endl < Mark();

        auto R = ColorID(Token);
        if (!R) return R() < " in ParseFg";
        A.Bg = *R;
        A.Assign.Bac = 1;
        rem::push_output() < " Compiler::ParseFg - Token:" < rem::endl < Token.Mark(B);

        return CheckEos(A);
    }

   std::string text::Compiler::Mark()
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






    result<> text::Compiler::ParseColor(text::Attribute& A)
    {

        auto Token = text::token_data::Scan(C);

        if (Token.M != text::token_data::mnemonic::ArgSeq)
            return rem::push_syntax(source_fl) < rem::unexpected < rem::endl < Token.Mark(B);

        Eat(Token);

        Token = ScanIdentifier();
        if(!Token)
            return rem::push_syntax(source_fl) < " Expected Identifier token." < rem::endl < Mark();

        auto R = ColorID(Token);
        if (!R) return R();

        A.Fg = *R;
        A.Assign.For = 1;
        // Ici on doit verfifer si on a une virgule ou eos ou closing tag;
        // result "," | ';' | '>'.

        Token = text::token_data::Scan(C);
        if ((Token.M != text::token_data::mnemonic::ArgSep) && (Token.M != text::token_data::mnemonic::Eos) && (Token.M != text::token_data::mnemonic::ClosingTag))
            return rem::push_syntax(source_fl) < "Expected ',' (arg separator) or eos (';') or closing tag ('>') " < rem::endl < Token.Mark(B);

        if ((Token.M == text::token_data::mnemonic::Eos) || (Token.M == text::token_data::mnemonic::ClosingTag))
        {
            if (A.Fg == color::Reset)
            {
                A.Bg = A.Fg;
                if(Token.M == text::token_data::mnemonic::Eos)
                    Eat(Token);

                return rem::accepted;
            }

        }
        // Ici on a obligatoirement argsep:

        Eat(Token);

        Token = ScanIdentifier();
        //...
        R = ColorID(Token);
        if (!R) return R();
        A.Bg = *R;
        A.Assign.Bac = 1;
        return CheckEos(A);
    }



    result<> text::Compiler::ParseLineBreak(text::Attribute& A)
    {

        return rem::notimplemented;
    }

    result<> text::Compiler::CloseAttribute(text::Attribute& A)
    {
        //TextRef.PushAttribute(A);
        return rem::accepted;
    }

    result<> text::Compiler::CheckEos(text::Attribute& A)
    {
        //Skip();
        auto Token = text::token_data::Scan(C);
        if ( (!Token) || ((Token.M != text::token_data::mnemonic::Eos) && (Token.M != text::token_data::mnemonic::ClosingTag)))
            return rem::push_syntax(source_fl) < " Expected ';'" < rem::endl < Token.Mark(B);

        if (Token.M == text::token_data::mnemonic::ClosingTag)
        {

            return rem::accepted;
        }
        Eat(Token);
        A.End = Token.Loc.End;
        return rem::accepted;
    }


    text::token_data text::Compiler::ScanIdentifier()
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
        Token.Loc.Begin = C;
        Token.Loc.End = Sc;
        Token.L = C;
        Token.T = token_data::type::Identifier;
        return Token;
    }

    result<> text::Compiler::Eat(text::token_data& Token)
    {
        C = Token.Loc.End;
        C++;
        return rem::accepted;
    }


    result<color::type> text::Compiler::ColorID(token_data& Token)
    {
        auto Str = Token();
        color::type Colr = textattr::scan(Str);
        if (Colr == color::Reset)
        {
            if (Str != "Reset")
                return rem::push_error() < " Expected color::type name (strict case match). Got '" < color::Yellow < Str < color::White < "' instead:" < rem::endl
                < Token.Mark(B);
        }
        Eat(Token);
        return Colr;
    }



    result<Icon::Type> text::Compiler::IconID(token_data& Token)
    {
        auto Str = Token();
        Icon::Type IconId = Icon::Scan(Str);
        if (IconId == Icon::NullPtr)
            return rem::push_error() < " Expected Icon::type name, got '" < color::Yellow < Str < color::White < "' instead:" < rem::endl < Token.Mark(B);
        Eat(Token);
        return IconId;
    }


    text::Attribute::Attribute(token_data& aInfo) : Begin(aInfo.Loc.Begin-1), End(aInfo.Loc.End), C(nullptr) {}

    std::string text::Attribute::operator()() const
    {
        if (End)
        {
            std::string Str{ Begin, size_t(End - Begin) + 1 };
            return Str;
        }
        else
        {
            std::string Str{ Begin };
            return Str;
        }
    }

    std::string text::Attribute::Infos()
    {
        return "implement";
    }



}
