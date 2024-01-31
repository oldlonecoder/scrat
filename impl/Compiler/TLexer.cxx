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
    return Book::Result::Success;
}

TToken::List &TLexer::Tokens()
{
    if(!mConfig.TokenTable)
        throw AppBook::Exception() [AppBook::Fatal() << "Attempt to get the tokens stream from null Tokens Table."];

    return mConfig.TokenTable->Tokens();
}

Book::Result TLexer::NumScanner::operator()(TLexer::NumScanner::CPos Start)
{
    CPos A = Start;
    Loop:
    while(std::isdigit(*A)) ++A;
    if(*A == '.')
    {
        if (Real)
        {
            Seq = {Start,A};
            goto SeqEnd;
        }
        Real = true;
        goto Loop;
    }
SeqEnd:

    if(Real)
    {

    }




    return Book::Result::Ok;
}
} // scrat