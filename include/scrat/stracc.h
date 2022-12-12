#pragma once

#include <scrat/dlconfig.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <functional>
#include <string>
#include <string_view>
#include <format>
#include <chrono>
#include <stack>

#include <scrat/encodings/fmtio.h>




namespace scrat
{



	class SCRAT_API stracc final
	{
		_decl_objname


		std::string _d; ///< private std::string instance;
    public:
		using list = std::vector<std::string>;
        using stack = std::stack<std::string>;

    private:
        /*!
            
            @ref https://en.cppreference.com/w/cpp/utility/format/format
        */
        template <typename... Args> std::string _format(std::string_view rt_fmt_str, Args&&... args) 
        {
            return std::vformat(rt_fmt_str, std::make_format_args(args...));
        }



	public:

		stracc();
		~stracc();

        //...
        stracc& operator << (color::type arg_);

        template<typename T> stracc& operator << (T arg_)
        {
            std::string_view s = _d.c_str();
            _d = _format(s.data(), arg_);
            return *this;
        }

        stracc& operator = (const std::string& str) { _d = str; return *this; }
        stracc& operator = (const char* str) { _d = str; return *this; }
        stracc& operator = (char* str) { _d = str; return *this; }
        stracc& operator = (char cc) { _d = cc; return *this; };
        stracc& operator = (std::string_view str) { _d = str.data(); return *this; }
        stracc& operator = (std::string_view&& rstr) { _d = std::move(rstr.data()); return *this; }

        template<typename T> stracc& operator , (const T& arg_)
        {
            _d = "{}"; // J'ai des doutes. .... 
            _d = _format(_d.c_str(), arg_);
            return *this;
        }

        stracc& operator += (std::string_view str) { _d += str.data(); return *this; };
        stracc& operator += (std::string_view&& rstr) { _d += std::move(rstr.data()); return *this; }
        stracc& operator += (std::string str) { _d += str; return *this; }
        stracc& operator += (const char* str) { _d += str; return *this; }
        stracc& operator += (char* str) { _d += str; return *this; }
        stracc& operator += (char cc) { _d += cc; return *this; }
        stracc& operator += (color::type c_) { _d += attr< textattr::format::ansi256 >::fg(c_); return *this; }

       

        template<typename T> stracc& operator += (const T& arg_)
        {
            _d += "{}";
            _format(_d, _d.c_str(), arg_);
            return *this;
        }

            
        const std::string& operator()() const { return _d; }


        void clear();
	};

}


/*
    stracc str = " ceci est mon nom: {12s}" ;
    str << "Serge Lussier";


    //...
    stracc& operator<<(std::string_view str)
    {
        std::format(_d, str);
        return *this;
    }



    replaces :

template<typename T> String& String::_Format(const T& _argv)
{
    String::Format fmt = { _mStr };
    char     buf[256];
    std::memset(buf, 0, 200);
    //LFnl << "\n";

    // Commentaires &eacute;crits en anglais seulement pour simplifier le texte.
    std::string::iterator c = _mStr.begin() + _mArgPos;
    std::string::iterator n, beg, l;
    beg = n = c;
    ++c;
    // %[flag] :

    switch (*c)
    {
    case '-':
    case '+':
    case '#':
    case '0':fmt.F = *c++;
        break;
    default:
        //++m;
        break;
    }

    n = c;
    // %[width]:
    while ((n != _mStr.end()) && isdigit(*n))
        ++n;
    l = n; // save  ahead 'cursor position'
    --n;
    if (n >= c)
    {
        int t = 0;
        while (n >= c)
            // interpret format width value:
            fmt.W = fmt.W + (*(n--) - '0') * static_cast<int>(pow(10, t++));
    }
    else
        fmt.W = 0; // no width
    c = l; // update effective cursor position

    // check floating point format: m,n,l:=>  read positions
    if (*c == '.')
    {
        fmt.R = fmt.P;
        ++c;
        n = c;
        while ((n != _mStr.end()) && isdigit(*n))
            ++n;
        l = n;
        --n;
        int t = 0;
        fmt.R = 0;
        while (n >= c)
            fmt.R = fmt.R + (*(n--) - '0') * static_cast<int>(pow(10, t++));
        c = l;
    }
    else
        fmt.R = fmt.P; // no floating point format

    //[.precision]
    n = c;
    //% ([Length]) [specifier]
    // format prefix ends;
    // Now the format type mnemonic:
    //  - remove the format string.
    //  - insert 'formatted/interpreted' value substring at the _arg_position:
    //  - reset _arg_position
    switch (*c)
    {
    case 'b':
    {
        if constexpr (!std::is_same<T, const std::string&>::value)
        {
            std::string BinaryStr;
            bool        pad = fmt.F == '0';
            BinaryStr = String::to_binary<T>(_argv, pad,
                fmt.W <= 128 ? fmt.W : 128); // Limit grouping digits to 128 ...

            //std::sprintf(buf, "%s", BinaryStr.c_str());
            fmt.Len = (c + 1) - beg;  //save format substring's Length
            _mStr.erase(_mArgPos, fmt.Len);
            _mStr.insert(_mArgPos, BinaryStr.c_str(), BinaryStr.length());
            _mArgPos = 0;
            return *this;
        }
        break;
    }

    case 'd': // Decimale ou entier
    case 'i':fmt.S = *c++;
        break;
    case 'x':
    case 'X':fmt.S = *c++;
        break;
    case 'f':
    case 'F':
    case 'g':
    case 'G':fmt.S = *c++;
        break;
    case 's':
    case 'c':fmt.S = *c++;
        break;
    default:break;
    }

    fmt.Len = c - beg;
    //std::cout << __PRETTY_FUNCTION__ << '\n' << __LINE__ << " _D:\n'" << _D << "':\n";
    std::string ff(_mStr, _mArgPos, fmt.Len);
    //std::cout << "ff(_mStr, _mArgPos, fmt.Len): '" << ff << "'\n";
    // -- Clang-tidy:error: cannot pass object of non-trivial type 'const std::__cxx11::basic_string<char>' through variadic function
    //
    if constexpr (std::is_same<T, const std::string&>::value)
    {
        std::snprintf(buf, 199, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=const std::string&]:'" << buf << "'\n";
    }
    else if constexpr (std::is_same<T, std::string>::value)
    {
        std::snprintf(buf, 199, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=std::string]:'" << buf << "'\n";
    }
    else
    {
        std::sprintf(buf, ff.c_str(), _argv);
        //std::cout << "\nbuffer[argv=T (const T&)]:'" << buf << "'\n";
    }
    _mStr.erase(_mArgPos, fmt.Len);
    _mStr.insert(_mArgPos, buf, std::strlen(buf));
    _mArgPos = 0;
    return *this;
};

    */

