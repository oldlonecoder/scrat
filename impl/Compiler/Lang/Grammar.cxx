//
// Created by oldlonecoder on 21-12-12.
//

#include <Spp/Compiler/Lang/Grammar.h>
#include <AppBook/Util/Colors.h>
#include <AppBook/Book/AppBook.h>



namespace Spp::Lang
{
//
// Created by slussier on 19-08-25.
//


std::map<Grammar::Element::ElementType, Color::Code> colours =
{
    {Grammar::Element::ElementType::M, Color::LightSteelBlue3},
    {Grammar::Element::ElementType::R, Color::LightSalmon4},
    {Grammar::Element::ElementType::S, Color::Lime},
    {Grammar::Element::ElementType::S, Color::White}
};





Grammar::Dictionary Grammar::GrammarDictionnary = {
    {':', &Grammar::EnterRuleDef},
    {'|', &Grammar::NewSequence},
    {'.', &Grammar::EndRule},
    {'+', &Grammar::SetRepeat},
    {'*', &Grammar::SetOptional},
    {'?', &Grammar::SetOneof}, // One of
    {'\'', &Grammar::EnterLitteral},
    {'"', &Grammar::EnterLitteral},
    {'#', &Grammar::SetParserctrl},
};

Grammar::Rule::Array Grammar::Rules;

std::string grammar_text = R"(
amu                : 'Amu' *Id '::' Id ';' | 'Amu' Id ';'.
stmts              : +statement.
statement          : ';' | instruction | assignstmt ';'| declvar ';'| #expression ';'| VarID ';'.
assignstmt         : declvar Assign #expression | VarID Assign #expression.
declvar            : *typename #newvar.
funcsig            : *typename function_id '(' *params ')'.
declfunc           : funcsig ';'| funcsig statement ';' | funcsig bloc.
paramseq           : ',' param.
param              : *typename Id.
function_call      : function_id '(' Args ')' | objcfncall.
params             : param *+paramseq.
objcarg            : Id ':' #expression.
arg                : objcarg | #expression.
argseq             : ',' arg.
Args               : arg *+argseq.
typename           : *'static' ?'i8' ?'u8' ?'i16' ?'u16' ?'i32' ?'u32' ?'i64' ?'u64' ?'real' ?'number' ?'string' ?#objectid.
instruction        : ?'if' ?'switch'  ?'for' ?'while' ?'repeat' ?'until' ?'do' ?'return'.
if                 : 'if' condexpr ifbody | 'if' '(' condexpr ')' ifbody.
bloc               :  '{' stmts '}'.
truebloc           : *'then' bloc | *'then' statement.
elsebloc           : 'else' bloc | 'else' statement.
ifbody             : truebloc *elsebloc.
condexpr           : assignstmt | assignstmt ';' #expression | #expression.
function_id        : *'::' #functionid | #AmuID '::' #functionid | #objectid '::' #functionid | #obj_instance '.' #functionid.
VarID              : #var_id | AmuID'::' #var_id | *'::' #var_id.
objcfncall         : '[' function_id  *Args ']'.

)";

Grammar::Grammar()
{

}

Grammar::~Grammar()
{


}

