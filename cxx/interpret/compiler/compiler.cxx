#include <scrat/interpret/compiler/compiler.h>
//#include "compiler.h"
#include <scrat/interpret/segment.h>






namespace scrat::script
{


scrat::script::compiler::~compiler()
{
}


result<> scrat::script::compiler::cc(const compiler::unit_data& unit_)
{
    _u = unit_;
    lex.config() = {
        _u._source.data(),
        &_u.tokens
    };

    /// starting rule will be "module", then "init>declare_section".


    return rem::push_warning(source_ffl) < rem::notimplemented;
}



// result<> compiler::cc(bloc* blk, grammar::rule const* rule)
// {
//
//     return rem::notimplemented;
// }


// result<> compiler::cc(bloc* blk_, std::string_view source)
// {
//     lex.config() = {
//         .Source = source.data(),
//         .Tokens = &tokens
//     };
//     root_bloc = blk_;
//     //...
//     ///@note -- LAST INPUT HERE --
//     return rem::notimplemented;
// }

// result<> compiler::cc(bloc* blk_, std::string_view rule, std::string_view src)
// {
//
//     rem::push_debug(source_ffl) << " rule : '" << color::Yellow << rule << color::Reset << "' :";
//
//     lex.config() = {
//         .Source = src.data(),
//         .Tokens = &tokens
//     };
//     root_bloc = blk_;
//     context() = {
//         .blk = root_bloc,
//         .i = _u.tokens.begin(),
//         .stop = _u.tokens.begin(),
//         .rule = nullptr,
//         .instruction = nullptr
//     };
//
//     auto e = lex();
//     if (e != rem::accepted)
//         return rem::push_info() << " Lexical analyse has stopped because of error.";
//
//     for (auto const& T : tokens)
//             rem::push_output() << T.details(true);
//
//     crs = _u.tokens.begin();
//     auto x = cc_expression(crs, root_bloc);
//
//     return rem::ok;
// }


result<xio *> compiler::cc_expression(token_data::iterator start, bloc * blk)
{
    rem::push_debug(source_ffl) << " Start token [iterator] '" << color::Yellow << start->text() << color::Reset << "':";
    auto token = start;
try
    {


        xio* xio_root = xio::begin(blk,&(*token));
        xio* xio_node = xio_root;
        rem::push_debug(source_ffl) <<  " number of tokens: " << _u.tokens.size();

        while ((token != _u.tokens.end()) && (token->_flags.V ||  token->s & scrat::script::type::id_t))
        {
            if (token->t == type::id_t)
            {
                auto node = cc_identifier(token, blk);
                if (!node)
                    return rem::push_info() << " compilation stopped in expression parsing on " << rem::endl << token->mark();
                if(xio_node != xio_root)
                    xio_node = xio_node->input(blk, &(*token));
                if(!xio_node)
                    {
                        rem::push_info() << " compilation stopped in expression parsing on " << rem::endl << token->mark();
                        break;
                    }
                token++;
                continue;
            }
            xio_node = xio_node->input(blk, &(*token));
            if (!xio_node) break;
            token++;
        }
        xio_root = xio_root->tree_close();
        if (!xio_root)
            return rem::push_fatal(source_ffl) << " Oops! Internal catastrophe! Cannot close the xio tree! ( read the error messages). ";


        stracc Stream;
        xio::dot_tree_start(Stream, lex.config().Source);
        xio::dot_tree(xio_root, Stream);
        xio::dot_tree_close(Stream);
        rem::push_debug() << " Here the dot output of the xio:" << rem::endl << Stream() << rem::endl;
        return xio_root;
    }

    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
        rem::push_debug() << " " << e.what() << rem::endl << token->mark();
        return nullptr;
    }

    return nullptr;
}

/// @brief Check if, and parse a function call
/// @param start sarting token iterator.
/// @param blk  current bloc where the parsing execute.
/// @return result<xio*>: nullptr if not a function call, the xio* on the function call instance.
result<xio *> compiler::cc_function_call(token_data::iterator start, bloc *blk)
{
    // cc_rule(gr["function_Call"]);

    return nullptr;
}



/*!
    @brief compile identifier token.

    As of now (2022-12-5), scrat::script handles simple variables creation and use.
    For forcing duplicated names of variables in nested blocs/scopes, we use declaration rule syntax;
     - Pour forcer la creation d'une nouvelle variable locale qui porte le meme nom que celle de la portee parent, on utilise la syntaxe declarative...
    It will eventually handle function declaration, function-call, object instance object::method call, object::attribute instance addressing
*/
result<variable*> compiler::cc_identifier(token_data::iterator start, bloc* blk_)
{
    auto var = blk_->query_var(start->text());
    if(!var)
        var = blk_->new_var(&(*start));

    return *var;
}

result<xio *> compiler::cc_rule(const grammar::rule* rule, token_data::iterator start, bloc *blk_)
{

    return result<xio *>();
}
// force update

result<xio*> compiler::cc_sequence(const grammar::term_seq& s, token_data::iterator start, bloc * blk_)
{
    token_data::iterator i = start;
    rem::push_debug(source_ffl) << " start token : " << color::Yellow << start->text() << color::Reset << ":";
    xio* x = nullptr;
    for(auto t : s.terms)
    {
        if(t == *i)
        {
            // Correspondance entre l'element de la regle et le token.
            rem::push_output() << "Match between '" << color::Yellow << t() << color::Reset << " and " << color::Yellow << i->text() << color::Reset;
            //...
            if(t.is_rule())
            {
                push_context(ctx);
                auto x = cc_rule(gr[t()],i,blk_);
                if(!x)
                {
                    // Sequence rejected;
                    ///@todo cleanup context data;
                    ///@todo [MARK] Continuer a partir ici
                    return nullptr;
                }


            }
            ///@todo  in case of typename, we must save it here for the next iteration and assign the type to the next xio.
            if(t.is_oneof()) return x;
            ++i;
            continue;
        }
        if(t.is_optional()){ ++i; continue;}
        if(t.is_oneof()){++i;continue;}
    }

    return nullptr;
}

void compiler::push_context(const compiler::context_data& ctx_)
{
    ctx_stack.push(ctx_);
}

compiler::context_data scrat::script::compiler::pop_context(const compiler::context_data& ctx_)
{
    context_data cx;
    if(ctx_stack.empty()) return {};
    cx = ctx_stack.top();
    ctx_stack.pop();
    ///@todo Update
    return cx;
}

result<segment *> compiler::segment_bloc()
{
    return dynamic_cast<segment*>(_u.segment_bloc);
}


}

