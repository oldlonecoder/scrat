#include <scrat/interpret/alu.h>


namespace scrat::script
{

std::string  SCRAT_API alu::mNil = "";

#define UCTor(rhs) { T |= script::type::number_t|script::type::leaf_t; v.d=(rhs); }
#define SCTor(rhs) { T |= script::type::number_t|script::type::sign_t|script::type::leaf_t;v.d=(rhs); }
#define _assign_u T |= script::type::number_t|script::type::leaf_t; v..reset(); v. = (rhs);
#define _assign_s T |= script::type::number_t|script::type::sign_t|script::type::leaf_t; v..reset(); v. = (rhs);

    alu::alu(int8_t rhs) : T(script::type::i8_t)
    SCTor(rhs)
    alu::alu(int16_t rhs) : T(script::type::i16_t)
    SCTor(rhs)
    alu::alu(int32_t rhs) : T(script::type::i32_t)
    SCTor(rhs)
    alu::alu(int64_t rhs) : T(script::type::i64_t)
    SCTor(static_cast<double>(rhs))
    alu::alu(uint8_t rhs) : T(script::type::u8_t)
    UCTor(rhs)
    alu::alu(uint16_t rhs) : T(script::type::u16_t)
    UCTor(rhs)
    alu::alu(uint32_t rhs) : T(script::type::u32_t)
    UCTor(rhs)
    alu::alu(uint64_t rhs) : T(script::type::u64_t)
    UCTor(static_cast<double>(rhs))
    alu::alu(bool rhs) : T(script::type::bool_t)
    SCTor(rhs)

#define cast(rhs)\
(rhs).T & script::type::bool_t  ? (rhs).v.i != false :\
(rhs).T & script::type::i8_t       ? (rhs).v.i:\
(rhs).T & script::type::i16_t      ? (rhs).v.i:\
(rhs).T & script::type::i32_t      ? (rhs).v.i:\
(rhs).T & script::type::i64_t      ? (rhs).v.i:\
(rhs).T & script::type::u8_t       ? (rhs).v.u:\
(rhs).T & script::type::u16_t      ? (rhs).v.u:\
(rhs).T & script::type::u32_t      ? (rhs).v.u:\
(rhs).T & script::type::u64_t      ? (rhs).v.u:\
(rhs).T & script::type::float_t    ? (rhs).v.d :0




    alu::alu(double rhs)
    {
        T = script::type::number_t | script::type::float_t | script::type::leaf_t;
        v.d = rhs;
    }

    alu::alu() {}

    alu::alu(float rhs)
    {
        T = script::type::number_t | script::type::float_t | script::type::leaf_t;
        v.d = (double)rhs;
    }

    alu::alu(const std::string& rhs)
    {
        T = script::type::text_t | script::type::leaf_t;
        v.t = &text; text = rhs;
    }

    alu::alu(const char* rhs)
    {

        T = script::type::text_t | script::type::leaf_t;
        v.t = &text; text = rhs;
    }

    alu::alu(void* rhs)
    {
        T = script::type::any_t | script::type::void_ptr;
        v.p = rhs;
    }

    bool alu::type_size(const alu& Rhs_) const
    {
        using namespace script::type;
        script::type::T me = T & (i8_t | i16_t | i32_t | i64_t | u8_t | u16_t | u32_t | u64_t | float_t);
        script::type::T r = Rhs_.T & (i8_t | i16_t | i32_t | i64_t | u8_t | u16_t | u32_t | u64_t | float_t);
        return me - r >= 0;
    }

