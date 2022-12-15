// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause


#pragma once

#include <scrat/interpret/xio.h>




namespace scrat::script {

/**
 * @todo write docs
 */
class SCRAT_API variable : public xio
{

public:
    using list = std::vector<variable*>;
    // will use sub-template when I will write the interpret objects operations... this var will holds the objet  instance...
    /**
     * Default constructor
     */
    variable();

    /**
     * Copy constructor
     *
     * @param other TODO
     */
    variable(const variable& other);
    variable(xio* parent_, token_data* info_, alu* v = nullptr);

    /**
     * Destructor
     */
    ~variable();

};

}


