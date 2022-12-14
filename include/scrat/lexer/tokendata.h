#pragma once

#include <scrat/lexer/types.h>
#include <scrat/stracc.h>

namespace scrat
{
namespace script
{

    class SCRAT_API token_data
    {
    public:
        mnemonic       c = mnemonic::noop_;
        script::type::T  t = script::type::null_t;    ///< Primitive script::type bit.
        script::type::T  s = script::type::null_t;    ///< Semantic script::type bits field
        script::type::distance::T d = script::type::distance::noop_;
        void* vdata = nullptr;

        using collection = std::vector<token_data>;
        // ----------------------------------------
        using cache = std::vector<token_data*>;
        using pointer = cache::iterator;
        // ----------------------------------------
        using iterator = collection::iterator;


        struct SCRAT_API  location_data
        {
            const char* begin = nullptr;
            const char* end = nullptr;
            int     linenum = -1; ///< line number
            int     colnum = -1; ///< Column number
            int64_t offset = -1; ///< Absolute Offset from the beginning of the Source Text.
            int     ln = 0; ///< Length of this token attribute.
            std::string operator()() const;
            std::string text() const;
            std::string position() const;
        }  mLoc;

        struct SCRAT_API Flag
        {
            uint8_t V : 1; ///< Pre-parsed as a value Token;
            uint8_t S : 1; ///< Post parsed as assignable
            uint8_t M : 1; ///< Virtual multiplication operator. (...4ac...)
        }   _flags = { 0, 0, 0 };

        static token_data mNull;

        token_data() = default;
        ~token_data() = default;

        token_data(mnemonic Code_, script::type::T Type_, script::type::T Sem_, script::type::distance::T Delta_, lexem::T aLexem, uint8_t V);
        token_data(mnemonic Code_, script::type::T Type_, script::type::T Sem_, script::type::distance::T Delta_, token_data::location_data LocationData_, token_data::Flag Flags_, void* Ptr_ = nullptr);
        token_data(const token_data& Token_);
        token_data(token_data&& Token_) noexcept;

        token_data& operator=(token_data&& Token_) noexcept;
        token_data& operator=(const token_data& Token_);
        token_data operator[](mnemonic CC);
        //ArgToken* ptr() { return this; }
        std::string mark() const;

        explicit operator bool() const
        {
            return mLoc.begin != nullptr;
        }
        bool operator||(script::type::T Sem_) const
        {
            return s || Sem_;
        }

        [[nodiscard]] std::string text() const
        {
            if ((_flags.M) && (c == mnemonic::k_mul)) return lexem::k_multiply; // Overwrite source location.
            return mLoc();
        }
        [[nodiscard]] std::string location() const;
        [[nodiscard]] std::string semantic_types() const;
        [[nodiscard]] std::string type_name() const;
        [[nodiscard]] std::string details(bool Mark_ = false) const;

        static token_data scan(const char* C_);

        [[nodiscard]] bool is_text() const
        {
            return t == type::text_t;
        }
        [[nodiscard]] bool is_binary() const
        {
            return s & script::type::binary_t;
        }
        [[nodiscard]] bool is_unary() const
        {
            return s & script::type::unary_t;
        }
        [[nodiscard]] bool is_identifier() const
        {
            return s & script::type::id_t;
        }
        [[nodiscard]] bool is_leaf() const
        {
            return s & script::type::leaf_t;
        }
        [[nodiscard]] bool is_operator() const
        {
            return s & script::type::operator_t;
        }
        [[nodiscard]] bool is_closing_pair() const
        {
            return s & script::type::close_pair_t;
        }
        [[nodiscard]] bool is_punctuation() const
        {
            return s & script::type::punc_t;
        }
        [[nodiscard]] bool is_number() const
        {
            return s & script::type::number_t;
        }
        [[nodiscard]] bool is_hexadecimal() const
        {
            return s & script::type::hex_t;
        }
        [[nodiscard]] bool is_prefix() const
        {
            return s & script::type::prefix_t;
        }
        [[nodiscard]] bool is_postfix() const
        {
            return s & script::type::postfix_t;
        }
        [[nodiscard]] bool is_instruction() const
        {
            return !(s & (script::type::operator_t | script::type::leaf_t));
        }


        static script::type::T ktype(mnemonic m);
    };
}

}
