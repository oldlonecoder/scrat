//
// Created by oldlonecoder on 21-12-12.
//



/**********************************************************************************************************************************
    Copyright 2021 Serge Lussier (oldlonecoder), lussier.serge@gmail.com
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
    THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*************************************************************************************************************************************/


#pragma once

#include <scrat/result.h>
#include <scrat/lexer/tokendata.h>
#include <scrat/text.h>
#include <map>
#include <stack>

namespace scrat::script
{
//struct rule;

//class grammar;

/*!
 *    @brief the scrat gammar class .
 *
 *    @author &copy;2021, oldlonecoder (lussier.serge@gmail.com).
 *
 *    @note offset really don't know why offset need to do a complete syntax grammar (generator) for the scrat++ interpreter.
 *    My first intention was to create a specific arithmetic expression parser and then create a hand made parser for the the rest of the syntax.
 */
class SCRAT_API grammar
{
public:
    struct SCRAT_API rule;

    /*!
     * @brief
     * @todo Holly shit!! Confused on X flag: accept state and Implicit-Rule Flag!!!! ^^ ?
     *
     */
    struct SCRAT_API term_properties
    {
        uint8_t Z: 1; ///< Zero or one (optional * )
        uint8_t R: 1; ///< Repeat      (        + )
        uint8_t L: 1; ///< List        (one of  ~ ?)
        uint8_t X: 1; ///< directive    ( exclude from normal ast build : ast directive [Implicit specific Parser term_rule)
        uint8_t S: 1; ///< State!! 0 = unset/rejected; 1 = set/accepted !!!

        term_properties &operator|(term_properties const &_)
        {
            Z |= _.Z;
            R |= _.R;
            L |= _.L;
            X |= _.X;
            //s = A.s;
            return *this;
        }

        term_properties &operator+()
        {
            R = 1;
            return *this;
        }
        term_properties &operator*()
        {
            Z = 1;
            return *this;
        }
        term_properties &operator!()
        {
            X = 1;
            return *this;
        }
        term_properties &operator~()
        {
            L = 1;
            return *this;
        }
        void Reset()
        { Z = R = L = X = S = 0; }
        std::string operator()();
        [[nodiscard]] bool is_optional() const
        { return Z != 0; }
        [[nodiscard]] bool is_oneof() const
        { return L != 0; }
        [[nodiscard]] bool is_strict() const
        { return Z == 0 && L == 0 && R == 0; }
        [[nodiscard]] bool is_repeat() const
        { return R != 0; }
        [[nodiscard]] bool accepted() const
        { return S != 0; }
        void accept()
        { S = 1; }
        void reject()
        { S = 0; }
    };

    struct SCRAT_API term
    {
        mutable term_properties a = {0, 0, 0, 0, 0}; ///< default : punctual, strict match

        enum class type : uint8_t
        {
            //term,
            r = 0,
            s,
            m,
            nil
        };

        void accept() { a.accept(); }
        void Reject() { a.reject(); }

        term::type _type  = term::type::nil;

        union info
        {
            grammar::rule *r;
            scrat::script::type::T sem;
            scrat::script::mnemonic      m;
        }object = {nullptr};

        using collection = std::vector<term>;
        using iterator = collection::iterator;
        using const_iterator = collection::const_iterator;

        // pas le choix... 'y faut un parser....

        term() = default;
        term(grammar::rule *r, grammar::term_properties a_ = {0, 0, 0, 0, 0});
        term(script::type::T a_sem, grammar::term_properties a_ = {0, 0, 0, 0, 0});
        term(mnemonic a_code, grammar::term_properties a_ = {0, 0, 0, 0, 0});

        term(const std::string &a_lexem);

        term(term &&_t) noexcept;
        term(const term &_t);

        term &operator=(term &&_t) noexcept;
        term &operator=(const term &_t);

        bool operator==(const term &t) const;
        bool operator==(const token_data &t) const;
        bool operator!=(const token_data &t) const;

        operator bool()
        { return _type != term::type::nil; }
        ~term();

        term &operator*()
        {
            *a;
            return *this;
        }
        term &operator+()
        {
            +a;
            return *this;
        }
        term &operator!()
        {
            !a;
            return *this;
        }
        term &operator~()
        {
            ~a;
            return *this;
        }

        bool is_optional() { return a.Z;}
        bool is_oneof() {return a.L;}
        bool is_repeat() { return a.R; }

        std::string operator()() const;