    alu::operator bool()
    {
        if (!v.i)
            return false;
        if (!T)
            return false;
        if (T & script::type::text_t)
            return !text.empty();
        if (T & script::type::number_t)
            return v.d != 0.0f;
        if (T & script::type::void_ptr)
            return value<void *>() != nullptr;
        return false;
    }

#define lr_number(rv) if ((T&script::type::number_t) && ((rv).T&script::type::number_t))
#define lr_text(rv)   if ((T&script::type::text_t) && ((rv).T&script::type::text_t))
#define lr_any(rv)    if ((T&script::type::any_t)  && ((rv).T&script::type::any_t))
#define is_integrale(rv) if (!((rv).T&script::type::float_t))
#define is_text(i)    if((i).T & script::type::text_t)
#define is_number if ((T&(script::type::float_t|script::type::number_t)))
    // #define delstring_if(rhs) if ((T&script::type::text_t) && (!(rhs).T & script::type::text_t)) delete v..t;  Vieux stuff

    alu& alu::operator=(const alu& rhs)
    {
        if (&rhs != this)
        {
            T = rhs.T;
            v = rhs.v;
        }
        return *this;
    }

    alu& alu::operator=(alu&& rhs) noexcept
    {
        std::swap(T, rhs.T);
        std::swap(v, rhs.v);
        return *this;
    }

    alu alu::operator==(const alu& rhs) const
    {

        if (T & script::type::number_t)
            return {(bool)(((cast(*this)) == (cast(rhs))))};
        if (T & script::type::text_t)
            return {text == rhs.text};

        if (T & script::type::any_t)
            return v.p == rhs.v.p;
        return {false};
    }

[[maybe_unused]] std::string alu::types() const
    {
        std::string str;
        return script::type::name(T);
    }

    alu alu::operator+(const alu& rhs) const
    {
        lr_text(rhs)
            return {text + rhs.text};
        else lr_number(rhs)
            return {v.d + rhs.v.d}; // Slow?

        std::string str;
        rem::push_warning() < " [" < script::type::name(T) < "] and [" < script::type::name(rhs.T) < "] are incompatible.";
        return {false};
    }

    alu alu::operator[](alu::CNV c)
    {
        is_text(*this)
        {
            return {0.0f};
        }
        auto d = v.d;

        alu    a;
        switch (c)
        {
            case alu::DEG:
                a = d * 180 / 3.141592653589793;
                break;
            case alu::RAD:
                a = d * 3.141592653589793 / 180;
                break;
            default:
                a = 0.0;
                break;
        }
        return a;
    }

    alu alu::operator-(const alu& rv)
    {

        lr_text(rv)
        {
            rem::push_warning() < " substraction operator strings is not yet supported";
            //return remove_substr_copy(rv.text);
            return *this;
        }

        lr_number(rv)
            return {v.d - rv.v.d};
        lr_any(rv)
            return {reinterpret_cast<uint64_t>(value<void *>()) - reinterpret_cast<uint64_t>(rv.value<void *>())};

        rem::push_warning() < "cannot apply sign on " < script::type::name(T);
        return {false};
    }


    alu alu::operator*(const alu& rv) const
    {
        alu f;
        lr_text(rv)
        {
            std::size_t pos = rv.text.find(text);
            return {pos != std::string::npos};
        }

        //else
        lr_number(rv)
            return {v.d * rv.v.d};

        rem::push_warning() < "cannot multiply" < script::type::name(T) < " and " < script::type::name(rv.T) < ".";
        return {};
    }

    alu alu::operator/(const alu& rv)
    {
        lr_text(rv)
        {
            rem::push_warning() < "cannot divide strings";
            return {};
        }
        //    f = text + rv.text;
        //else
        lr_number(rv)
        {
            double vr = rv.v.d;
            double lr = v.d;

            if (vr == 0.0L)
            {
                rem::push_exception() < " In alu operation: division by zero error. (" < (*this)() < " / " < rv() < ")";
                return {};
            }
            return {lr / vr};
        }

        rem::push_warning() < "cannot divide " < script::type::name(T) < " and " < script::type::name(rv.T) < ".";
        return {};
    }