Book::Enums::Code Grammar::Build()
{
    AppBook::Info() <<  ":";
    if(IsBuilt())
    {
        AppBook::Debug() << "Grammar is already IsBuilt - ok" ;
        return Book::Enums::Code::Ok;
    }
    if(_text().empty())
        _text = grammar_text;

    AppBook::Debug()
        << Color::White << ":Building rules :\n----------------------------\n"
        << Color::Yellow << _text
        << Color::White << "\n----------------------------------\n";
    _words = StrBreak();
    StrBreak::ConfigData Data = {_text(), ":.+*?#", StrBreak::Keep};

    AppBook::Debug() << "Tokenizing rules source Text:";
    std::size_t count = _words(Data);

    auto crs=Data.Words.begin();

    if(!count)
    {
        AppBook::Fatal() << ": Grammar. (internal error)";
        return Book::Enums::Code::Empty;
    }

    AppBook::Debug() << Data.Words.size() << " data.words.";
    _state = st_begin;
    AppBook::Debug() << " state machine set to '" << Color::Yellow << "st_begin" << Color::Reset << "'";
    do
    {
        Book::Enums::Code r;
        auto      p = Grammar::GrammarDictionnary.find((*crs)().c_str()[0]);

        if(p != Grammar::GrammarDictionnary.end())
        {
            r = (this->*(p->second))(crs);
        }
        else
        {
            r = ParseIdentifier(crs);
        }
        if(r != Book::Enums::Code::Accepted)
            return r;
    }
    while(crs != Data.Words.end());

    AppBook::Message() << " finalizing rules:";

    for (auto& [rule_name, rule_addr] : Rules)
    {
        StrAcc str;
        str << "'" << Color::Yellow << rule_name << Color::Reset << "' :";
        if (rule_addr->Empty())
        {
            str << " set to direct parser control";
            rule_addr->A.X = 1;
        }
        else
            str << "has " << Color::Yellow << rule_addr->Sequences.size() << Color::Reset << " sequence(s) of terms ( and / or sub rules).";
        AppBook::Info() << str;
    }

    AppBook::Message() << "done building rules...";
    return Book::Enums::Code::Accepted;
}

std::string Grammar::DebugDumpSequence(const ElementSeq& seq)
{
    StrAcc Out;
    Out << Color::White << " | ";// << Ends;
    for(auto& t: seq.terms) Out << t() << ' ';// << Ends;
    Out << Color::Reset;
    return Out();
}


void Grammar::Dump()
{

    AppBook::Out() << Color::Red4 << "%-15s" << "rule" <<  Color::Black << " | terms sequence:";
    AppBook::Out() << "-----------------------------------------------------------------------------------";
    StrAcc Out;
    for(const auto &rule: Rules)
    {
        Out << Color::Red4 << "%-15s" << rule.second->Id;
        for(auto const& seq: rule.second->Sequences)
        {
            StrAcc SeqStr;
            SeqStr << DebugDumpSequence(seq);
            Out << SeqStr;
        }
        Out << '\n';
    }
    AppBook::Out() << Out();
}

void Grammar::InitRules()
{

}

Book::Enums::Code Grammar::ParseIdentifier(StrBreak::Word::Iterator  &crs)
{
    Rule *r = QueryRule((*crs)());
    switch(_state)
    {
        case st_begin:
        //AppBook::Debug() << " st_begin:" << (*crs)();
            if(r != nullptr)
            {
                if(!r->Empty())
                {
                    AppBook::Fatal() << " rule, identified by '" << Color::Yellow << (*crs)() << Color::Red4 <<" already exists in the context of a new rule definition = " << Book::Enums::Code::Rejected;
                    return Book::Enums::Code::Rejected;
                }
                _rule = r;
            }
            else
            {
                std::string rule_name = (*crs)();
                _rule = new Rule(rule_name);
                _rule->A = A;
                Rules[rule_name] = _rule;
            }
            A.Reset();
            _state = st_init_rule; //  expect ':' as next token in main loop.
            break;
        case st_init_rule:_state = st_seq;
            break;
        default: break;
        case st_option:
        case st_seq:_state = st_seq;
            // lexem::T ?
            /*lexer::lexem::lexer::lexem::Mnemonic m = lexem::m(crs->c_str());
            if( m != lexer::lexem::lexer::lexem::Mnemonic::knull ) {
                _rule->a = a;
                (*_rule) | m;
                a.reset();
                break;
            }*/
            //AppBook::Out() << " `in sequence state`: Checking type (token text) '" << Color::LightGoldenrod3 << (*crs)() << Color::Reset << "':";
            Type::T t = Type::FromStr((*crs)());
            //AppBook::Debug() << " Spp:type(" << Color::Yellow <<  (*crs)() << Color::Reset << "):'" << Type::name(t);
            if(t)// & teacc::type::bloc_t) // Quick and d_words.config() = data;irty hack about bypassing the lexer::teacc::type::bloc type:
            {
               // AppBook::Out() << " type name:'" << type::name(t) << "' <==> token:'" << (*crs)() << " properties: [" << a() << "]";
                _rule->A = A;
                (*_rule) | t;
                A.Reset();
                break;
            }

            if(r)
            {
                //AppBook::Out() << " rule name:'" << _rule->name() << "' <==> token:'" << (*crs)() << " properties: [" << a() << "]";
                _rule->A = A;
                (*_rule) | r;
                A.Reset();
                break;
            }
            else
            {
                r = new Rule((*crs)());
                //AppBook::Out() << "Creating new forward rule ( rule term ) identified by:" << Color::LightGoldenrod4 << r->name()
                //    << Color::Reset << " - properties: " << a();
                Rules[(*crs)()] = r;
                _rule->A = A;
                _state = st_seq; //  expect ':' as next token in main loop.
                (*_rule) | r;
                A.Reset();
            }
            break;
    }
    ++crs;
    return Book::Enums::Code::Accepted;
}

