// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <scrat/interpret/interpret.h>


namespace scrat::script
{

interpret* interpret::_inst = nullptr;

segment::list interpret::units;


interpret::interpret():segment(this, "main")
{
    interpret::_inst = this;
}


interpret::~interpret()
{
    interpret::_inst = nullptr;
}

/*!
    @brief init the scrat interpreter (singleton) instance.

    @note For now it only creates the instance. Later, it will takes cmdargs to process cmd line arguments. ...Or not.

    @author &copy; 2022, Serge Lussier < oldlonecoder, lussier.serge@gmail.com >
*/
result<> interpret::init()
{
    interpret::_inst = new interpret();
    interpret::units["main"] = interpret::_inst;
    return rem::ok;
}


void interpret::set_location(const std::string& path_)
{
    if(!interpret::_inst)
        throw rem::push_error() < rem::null_ptr < " no 'scrat interpreter' instance. Use interpret::init()!";

    interpret::_inst->_location_path = path_;
    //@Todo : Check for existing path.
    //...
}



std::string interpret::location_path()
{
    return interpret::_inst->_location_path;
}

segment * interpret::segment_bloc(std::string_view seg_name_)
{
    auto s = interpret::units.find(seg_name_);
    if(s == interpret::units.end())
        return nullptr;
    return s->second;
}

result<> interpret::compile_file(const std::string& src_file)
{
    try
    {
        std::string file = interpret::location_path() + '/' + src_file;
        load_source(file);

        //...
    }
    catch(rem& r)
    {
        //rethrow...
        throw rem::push_error() < " from interpret::compile_file.";
    }

    return rem::notimplemented; ///< False success by returning expected type.
}





result<variable *> interpret::query_var(const std::string& segment_id_, const std::string& var_id_)
{
    auto s = interpret::query_segment(segment_id_);
    if(!s)
        return s();

    return s->query_local_var(var_id_);
}

}


scrat::result<scrat::script::segment *> scrat::script::interpret::query_segment(const std::string& seg_id_)
{
    auto s = interpret::units.find(seg_id_);
    if(s == interpret::units.end())
        return rem::push_error() < rem::null_ptr < " querying segment bloc id'" < color::Yellow < seg_id_ < color::Reset < '\'';

    return s->second;
}











