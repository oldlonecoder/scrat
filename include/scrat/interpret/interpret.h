// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <scrat/cmdargs.h>
#include <scrat/interpret/bloc.h>

namespace scrat::script {

/**
    @brief the scrat interpreter.

    @note I know it is bery weird name...( let's it alone for now)
 */
class SCRAT_API interpret : public bloc
{
    std::string _location_path = "./";
    static interpret* _inst;
    interpret();
public:

    static result<> init();
    ~interpret() override;
    interpret& operator=(const interpret& other);
    bool operator==(const interpret& other) const;
    bool operator!=(const interpret& other) const;

    void set_location_path(const std::string& path_);



};

}