    alu alu::operator%(const alu& rv) const
    {
        alu f;
        lr_text(rv)
        {
            rem::push_warning() < "cannot modulo strings";
            return {};
        }
        lr_number(rv)
            return {v.u % rv.v.u};

        rem::push_warning() < "cannot modulo " < script::type::name(T) < " and " < script::type::name(rv.T) < ".";
        return {};
    }

    alu alu::operator^(const alu& rv) const
    {
        alu f;
        lr_text(rv)
        {
            rem::push_warning() < "cannot xor strings";
            return {};
        }

        lr_number(rv)
            return {v.u ^ rv.v.u};

        rem::push_warning() < "cannot xor " < script::type::name(T) < " and " < script::type::name(rv.T) < ".";
        return {};
    }

    alu alu::operator<<(const alu& rhs) const
    {
        alu f;
        lr_text(rhs)
        {
            stracc str = text;
            str << rhs.text;
            return {str()};
        }

        lr_number(rhs)
            return {v.u << rhs.v.u};

        rem::push_warning() < "cannot execute bitwise left-shift or insert with " < script::type::name(T) < " and " < script::type::name(rhs.T) < ".";
        return f;
    }
    alu alu::operator>>(const alu& rhs) const
    {
        alu f;
        lr_text(rhs)
        {
            rem::push_warning() < "cannot execute bitwise right-shift or extract/write/send string on string " < script::type::name(T) < " and " < script::type::name(rhs.T) < ".";
        }
        lr_number(rhs)
            return {v.u >> rhs.v.u};

        rem::push_warning() < "cannot execute bitwise right-shift or extract/write/send with " < script::type::name(T) < " and " < script::type::name(rhs.T) < ".";
        return f;
    }

    alu alu::operator|(const alu& rhs) const
    {

        lr_text(rhs)
        {
            return {text + rhs.text};
        }
        lr_number(rhs)
            return {v.u | rhs.v.u};

        rem::push_warning() < "cannot execute bitwise or with " < script::type::name(T) < " and " < script::type::name(rhs.T) < ".";
        return {};
    }

    alu alu::operator&(const alu& rhs) const
    {
        lr_text(rhs)
        {
            rem::push_warning() < "cannot execute bitwise and on string ";
        }
        lr_number(rhs)
            return {v.u & rhs.v.u};

        rem::push_warning() < "cannot execute bitwise and between " < script::type::name(T) < " and " < script::type::name(rhs.T) < ".";
        return {};
    }

    alu alu::operator~()
    {
        is_text(*this)
        {
            rem::push_warning() < "cannot execute bitwise invert on string ";
        }

        is_number
        {
            if (T & script::type::float_t)
                return {1 / v.d};
            return {~v.u};
        }

        rem::push_warning() < "cannot execute bitwise invert on " < script::type::name(T);
        return {};
    }

    // assign_t operators:
    alu& alu::operator+=(const alu& rv)
    {
        lr_text(rv)
        {
            text = text + rv.text;
            return *this;
        }

        lr_number(rv)
        {
            v.d += rv.v.d;
            return *this;
        }

        rem::push_warning() < "cannot execute assign add between " < script::type::name(T) < " and " < script::type::name(rv.T) < ".";
        return *this;
    }

    alu& alu::operator-=(const alu& rv)
    {
        lr_text(rv) return *this;
        //return remove_substr(rv.text);

        lr_number(rv)
        {
            v.d  -= rv.v.d;
            return *this;
        }

        rem::push_warning() < "cannot execute assign substraction between " < script::type::name(T) < " and " < script::type::name(rv.T) < ".";
        return *this;
    }

