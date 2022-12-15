// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <scrat/interpret/variable.h>

namespace scrat::script {

/**
 * @todo write docs
 */


class SCRAT_API bloc : public xio
{

public:
    using list = std::vector<bloc*>;
protected:

    xio::list * _instructions = nullptr;
    variable::list* _variables  = nullptr; ///< Represent the stack bloc.
    // bloc::list* _functions = nullptr; ///< ...
    // bloc::list* _structs = nullptr;

public:
    /**
     * Default constructor
     */
    bloc();
    bloc(bloc* parent_, token_data* info_ = nullptr, alu* a_ = nullptr);
    bloc(xio* parent_, token_data* info_ = nullptr, alu* a_ = nullptr);

    /**
     * Copy constructor
     *
     * @param other TODO
     */
    bloc(const bloc& other);

    /**
     * Destructor
     */
    ~bloc();

    /**
     * Assignment operator
     *
     * @param other TODO
     * @return TODO
     */
    bloc& operator=(const bloc& other);

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator!=(const bloc& other) const;

    alu jsr() override;
    result<variable*> query_var(const std::string& id_);
    result<variable*> query_local_var(const std::string& id_);
    result<variable*> new_var(xio* var_); ///< ?? When parsing (compiling) the xio node is created (...or not?)
    result<variable*> new_var(token_data* info_); ///< ?? When parsing (compiling) the xio node is created (...or not?)


};

}



