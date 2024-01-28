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

#include "scrat/Compiler/Lang/TokenTable.h"

//#ifndef SCRAT_TESTS_H
//#define SCRAT_TESTS_H






namespace test
{


class MyConfig : public scrat::Lang::TTokenTable
{
public:
    size_t DeclareTable() override;


};


class Tests
{

};

} // test

//#endif //SCRAT_TESTS_H
