// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <scrat/interpret/variable.h>

namespace scrat::script
{

variable::variable() : xio()
{

}

variable::variable(xio* parent_, token_data* info_, alu* v): xio(parent_, info_, v)
{
}


variable::variable(const variable& other): xio()
{

}

variable::~variable()
{

}


}