        bool is_rule() const
        { return _type == type::r; }
        bool is_semantic() const
        { return _type == type::s; }
        bool is_mnemonic() const
        { return _type == type::m; }
        bool is_null() const
        { return _type == type::nil; }

        //static term query(const char *C_);
        //static term query(type::T T_);
        //static term query(mnemonic M_);
    };

    struct SCRAT_API term_seq
    {
        grammar::term_properties a = {0, 0, 0, 0, 0}; ///< default : punctual, strict match


        grammar::term::collection terms;

        using collection = std::vector<term_seq>;
        using const_iterator = collection::const_iterator;
        using iterator = collection::iterator;
        using stack = std::stack<iterator>;

        term_seq() = default;

        term::const_iterator begin() const
        { return terms.cbegin(); }

        bool end(term::const_iterator t) const
        {
            return terms.cend() == t;
        }

        grammar::term next(grammar::term::const_iterator &it) const;

        ~term_seq()
        {
            terms.clear();
        }
        term_seq &operator<<(grammar::term a_t);

        // Emplace_back:
        term_seq &operator<<(type::T a_t);
        term_seq &operator<<(mnemonic a_t);
        term_seq &operator<<(grammar::rule *a_t);
    };

    struct SCRAT_API rule
    {
        grammar::term_seq::collection sequences;
        // ---------------------------------------
        grammar::term_seq::iterator   seq; // Temporary held for building this rule.
        // ---------------------------------------
        //term_seq::stack seq_stack;

        using collection = std::map<std::string, rule *>;
        using iterator = collection::const_iterator;

        grammar::term_properties a      = {0, 0, 0, 0, 0}; /// Volatile attribute infos. ( Copied into the rule's recursion context  )
        grammar::term_properties inject = {0, 0, 0, 0, 0};

        std::string _id;

        rule() = default;
        rule(const std::string &a_id);

        rule(int)
        {}
        ~rule();
        bool empty() const
        { return sequences.empty() ? true : sequences.begin()->terms.empty(); }
        void inject_attr(term_properties a_a)
        { inject = a_a; }
        rule &new_sequence();
        rule &operator|(rule *_r);
        //rule& operator |(const char*   _t);
        rule &operator|(type::T _t);
        rule &operator|(mnemonic _t);
        std::string name() const
        { return _id; }
        grammar::term_seq::const_iterator begin() const
        { return sequences.cbegin(); }
        bool end(grammar::term_seq::const_iterator s) const
        { return s == sequences.cend(); }
    };


public:
    grammar();

    ~grammar();
    stracc &text() { return _text; }

    rem::code build();

    void dump();
    std::string dump_sequence(const term_seq& seq);

private:
    enum state_mac
    {
        st_begin = 0, ///< about to define new rule: cursor on rule id.
        st_init_rule, ///< initialize sequence of terms: cursor on ':'.
        st_seq,       ///< back and forth between st_seq<->st_init_term.
        st_option,    ///< one of ' *+? ' option state.
        st_init_term, ///< query {rule | type semantic | e_code | litteral} : cursor on identifier.
        st_add_seq,   ///< terminate current sequence and start a new sequence on the rule: cursor on ','.
        st_end_rule   ///< terminate rule : cursor on '.'.
    };

    term_properties a = {0, 0, 0, 0, 0}; ///< default : punctual, strict match

    state_mac               _state = grammar::st_begin;

    [[maybe_unused]] int init();
    rule::collection rules;
    rule                    *_rule = nullptr;
    rule *query_rule(const std::string &a_id);

    using RuleScanner = rem::code(grammar::*)(stracc::iterator &);
    text_tokenizer::token_t::list tokens;
    stracc               _text;

    using Dictionary = std::map<char, grammar::RuleScanner>;
    static Dictionary grammar_dictionnary;

public:
    const rule *operator[](const std::string &r_id) const;
    size_t clear();
    void static destroy_rules();

    bool built()
    { return rules.size() != 0; }

    //static void destroy_default_rules();
private:
    grammar::Dictionary::iterator RuleIt;

    //--------------- Rules builders -------------------

    rem::code parse_identifier(stracc::iterator &crs);
    rem::code enter_rule_def(stracc::iterator &crs);
    rem::code new_sequence(stracc::iterator &crs);
    rem::code end_rule(stracc::iterator &crs);
    rem::code set_repeat(stracc::iterator &crs);
    rem::code set_optional(stracc::iterator &crs);
    rem::code enter_litteral(stracc::iterator &crs);
    rem::code set_oneof(stracc::iterator &crs);
    rem::code set_internal(stracc::iterator &crs);


    //--------------------------------------------------




};

}

