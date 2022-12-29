#pragma once


#include <scrat/result.h>
#include <scrat/lexer/types.h>
#include <memory>
#include <vector>
#include <map>


namespace scrat::script
{
    /*!
     * @brief Arithmetic Logical Unit ( Sort of...)
     *
     *
     * One local instance of/or pointer-to alu is held by each and every xio; alu takes the role of the xio accumulator
     * holding one and only one value (accumulator) /arithmetic logical unit. On the active role, alu also performs the base arithmetic operations between alu's within
     * xio execution unit.
     * @author &copy; 2014(using union),2020(using std::any),2022(back to union); Serge Lussier, oldlonecoder/bretzel/Bretzelus/Bretzeltux, (lussier.serge@gmail.com)
     *
     */

struct SCRAT_API alu
{
    using list = std::vector<alu>;
    using linkarray = std::vector<alu*>;
    using shared = std::shared_ptr<alu>;

    static std::string mNil;

    script::type::T  T = script::type::float_t; ///< Default type.

    std::string text;
    union piudt
    {// Create std::string pool for managing assign-release cycle of std::string* in alu;
        std::string* t;
        double       d;
        uint64_t     u;
        int64_t      i;
        void*        p;

        piudt& operator=(double v_) { d = v_; return *this;}
        piudt& operator=(uint64_t v_) { u = v_; return *this;}
        piudt& operator=(int64_t v_) { i = v_; return *this;}
        piudt& operator=(void* v_) { p = v_; return *this;}
        piudt& operator=(std::string* v_) { t = v_; return *this;}
    }v = {nullptr};

    enum CNV {
        DEG,
        RAD
    };

    alu();

    alu(const alu&) = default;
    alu(double A_);
    alu(float A_);
    //alu(token_t* rhs);

    alu(const std::string & A_);
    alu(const char* A_);
    alu(void* A_);
    alu(alu&&) noexcept = default;
    alu(int8_t A_);
    alu(int16_t A_);
    alu(int32_t A_);
    alu(int64_t A_);
    alu(uint8_t A_);
    alu(uint16_t A_);
    alu(uint32_t A_);
    alu(uint64_t A_);
    alu(bool A_);



    alu& operator =(const alu & Lhs_);// { a = lf.a; T = lf.T; return *this; }
    alu& operator =(alu && rhs) noexcept;
    alu operator==(const alu & lf) const;

    [[nodiscard]] script::type::T TypeOf() const { return T; } // [const] Sous r�serve; [const] provided that it is not screwing access...

#pragma region Huge_Overload
    template<typename oftype> [[nodiscard]]oftype value() const {

        if constexpr ((std::is_same<oftype, std::string>::value) || (std::is_same<oftype, const std::string&>::value))
            return (T & script::type::text_t ? text : "");
        else
            if constexpr (std::is_same<oftype, const char*>::value)
                return (T & script::type::text_t ? text.c_str() : nullptr);
            else
                if constexpr (std::is_same<oftype, void*>::value)
                    return v.p;
                else
                    return static_cast<oftype>(
                        T & script::type::i8_t ? v.i :
                        T & script::type::i16_t ? v.i :
                        T & script::type::i32_t ? v.i :
                        T & script::type::i64_t ? v.i :
                        T & script::type::u8_t ? v.u :
                        T & script::type::u16_t ? v.u :
                        T & script::type::u32_t ? v.u :
                        T & script::type::u64_t ? v.u :
                        T & script::type::bool_t ? v.u :
                        T & script::type::float_t ? v.d : 0

                        );
    }

    template<typename of_type> [[nodiscard]] of_type number() const
    {
        if constexpr (std::is_same<of_type, void*>::value)
            return v.p;
        else
            if constexpr (std::is_same<of_type, std::string>::value)
                return (std::string)*this;
            else
                return static_cast<of_type>(
                            T & script::type::i8_t ? v.i :
                            T & script::type::i16_t ? v.i :
                            T & script::type::i32_t ? v.i :
                            T & script::type::i64_t ? v.i :
                            T & script::type::u8_t ? v.u :
                            T & script::type::u16_t ? v.u :
                            T & script::type::u32_t ? v.u :
                            T & script::type::u64_t ? v.u :
                            T & script::type::bool_t ? v.u :
                            T & script::type::float_t ? v.d : 0
                    );

    }

    template<script::type::T V> bool is() {
        return T == V;
    }

#pragma endregion Huge_Overload
    [[maybe_unused]]bool is_text() const { return T & script::type::text_t; }
    [[maybe_unused]] std::string to_str() const;
    [[maybe_unused]] bool isnum() const;
    [[maybe_unused]] [[nodiscard]]std::string types() const;
    virtual ~alu();



    // Arithmetic operations overload between two alu's:

    // Binary operators:

    alu operator +(const alu & Rhs_) const;
    alu operator -(const alu & Rhs_);
    alu operator *(const alu & Rhs_) const;
    alu operator [](alu::CNV Cnv_);

    alu operator /(const alu & Rhs_);
    alu operator %(const alu & Rhs_) const;
    alu operator ^(const alu & Rhs_) const;
    alu operator <<(const alu & Lhs_) const;
    alu operator >>(const alu & Lhs_) const;
    alu operator |(const alu & Lhs_) const;
    alu operator &(const alu & Lhs_) const;
    alu operator ~();
    // assign_t operators:
    alu& operator +=(const alu & Rhs_);
    alu& operator -=(const alu & Rhs_);
    alu& operator *=(const alu & Rhs_);
    alu& operator /=(const alu & Rhs_);
    alu& operator %=(const alu & Rhs_);
    alu& operator |=(const alu & Rhs_);
    alu& operator &=(const alu & Rhs_);
    alu& operator ^=(const alu & Rhs_);

    alu& operator <<=(const alu & Rhs_);
    alu& operator >>=(const alu & Rhs_);


    // boolean operators:
    alu operator >(const alu & Rhs_);
    alu operator <(const alu & Rhs_);
    alu operator >=(const alu & Rhs_);
    alu operator <=(const alu & Rhs_);
    alu operator !=(const alu & Rhs_);
    alu operator ||(const alu & Rhs_);
    alu operator &&(const alu & Rhs_);

    bool operator !();
    //bool IsSet() { return r.has_value(); }
    operator bool();

    alu operator -();
    alu operator +();

    alu operator ++(int);
    alu operator ++();
    alu operator --();
    alu operator --(int);

    alu radical(const alu & Lhs_) const;
    alu factorial(const alu & Lhs_) const;
    alu factorial() const;
    bool type_size(const alu & Rhs_) const;
    std::string operator()()const { return (std::string)(*this); }
    explicit operator std::string() const;

    //    private:
    //    alu remove_substr_copy(const std::string& to_erase);
    //    alu completely_remove_substr_copy(const std::string& to_erase);
    //    alu& remove_substr(const std::string& to_erase);
    //    alu& completely_remove_substr(const std::string& to_erase);


};
}

