// SPDX-FileCopyrightText: 2022 Serge Lussier <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <scrat/interpret/interpret.h>


namespace scrat::script
{

interpret* interpret::_inst = nullptr;

segment::list interpret::units;


interpret::interpret()
{
    interpret::_inst = this;
}


interpret::~interpret()
{
    interpret::_inst = nullptr;
}

/*!
    @brief init the scrat interpreter (singleton) instance.

    @note For now it only creates the instance. Later, it will takes arc, argv arguments and process it.

    @author &copy; 2022, Serge Lussier < oldlonecoder, lussier.serge@gmail.com >
*/
result<> interpret::init()
{
    interpret::_inst = new interpret();
    return rem::ok;
}

void interpret::set_location_path(const std::string& path_)
{
    if(!interpret::_inst)
        throw rem::push_error() < rem::null_ptr < " no 'scrat interpreter' instance. Use interpret::init()!";

    interpret::_inst->_location_path = path_;
    //@Todo : Check for existing path.
    //...
}


}