Book::Enums::Code Grammar::EnterRuleDef(StrBreak::Word::Iterator &crs)
{
    // ':' :
    if(_state == st_init_rule)
    {
        _state = st_seq;
        A.Reset();
        ++crs;
        return Book::Enums::Code::Accepted;
    }
    AppBook::Fatal() << " misplaced ':' ";
    return Book::Enums::Code::Rejected;
}

Book::Enums::Code Grammar::NewSequence(StrBreak::Word::Iterator &crs)
{
    // logdebug
    //     << Color::HCyan << __FUNCTION__
    //     << Color::White << ": ["
    //     << Color::Yellow << *crs
    //     << Color::White << ']'
    //     << Ends;

    if(_state == st_option)
    {
        AppBook::Fatal() << " syntax error: " << (*crs)() << "' is invalid in this context";
        return Book::Enums::Code::Rejected;
    }

    _rule->NewSequence();
    _state = st_seq;
    A.Reset();
    ++crs;
    return Book::Enums::Code::Accepted;
}

Book::Enums::Code Grammar::EndRule(StrBreak::Word::Iterator &crs)
{
    // logdebug
    //     << Color::HCyan << __FUNCTION__
    //     << Color::White << ": ["
    //     << Color::Yellow << *crs
    //     << Color::White << ']'
    //     << Ends;
    _state = st_begin;
    ++crs;
    return Book::Enums::Code::Accepted;
}

Book::Enums::Code Grammar::SetRepeat(StrBreak::Word::Iterator &crs)
{
    //logdebug
    //    << Color::HCyan << __FUNCTION__
    //    << Color::White << ": ["
    //    << Color::Yellow << *crs
    //    << Color::White << ']'
    //    << Ends;
    _state = st_option;
    +A;
    ++crs;
    return Book::Enums::Code::Accepted;
}

Book::Enums::Code Grammar::SetParserctrl(StrBreak::Word::Iterator &crs)
{
    !A;
    _state = st_option;
    /*
               AppBook::Debug()
               << Color::Yellow << a()
               << Color::White << ": ["
               << Color::Yellow << *crs
               << Color::White << ']';
   */

    ++crs;
    return Book::Enums::Code::Accepted;
}

Book::Enums::Code Grammar::SetOptional(StrBreak::Word::Iterator &crs)
{
    //logdebug
    //    << Color::HCyan << __FUNCTION__
    //    << Color::White << ": ["
    //    << Color::Yellow << *crs
    //    << Color::White << ']'
    //    << Ends;
    *A;
    ++crs;
    _state = st_option;
    return Book::Enums::Code::Accepted;
}

