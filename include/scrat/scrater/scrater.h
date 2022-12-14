// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <scrat/cmdargs.h>

namespace scrat {

/**
    @brief the scrat interpreter.

    @note I know it is bery weird name...( let's it alone for now)
 */
class SCRAT_API scrature
{
public:
    /**
     * Default constructor
     */
    scrature();

    /**
     * Copy constructor
     *
     * @param other TODO
     */


    /**
     * Destructor
     */
    ~scrature();

    /**
     * Assignment operator
     *
     * @param other TODO
     * @return TODO
     */
    scrature& operator=(const scrature& other);

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator==(const scrature& other) const;

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator!=(const scrature& other) const;

};

}


