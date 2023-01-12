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
#include <scrat/scrat_signals.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include <unistd.h>


namespace scrat::io
{
struct SCRAT_API ifd final
{

    using list = std::vector<ifd>;
    using iterator = ifd::list::iterator;

    scrat::signal<ifd&>
        read_signal,
        write_signal,
        idle_signal,
        zero_signal,
        window_complete_signal;

    // Option flags - yes static constexpr:
    static constexpr uint32_t O_READ  = 0x01; ///< readeable
    static constexpr uint32_t O_WRITE = 0x02; ///< writeable
    static constexpr uint32_t P_RDWR  = 0x03; ///< same as (I_READ + I_WRITE);
    static constexpr uint32_t O_BLOCK = 0x04; ///< fills internal or external buffer bytes before signaling dataready to delegates ( I_BUF option automaticaly set )
    static constexpr uint32_t O_BUF   = 0x08; ///< Uses internal buffer : size determined with provided buffer size req.
    static constexpr uint32_t O_XBUF  = 0x10; ///< sets internal buffer ptr to external address - do not auto-delete
    static constexpr uint32_t O_IMM   = 0x20; ///< Notify to delegates immediately when a read is made.
    static constexpr uint32_t O_WINDOWED = 0x40; ///< Wait/Window size to be received/sent/written (from internal automatic buffer/ or external temp file) enabled. ifd::signal_t emitted only when window filled/flushed @note anything past m_wsize is discarded/ignored
    static constexpr uint32_t I_AUTOFILL = 0x80; ///< Auto-fill internal/or external buffer before sending read or write signal. So the triggered read and write are done after the data bloc is read or written.
    int fd = -1;
    uint32_t options;   ///< see option flags
    std::size_t pksize;    ///< current packet size toread.
    std::size_t wsize;     ///< Wait/Windodw size
    std::size_t wpos;      ///< Wait/Window index where m_wpos >= 0 < m_wsize. | m_wpos==m_wsize-1 => Wait/Window receive/send/write complete.
    u_int8_t* internal_buffer;
    struct state_flags
    {
        uint8_t active  :1;    ///< This descriptor is active or not
        uint8_t destroy :1;    ///< this descriptor is marked to be deleted
        uint8_t writeable:1;   ///< this descriptor's fd is ready for write ( socketfd write ready event from epoll_wait )
        uint8_t readable:1;    ///< this descriptor's fd is ready for read ( socketfd read ready event from epoll_wait )
    }state = {0,0,0,0};

    ifd();
    ifd(int fd_, uint32_t options_);
    ifd(ifd&&) noexcept;
    ifd(const ifd&) = default;
    ifd& operator=(ifd&&) noexcept;
    ifd& operator=(const ifd&) = default;

    result<std::size_t> set_window_size(uint32_t sz);
    result<std::size_t> data_in();
    uint32_t set_options(u_int32_t opt);
    std::size_t toread();
    result<> clear();
    result<std::size_t> out(uint8_t* datablock, std::size_t sz, bool wait_completed=true) const;

    ~ifd();




};

}