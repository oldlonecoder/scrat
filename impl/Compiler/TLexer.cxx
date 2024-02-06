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
    if(Base2() != Book::Result::Accepted)
        if(Base16() != Book::Result::Accepted)
            if(Base8() != Book::Result::Accepted)
                if(Base10() != Book::Result::Accepted)
                    return Book::Result::Rejected;

    // Numeric Base, N and Seq sub_view string are  all set :
    return Book::Result::Accepted;
}

Book::Result TLexer::NumScanner::Base2()
{

    // 0b11010101010101010101010101010101010                --> Parsed
    // 0b1101'0101'0101'''''0101''''0101'0101'''0101'0101'0100'1010  --> Parsed
    // 0b1101 0101 0101 0101 0101 0101 0101 0101 0100'1010  --> Parsed
    // 0b/0B11010101 01010101 01010101 01010101 01001010    --> Parsed
    // 11010101'01010101'01010101'01010101'01001010B        --> Not Parsed Yet
    // 11010101 01010101 01010101 01010101 01001010b        --> Not Parsed Yet


    auto A = Text();
    StrAcc Buf;
    Base = Binary;
    if(std::toupper(*A) == 'B')
        ++A;
    else
    {
        if(*A == '0')
        {
            if(std::toupper(*(A+1)) == 'B')
                A += 2;
        }
    }

    while(!Text.Eof() && std::isdigit(*A))
    {
        if(*A == '`'){ ++A; continue; }
        if(*A>='2') return Book::Result::Rejected;

        Buf << *A++;

    }
    if(A==Text()) return Book::Result::Rejected;
    Seq = {Text(), --A};
    Buf >> N;
    UpdateScale();
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
    StrAcc Buf;
    Base = Octal;

    std::string_view Prefixes = "oOq@$&";

    if(*A == '0') ++A;
    if(auto pos = Prefixes.find(*A) != std::string_view::npos ) ++A;
    else Buf <<*A;


    while(!Text.Eof() && std::isdigit(*A) && *A <= '7')
    {
        Buf << *A++;
    }
    if(*A >= '7') return Book::Result::Rejected;

    Buf >>N;
    UpdateScale();

    return Book::Result::Accepted;
}



/*!
 * @brief Process Base10 litteral numeric.
 * @return Accepted: view sequence accepted and converted to base 10 numeric litteral seq.
 *         Rejected if not.
 */
Book::Result TLexer::NumScanner::Base10()
{
    auto Cur = Text(); // Get the current iterrator value...
    Base = Decimal;
    if(!std::isdigit(*Cur) ) return Book::Result::Rejected;

    StrAcc Buf;
    while(!Text.Eof() && std::isdigit(*Cur))
    {
        if(*Cur == '`') { ++Cur;continue; }
        if((*Cur == '.') || (*Cur == ','))
        {
            if(Real) break;
            Real = true;
            continue;
        }
        Scale = Type::I8;
        //Base = *Cur <= '7' ? Octal : Decimal;
        Buf << *Cur ++;
    }
    if(Cur == Text.B) return Book::Result::Rejected;
    Seq = {Text(),Cur-1};
    Buf >> N;
    if(Real)
    {
        Scale = Type::Float | (( N <= 2147483648.0f) ? Type::Float | Type::F32 : Type::F64);

        return Book::Result::Accepted;
    }

    UpdateScale();
    return Book::Result::Accepted;
}

Book::Result TLexer::NumScanner::Base16()
{
    auto A = Text();
    StrAcc Buf;
    Base = Binary;
    if(std::toupper(*A) == 'X')
        ++A;
    else
    {
        if(*A == '0')
        {
            if(std::toupper(*(A+1)) == 'X')
                A += 2;
        }
    }

    while(!Text.Eof() && std::isxdigit(*A))
    {
        Buf << *A++;
        if(*A == '`'){ ++A; continue; }
    }
    if(A==Text()) return Book::Result::Rejected;
    Seq = {Text(), --A};
    Buf >> N;
    UpdateScale();
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

void TLexer::NumScanner::UpdateScale()
{
    if(Real) return;

    Scale |= (N <= 127) ? Type::I8 :
            (N <= 255) ? Type::U8 :
            (N <= 32767) ? Type::I16 :
            (N <= 65535) ? Type::U16 :
            (N <= 2147483648) ? Type::I32 :
            (N <= 1.84467440737e+19) ? Type::I64 : Type::U64;

}


} // scrat