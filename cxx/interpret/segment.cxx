// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <scrat/interpret/segment.h>
#include <fstream>

namespace scrat::script


{
segment::segment()
{

}


segment::segment(const segment& seg_)
{


}


scrat::script::segment::segment(scrat::script::bloc* parent_, const std::string& id_):bloc(parent_, nullptr,nullptr)
{
    unit._id = id_;
    unit.segment_bloc = parent_;

}

segment::segment(bloc* parent_, compiler::unit_data && unit_):bloc(parent_)
{
    unit = std::move(unit_);
}






segment::~segment()
{
    //...
    delete [] _src;
}



alu segment::jsr()
{
    // Does NOTHING excepted init. this bloc class has no instructions / statement scope.
    rem::push_warning() < " segment::jsr() : No bloc/variables initialisation implementation ...yet";

    return *acc;
}

result<> segment::load_source(const std::string& _input_file)
{
    rem::push_debug(source_ffl) < " input :";
    // Update unit_data:
    unit.file = _input_file;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        file.seekg(0, std::ios_base::end);
        auto sz = file.tellg();
        _src = new char[sz];

        file.seekg(0,std::ios_base::beg);
        file.read(_src,sz); // should throw exception...
        _src[sz]=0;
        unit._source_ptr = _src; ///< std::string_view takes the address of the _src contents.
        rem::push_test() < "source read:";
        rem::push_output() < rem::endl < color::White < unit._source_ptr < rem::endl < color::Reset;
    }
    catch (std::system_error& e)
    {
        throw  rem::push_error() << "scrat::script::segment::load_source '" << color::Yellow << _input_file << color::Reset << "' :" << strerror(errno);
    }

    return rem::accepted;
}

result<> segment::cc()
{
    compiler cc;
    auto r = cc.cc(unit);


    return rem::push_warning(source_ffl) < rem::notimplemented;
}

void segment::set_source(std::string_view src_)
{
    unit._source_ptr = src_.data();
}



}

















