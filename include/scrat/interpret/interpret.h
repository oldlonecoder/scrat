// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <scrat/cmdargs.h>
#include <scrat/interpret/segment.h>

namespace scrat::script {


class SCRAT_API rtf_base :public bloc
{

    std::string _id;

public:
    using dictionary_t = std::map<std::string, rtf_base*>;
    rtf_base() : bloc(){}
    rtf_base(xio* a_parent, const std::string& a_id) : bloc(a_parent, 0, 0), _id(a_id) {}
    ~rtf_base() override;

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
    rtf(xio* a_parent, const std::string& a_name, class_& rt_inst, rtfptr_t a_fn) : rtf_base(a_parent, a_name), rt_obj(&rt_inst), rtf_ptr(a_fn) {}
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
           rem::push_debug() < "arg: [" < color::Yellow << a() << color::Reset << "]";
           //...
        }
        return accumulate(params, std::index_sequence_for<params_...>{});
    }

    template <std::size_t ... Is> alu accumulate(alu::list& params, std::index_sequence<Is...> const&){
        return (rt_obj->*rtf_ptr)(params[Is].value<params_>()...);
    }

};




/**
    @brief the scrat interpreter.

    @note I know it is bery weird name...( let's it alone for now)
 */
class SCRAT_API interpret : segment
{
    std::string _location_path = "./";
    static interpret* _inst;
    rtf_base::dictionary_t rtf;
    static segment::list units;


    interpret();


    /*!
        @brief Select segment bloc instance identified by seg_name_

        This private instance is for quick internal call that don't need the result to be managed.
        @param std::string_view seg_name_
        @return pointer to the instance of the segment bloc, or nullptr if no such segment bloc in the units container map.
        @author &copy;2022, Serge Lussier <oldlonecoder, lussier.serge@gmail.com>
    */
    segment* segment_bloc(std::string_view seg_name_);

public:


    static result<> init();
    ~interpret() override;

    static void set_location(const std::string& path_);
    std::string location_path();

    result<> compile_file(const std::string& src_file);


    /*!
        @brief select the given variable name in the given segment bloc
     */
    static result<variable*> query_var(const std::string& segment_id_, const std::string& var_id_);

    /*!
        @brief Select the named segment segment_instance



        @param id of the segment
        @author &copy; 2022, Serge Lussier <oldlonecoder, lussier.serge@gmail.com>
    */
    static result<segment*> query_segment(const std::string& seg_id_);
#pragma region not_implemented
    // --------- Not implemented yet : -------------------------------
    // static result<bloc*> query_function(const std::string& segment_id_, const std::string& fn_id_);
    // static result<bloc*> query_object(const std::string& segment_id_, const std::string& obj_id_);
    // static result<bloc*> query_class(const std::string& segment_id_, const std::string& cls_id_);
#pragma endregion not_implemented

};

}


