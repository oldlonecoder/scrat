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
        explicit operator bool() const { return !Text.Empty() && TokenTable; };
    };

    TLexer::TConfig Source;

    struct NumScanner
    {
        using CPos = std::string_view::iterator;
        enum NumBase
        {
            Unset=0, Bina=1, Octo=2, Deci=3, Hexa=4,
        }Base{Unset};

        bool Real{false};
        CPos Begin, Cur, End;

    };
    TLexer() =default;
    ~TLexer() = default;

    [[maybe_unused]] [[nodiscard]] TToken::List& Tokens();

    Book::Result operator()(TLexer::TConfig Data);

};

} // scrat

//#endif //SCRAT_TLEXER_H
