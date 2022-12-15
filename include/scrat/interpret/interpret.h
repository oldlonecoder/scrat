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
public:
    /**
     * Default constructor
     */
    interpret();

    /**
     * Copy constructor
     *
     * @param other TODO
     */


    /**
     * Destructor
     */
    ~interpret();

    /**
     * Assignment operator
     *
     * @param other TODO
     * @return TODO
     */
    interpret& operator=(const interpret& other);

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator==(const interpret& other) const;

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator!=(const interpret& other) const;

};

}


