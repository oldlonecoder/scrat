//
// Created by oldlonecoder on 21-12-12.
//

#include <scrat/interpret/compiler/grammar.h>

namespace scrat::script
{
//
// Created by slussier on 19-08-25.
//



grammar::Dictionary grammar::grammar_dictionnary = {
    {':', &grammar::enter_rule_def},
    {'|', &grammar::new_sequence},
    {'.', &grammar::end_rule},
    {'+', &grammar::set_repeat},
    {'*', &grammar::set_optional},
    {'?', &grammar::set_oneof}, // One of
    {'\'', &grammar::enter_litteral},
    {'"', &grammar::enter_litteral},
    {'@', &grammar::set_internal},
};


grammar::rule::list default_rules;
static bool      built                            = false;

std::string grammar_text = R"(
module             : 'module' id_t ';'.
declarations       : declvar assign_t @expression | declfunc | declvar.
stmts              : +statement.
statement          : ';' | instruction | assignstmt ';'| declvar ';'| @expression ';'| @varid ';'.
assignstmt         : declvar assign_t @expression ';' | @varid assign_t @expression ';'.
declvar            : *typename @newvar.
funcsig            : *typename function_id '(' *params ')'.
declfunc           : funcsig ';'| funcsig statement ';' | funcsig bloc.
paramseq           : ',' param.
param              : *typename id_t.
function_call      : function_id '(' args ')' | objcfncall.
params             : param *+paramseq.
objcarg            : id_t ':' @expression.
arg                : objcarg | @expression.
argseq             : ',' arg.
args               : arg *+argseq.
typename           : *'static' ?'i8' ?'u8' ?'i16' ?'u16' ?'i32' ?'u32' ?'i64' ?'u64' ?'real' ?'number' ?'string' ?@objectid.
instruction        : ?'if' ?'then'  ?'switch' ?'case' ?'for' ?'while' ?'do' ?'return' ?'loop'.
if                 : 'if' condexpr ifbody | 'if' '(' condexpr ')' ifbody.
bloc               :  '{' stmts '}'.
truebloc           : *'then' bloc | *'then' statement.
elsebloc           : 'else' bloc | 'else' statement.
ifbody             : truebloc *elsebloc.
loop_keyword       : ?'loop' ?'for' ?'do' ?'while'.
loop_params        : *assignstmt | *@expression | @container ':' range | @newvar : @container.
loop               : loop_keyword loop_params bloc.
range              : '[' @expression '..' @expression ']'.
condexpr           : assignstmt | @expression.
function_id        : *'::' @functionid | @objectid '::' @functionid | @obj_instance '.' @functionid.
objcfncall         : '[' function_id  *args ']'.

)";

// Objective-C member-function call: [ objectinstance.function-member param_name: expr, param_name:expr, ... ];

grammar::grammar()
{

}

grammar::~grammar()
{


}

rem::code grammar::build()
{
    rem::push_info(source_ffl) <<  ":";
    if(built())
    {
       rem::push_debug() << "grammar is already built - ok";
        return rem::ok;
    }
    if(_text().empty())
        _text = grammar_text;

   rem::push_debug()
        << color::White << ":Building rules :\n----------------------------\n"
        << color::Yellow << _text()
        << color::White << "\n----------------------------------\n";

    text_tokenizer txt = _text().c_str();
    std::size_t   count = txt(tokens, ":.+*?@", true);
    stracc::list List;
   rem::push_debug() << "building words list...";
    for(auto s: tokens)
        List.push_back(s());

    if(!count)
    {
        rem::push_fatal() << ": grammar. (internal error)";
        return rem::empty;
    }

   rem::push_debug() << List.size() << " words.";
    auto s = List.begin();
    _state = st_begin;
    do
    {
        rem::code r;
        auto      p = grammar::grammar_dictionnary.find((*s)[0]);
        if(p != grammar::grammar_dictionnary.end())
        {
            r = (this->*(p->second))(s);
        }
        else
        {
            r = parse_identifier(s);
        }
        if(r != rem::accepted)
            return r;
    }
    while(s != List.end());
   rem::push_debug() << ":";
    // dump(); // disabled
    return rem::accepted;
}

