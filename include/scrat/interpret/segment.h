// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <scrat/interpret/compiler/compiler.h>
#include <map>

namespace scrat {
namespace script {

/**
    @brief A Segment is a "module-bloc" scope of sibbling segments relative to the master interpret bloc. ( like but not fully as PASCAL unit; )


    @note Segment has no execution. it holds scoped objects,variables and functions which are the execution and addressing calls through the segment bloc scope.
    for now, the bloc implementation is used (variables).
    @author &copy; 2002, Serge Lussier <oldlonecoder,bretzel> lussier.serge@gmail.com
 */
class SCRAT_API segment :  public bloc
{

    friend class interpret;
    compiler::unit_data unit;
    char*  _src = nullptr;


public:

    using list = std::map<std::string_view,segment*>;

    segment();
    segment(bloc* parent_, const std::string& id_);
    segment(bloc* parent_, compiler::unit_data&& unit_);
    segment(const script::segment& seg_);

    ~segment() override;

    segment& self() { return *this; }

    void set_source(std::string_view src_);
    /*!
        @brief read the source content from the given filename located in the path also previously given to the interpret. \see interpret::set_location_path(...).
     */
    scrat::result<  > load_source(const std::string& _input_file);


    alu jsr() override; ///< At this level, execution serves to initialize local [static] memory blocs and local [static] variables with const expr arithmetics.

    result<> cc();

};

}
}


