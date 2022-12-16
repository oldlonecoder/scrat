// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <scrat/interpret/bloc.h>
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
    std::string _id;
    char*  _src;
    std::string_view _source;
public:

    using list = std::map<std::string_view,segment*>;

    /**
     * Default constructor
     */
    segment();

    segment(bloc* parent_, const std::string& id_);

    /**
     * Copy constructor
     *
     * @param other TODO
     */
    segment(const script::segment& seg_);

    /**
     * Destructor
     */
    ~segment() override;

    /*!
        @brief Derefence directly from the pointer;
     */
    segment& self() { return *this; }

    void set_source(std::string_view src_);

    result<> input_file(std::ifstream& _input_file);

    alu jsr() override;

    /**
     * Assignment operator
     *
     * @param other TODO
     * @return TODO
     */
    script::segment& operator=(const script::segment& seg_);

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator==(const segment& other) const;

    /**
     * @todo write docs
     *
     * @param other TODO
     * @return TODO
     */
    bool operator!=(const segment& other) const;

};

}
}