std::string grammar::dump_sequence(const grammar::term_seq& seq)
{
    stracc Out;
    Out << color::White << " | ";// << Ends;
    for(auto t: seq.terms)
    {
        if(t.a.is_oneof())
            Out << color::Cornsilk1;
        else if(t.a.is_optional())
            Out << color::Wheat4;
        else if(t.a.is_strict())
            Out << color::RoyalBlue1;
        else if(t.a.X)
            Out << color::RoyalBlue1;
        else
            Out << color::Black;

        switch(t._type)
        {
            case term::type::m:Out << color::LightCoral;
            break;
            case term::type::r:
            {
                if(t.a.X)
                    Out << color::RoyalBlue1;
                else
                    Out << color::Orange1;
            }
            break;
            case term::type::s:
                Out << color::LightCyan3;
                break;
            default: break;
        }
        Out << t() << ' ';// << Ends;
    }
    Out << color::Reset;
    return Out();
}


void grammar::dump()
{

    rem::push_info() << color::LightCoral << "mnemonic" << color::White << ',' <<
                   color::Orange1 << "rule" << color::White << ',' <<
                   color::LightCyan3 << "semantic type" << color::White << ',' << color::RoyalBlue1 << " Strict (implicit) rule:\n";
    stracc Out;
    for(const auto &rule: rules)
    {
        Out << color::Red1 << "%-15s" << rule.second->_id << color::White << ':';

        for(auto seq: rule.second->sequences)
        {
            stracc SeqStr;
            SeqStr <<  dump_sequence(seq);
            Out << SeqStr();
        }
        Out << '\n';
    }
    rem::push_output() << rem::endl  << Out();
}


rem::code grammar::parse_identifier(stracc::iterator &crs)
{
    rule *r = query_rule(*crs);
    switch(_state)
    {
        case st_begin:
            if(r)
            {
                if(!r->empty())
                {
                    rem::push_fatal() << " rule, named: " << *crs << " already exists in the context of a new rule definition.";
                    return rem::rejected;
                }
                _rule = r;
            }
            else
            {
                _rule = new rule(*crs);
                rules[*crs] = _rule;
            }
            a.Reset();
            _state = st_init_rule; //  expect ':' as next token in main loop.
            break;
        case st_init_rule:_state = st_seq;
            break;
        case st_option:
        case st_seq:
        {
            _state = st_seq;
            // lexem::T ?
            /*lexer::lexem::lexer::lexem::mnemonic m = lexem::m(crs->c_str());
            if( m != lexer::lexem::lexer::lexem::mnemonic::knull ) {
                _rule->a = a;
                (*_rule) | m;
                a.reset();
                break;
            }*/

            type::T t = type::from_str(*crs);
            if(t)// & scratcc::type::bloc_t) // Quick and dirty hack about bypassing the lexer::scratcc::type::bloc type:
            {
                _rule->a = a;
                (*_rule) | t;
                a.Reset();
                break;
            }

            if(r)
            {
                _rule->a = a;
                (*_rule) | r;
                a.Reset();
                break;
            }
            else
            {
                r = new rule(*crs);
                rules[*crs] = r;
                _rule->a = a;
                _state = st_seq; //  expect ':' as next token in main loop.
                (*_rule) | r;
                a.Reset();
            }
            break;
        }
        default: break;
    }
    ++crs;
    return rem::accepted;
}

rem::code grammar::enter_rule_def(stracc::iterator &crs)
{
    // ':' :
    if(_state == st_init_rule)
    {
        _state = st_seq;
        a.Reset();
        ++crs;
        return rem::accepted;
    }
    rem::push_fatal(source_ffl) << " misplaced ':' ";
    return rem::rejected;
}

rem::code grammar::new_sequence(stracc::iterator &crs)
{
    // logdebug
    //     << color::HCyan << __FUNCTION__
    //     << color::White << ": ["
    //     << color::Yellow << *crs
    //     << color::White << ']'
    //     << Ends;

    if(_state == st_option)
    {
        rem::push_fatal() << " syntax error: " << *crs << "' is invalid in this context";
        return rem::rejected;
    }

    _rule->new_sequence();
    _state = st_seq;
    a.Reset();
    ++crs;
    return rem::accepted;
}

