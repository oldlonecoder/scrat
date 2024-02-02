//
// Created by oldlonecoder on 24-01-27.
//

#include "scrat/Compiler/TLexer.h"

namespace scrat
{
Book::Result TLexer::operator()()
{

    Text = mConfig.Source;

    // - Start testing the AppBook::TextCursor:
    // ------------------------------------
    AppBook::Debug() << " Source: " << Text.Text;
    AppBook::Test() << "Invoke TextCursor::operator ++():";
    Text++;
    auto Loc = Text.Sync();
    std::string Str;

    AppBook::Debug() << "Offset: " << Color::Yellow << (Loc >> Str).Offset << Color::Reset << ", " << Color::Yellow << Str;

    NumScanner Num{Text};
    if(Num() != Book::Result::Accepted)
    {
        AppBook::Debug() << " not a number... ";
        //...
    }
    // It is a number;




    return Book::Result::Success;
}

TToken::List &TLexer::Tokens()
{
    if(!mConfig.TokenTable)
        throw AppBook::Exception() [AppBook::Fatal() << "Attempt to get the tokens stream from null Tokens Table."];

    return mConfig.TokenTable->Tokens();
}

Book::Result TLexer::ScanLitteral(TToken &Token)
{
    return Book::Result::Ok;
}

Book::Result TLexer::InputBinary(TToken &Token)
{
    return Book::Result::Ok;
}

Book::Result TLexer::InpputUnary(TToken &Token)
{
    return Book::Result::Ok;
}

Book::Result TLexer::InpputPrefix(TToken &Token)
{
    return Book::Result::Ok;
}

Book::Result TLexer::InpputPostfix(TToken &Token)
{
    return Book::Result::Ok;
}

Book::Result TLexer::InputKeyword(TToken &Token)
{
    return Book::Result::Ok;
}

Book::Result TLexer::InputPunctuation(TToken &Token)
{
    return Book::Result::Ok;
}

Book::Result TLexer::InputOpenPair(TToken &Token)
{
    return Book::Result::Ok;
}

Book::Result TLexer::InputClosingPair(TToken &Token)
{
    return Book::Result::Ok;
}


// 145.6789....
Book::Result TLexer::NumScanner::operator()()
{
    Sign();
    if(Base10() != Book::Result::Accepted)
        if(Base16() != Book::Result::Accepted)
            if(Base8() != Book::Result::Accepted)
                if(Base2() != Book::Result::Accepted)
                    return Book::Result::Rejected;

    // Numeric Base, N and Seq sub_view string are  all set :
    if(Real)
        return Book::Result::Accepted;

    if(N < 0){}

    return Book::Result::Accepted;
}

Book::Result TLexer::NumScanner::Base2()
{

    // 0b11010101010101010101010101010101010
    // 0b1101'0101'0101'0101'0101'0101'0101'0101'0100'1010
    // 0b1101 0101 0101 0101 0101 0101 0101 0101 0100'1010
    // 0b/0B11010101 01010101 01010101 01010101 01001010
    // b/B11010101'01010101'01010101'01010101'01001010
    // 11010101'01010101'01010101'01010101'01001010B
    // 11010101 01010101 01010101 01010101 01001010b

    return Book::Result::Accepted;
}



/*!
 * @brief
 * @return
 *
 * @note {wiki:Octal ) ..."For example, the literal 73 (base 8) might be represented as 073, o73, q73, 0o73, (\73)-removed from my handling, @73, &73, $73 or 73o in various languages."
 *
 */
Book::Result TLexer::NumScanner::Base8()
{
    auto A = Text();

    return Book::Result::Accepted;
}

Book::Result TLexer::NumScanner::Base10()
{
    auto Cur = Text(); // Get the current iterrator value...
    while(!Text.Eof() && std::isdigit(*Cur)) ++Cur;
    if(Text.Eof())
    {
        if(Cur > Text())
        {
            Seq = {Text(), Cur};
            Base = Decimal;
            StrAcc(Seq) >> N;
            return Book::Result::Accepted;
        }
        return Book::Result::Rejected;
    }

    return Book::Result::Accepted;
}

Book::Result TLexer::NumScanner::Base16()
{
    return Book::Result::Accepted;
}

void TLexer::NumScanner::Sign()
{
    if( (*Text() == '-') || (*Text() == '+') )
    {
        Scale |= Type::Sign;
        Text ++;
    }
    Scale &= ~Scale;
}


TLexer::NumScanner::NumScanner(const Book::TextCursor &View)
{
    Text = View;
}


} // scrat