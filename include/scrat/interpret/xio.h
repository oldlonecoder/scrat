#pragma once

#include <functional>
#include <utility>
#include <stack>
#include <map>


//-- Common includes for this and derived classes...
//#include <scrat/object.h>
#include <scrat/interpret/alu.h>
#include <scrat/lexer/tokendata.h>


namespace scrat::script
{
/*!
	@brief Executable Instruction Object.

	@author copy;2022, oldlonecoder (Serge Lussier, lussier.serge@gmail.com)
*/

class SCRAT_API xio
{
    friend class compiler;
    _decl_objname

protected:
    xio* op = nullptr; ///< Parent Operator
    xio* lhs = nullptr; ///< Left Hand Side Operand.
    xio* rhs = nullptr; ///< Right Hand Size Operand

    xio* _parent = nullptr;
    token_data* t0 = nullptr; ///< Token informations.

    alu* acc = nullptr; ///< Arithmetic Logical Unit, or Accumulator, or whatever!
    struct storage_attr {
        uint8_t lvalue : 1; // 0 = rvalue non-assignable (such as const,leaf). 1= left value; assignable.
        uint8_t own : 1; // 1 = this xio owns its acc storage - so will delete it in the destructor.
        uint8_t leave : 1; // 1 = leave flag because this xio is a return statement.
        uint8_t sstatic : 1; // 1 = static storage.
        uint8_t xshared : 1; // 1 = cannot be deleted by the parent's destructor.
        // ...
    }mem = { 0,0,0,0,0 }; ///< Storage types of this xio.
    friend class var;
    friend class bloc;
    // friend class compiler;
    // friend class interpreter_t;

public:


    using list = std::vector<xio*>;

    xio();
    explicit xio(xio* a_parent);

    xio(xio* a_parent, token_data* a_token, alu* a_alu = nullptr);

    virtual ~xio();

    xio& operator =(const xio&) = delete;
    xio& operator =(xio&&) = delete;

    using expected = rem::code;

    using maker = std::function<xio* (token_data*)>;
    alu acu() { return (acc ? *acc : alu(0.0f)); }

    virtual alu jsr(); // "Jump Subroutine" :)


    virtual std::string type_name();

    xio* oper() { return op; }
    token_data* token() { return t0; }

    template<class T> T* to() { return dynamic_cast<T*>(this); }
    //static void illegal_rvalue(xio* x, token_data* token);
    bool is_leave() { return mem.leave; }

    xio::list::iterator query(xio* c);

    template<class T> T* parent()
    {
        xio* e = this;
        while(e)
        {
            e = e->_parent;
            if(e)
            {
                if(T* p; (p = dynamic_cast<T*>(e))) return p;
            }
        }
        return nullptr;
    }

    result<> append_child(xio* c);
    result<> detach(xio* c);

protected:


    using input_table_t = std::vector<std::pair<std::pair<type::T, type::T>, std::string>>;
    using move_table_t = std::vector<std::pair<std::pair<type::T, type::T>, xio* (xio::*)(xio*)>>;
    static input_table_t input_tbl;
    static move_table_t move_tbl;

    using xio_opfn_t = alu(xio::*)();
    using xio_opfn_table_t = std::map<mnemonic, xio_opfn_t>;


    xio_opfn_t xio_fn = nullptr;

    xio::list _children;



    void SyntaxError(xio*);
    //xio* Warning(xio*);
    //xio* Fatal(xio*);

    static void Error(rem::code ErrCode, xio* source_node, xio* input_node);
    static void Error(rem::code ErrCode, xio* source_node, token_data* input_token);
    void header(xio* input_node, source_location&& Loc = {});


    // -------- Arithmetic binary tree INPUT: -----------------------------------
#pragma region INPUT

    xio* input(xio* parent_bloc, token_data* token);  ///< input nullptr token for telling to finalyse the tree... :)
    xio* _binary(xio* a);
    //xio* _prefix(xio* a);
    //xio* _postfix(xio* a);
    //xio* _leaf(xio* a);
    //xio* _open_pair(xio* a);
    xio* _close_pair(xio* a);
    //xio* _assign(xio* a);
    //xio* _rpar_leaf(xio* a);
    xio* _close_par(xio* a);
    xio* collapse_par_pair(xio* a);
    xio* op_to_right(xio* a);
    // (fr) D&egrave;s la phase d'association, on peut d&eacute;j&agrave; conna&icirc;tre le mouvement:
    //  - en validant l'appel de la fonction
    //  - en r&eacute;-associant le nouveau xio en se servant de la table move_tbl;
    // -----------------------------------------------------------------------------------------------


#pragma endregion INPUT

    static xio* begin(xio* parent_, token_data* token);
    xio* tree_close();
    xio* tree_root(bool skip_syntax);

#pragma region MOVE

    xio* to_right(xio* in_rhs);
    xio* to_left(xio* in_lhs);

#pragma endregion MOVE

    //std::string attribute() { return t0->text(); }

    auto query(xio* lhs, xio* rhs);

    //static void clear_static_pool();

    static std::stack<xio*> pars;
    static int push_par(xio* a);
    static xio* pop_par();


#pragma region TRIGRAPH

    static void dot_tree_start(stracc& a_out, const stracc& Title);
    static void dot_node(xio* A, stracc& a_out);
    static void dot_null_node(xio*, int, stracc&);
    static void dot_tree(xio* a_root, stracc& a_out);
    static void dot_tree_close(stracc& a_out);
    void dot_attr(stracc& a_out);

    //    static std::string trace_connect_binary_operands2(xio* x);

#pragma endregion TRIGRAPH
    static std::string trace_connect_binary_operands(xio* x);
private:

    static xio* _global_root_bloc;

    // --------------------------------------------------------------------------
    static xio_opfn_table_t xio_operators_table;


    // ----------- Primary operators ----------------------------
#pragma region ARITMETIC
    virtual alu  lshift();
    virtual alu  radical();
    virtual alu  exponent();
    virtual alu  rshift();
    virtual alu  decr();
    virtual alu  incr();
    virtual alu  assignadd();
    virtual alu  assignsub();
    virtual alu  assignmul();
    virtual alu  assigndiv();
    virtual alu  assignmod();
    virtual alu  assignand();
    virtual alu  assignor();
    virtual alu  assignxor();
    virtual alu  assignx1();
    virtual alu  assignlshift();
    virtual alu  assignrshift();
    virtual alu  leq();
    virtual alu  geq();
    virtual alu  eq();
    virtual alu  neq();
    virtual alu  add();
    virtual alu  sub();
    virtual alu  mul();
    virtual alu  modulo();
    virtual alu  lt();
    virtual alu  gt();
    virtual alu  assign();
    virtual alu  binand();
    virtual alu  binor();
    virtual alu  bitxor();
    virtual alu  x1();
    virtual alu  x2();
    virtual alu  bitnot();
    virtual alu  booland();
    virtual alu  boolor();
    virtual alu  division();
    virtual alu  factorial();
    virtual alu  positive();
    virtual alu  negative();
    virtual alu  kpi();
    virtual alu  kcos();
    virtual alu  kacos();
    virtual alu  ktan();
    virtual alu  katan();
    virtual alu  ksin();
    virtual alu  kasin();
    virtual alu  knumber();
    virtual alu  ku8();
    virtual alu  ku16();
    virtual alu  ku32();
    virtual alu  ku64();
    virtual alu  ki8();
    virtual alu  ki16();
    virtual alu  ki32();
    virtual alu  ki64();
    virtual alu  kreal();
    virtual alu  kstring();

#pragma endregion ARITHMETICS

    std::string attribute();
};

}