rem::code grammar::end_rule(stracc::iterator &crs)
{
    // logdebug
    //     << color::HCyan << __FUNCTION__
    //     << color::White << ": ["
    //     << color::Yellow << *crs
    //     << color::White << ']'
    //     << Ends;
    _state = st_begin;
    ++crs;
    return rem::accepted;
}

rem::code grammar::set_repeat(stracc::iterator &crs)
{
    //logdebug
    //    << color::HCyan << __FUNCTION__
    //    << color::White << ": ["
    //    << color::Yellow << *crs
    //    << color::White << ']'
    //    << Ends;
    _state = st_option;
    +a;
    ++crs;
    return rem::accepted;
}

rem::code grammar::set_internal(stracc::iterator &crs)
{
    !a;
    _state = st_option;
    /*
              rem::push_debug()
               << color::Yellow << a()
               << color::White << ": ["
               << color::Yellow << *crs
               << color::White << ']';
   */

    ++crs;
    return rem::accepted;
}

rem::code grammar::set_optional(stracc::iterator &crs)
{
    //logdebug
    //    << color::HCyan << __FUNCTION__
    //    << color::White << ": ["
    //    << color::Yellow << *crs
    //    << color::White << ']'
    //    << Ends;
    *a;
    ++crs;
    _state = st_option;
    return rem::accepted;
}

rem::code grammar::enter_litteral(stracc::iterator &crs)
{

    //   rem::push_debug()
    //    << color::White << ": ["
    //    << color::Yellow << *crs
    //    << color::White << ']';

    if((_state != st_seq) && (_state != st_option))
    {
        rem::push_fatal() << "syntax error '" << *crs << "' is not a valid xio interpret grammar token in context" << "(state machine:" << (int) _state << ")";
        return rem::rejected;
    }

    stracc::iterator i, quote;
    i = quote = crs;
    ++i;
    if(*i == *quote)
    {
        rem::push_fatal() << "error: litteral grammar element cannot be empty";
        return rem::rejected;
    }

    token_data token = token_data::scan(i->c_str());
    if(token)
    {
        _rule->a = a;
        (*_rule) | token.c;
        a.Reset();
    }
    else
    {
        rem::push_fatal() << "syntax error '" << *i << "' is not a valid xio grammar token";
        return rem::rejected;
    }
    crs = i;
    ++crs;
    if(*crs == *quote)
        ++crs;
    //++crs; // will be on the next token.

    return rem::accepted;
}

rem::code grammar::set_oneof(stracc::iterator &crs)
{
    //     logdebug
    //         << color::HCyan << __FUNCTION__
    //         << color::White << ": ["
    //         << color::Yellow << *crs
    //         << color::White << ']'
    //         << Ends;
    ~a;
    ++crs;
    return rem::accepted;
}

[[maybe_unused]] int grammar::init()
{
    _text = "";
    return 0;
}

grammar::rule *grammar::query_rule(const std::string &a_id)
{
    auto i = rules.find(a_id);
    return i == rules.end() ? nullptr : i->second;
}


const grammar::rule *grammar::operator[](const std::string &r_id) const
{
    rem::push_debug(source_ffl) << " query: '" << r_id << "':\n";
    auto i = grammar::rules.find(r_id);
    if(i == grammar::rules.end())
    {
        rem::push_error() << " no such rule: '" << r_id << "'";
        return nullptr;
    }
   rem::push_debug() << " rule '" << r_id << "' found.";
    return i->second;
}
size_t grammar::clear()
{
    auto sz = rules.size();
    for (auto [id,r] : rules)
    {
        delete r;
    }
    return sz;
}

void grammar::destroy_rules()
{
    for (auto[id,r] : default_rules)
     {
        delete r;
     }
    default_rules.clear();
}


grammar::term::term(grammar::rule *r, grammar::term_properties a_)
{
    a = a_;
    object.r = r;
    _type = term::type::r;
}

grammar::term::term(script::type::T a_sem, grammar::term_properties a_)
{
    a = a_;
    object.sem = a_sem;
    _type = term::type::s;
}

grammar::term::term(mnemonic a_code, term_properties a_)
{
    a = a_;
    object.m = a_code;
    _type = term::type::m;
}