Book::Enums::Code Grammar::EnterLitteral(StrBreak::Word::Iterator &crs)
{

    //    AppBook::Debug()
    //    << Color::White << ": ["
    //    << Color::Yellow << *crs
    //    << Color::White << ']';

    if((_state != st_seq) && (_state != st_option))
    {
        AppBook::Fatal() << "syntax error '" << (*crs)() << "' is not a valid s++ Grammar token in context" << "(state machine:" << (int) _state << ")";
        return Book::Enums::Code::Rejected;
    }

    StrBreak::Word::Iterator i, quote;
    i = quote = crs;
    ++i;
    if((*i)() == (*quote)())
    {
        AppBook::Fatal() << "error: litteral Grammar element cannot be Empty";
        return Book::Enums::Code::Rejected;
    }

    SppToken token = SppToken::Scan((*i)().c_str());
    if(token)
    {
        _rule->A = A;
        (*_rule) | token.M;
        A.Reset();
    }
    else
    {
        AppBook::Fatal() << "syntax error '" << (*i)() << "' is not a valid s++ Grammar token";
        return Book::Enums::Code::Rejected;
    }
    crs = i;
    ++crs;
    if((*crs)() == (*quote)())
        ++crs;
    //++crs; // will be on the next token.

    return Book::Enums::Code::Accepted;
}

Book::Enums::Code Grammar::SetOneof(StrBreak::Word::Iterator &crs)
{
    //     logdebug
    //         << Color::HCyan << __FUNCTION__
    //         << Color::White << ": ["
    //         << Color::Yellow << *crs
    //         << Color::White << ']'
    //         << Ends;
    ~A;
    ++crs;
    return Book::Enums::Code::Accepted;
}

[[maybe_unused]] int Grammar::Init()
{
    _text = "";
    return 0;
}

Grammar::Rule *Grammar::QueryRule(const std::string &a_id)
{
    //AppBook::Debug() << " '" << Color::Yellow << rule_id << Color::Reset << "':";
    auto i = Grammar::Rules.find(a_id);
    return i != Grammar::Rules.end() ? i->second : nullptr;
}


const Grammar::Rule *Grammar::operator[](const std::string &r_id) const
{
    AppBook::Debug() << " query: '" << r_id << "':\n";
    auto i = Grammar::Rules.find(r_id);
    if(i == Grammar::Rules.end())
    {
        //Book::error() << " no such rule: '" << r_id << "'";
        return nullptr;
    }
    AppBook::Debug() << " rule '" << r_id << "' found.";
    return i->second;
}
void Grammar::DestroyRules()
{
     for(auto const& rit: Grammar::Rules)
     {
         Rule *r = rit.second;
         delete r;
     }
    Grammar::Rules.clear();
}

Grammar::Element::Element(Rule *r, Properties a_)
{
    a = a_;
    Mem.R = r;
    _type = Element::ElementType::R;
}

Grammar::Element::Element(Spp::Type::T a_sem, Properties a_)
{
    a = a_;
    Mem.Sem = a_sem;
    _type = Element::ElementType::S;
}

Grammar::Element::Element(Mnemonic a_code, Properties a_)
{
    a = a_;
    Mem.M = a_code;
    _type = Element::ElementType::M;
}

Grammar::Element::Element(const std::string &a_lexem)
{
    _type = Element::ElementType::M;
    Mem.M = Lexem::FromStr(a_lexem);
}

Grammar::Element::Element(Element &&_t)
noexcept
{
    //     logdebugfn << ":" << Ends;
    using std::swap;
    swap(Mem, _t.Mem);
    _type = _t._type;
    swap(a, _t.a);
}

Grammar::Element::Element(const Grammar::Element &_t)
{
    Mem = _t.Mem;
    _type  = _t._type;
    a      = _t.a;
}

Grammar::Element & Grammar::Element::operator=(Element &&_t) noexcept
{
    using std::swap;
    swap(Mem, _t.Mem);
    _type = _t._type;
    swap(a, _t.a);
    return *this;
}

