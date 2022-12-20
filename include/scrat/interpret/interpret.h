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
class SCRAT_API interpret : segment
{
    std::string _location_path = "./";
    static interpret* _inst;

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