    alu& alu::operator*=(const alu& rv)
    {
        lr_text(rv)
        {
            rem::push_warning() < "cannot execute assign multiply on strings [" < (*this)() < " *= " < rv() < "].";
            return *this;
        }

        lr_number(rv)
        {
            rem::push_debug()
                < color::Orange5 < "alu"
                < color::White < "::"

                < color::White < rem::endl < ":" < rem::endl < "{" < rem::endl
                < color::Yellow < v.d < color::White < '(' < color::LightSeaGreen < script::type::name(T) < color::White < ')'
                < color::CornflowerBlue < " *= "
                < color::Yellow < rv.v.d < color::White < '(' < color::LightSeaGreen < script::type::name(rv.T) < color::White < ')';

            v.d *= rv.v.d;
            rem::push_output() < color::White < " = " < color::Yellow < v.d  < color::White < rem::endl < '}' < rem::endl;
            return *this;
        }

        rem::push_warning() < "cannot execute assign multiply between " < script::type::name(T) < " and " < script::type::name(rv.T) < ".";
        return *this;
    }

    alu& alu::operator/=(const alu& rv)
    {
        lr_text(rv)
        {
            rem::push_warning() < "cannot execute assign divide on strings [" < (*this)() < " /= " < rv() < "].";
            return *this;
        }

        lr_number(rv)
        {

            if (v.d==0.0f)
            {
                rem::push_warning() < "alu: cannot divide by zero."; // Oops plutôt erreur fatale!!
                v.d = 0.0;
                T = script::type::null_t;
                return *this;
            }
            v.d /= rv.v.d;
            return *this;
        }
        rem::push_warning() < "cannot execute assign divide between " < script::type::name(T) < " and " < script::type::name(rv.T) < ".";
        return *this;
    }

    alu::~alu() = default;

    alu& alu::operator%=(const alu& rv)
    {
        lr_text(rv) return *this;
        //return completely_remove_substr(rv.text);

        lr_number(rv)
        {
            v.u %= rv.v.u;
            return *this;
        }
        return *this;
    }

    alu& alu::operator|=(const alu& rv)
    {

        lr_text(rv)
        {
            rem::push_warning() < "cannot execute assign bitwise or on strings [" < (*this)() < " |= " < rv() < "].";
            return *this;
        }

        lr_number(rv)
        {
            v.u |= rv.v.u;
            return *this;
        }
        rem::push_warning() < "cannot execute assign bitwise or between " < script::type::name(T) < " and " < script::type::name(rv.T) < ".";
        return *this;
    }

    alu& alu::operator&=(const alu& rv)
    {
        lr_text(rv)
        {
            rem::push_warning() << "cannot execute assign bitwise and on strings [" << (*this)() << " &= " << rv() << "].";
            return *this;
        }
        //    a = std::string(text + rv.text);
        //else
        lr_number(rv)
        {
            v.u &=  rv.v.u;
            return *this;
        }
        rem::push_warning() << "cannot execute assign bitwise and between " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return *this;
    }

    alu& alu::operator^=(const alu& rv)
    {
        lr_text(rv)
        {
            rem::push_warning() << "cannot execute assign xor on strings [" << (*this)() << " ><= " << rv() << "].";
            return *this;
        }

        lr_number(rv)
        {
            /// @note xor ?
            v.u ^= rv.v.u;
            return *this;
        }

        rem::push_warning() << "cannot execute assign xor between " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return *this;
    }

    // boolean operators:
    alu alu::operator>(const alu& rv)
    {
        lr_text(rv)
            return {text > rv.text};

        lr_number(rv)
            return {v.d > rv.v.d};

        rem::push_warning() << "cannot execute relational operations between " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return {false};
    }

    alu alu::operator<(const alu& rv)
    {
        lr_text(rv)
            return {text < rv.text};

        lr_number(rv)
            return {v.d < rv.v.d};

        rem::push_warning() << "cannot execute relational operations between " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return {false};

    }
    alu alu::operator>=(const alu& rv)
    {
        lr_text(rv)
            return {text >= rv.text};

        lr_number(rv)
            return {v.d >= rv.v.d};

        rem::push_warning() << "cannot execute relational operations between " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return {false};

    }

