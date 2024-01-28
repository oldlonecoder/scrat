//
// Created by oldlonecoder on 24-01-27.
//

#include "scrat/Compiler/TLexer.h"

namespace scrat
{
Book::Result TLexer::operator()(TLexer::TConfig Data)
{
    Source = Data; // Should discard Source.Source.
    Text = Source.Source;

    // - Start testing the AppBook::TextCursor:
    // ------------------------------------

    auto c = Text();


    return Book::Result::Ok;
}

TToken::List &TLexer::Tokens()
{
    if(!Source.TokenTable)
        throw AppBook::Exception() [AppBook::Fatal() << "Attempt to get the tokens stream from null Tokens Table."];

    return Source.TokenTable->Tokens();
}
} // scrat