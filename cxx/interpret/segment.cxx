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


scrat::script::segment::segment(scrat::script::bloc* parent_, const std::string& id_):bloc(parent_, nullptr,nullptr),
_id (id_)
{
}
segment::~segment()
{
    //...
    delete [] _src;
}



alu segment::jsr()
{
    // Does NOTHING excepted init. this bloc class has no instructions / statement scope.
    rem::push_warning(source_pffl) < " segment class instances has no blocscope execution!";

    return *acc;
}

result<> segment::input_file(std::ifstream& _input_file)
{
    rem::push_debug(source_ffl) < " input :";

    _input_file.seekg(0, std::ios_base::end);
    auto sz = _input_file.tellg();
    _src = new char[sz];

    _input_file.seekg(0,std::ios_base::beg);
    _input_file.read(_src,sz); // should throw exception...
    _src[sz]=0;
    _source = _src;
    rem::push_test(source_ffl) < "source read: '" < color::White < _src < color::Reset < '\'';
    return rem::accepted;
}

result<> segment::cc()
{
    return rem::push_warning(source_ffl) < rem::notimplemented;
}

void segment::set_source(std::string_view src_)
{
    _source = src_;
}



}

















