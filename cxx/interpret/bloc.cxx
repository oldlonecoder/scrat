// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <scrat/interpret/bloc.h>

namespace scrat::script
{

bloc::bloc()
{

}

bloc::bloc(const bloc& other)
{

}

bloc::bloc(bloc* parent_, token_data* info_, alu* a_): xio(parent_,info_,a_)
{
}

bloc::bloc(xio* parent_, token_data* info_, alu* a_) :xio(parent_,info_,a_)
{
}




bloc::~bloc()
{
    if(!mem.xshared)
    {
        if(_instructions)
            for(auto* i: *_instructions) delete i;
        // if(_functions)
        //     for(auto* f : *_functions) delete f;
        // if(_structs)
        //     for(auto s :*_structs) delete s;
    }
    if(_variables)
    {
        for(auto v : *_variables) delete v;
    }

    //...
}



/*!
    @brief deep copy the rhs bloc for duplicating its vars and sub-bloc stack contents. At this level,  it is implicitely preparing generation of struct and function calls

    @note - Call this strickly for generate copy of stack such as function calls and other instaciation of bloc-based variables
*/

bloc& bloc::operator=(const bloc& rhs)
{
    mem.xshared = 1;
    if(rhs._variables)
    {
        _variables = new variable::list;
        *_variables = *rhs._variables;
    }
    _instructions = rhs._instructions;
    ///@todo Copy also the local function declarations // Needs precision on what I think
    ///@todo Copy also the local struct declarations   // Needs precision on what I think
    //...
    return *this;
}


bool bloc::operator!=(const bloc& other) const
{
    return true;
}


/*!
    @brief Creates a new variable inside this bloc scope.

    @return variable pointer or a rem::error/warning if the variable already exists in this bloc or in the parent scopes...
    @author @copy; 2022, Serge Lussier <oldlonecoder; bretzel> lussier.serge@gmail.com

    @note ( dev note: Make declaration rule syntax mandatory for creating any var, anywhere, thus prevent using new vars into middle of expression )
 */
result<variable *> bloc::new_var(token_data* info_)
{
    auto v = query_var(info_->text());
    if(!v || (!*v))
    {
        if(!_variables) _variables = new variable::list;
        _variables->push_back(new variable(this, info_));
        return _variables->back();
    }
    return rem::push_error() < " variable '" < color::Yellow < info_->text() < color::Reset < rem::exist;

}

/*!
    @brief Creates a new variable inside this bloc scope.

    @return variable pointer or a rem::error/warning if the variable already exists in this bloc or in the parent scopes...
    @author @copy; 2022, Serge Lussier <oldlonecoder; bretzel> lussier.serge@gmail.com

    @note ( dev note: Make declaration rule syntax mandatory for creating any var, anywhere, thus prevent using new vars into middle of expression )
 */
result<variable *> bloc::new_var(xio* var_)
{
    return rem::push_message(source_ffl) < " Not there yet...Will be implemented."; ///< Not there yet; It depends on the arithmetics expression tree building...
}

result<variable *> bloc::query_local_var(const std::string& id_)
{
    if(!_variables) return nullptr;
    for(auto * v : *_variables)  if(v) return v;
    return nullptr;
}

result<variable *> bloc::query_var(const std::string& id_)
{
    auto *p = parent<bloc>();
    auto e = query_local_var(id_);
    if((!e) || (!*e))
    {
        if(p) return p->query_var(id_);
    }

    return e;
}

alu bloc::jsr()
{
    if(!_instructions) return *acc;
    for(auto * i : *_instructions)
    {
        *acc = i->jsr();
        if(i->mem.leave)
            return *acc;
    }
    return *acc; ///< abritrary; if ever something as a bloc returns a value, or the callee needs this value;
}




}











