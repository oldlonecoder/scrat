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
#include <AppBook/Util/Delegate.h>
#include <AppBook/Book/Tests/Test.h>


//#ifndef SCRAT_TESTS_H
//#define SCRAT_TESTS_H
namespace test
{

class MyConfig : public scrat::Lang::TTokenTable
{
public:
    MyConfig(Util::Object* ParenObj, const std::string& ID);
    ~MyConfig() override = default;

    size_t DeclareTable() override;


};

}

//class Test : public Book::Test
//{
//    //...
//    MyConfig Config;
//
//public:
//    //...
//};


//#endif //SCRAT_TESTS_H
