//Copyright(C) 2011; 2022  oldlonecoder / lussier.serge@gmail.com
//
//This program is free software; you can redistribute itand /or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.


#pragma once
#include <scrat/dlconfig.h>
#include <cstdint>
#include <vector>


namespace scrat::io
{
struct SCRAT_API ifd final
{

    using list = std::vector<ifd>;


    int _fd = -1;
    struct opt
    {
        uint8_t read  : 1; ///< ready to read;
        uint8_t write : 1; ///< ready to write
        uint8_t del   : 1; ///< to be deleted/closed/destroyed;
        uint8_t active: 1; ///< active or not.
        uint8_t autoread : 1; ///< Immediate read (eg.: keyboard input), then signal after; 0: signal then let the owner read.
    }bits;

    uint32_t seglength = 0; ///< segment length to be read;
    std::vector<uint8_t> buffer; ///< limited indexable bytes buffer ( such as keyboard input sequence and  application network control protocole sequence )
    ifd(int fd_=-1);
    ~ifd();

    uint32_t to_read();


};

}