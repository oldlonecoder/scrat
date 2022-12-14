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

#include <scrat/stracc>
#include <mutex>

#include <scrat/geometry>


namespace scrat
{

	struct SCRAT_API source_location
	{
		std::string_view _fn;
		std::string_view _file;
		int line = 0;
	};


#define source_pfn { __PRETTY_FUNCTION__, "", 0 }

#define source_pffl  { __PRETTY_FUNCTION__, __FILE__, __LINE__ }
#define source_aaa  { __PRETTY_FUNCTION__, __FILE__, __LINE__ }

#define source_pfnl {  __PRETTY_FUNCTION__, "",  __LINE__	}

#define source_sf {__FUNCTION__, "", 0}

#define source_fl {._fn = "", ._file = __FILE__, .line = __LINE__}
#define source_fnl {._fn = __FUNCTION__ , ._file = __FILE__, .line = __LINE__}
#define source_ffl {._fn = __FUNCTION__ , ._file = __FILE__, .line = __LINE__}


	class SCRAT_API rem
	{
		scrat::source_location _src = {};
		stracc::list _components;
		stracc  _text;
		static attribute_list colors;
	public:
		using list = std::vector<rem>;
		std::string cc();
	private:

		static rem::list _application_message_q;
	public:
		using ctype = int8_t;
		using code = int16_t;

		static constexpr rem::ctype nulltype = -1;
		static constexpr rem::code nullcode = -1;

		// - class of messages, constructible, create events:
		static constexpr rem::ctype err = 1;
		static constexpr rem::ctype warning = 2;
		static constexpr rem::ctype fatal = 3;
		static constexpr rem::ctype except = 4;
		static constexpr rem::ctype message = 5;
		static constexpr rem::ctype output = 6;
		static constexpr rem::ctype debug = 7;
		static constexpr rem::ctype info = 8;
		static constexpr rem::ctype comment = 9;
		static constexpr rem::ctype syntax = 10;
		static constexpr rem::ctype status = 11;
		static constexpr rem::ctype test = 12;
		// ...
		// - message codes status codes, error codes:
		static constexpr rem::code ok = 0; ///< Obviously;
		static constexpr rem::code accepted = 1;
		static constexpr rem::code success = 2;
		static constexpr rem::code rejected = 3;
		static constexpr rem::code failed = 4;
		static constexpr rem::code empty = 5;
		static constexpr rem::code full = 6;
		static constexpr rem::code notempty = 7;
		static constexpr rem::code implemented = 9; ///< Like notimplemented or already implemented
		static constexpr rem::code notimplemented = 10; ///< "Please, implement"
		static constexpr rem::code untested = 12;
		static constexpr rem::code eof = 13; ///< end of file
		static constexpr rem::code eos = 14; ///< end of stream or string or statement or state ( machine state )
		static constexpr rem::code null_ptr = 15; ///< It will happen. Trust me :)
		static constexpr rem::code endl = 16; ///< end of line code, input format
		static constexpr rem::code enter = 17; ///< enter bloc or indent
		static constexpr rem::code leave = 18; ///< leaving bloc or unindent
		static constexpr rem::code begin = 19; ///< begin (sel)section or indent
		static constexpr rem::code end = 20; ///< end (sel)section or unindent
		static constexpr rem::code notexist = 21; ///< does not exist
		static constexpr rem::code exist = 26; ///< does not exist
		static constexpr rem::code unexpected = 22; ///< nexpected
		static constexpr rem::code expected = 23; ///< does not exist
		static constexpr rem::code blocked = 24; ///< thread trying to lock a mutex has failed because the mutex is already locked in another thread...
		static constexpr rem::code locked = 25; ///< thread trying to lock a mutex has became the owner of the lock.
		//...etc....
		// source_location:
		static constexpr rem::code function = 1000;
		static constexpr rem::code file = 1001;
		static constexpr rem::code line = 1002;
		static constexpr rem::code stamp = 4000; ///< fully detailed timestamp
		static constexpr rem::code hour = 4001;
		static constexpr rem::code minute = 4002;
		static constexpr rem::code seconds = 4003;
		static constexpr rem::code weekday = 4004;



		// --------------------------------------------------------------------------------------------------


		rem() = default;
		~rem();

		rem(const rem&);
		rem(rem&& m) noexcept ;
		rem(rem::ctype ty, source_location = {});

		rem& operator = (rem&& m) noexcept;
		rem& operator = (const rem& m);

		// , | || + << < & && +=
		rem& operator < (rem::code c_);
		rem& operator < (color::type c_);
		rem& operator < (textattr::pair cf);
		rem& operator < (const std::string& arg_);
		rem& operator < (const char* arg_);

		rem& operator << (rem::code c_);
		rem& operator << (color::type c_);
		rem& operator << (textattr::pair cf);
		rem& operator << (const std::string& arg_);
		rem& operator << (const char* arg_);
		template<typename T> rem& operator < (const T& arg_)
		{
			stracc str;
			str += arg_;
			rem::_components.push_back(str());
			return *this;
		}

		template<typename T> rem& operator << (const T& arg_)
		{
			stracc str;
			str += arg_;
			rem::_components.push_back(str());
			return *this;
		}
		static std::string now(std::string_view fmt =  "[%d/%m/%Y - %X]");

		static rem& push_error(source_location src_ = {});
		static rem& push_warning(source_location src_ = {});
		static rem& push_fatal(source_location src_ = {});
		static rem& push_exception(source_location src_ = {});
		static rem& push_message	(source_location src_ = {});
		static rem& push_output	(source_location src_ = {});
		static rem& push_debug		(source_location src_ = {});
		static rem& push_info		(source_location src_ = {});
		static rem& push_comment	(source_location src_ = {});
		static rem& push_syntax	(source_location src_ = {});
		static rem& push_status	(source_location src_ = {});
		static rem& push_test		(source_location src_ = {});

		static rem::code clear(std::function<void(rem&)> fn_);
	private:
		rem::ctype _mclass = rem::nulltype;
		rem::code _mcode = rem::nullcode;
		std::mutex _mcomponents_lock;
		static std::mutex _mclasses_lock;
		static std::mutex mtx_app_q;
		static std::mutex _mcodes_lock;
	};
}

