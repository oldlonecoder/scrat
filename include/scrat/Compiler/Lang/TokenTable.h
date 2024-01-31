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
#include "scrat/rt/TToken.h"
#include <AppBook/Util/Object.h>



//#ifndef SCRAT_TOKENTABLE_H
//#define SCRAT_TOKENTABLE_H

namespace scrat::Lang
{

class SCRAT_API TTokenTable : public Util::Object
{
    TToken::List Ref;

public:

    TTokenTable() = default;
    TTokenTable(Util::Object* Par, const std::string& TableName);
    ~TTokenTable() override;

    TToken Scan(const char* C);

    [[maybe_unused]] virtual size_t DeclareTable();
    TToken::List& Tokens() { return Ref; }
};

} // scrat::Lang

//#endif //SCRAT_TOKENTABLE_H
