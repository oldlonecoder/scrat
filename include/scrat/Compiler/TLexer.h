//
// Created by oldlonecoder on 24-01-27.
//

/******************************************************************************************
 *   Copyright (C) /.../2024 by Serge Lussier                                             *
 *   lussier.serge@gmail.com                                                              *
 *                                                                                        *
 *   Unless otherwise specified, all code in this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply to the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 *****************************************************************************************/

#pragma once


//#ifndef SCRAT_TLEXER_H
//#define SCRAT_TLEXER_H
#include "scrat/Compiler/Lang/TokenTable.h"
#include <AppBook/Util/TextCursor.h>


#include <map>

namespace scrat
{

class SCRAT_API TLexer
{

    Book::TextCursor  Text{};
public:

    struct SCRAT_API TConfig
    {
        std::string_view Source;
        Lang::TTokenTable* TokenTable{nullptr};
        explicit operator bool() const { return !Source.empty() && TokenTable; };
    };



    struct NumScanner
    {

        std::string_view Seq{};
        Book::TextCursor Text{};
        float N{.0f};
        scrat::Type::T Scale { scrat::Type::Null };


        enum NumBase
        {
            Unset=0, Binary=1, Octal=2, Decimal=3, Hexadecimal=4,
        }Base{Unset};

        bool Real{false};

        NumScanner() = default;
        NumScanner(const Book::TextCursor& View);

        ~NumScanner() = default;
        Book::Result operator()();


        Book::Result Base2();
        Book::Result Base8();
        Book::Result Base10();
        Book::Result Base16();

        void Sign();
    };
    TLexer() =default;
    ~TLexer() = default;

    [[maybe_unused]] [[nodiscard]] TToken::List& Tokens();

    Book::Result operator()();
    TLexer::TConfig& Config() { return mConfig; }

private:


    Book::Result  ScanLitteral(TToken& Token); // Number string; comments
    Book::Result  InputBinary(TToken& Token);
    Book::Result  InpputUnary(TToken& Token);
    Book::Result  InpputPrefix(TToken& Token);
    Book::Result  InpputPostfix(TToken& Token);
    Book::Result  InputKeyword(TToken& Token);
    Book::Result  InputPunctuation(TToken& Token);
    Book::Result  InputOpenPair(TToken& Token);
    Book::Result  InputClosingPair(TToken& Token);





    TLexer::TConfig mConfig;

};

} // scrat

//#endif //SCRAT_TLEXER_H