Grammar::Element & Grammar::Element::operator=(const Element &_t)
{
    Mem = _t.Mem;
    _type  = _t._type;
    a      = _t.a;
    return *this;
}

bool Grammar::Element::operator==(const Element &t) const
{
    if(_type != t._type)
        return false;

    switch(_type)
    {
        case ElementType::M:return Mem.M == t.Mem.M;
        case ElementType::R:return Mem.R == t.Mem.R;
        case ElementType::S:return (Mem.Sem & t.Mem.Sem) != 0;
        case ElementType::Nil:return false;
    }
    return false;
}

bool Grammar::Element::operator==(const SppToken &t) const
{
    switch(_type)
    {
        case ElementType::M:return Mem.M == t.M;
        case ElementType::S:return (Mem.Sem & t.S) != 0;
        case ElementType::Nil:
        default:break;
    }
    return false;
}

bool Grammar::Element::operator!=(const SppToken &t) const
{
    switch(_type)
    {
        case ElementType::M:return Mem.M != t.M;
        case ElementType::S:return (Mem.Sem & t.S) == 0;
        case ElementType::Nil:
        default:break;
    }
    return true;
}

Grammar::Element::~Element()
{
    if(Grammar::Rules.empty()) return;
    for(auto [id, R] : Grammar::Rules)
    {
        if(!R) continue;

    }

}

std::string Grammar::Element::operator()() const
{
    StrAcc str;
    str << a() << colours[_type];

    switch(_type)
    {
        case Element::ElementType::M:
        {
            SppToken tok = SppToken()[Mem.M];
            str << tok.Text(); // String (TEXT)
        }
            break;
        case Element::ElementType::R:
            // Can't happen but we never know: (nullptr)
            if(Mem.R)
                str << Mem.R->Id;
            break;
        case Element::ElementType::S:
            str << Type::Name(Mem.Sem);
            break;
        default:str << "nil";
            break;
    }

    return str();
}

Grammar::Rule::Rule(const std::string &a_id)
{
    Id = a_id;
    Sequences.emplace_back();
    SeqIt = Sequences.begin();
}

Grammar::Rule::~Rule()
{
    Sequences.clear();
    Id.clear();
}

Grammar::Rule & Grammar::Rule::NewSequence()
{
    Sequences.emplace_back();
    SeqIt = --Sequences.end();
    A.Reset();
    return *this;
}

Grammar::Rule & Grammar::Rule::operator|(Rule *_r)
{
    Element t = Element(_r, _r->A);
    t.a = A;
    A.Reset();
    *SeqIt << t;
    return *this;
}

Grammar::Rule & Grammar::Rule::operator|(Type::T _t)
{
    Element t = Element(_t);
    t.a = A;
    A.Reset();
    *SeqIt << t;
    return *this;
}

Grammar::Rule & Grammar::Rule::operator|(Mnemonic _t)
{
    Element t = Element(_t);
    t.a = A;
    A.Reset();
    *SeqIt << t;
    return *this;
}

Grammar::Element Grammar::ElementSeq::Next(Element::ConstIterator &it) const
{
    if(it != terms.end())
        ++it;
    return *it;
}

Grammar::ElementSeq & Grammar::ElementSeq::operator<<(Element a_t)
{
    terms.push_back(a_t);
    return *this;
}

Grammar::ElementSeq & Grammar::ElementSeq::operator<<(Type::T a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

Grammar::ElementSeq & Grammar::ElementSeq::operator<<(Mnemonic a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

Grammar::ElementSeq & Grammar::ElementSeq::operator<<(Rule *a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

std::string Grammar::Properties::operator()()
{
    StrAcc str;
    if(Z)
        str << Color::LightCyan3 << '*';
    if(R)
        str << Color::White << '+';
    if(L)
        str << Color::CadetBlue << '?';
    if(X)
        str << Color::LighcoreateBlue << '#';
    return str();
}

}