grammar::term::term(const std::string &a_lexem)
{
    _type = term::type::m;
    object.m = lexem::from_str(a_lexem);
}

grammar::term::term(term &&_t)
noexcept
{
    //     logdebugfn << ":" << Ends;
    using std::swap;
    swap(object, _t.object);
    _type = _t._type;
    swap(a, _t.a);
}

grammar::term::term(const term &_t)
{
    object = _t.object;
    _type  = _t._type;
    a      = _t.a;
}

grammar::term &grammar::term::operator=(term &&_t) noexcept
{
    using std::swap;
    swap(object, _t.object);
    _type = _t._type;
    swap(a, _t.a);
    return *this;
}

grammar::term &grammar::term::operator=(const term &_t)
{
    object = _t.object;
    _type  = _t._type;
    a      = _t.a;
    return *this;
}

bool grammar::term::operator==(const term &t) const
{
    if(_type != t._type)
        return false;

    switch(_type)
    {
        case type::m:return object.m == t.object.m;
        case type::r:return object.r == t.object.r;
        case type::s:return (object.sem & t.object.sem) != 0;
        case type::nil:return false;
    }
    return false;
}

bool grammar::term::operator==(const token_data &t) const
{
    switch(_type)
    {
        case type::m:return object.m == t.c;
        case type::s:return (object.sem & t.s) != 0;
        case type::nil:
        default:return false;
    }
    return false;
}

bool grammar::term::operator!=(const token_data &t) const
{
    switch(_type)
    {
        case type::m:return object.m != t.c;
        case type::s:return (object.sem & t.s) == 0;
        case type::nil:
        default:return true;
    }
    return true;
}

grammar::term::~term()
{
}

std::string grammar::term::operator()() const
{
    stracc str;
    str << a();

    //    std::map<term::type, std::string> _{
    //        {term::type::rule, color::attribute(color::HRed)},
    //        {term::type::s,  color::attribute(color::HGreen)},
    //        {term::type::m, color::attribute(color::HBlue)}
    //    };
    //
    //    str << _[_type];
    switch(_type)
    {
        case term::type::m:
        {
            token_data tok = token_data()[object.m];
            str << tok.text(); // stracc (TEXT)
        }
            break;
        case term::type::r:
            // Can't happen but we never know: (nullptr)
            if(object.r)
                str << object.r->_id;
            break;
        case term::type::s:str <<script::type::name(object.sem);
            break;
        default:str << "nil";
            break;
    }

    return str();
}

grammar::rule::rule(const std::string &a_id)
{
    _id = a_id;
    sequences.push_back({});
    seq = sequences.begin();
}

grammar::rule::~rule()
{
    sequences.clear();
    _id.clear();
}

grammar::rule &grammar::rule::new_sequence()
{
    sequences.push_back(term_seq());
    seq = --sequences.end();
    a.Reset();
    return *this;
}

grammar::rule &grammar::rule::operator|(grammar::rule *_r)
{
    grammar::term t = grammar::term(_r);
    t.a = a;
    a.Reset();
    *seq << t;
    return *this;
}

grammar::rule &grammar::rule::operator|(script::type::T _t)
{
    term t = term(_t);
    t.a = a;
    a.Reset();
    *seq << t;
    return *this;
}

grammar::rule &grammar::rule::operator|(mnemonic _t)
{
    term t = term(_t);
    t.a = a;
    a.Reset();
    *seq << t;
    return *this;
}

grammar::term grammar::term_seq::next(grammar::term::const_iterator &it) const
{
    if(it != terms.end())
        ++it;
    return *it;
}

grammar::term_seq &grammar::term_seq::operator<<(grammar::term a_t)
{
    terms.push_back(a_t);
    return *this;
}

grammar::term_seq &grammar::term_seq::operator<<(script::type::T a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

grammar::term_seq &grammar::term_seq::operator<<(mnemonic a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

grammar::term_seq &grammar::term_seq::operator<<(grammar::rule *a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

std::string grammar::term_properties::operator()()
{
    stracc str;
    if(Z)
        str << "*";
    if(R)
        str << "+";
    if(L)
        str << "?";
    if(X)
        str << "@";
    return str();
}

}