    alu alu::operator<=(const alu& rv)
    {
        lr_text(rv)
            return {text <= rv.text};

        lr_number(rv)
            return {v.d <= rv.v.d};

        rem::push_warning() << "cannot execute relational operations between " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return {false};

    }

    alu alu::operator!=(const alu& rv)
    {
        lr_text(rv)
            return {text != rv.text};

        lr_number(rv)
            return {v.d != rv.v.d};

        rem::push_warning() << "cannot execute relational operations between " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return {false};
    }

    alu alu::operator||(const alu& rv)
    {
        lr_text(rv)
            return {!text.empty() || (!rv.text.empty())};

        lr_number(rv)
            return {v.d || rv.v.d};

        rem::push_warning() << "cannot execute relational operations between " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return {false};
    }

    alu alu::operator&&(const alu& rv)
    {
        lr_text(rv)
            return {!text.empty() && (!rv.text.empty())};

        lr_number(rv)
            return {v.u && rv.v.u};

        rem::push_warning() << "cannot execute relational operations between " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return {false};
    }

    bool alu::operator!()
    {
        return v.d ==0.0f;
    }

    alu alu::operator-()
    {
        is_text(*this)
        {
            rem::push_warning() << "cannot execute sign operator on string -(\"" << (*this)() << "\").";
        }
        rem::push_warning() << "cannot execute relational operations on " << script::type::name(T);
        return -v.d;
    }

    alu alu::operator+()
    {
        is_text(*this)
        {
            rem::push_warning() << "cannot execute sign operator on string +(\"" << (*this)() << "\").";
        }

        is_number
            if (v.d < 0.0f)
                return {v.d * -1};

        rem::push_warning() << "cannot execute relational operations on " << script::type::name(T);
        return {false};
    }

    alu alu::operator++(int)
    {

        is_text(*this)
        {
            rem::push_warning() << "cannot increment string (\"" << (*this)() << "\")++.";
        }

        is_number
        {
            auto f = v.d;
            alu    ff(f);
            ++f;
            v.d = f;
            return ff;
        }
        rem::push_warning() << "cannot execute increment operations on " << script::type::name(T);
        return {false};
    }

    alu alu::operator++()
    {
        is_text(*this)
        {
            rem::push_warning() << "cannot increment string ++(\"" << (*this)() << "\").";
        }

        is_number
        {
            auto f = v.d;
            ++f;
            v.d = f;
            return *this;
        }
        rem::push_warning() << "cannot execute increment operations on " << script::type::name(T);
        return {false};
    }

    alu alu::operator--()
    {

        is_text(*this)
        {
            rem::push_warning() << "cannot decrement string --(\"" << (*this)() << "\").";
        }

        is_number
        {
            auto f = v.d;
            ++f;
            v.d = f;
            return *this;
        }
        rem::push_warning() << "cannot execute decrement operations on " << script::type::name(T);
        return {false};
    }

    alu alu::operator--(int)
    {
        is_text(*this)
        {
            rem::push_warning() << "cannot decrement string (\"" << (*this)() << "\")++."; // oops... What if long text string?
        }

        is_number
        {
            auto f = v.d;
            alu    ff(f);
            ++f;
            v.d = f;
            return ff;
        }
        rem::push_warning() << "cannot execute decrement operations on " << script::type::name(T);
        return {false};
    }

    alu alu::radical(const alu& Lhs_) const
    {
        alu al;
        lr_number(Lhs_)
        {
            al = alu(std::pow(v.d, 1 / Lhs_.v.d));
            return al;
        }

        rem::push_warning() << "cannot execute radical operator on " << script::type::name(T);
        return {false};
    }

    alu alu::factorial(const alu& Lhs_) const
    {
        lr_number(Lhs_)
        {
            int fac = 1;
            int N = Lhs_.number<int>();
            if (N >= 10)
                return {};
            for (int j = 1; j <= N; j++)
                fac *= j;
            return {fac};
        }
        rem::push_warning() << "cannot execute factorial operator on " << script::type::name(T);
        return {false};
    }

