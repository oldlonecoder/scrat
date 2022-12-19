#pragma once

#include <scrat/interpret/bloc.h>
#include <scrat/interpret/compiler/grammar.h>
#include <stack>
#include <scrat/lexer/lexer.h>

namespace scrat::script
{


class segment;


class SCRAT_API compiler
{
     struct context_data
     {
         bloc* blk = nullptr;
         token_data::iterator   i,stop;     ///< start and end iterators of this rule/context, used if the rule is unmet ( maybe discarded/unused if same as in the loop parser bloc )
         xio::list list;                 ///< buffered instructions of this rule/context.
         const grammar::rule* rule = nullptr;  ///< this rule
         xio* instruction = nullptr;     ///< finalised instruction.
         using stack = std::stack<context_data>;
     };

//     context_data ctx;                   ///< Current context data.
//     context_data::stack_t stack;        ///< Contexts stack
//
    // Internal data:

    context_data::stack ctx_stack;

    lexer lex;
    grammar  gr; ///< That'it ...no need to ever init something...
    bloc* root_bloc = nullptr;
    context_data ctx;
 public:
    struct SCRAT_API unit_data
    {
        std::string _id;
        //std::string_view _source;
        const char* _source_ptr = nullptr;
        token_data::collection tokens;
        std::string file; ///< Leave empty for implicitely providing the source code in _source; So Open fiel will be bypassed
        bloc* segment_bloc = nullptr; ///< segment owner;
        result<> input_file();
    };


     compiler() = default;
    ~compiler();

    compiler::context_data& context() { return ctx;}



    result<> cc(const compiler::unit_data& unit_);


    result<segment*> segment_bloc();

private:
    token_data::iterator crs;
    result<xio*> cc_expression(token_data::iterator start, bloc* blk);
    result<xio*> cc_function_call(token_data::iterator start, bloc* blk);
    result<variable*> cc_identifier(token_data::iterator start, bloc* blk_);

    result<xio*> cc_rule(const grammar::rule* rule, token_data::iterator start, bloc* blk_);
    result<xio*> cc_sequence(const grammar::term_seq& s, token_data::iterator start, bloc* blk_);

    void push_context(compiler::context_data const& ctx_);
    compiler::context_data  pop_context(compiler::context_data const& ctx_);
    unit_data _u;

};

}
