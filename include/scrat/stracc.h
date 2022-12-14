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
#include <chrono>
#include <stack>
#include <scrat/textattr.h>




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
    // template <typename... Args> std::string _format(std::string_view rt_fmt_str, Args&&... args)
    // {
    //     return std::vformat(rt_fmt_str, std::make_format_args(args...));
    // }

        // %[flags][width][.precision][Length]specifier
    struct format_data
    {
        uint8_t     F = 0; // Flag ( - + . # 0 ) => if s='s' then '-' => justify right; '+' => justify left; '^' => justify center.
        uint8_t     W = 0; // Width ( Length ) Value
        uint8_t     R = 0; // Custom flag set if this format requires floating point spec.
        uint8_t     P = 6; // Precision (Same as  default).
        uint8_t     L = 0; // Length modifier ( linenum,ll,h,hh )
        std::size_t len = 0; // Format Length.
        std::size_t position = 0; // Arg index position within _d.
        char        s = 0; // Effective characeter code specifier such as 'd'; 's'; 'f'; 'l'; 'p'...
        const char* c = nullptr;

        format_data(std::string& str_) : c(str_.c_str())
        {}

        std::string operator()();

    };
    std::string::size_type _arg_pos = std::string::npos;

    std::string::size_type _scan_arg();
    template<typename T> stracc& _format(const T& _argv);
public:

    stracc();
    stracc(const std::string& instr) { _d = instr;}
    stracc(const char* instr) { _d = instr;}
    stracc(std::string_view instr) { _d = instr.data();}


    ~stracc();

    //...
    stracc& operator << (color::type arg_);

    template<typename T> stracc& operator >> (T& out)
    {
        std::istringstream in(_d); //  When String was derived from std::string ... Oops!  std::istringstream in(*this);

        if constexpr (
            std::is_same<float, T>::value || std::is_same<double, T>::value ||
            std::is_same<long double, T>::value ||
            std::is_same<int, T>::value || std::is_same<unsigned int, T>::value ||
            std::is_same<int8_t, T>::value || std::is_same<uint8_t, T>::value ||
            std::is_same<int16_t, T>::value || std::is_same<uint16_t, T>::value ||
            std::is_same<int32_t, T>::value || std::is_same<uint32_t, T>::value ||
            std::is_same<int64_t, T>::value || std::is_same<uint64_t, T>::value
            )
        {
            //in.precision(_mPrecision);
            out = 0;
            //in.precision(_mPrecision);
            in >> out;
            return *this;
        }
        T R;
        in >> R;
        out = R;
        return *this;
    }

    template<typename T> stracc& operator << (T arg_)
    {
        if(_scan_arg() != std::string::npos)
            return _format(arg_);

        return this->operator+=(arg_);
    }

    stracc& operator = (const std::string& str) { _d = str; return *this; }
    stracc& operator = (const char* str) { _d = str; return *this; }
    stracc& operator = (char* str) { _d = str; return *this; }
    stracc& operator = (char cc) { _d = cc; return *this; };
    stracc& operator = (std::string_view str) { _d = str.data(); return *this; }
    stracc& operator = (std::string_view&& rstr) { _d = std::move(rstr.data()); return *this; }



    template<typename T> stracc& operator , (const T& arg_)
    {
        if(_scan_arg() != std::string::npos)
            return _format(arg_);
        return this->operator+=(arg_);
    }

    template<typename T> stracc& operator + (const T& arg_)
    {
        if(_scan_arg() != std::string::npos) return _format(arg_);
        return this->operator+=(arg_);
    }

    stracc& operator += (std::string_view str) { _d += str.data(); return *this; };
    stracc& operator += (std::string_view&& rstr) { _d += std::move(rstr.data()); return *this; }
    stracc& operator += (std::string str) { _d += str; return *this; }
    stracc& operator += (const char* str) { _d += str; return *this; }
    stracc& operator += (char* str) { _d += str; return *this; }
    stracc& operator += (char cc) { _d += cc; return *this; }
    stracc& operator += (color::type c_) { _d += attr< textattr::format::ansi256 >::fg(c_); return *this; }


    template<typename T> stracc& hexadecimal(T& v)
    {
        std::stringstream is(_d.c_str() + 2);
        //std::cerr << " this:'" << _D.c_str()+2 << "' -> ";
        is >> std::hex >> v;
        //std::cerr << v << '\n';
        return *this;
    }

    template<typename T> stracc& operator += (const T& arg_)
    {
        std::ostringstream out;
        out << arg_;
        _d += out.str();
        return *this;
        // return _format(arg_); duh!
    }

    std::string& str() { return _d; }
    template<typename T> static std::string to_binary(T __arg, bool padd = false, int f = 128)
    {
        uint8_t seq;
        int     nbytes = sizeof(T);

        uint8_t tableau[sizeof(T)];
        memcpy(tableau, (uint8_t*)&__arg, nbytes);
        std::string stream = "";
        int         s = 0;
        //bool discard = false;
        for (int x = 1; x <= nbytes; x++)
        {
            seq = tableau[nbytes - x];
            if ((x == 1 && !padd && !seq) || (stream.empty() && !padd && !seq))
                continue;
            for (int y = 7; y >= 0; y--)
            { // est-ce que le bit #y est � 1 ?
                if (s >= f)
                {
                    stream += ' ';
                    s = 0;
                }
                ++s;
                uint8_t b = 1 << y;
                if (b & seq)
                    stream += '1';
                else
                    stream += '0';
            }
        }
        /*tableau.Clear();*/
        return stream;
    }


    const std::string& operator()() const { return _d; }


    void clear();
};


template<typename T> stracc& stracc::_format(const T& _argv)
{
    if(_scan_arg() == std::string::npos)
        return this->operator+=(_argv);

    stracc::format_data fmt = { _d };
    char     buf[256];
    std::memset(buf, 0, 200);
    //LFnl << "\n";

    // Commentaires &eacute;crits en anglais seulement pour simplifier le texte.
    std::string::iterator c = _d.begin() + _arg_pos;
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
    while ((n != _d.end()) && isdigit(*n))
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
        while ((n != _d.end()) && isdigit(*n))
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
            BinaryStr = stracc::to_binary<T>(_argv, pad,
                fmt.W <= 128 ? fmt.W : 128); // Limit grouping digits to 128 ...

            fmt.len = (c + 1) - beg;  //save format substring's Length
            _d.erase(_arg_pos, fmt.len);
            _d.insert(_arg_pos, BinaryStr.c_str(), BinaryStr.length());
            _arg_pos = std::string::npos;
            return *this;
        }
        break;
    }

    case 'd': // Decimale ou entier
    case 'i':fmt.s = *c++;
        break;
    case 'x':
    case 'X':fmt.s = *c++;
        break;
    case 'f':
    case 'F':
    case 'g':
    case 'G':fmt.s = *c++;
        break;
    case 's':
    case 'c':fmt.s = *c++;
        break;
    default:break;
    }

    fmt.len = c - beg;
    //std::cout << __PRETTY_FUNCTION__ << '\n' << __LINE__ << " _D:\n'" << _D << "':\n";
    std::string ff(_d, _arg_pos, fmt.len);
    //std::cout << "ff(_d, _arg_pos, fmt.len): '" << ff << "'\n";
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
    _d.erase(_arg_pos, fmt.len);
    _d.insert(_arg_pos, buf, std::strlen(buf));
    _arg_pos = std::string::npos;
    return *this;
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
*/



