// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <scrat/cmdargs.h>
#include <scrat/interpret/segment.h>

namespace scrat::script {

/**
    @brief the scrat interpreter.

    @note I know it is bery weird name...( let's it alone for now)
 */
class SCRAT_API interpret
{
    std::string _location_path = "./";
    static interpret* _inst;

    static segment::list units;


    interpret();
public:

    static result<> init();
    ~interpret();

    static void set_location_path(const std::string& path_);
    static std::string_view location_path();


};

}


