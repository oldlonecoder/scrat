// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <scrat/interpret/variable.h>

namespace scrat::script {



class SCRAT_API rbase : public xio
{

    std::string _id;

public:
    using dictionary_t = std::map<std::string, rbase*>;
    rbase() : xio(){}
    rbase(xio* a_parent, const std::string& a_id) : xio(a_parent, 0, 0), _id(a_id) {}
    ~rbase() override;

    std::string& name() { return _id; }
    virtual alu operator()(const alu::list& args) = 0;

};

template<typename class_, typename ret_, typename ...params_> class rtf : public rbase
{
    class_*  rt_obj = nullptr;
public:
    using rtf_ptr_t = ret_(class_::*)(params_...);
private:
    typename rtf::rtf_ptr_t rtf_ptr = nullptr;
    rtf() = default;
    rtf(xio* a_parent, const std::string& a_name, class_& rt_inst, rtf_ptr_t a_fn) : rbase(a_parent, a_name), rt_obj(&rt_inst), rtf_ptr(a_fn) {}
    ~rtf();
    /*!
        @brief rt calls interpreter script source function.
    */
    ret_ operator()(const params_& ...args) {
        auto param = [](auto a) {
//                logdebugpfn << " a= '" << logger::Yellow << a << ":\n";
            return alu(a);
        };
        alu::list params = { param(args)... };
        // alu a = call_script_function(_name, params);
        // return a.value<R>();
        return ret_();
    }

    template <std::size_t ... Is> alu accumulate(alu::list const& params, std::index_sequence<Is...> const&){
        return (rt_obj->*rtf_ptr)(params[Is].value<params_>()...);
    }

    alu operator()(const alu::list& params) override{
        //          logdebug << " xio_stack::rt_function(const alu::list_t&):  " << Ends;
        for (auto a : params) {
           rem::push_debug() < "arg: [" < color::Yellow < a() < color::Reset < "]";
           //...
        }
        return accumulate(params, std::index_sequence_for<params_...>{});
    }

    template <std::size_t ... Is> alu accumulate(alu::list& params, std::index_sequence<Is...> const&){
        return (rt_obj->*rtf_ptr)(params[Is].value<params_>()...);
    }

};



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



