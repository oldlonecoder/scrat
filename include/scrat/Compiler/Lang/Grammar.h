/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                         *
 *   serge.lussier@oldlonecoder.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code in this project is written       *
 *   by the author (Serge Lussier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge Lussier also apply here      *
 ***************************************************************************/


#pragma once
#include <AppBook/Book/BookEnums.h>

#include <Spp/SppToken.h>
#include <map>
#include <AppBook/Util/StrBreak.h>

#include <stack>

namespace Spp::Lang
{




class SPP_EXPORT Grammar
{
    public:
    struct SPP_EXPORT Rule;
    private:
    friend class Compiler;
    friend class Interpretr;
    public:
    /*!
        * @brief
        * @todo Holly shit!! Confused on X flag: Accept state and Implicit-Rule Flag!!!! ^^ ?
        *
        */
    struct SPP_EXPORT Properties
    {
        uint8_t Z : 1; ///< Zero or one (optional * )
        uint8_t R : 1; ///< Repeat      (        + )
        uint8_t L : 1; ///< List        (one of  ~ ?)
        uint8_t X : 1; ///< parserctrl    ( Parser to takes control of this rule: ast Parser term)
        uint8_t S : 1; ///< State!! 0 = unset/rejected; 1 = set/Accepted !!!

        Properties& operator|(Properties const& _)
        {
            Z |= _.Z;
            R |= _.R;
            L |= _.L;
            X |= _.X;
            //s = A.s;
            return *this;
        }

        Properties& operator+()
        {
            R = 1;
            return *this;
        }
        Properties& operator*()
        {
            Z = 1;
            return *this;
        }
        Properties& operator!()
        {
            X = 1;
            return *this;
        }
        Properties& operator~()
        {
            L = 1;
            return *this;
        }
        void Reset()
        {
            Z = R = L = X = S = 0;
        }
        std::string operator()();
        [[nodiscard]] bool IsOptional() const
        {
            return Z != 0;
        }
        [[nodiscard]] bool IsOneof() const
        {
            return L != 0;
        }
        [[nodiscard]] bool IsStrict() const
        {
            return Z == 0 && L == 0 && R == 0;
        }
        [[nodiscard]] bool IsRepeat() const
        {
            return R != 0;
        }
        [[nodiscard]] bool Accepted() const
        {
            return S != 0;
        }
        [[nodiscard]] bool IsParserctrl()
        {
            return X == 1;
        }

        void Accept()
        {
            S = 1;
        }
        void Reject()
        {
            S = 0;
        }
    };

    struct SPP_EXPORT Element
    {
//        AST A{}; // Which AST (group) this element is part of, if matched with the SppToken.
        mutable Properties a = { 0, 0, 0, 0, 0 }; ///< default : punctual, strict match

        enum class ElementType : uint8_t
        {
            //Element,
            R = 0,
            S,
            M,
            Nil
        };

        void Accept()
        {
            a.Accept();
        }
        void Reject()
        {
            a.Reject();
        }

        Element::ElementType _type = Element::ElementType::Nil;

        union Memory
        {
            Rule*           R;
            Type::T   Sem;
            Mnemonic        M;
        }Mem = { nullptr };

        using Array = std::vector<Element>;
        using Iterator = Array::iterator;
        using ConstIterator = Array::const_iterator;

        // pas le choix... 'y faut un parser....

        Element() = default;
        Element(Rule* r, Properties a_ = { 0, 0, 0, 0, 0 });
        Element(Type::T a_sem, Properties a_ = { 0, 0, 0, 0, 0 });
        Element(Mnemonic a_code, Properties a_ = { 0, 0, 0, 0, 0 });

        Element(const std::string& a_lexem);

        Element(Element&& _t) noexcept;
        Element(const Element& _t);

        Element& operator=(Element&& _t) noexcept;
        Element& operator=(const Element& _t);

        bool operator==(const Element& t) const;
        bool operator==(const SppToken& t) const;
        bool operator!=(const SppToken& t) const;

        operator bool()
        {
            return _type != Element::ElementType::Nil;
        }
        ~Element();

        Element& operator*()
        {
            *a;
            return *this;
        }
        Element& operator+()
        {
            +a;
            return *this;
        }
        Element& operator!()
        {
            !a;
            return *this;
        }
        Element& operator~()
        {
            ~a;
            return *this;
        }

        std::string operator()() const;

        bool IsRule() const
        {
            return _type == ElementType::R;
        }
        bool IsSemantic() const
        {
            return _type == ElementType::S;
        }
        bool IsMnemonic() const
        {
            return _type == ElementType::M;
        }
        bool IsNull() const
        {
            return _type == ElementType::Nil;
        }

        //static term query(const char *C_);
        //static term query(xio::type::T T_);
        //static term query(Mnemonic M_);
    };

    struct SPP_EXPORT ElementSeq
    {
        Properties a = { 0, 0, 0, 0, 0 }; ///< default : punctual, strict match

        //using data = std::vector<term_list_t>;

        Element::Array terms;

        using Array = std::vector<ElementSeq>;
        using const_iterator = Array::const_iterator;
        using Iterator = Array::iterator;
        using stack = std::stack<Iterator>;

        ElementSeq() = default;

        [[nodiscard]] Element::ConstIterator Begin() const
        {
            return terms.cbegin();
        }

        bool End(Element::ConstIterator t) const
        {
            return terms.cend() == t;
        }

        Element Next(Element::ConstIterator& it) const;

        ~ElementSeq()
        {
            terms.clear();
        }
        ElementSeq& operator<<(Element a_t);

        // Emplace_back:
        ElementSeq& operator<<(Type::T a_t);
        ElementSeq& operator<<(Mnemonic a_t);
        ElementSeq& operator<<(Rule* a_t);
    };

    struct SPP_EXPORT Rule
    {
        ElementSeq::Array Sequences;
        // ---------------------------------------
        ElementSeq::Iterator   SeqIt; // Temporary held for building this rule.
        // ---------------------------------------
        //term_seq::stack seq_stack;

        using Array = std::map<std::string, Rule*>;
        using Iterator = Array::const_iterator;

        Properties A = { 0, 0, 0, 0, 0 }; /// Volatile Attribute infos. ( Copied into the rule's recursion context  )
        Properties Inject = { 0, 0, 0, 0, 0 };

        std::string Id;

        Rule() = default;
        Rule(const std::string& a_id);

        Rule(int)
        {}
        ~Rule();
        bool Empty() const
        {
            return Sequences.empty() || Sequences.begin()->terms.empty();
        }
        void InjectAttr(Properties a_a)
        {
            Inject = a_a;
        }
        Rule& NewSequence();
        Rule& operator|(Rule* _r);
        //rule& operator |(const char*   _t);
        Rule& operator|(Type::T _t);
        Rule& operator|(Mnemonic _t);
        std::string Name()
        {
            return Id;
        }
        [[nodiscard]] ElementSeq::const_iterator Begin() const
        {
            return Sequences.cbegin();
        }
        bool End(ElementSeq::const_iterator s) const
        {
            return s == Sequences.cend();
        }
    };

    /*!
        @brief the tea gammar class .

        @author &copy;2021, oldlonecoder (lussier.serge@gmail.com).

        @note offset really don't know why offset need to do a complete syntax Grammar (generator) for the tea++ interpreter.
        My first intention was to create a specific arithmetic expression parser and then create a hand made parser for the the rest of the syntax.
    */

    public:
    Grammar();

    ~Grammar();
    StrAcc& Text() { return _text; }

    Book::Enums::Code Build();

    void Dump();
    static std::string DebugDumpSequence(const ElementSeq& seq);

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

    Properties A = { 0, 0, 0, 0, 0 }; ///< default : punctual, strict match

    state_mac   _state = Grammar::st_begin;

    [[maybe_unused]] int Init();
    static Rule::Array Rules;
    Rule* _rule = nullptr;
    static Rule* QueryRule(const std::string& a_id);

    using RuleScanner = Book::Enums::Code(Grammar::*)(StrBreak::Word::Iterator&);

    StrAcc    _text;
    StrBreak  _words;

    using Dictionary = std::map<char, Grammar::RuleScanner>;
    static Dictionary GrammarDictionnary;

public:
    const Rule* operator[](const std::string& r_id) const;

    static bool IsBuilt()
    {
        return !Grammar::Rules.empty();
    }

    static void InitRules();
    static void DestroyRules();
    private:
    Grammar::Dictionary::iterator RuleIt;

    //--------------- Rules builders -------------------

    Book::Enums::Code ParseIdentifier(StrBreak::Word::Iterator& crs);
    Book::Enums::Code EnterRuleDef(StrBreak::Word::Iterator& crs);
    Book::Enums::Code NewSequence(StrBreak::Word::Iterator& crs);
    Book::Enums::Code EndRule(StrBreak::Word::Iterator& crs);
    Book::Enums::Code SetRepeat(StrBreak::Word::Iterator& crs);
    Book::Enums::Code SetOptional(StrBreak::Word::Iterator& crs);
    Book::Enums::Code EnterLitteral(StrBreak::Word::Iterator& crs);
    Book::Enums::Code SetOneof(StrBreak::Word::Iterator& crs);
    Book::Enums::Code SetParserctrl(StrBreak::Word::Iterator& crs);
};

}
//#endif //VXIO_GRAMMAR_H