    alu alu::factorial() const
    {
        lr_number(*this)
        {
            int fac = 1;
            int N = number<int>();
            if (N >= 10)
                return {};
            for (int j = 1; j <= N; j++)
                fac *= j;
            return { fac };
        }
        rem::push_warning() << "cannot execute factorial operator on " << script::type::name(T);
        return { false };

    }

    alu& alu::operator<<=(const alu& rv)
    {

        lr_text(rv)
        {
            rem::push_warning() << "cannot execute assign extraction/bitwise left-shift / output on strings.";
            return *this;
        }

        lr_number(rv)
        {
            v.u = v.u << rv.v.u;
            return *this;
        }

        rem::push_warning() << "cannot execute assign bitwise left-shift or insert with " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return *this;
    }

    alu& alu::operator>>=(const alu& rv)
    {

        lr_text(rv)
        {
            rem::push_warning() << "cannot execute assign extraction/bitwise right-shift / output on strings.";
            return *this;
        }

        lr_number(rv)
        {
            v.u = v.u << rv.v.u;
            return *this;
        }

        rem::push_warning() << "cannot execute assign bitwise right-shift or insert with " << script::type::name(T) << " and " << script::type::name(rv.T) << ".";
        return *this;
    }

    //alu alu::remove_substr_copy(const std::string &to_erase)
    //{
    //
    //    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
    //    auto   cp  = std::any_cast<std::string>(a);
    //    size_t pos = cp.find(to_erase);
    //
    //    if(pos != std::string::npos)
    //    {
    //        // If found then erase it from string
    //
    //        cp.erase(pos<< to_erase.length());
    //    }
    //    return alu(cp);
    //}
    //
    //alu alu::remove_all_substr_copy(const std::string &to_erase)
    //{
    //    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
    //
    //
    //    size_t pos = std::string::npos;
    //    auto   cp  = std::any_cast<std::string>(a);
    //    // Search for the substring in string in a loop untill nothing is found
    //    while((pos = cp.find(to_erase)) != std::string::npos)
    //    {
    //        // If found then erase it from string
    //        cp.erase(pos<< to_erase.length());
    //    }
    //
    //    return alu(cp);
    //}
    //
    //alu &alu::remove_substr(const std::string &to_erase)
    //{
    //
    //    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
    //    auto   cp  = std::any_cast<std::string &>(a);
    //    size_t pos = cp.find(to_erase);
    //
    //    if(pos != std::string::npos)
    //    {
    //        // If found then erase it from string
    //
    //        cp.erase(pos<< to_erase.length());
    //    }
    //    return *this;
    //}

[[maybe_unused]] bool alu::isnum() const
    {
        return (T & (script::type::float_t | script::type::number_t));
    }

    //alu &alu::completely_remove_substr(const std::string &to_erase)
    //{
    //    // J'suis paresseux :: copy-paste here (from thispointer :: how to remove substr: )
    //
    //
    //    size_t pos = std::string::npos;
    //    auto   cp  = std::any_cast<std::string &>(a);
    //    // Search for the substring in string in a loop untill nothing is found
    //    while((pos = cp.find(to_erase)) != std::string::npos)
    //    {
    //        // If found then erase it from string
    //        cp.erase(pos<< to_erase.length());
    //    }
    //
    //    return *this;
    //}

[[maybe_unused]] std::string alu::to_str() const
    {
        return (std::string)*this;
    }

    alu::operator std::string() const
    {
        stracc str;
        if (T & script::type::bool_t)
            str << (v.u ? "true" : "false");
        else
            if (T & script::type::number_t)
                str << v.d;
            else
                if (T & script::type::text_t)
                    return text;
                else
                    if (T & script::type::void_ptr) {
                        str = "@[%ld]";
                        str << v.p;
                    }
                    else
                        if (T & script::type::null_t)
                            str << "null";
        return str();
    }
}

