#include <scrat/scrater/xio.h>
#include <scrat/geometry.h>



namespace scrat::script
{

_object_name(xio)

using iostr = stracc;
using logger = rem;

xio* xio::_global_root_bloc = nullptr;

std::stack<xio*> xio::pars;

//xio* xio::get_global_root() { return xio::{type::binary_t,     type::open_pair_t},  &xio::set_right_global_root_bloc; }


#define deg2rad(_xio) (((_xio)->acc->v.d*3.1415) / 180)
constexpr auto _PI_ = 3.14159265358979323846;
// M_PI ?


// void xio::// discard()
// {
//     for (auto x : xio::xio_gc)
//         if (this == x) return;

//     xio::xio_gc.push_back(this);
//     lhs = rhs = op = nullptr; // Make sure we do not accidently reuse this;
// }



    // Primary arithmetic supported operators table:
xio::xio_opfn_table_t   xio::xio_operators_table{
    {mnemonic::k_left_shift,         &xio::lshift},
    {mnemonic::k_radical,            &xio::radical},
    {mnemonic::k_exponent,           &xio::exponent},
    {mnemonic::k_right_shift,        &xio::rshift},
    {mnemonic::k_decr,               &xio::decr},
    {mnemonic::k_incr,               &xio::incr},
    {mnemonic::k_assign_add,         &xio::assignadd},
    {mnemonic::k_assign_sub,         &xio::assignsub},
    {mnemonic::k_assign_mul,         &xio::assignmul},
    {mnemonic::k_assign_div,         &xio::assigndiv},
    {mnemonic::k_assign_mod,         &xio::assignmod},
    {mnemonic::k_assign_and,         &xio::assignand},
    {mnemonic::k_assign_or,          &xio::assignor},
    {mnemonic::k_assign_xor,         &xio::assignxor},
    {mnemonic::k_assign_c1,          &xio::assignx1},
    {mnemonic::k_assign_left_shift,  &xio::assignlshift},
    {mnemonic::k_assign_right_shift, &xio::assignrshift},
    {mnemonic::k_less_eq,            &xio::leq},
    {mnemonic::k_greater_eq,         &xio::geq},
    {mnemonic::k_equal,              &xio::eq},
    {mnemonic::k_not_equal,          &xio::neq},
    {mnemonic::k_add,                &xio::add},
    {mnemonic::k_sub,                &xio::sub},
    {mnemonic::k_mul,                &xio::mul},
    {mnemonic::k_modulo,             &xio::modulo},
    {mnemonic::k_less_than,          &xio::lt},
    {mnemonic::k_greater_than,       &xio::gt},
    {mnemonic::k_assign,             &xio::assign},
    {mnemonic::k_binary_and,         &xio::binand},
    {mnemonic::k_binary_or,          &xio::binor},
    {mnemonic::k_binary_xor,         &xio::bitxor},
    {mnemonic::k_c1,                 &xio::x1},
    {mnemonic::k_c2,                 &xio::x2},
    {mnemonic::k_binary_not,         &xio::bitnot},
    {mnemonic::k_logical_and,        &xio::booland},
    {mnemonic::k_logical_or,         &xio::boolor},
    {mnemonic::k_div,                &xio::division},
    {mnemonic::k_factorial,          &xio::factorial},
    {mnemonic::k_positive,           &xio::positive},
    {mnemonic::k_negative,           &xio::negative},
    {mnemonic::k_pi,                 &xio::kpi},
    {mnemonic::k_cosinus,            &xio::kcos},
    {mnemonic::k_arc_cosinus,        &xio::kacos},
    {mnemonic::k_tangent,            &xio::ktan},
    {mnemonic::k_arc_tangent,        &xio::katan},
    {mnemonic::k_sinus,              &xio::ksin},
    {mnemonic::k_arc_sinus,          &xio::kasin},
    {mnemonic::k_number,             &xio::knumber},
    {mnemonic::k_u8,                 &xio::ku8   },
    {mnemonic::k_u16,                &xio::ku16  },
    {mnemonic::k_u32,                &xio::ku32  },
    {mnemonic::k_u64,                &xio::ku64  },
    {mnemonic::k_i8,                 &xio::ki8   },
    {mnemonic::k_i16,                &xio::ki16  },
    {mnemonic::k_i32,                &xio::ki32  },
    {mnemonic::k_i64,                &xio::ki64  },
    {mnemonic::k_float,              &xio::kreal },
    {mnemonic::k_string,             &xio::kstring}
};


// xio::xio()
// {
//     //acc = std::make_shared<alu>(0.0);
// }



xio::xio()
{
    acc = new alu(0.0f); // Shit!
}

xio::xio(xio* parent_bloc)
{
    _parent = parent_bloc;

    //token_info = new token_data;
    acc = new alu;
}


xio::~xio()
{
    if (!mem.xshared) delete acc;
}

xio::xio(xio* parent_bloc, token_data* a_token, alu* a_alu)
{
    t0 = a_token;
    _parent = parent_bloc;
    if(_parent)
        _parent->append_child(this);

    logger::push_debug() << "xio::xio => token details:[" << (t0 ? t0->details() : "nullptr") << "] \n";

    if (a_alu != nullptr)
    {
        acc = a_alu;
        mem.own = 0;
        return;
    }

    acc = new alu(0.0f);

    if (!t0)
        return;

    switch (t0->t) {
    case type::text_t:
        *acc = a_token->text();
        return;
    case type::leaf_t:
    {
        ///@todo Attention enlever le court-circuit du type-size!!!
        if (t0->s & type::number_t)
        {
            double d;
            (iostr(a_token->text())) >> d;
            *acc = d;
            rem::push_debug() << " acc: " << color::Yellow << (*acc)();
        }
    }
    return;
    case type::hex_t:
    {
        uint64_t d;
        iostr(a_token->text()).hexadecimal(d);
        *acc = d;
        return;
    }
    return;
    case type::any_t:
    case type::void_ptr:
        *acc = (void*)nullptr;
        return;
    default:

        break;
    }

    auto i = xio::xio_operators_table.find(a_token->c);
    xio_fn = i != xio::xio_operators_table.end() ? i->second : nullptr;
    rem::push_debug(source_fl) << " acc: '" << color::Yellow << (*acc)() << color::White << "'";
}




// ------------------ ARITHMETIC OPERATORS IMPLEMENTATION ---------------------------------------------

alu xio::jsr()
{
    //...

    if (lhs) *acc = lhs->jsr(); // Always catch the lhs value so we return that one if there is no rhs operand.
    if (rhs) *acc = rhs->jsr(); // Always catch the rhs value because it is the value to be returned after being applied to the lhs (if applicable).

    if (xio_fn)
        return (this->*xio_fn)();// All operators assign acc.
    else {
        if (t0->is_operator()) {
            rem::push_warning() << "xio [" << color::Yellow << t0->text() << color::Reset << "] has no implementation (yet?).:\n" << t0->mark();
        }
    }
    t0->s |= acc->T;
    return *acc;
}

result<> xio::append_child(xio *c)
{
    auto e = query(c);
    if(e != _children.end())
        _children.push_back(c);
    else
        return rem::exist;

    return rem::accepted;
}


result<> xio::detach(xio * c)
{
    auto i = query(c);
    if(i != _children.end())
        _children.erase(i);
    else
        return rem::notexist;

    return rem::accepted;
}



alu xio::lshift()

{
    *acc = lhs->acc->number<uint64_t>() << rhs->acc->number<uint64_t>();
    return *acc;
}

alu xio::radical()
{
    *acc = std::pow(lhs->acc->number<double>(), 1 / rhs->acc->number<double>());
    return *acc;
}

alu xio::exponent()
{
    *acc = std::pow(lhs->acc->number<double>(), rhs->acc->number<double>());
    return *acc;
}

alu xio::rshift()
{
    *acc = lhs->acc->number<uint64_t>() >> rhs->acc->number<uint64_t>();
    return *acc;
}


alu xio::decr()
{
    *acc = t0->is_prefix() ? --(*rhs->acc) : (*lhs->acc)--;
    return *acc;
}


alu xio::incr()
{
    *acc = t0->is_prefix() ? ++(*rhs->acc) : (*lhs->acc)++;
    return *acc;
}


alu xio::assignadd()
{
    *acc = *(lhs->acc) += *(rhs->acc);
    return  *acc;
}


alu xio::assignsub()
{
    *acc = *(lhs->acc) -= *(rhs->acc);
    return  *acc;
}


alu xio::assignmul()
{
    *acc = *(lhs->acc) *= *(rhs->acc);
    return *acc;
}


alu xio::assigndiv()
{
    *acc = *(lhs->acc) /= *(rhs->acc);
    return *acc;
}
alu xio::assignmod()
{
    *acc = *(lhs->acc) %= *(rhs->acc);
    return *acc;

}
alu xio::assignand()
{
    *acc = *(lhs->acc) &= *(rhs->acc);
    return *acc;

}
alu xio::assignor()
{
    *acc = *(lhs->acc) |= *(rhs->acc);
    return *acc;

}
alu xio::assignxor()
{
    *acc = *(lhs->acc) ^= *(rhs->acc);
    return *acc;

}
alu xio::assignx1()
{
    *acc = ~(*rhs->acc);
    return *acc;
}
alu xio::assignlshift()
{
    *acc = *lhs->acc <<= *rhs->acc;
    return *acc;
}


alu xio::assignrshift()
{
    *acc = *lhs->acc >>= *rhs->acc;
    return *acc;
}


alu xio::leq()
{
    *acc = *lhs->acc <= *rhs->acc;
    return *acc;
}


alu xio::geq()
{
    *acc = *lhs->acc >= *rhs->acc;
    return *acc;
}


alu xio::eq()
{
    *acc = (*lhs->acc) == (*rhs->acc);
    return *acc;
}


alu xio::neq()
{
    *acc = *lhs->acc != *rhs->acc;
    return *acc;
}

alu xio::add()
{
    rem::push_debug(source_fl) << color::Yellow << lhs->acu()() << " " << color::CornflowerBlue << attribute() << " " << color::Yellow << rhs->acu()() << ":";
    *acc = *lhs->acc + *rhs->acc;
    rem::push_output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
    return *acc;
}
alu xio::sub()
{
    // hack... en attendant :
    if (t0->s & type::sign_t)
        return negative();
    rem::push_debug(source_fnl) << color::Lime
        << color::Yellow << lhs->acu()() << " " << color::CornflowerBlue << attribute() << " " << color::Yellow << rhs->acu()() << ":";
    *acc = *lhs->acc - *rhs->acc;
    rem::push_output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();

    return *acc;
}
alu xio::mul()
{
    rem::push_debug()
        << color::Yellow << lhs->attribute()
        << color::CornflowerBlue << attribute()
        << color::Yellow << rhs->attribute()
        << color::White << "=";
    *acc = *lhs->acc * *rhs->acc;
    rem::push_output() << (*acc)();
    return *acc;
}
alu xio::modulo()
{
    *acc = *lhs->acc % *rhs->acc;
    return *acc;
}
alu xio::lt()
{
    *acc = *lhs->acc < *rhs->acc;
    return *acc;
}
alu xio::gt()
{
    *acc = *lhs->acc > *rhs->acc;
    return *acc;
}
alu xio::assign()
{
    rem::push_debug(source_fnl) << color::Lime
        << color::Aquamarine3 << lhs->attribute() << color::Reset << " "
        << " " << color::CornflowerBlue << attribute() << " "
        << color::Yellow
        << rhs->acu()() << ":";

    *acc = *lhs->acc = *rhs->acc;
    rem::push_output() << color::CornflowerBlue << " => " << color::Lime << (*acc)();
    return *acc;
}
alu xio::binand()
{
    *acc = *lhs->acc & *rhs->acc;
    return *acc;
}
alu xio::binor()
{
    *acc = *lhs->acc | *rhs->acc;
    return *acc;
}
alu xio::bitxor()
{
    *acc = *lhs->acc ^ *rhs->acc;
    return *acc;
}
alu xio::x1()
{
    *acc = ~(*lhs->acc);
    return *acc;
}
alu xio::x2()
{
    *acc = ~(*lhs->acc) + alu(1);
    return *acc;
}


alu xio::bitnot()
{
    *acc = !rhs->acc->number<double>();
    return *acc;
}


alu xio::booland()
{
    *acc = *lhs->acc && *rhs->acc;
    return *acc;
}
alu xio::boolor()
{
    *acc = *lhs->acc || *rhs->acc;
    return *acc;

}

alu xio::division()
{
    rem::push_debug(source_fnl) << color::Lime
        << color::Yellow << lhs->acu()()
        << " " << color::CornflowerBlue << attribute() << " "
        << color::Yellow
        << rhs->acu()() << ":";

    *acc = *lhs->acc / *rhs->acc;
    return *acc;
    //rem::push_output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
}


alu xio::factorial()
{
    //*acc = acc->factorial(*lhs->acc);
    *acc = lhs->acc->factorial();
    return *acc;
}



alu xio::positive()
{
    if (rhs->acc->number<double>() < 0)
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    return *acc;
}


alu xio::negative()
{
    rem::push_debug(source_fnl) << color::Lime
        << color::CornflowerBlue << attribute()
        << color::Yellow << rhs->attribute()
        << color::White << "=";

    if (rhs->acc->number<double>() > 0) // ==> a = -1;  -a = ?
        *rhs->acc *= -1;

    *acc = *rhs->acc;
    rem::push_output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
    return *acc;
}


alu xio::kpi()
{
    *acc = alu(_PI_);
    return *acc;
}


alu xio::kcos()
{
    *acc = std::cos(deg2rad(rhs));
    return *acc;
}


alu xio::kacos()
{
    *acc = std::acos(deg2rad(rhs));
    return *acc;
}
alu xio::ktan()
{
    *acc = std::tan(deg2rad(rhs));
    return *acc;
}


alu xio::katan()
{
    *acc = std::atan(deg2rad(rhs));

    return *acc;
}

alu xio::ksin()
{
    *acc = std::sin(deg2rad(rhs));
    return *acc;
}

alu xio::kasin()
{
    *acc = std::asin(deg2rad(rhs));
    return *acc;
}

alu xio::knumber()
{

    return *acc;
}

alu xio::ku8()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFF;
    return alu();
}

alu xio::ku16()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFF;
    return *acc;
}

alu xio::ku32()
{
    *acc = rhs->acc->number<uint64_t>() & 0xFFFFFFFF;
    return *acc;
}

alu xio::ku64()
{
    *acc = (uint64_t)(rhs->acc->number<uint64_t>());// & 0xFFFFFFFFFFFFFFFF);
    return *acc;
}

alu xio::ki8()
{
    *acc = (int8_t)(rhs->acc->number<int64_t>() & 0xFF);
    return *acc;
}

alu xio::ki16()
{
    rem::push_debug() << rhs->t0->text();
    *acc = (int16_t)(rhs->acc->number<uint64_t>() & 0xFFFF);
    return *acc;
}

alu xio::ki32()
{
    *acc = (int32_t)(rhs->acc->number<uint64_t>() & 0xFFFFFFFF);
    return *acc;
}

alu xio::ki64()
{
    *acc = (int64_t)(rhs->acc->number<int64_t>() & 0xFFFFFFFFFFFFFFFF);
    return *acc;
}

alu xio::kreal()
{
    *acc = rhs->acc->number<double>();
    return *acc;
}

alu xio::kstring()
{
    *acc = std::string(*rhs->acc);
    return *acc;
}


std::string xio::type_name()
{
    return type::name(t0->t);
}




std::string xio::attribute() {
    return t0 ? t0->text() : "''";
}

auto xio::query(xio* in_lhs, xio* in_rhs)
{
    for (auto [lr, fn] : xio::move_tbl)
    {
        auto [l, r] = lr;
        if ((in_lhs->t0->t & l) && (in_rhs->t0->t & r)) return fn;
    }
    return static_cast<xio * (xio::*)(xio*)> (nullptr);
}



//---------------------------Arithmetic Expression Tree Building------------------------------------------------
xio::move_table_t xio::move_tbl =
{
    // to_left:
        {{type::open_pair_t, type::leaf_t | type::prefix_t | type::binary_t | type::id_t | type::number_t | type::const_t}, &xio::to_left},
        {{type::binary_t | type::prefix_t | type::sign_t | type::assign_t,  type::open_pair_t | type::leaf_t | type::prefix_t | type::number_t | type::id_t}, &xio::to_right},
        // rpar_leaf:
        //    {{type::close_pair_t, type::leaf_t}, &xio::_rpar_leaf},
        //// _assign:
        //    {{type::leaf_t|type::number_t|type::id_t, type::assign_t}, &xio::_assign},
        // _close_par:
        {{type::close_pair_t | type::prefix_t | type::postfix_t | type::id_t | type::number_t | type::const_t, type::close_pair_t }, &xio::_close_par},
        // close_par_seq:
        {{type::close_pair_t, type::binary_t | type::postfix_t | type::close_pair_t}, &xio::collapse_par_pair},
        // _binary:
        {{type::number_t | type::binary_t | type::prefix_t | type::postfix_t | type::id_t, type::assign_t | type::binary_t}, &xio::_binary},
        // op_to_right:
        {{type::number_t | type::id_t, type::postfix_t}, &xio::op_to_right}
};

xio::input_table_t xio::input_tbl =
{
    {{type::id_t | type::assign_t,    type::assign_t}, "&xio::_assign"},
    {{type::number_t | type::const_t | type::postfix_t | type::close_pair_t | type::id_t,    type::binary_t}, "&xio::_binary"},
    {{type::number_t | type::id_t | type::close_pair_t | type::const_t,    type::postfix_t},  "&xio::_postfix"},
    {{type::binary_t,    type::prefix_t},     "&xio::_prefix"},
    {{type::prefix_t | type::binary_t | type::open_pair_t | type::assign_t,    type::leaf_t | type::number_t | type::open_pair_t | type::id_t | type::prefix_t},  "&xio::_leaf"},
    {{type::prefix_t | type::binary_t | type::open_pair_t,    type::open_pair_t},    "&xio::_open_pair"},
    {{type::postfix_t | type::const_t | type::id_t | type::open_pair_t | type::close_pair_t,    type::close_pair_t}, "&xio::_close_pair"}
};


void xio::SyntaxError(xio* e)
{
    throw rem::push_syntax() << "at " << e->t0->location() << rem::endl << e->t0->mark();
}


void xio::Error(rem::code ErrCode, xio* source_node, xio* input_node)
{
    throw rem::push_error() << source_node->attribute()
        << " token input error: "
        << ErrCode
        << input_node->attribute()
        << rem::endl
        << input_node->t0->mark();
}

void xio::Error(rem::code ErrCode, xio* source_node, token_data* input_token)
{
    throw rem::push_error() << source_node->attribute()
        << " token input error: "
        << ErrCode
        << input_token->text()
        << rem::endl
        << input_token->mark();
}

void xio::header(xio* input_node, source_location&& Loc)
{
    rem::push_debug(std::move(Loc)) << t0->text() << "<-" << input_node->t0->text() << rem::endl << input_node->t0->mark();
}

xio* xio::input(xio* parent_bloc, token_data* token)
{
    for (auto [lr, fn] : xio::input_tbl)
    {
        auto [l, r] = lr;
        if ((t0->t & l) && (token->t & r))
        {
            rem::push_debug(source_fnl) << color::Yellow << t0->text() << " <- " << color::Yellow << token->text() << color::Reset << " Input token validated... ";
            ///@todo Check id tokens for function_call and other id-constructs before calling xio::input(...).
            xio* a = new xio(parent_bloc,token);
            auto fn = xio::query(this, a);
            if (!fn)
            {
                rem::push_syntax() << " invalid relational operands at " << token->location() << " - unexpected token:" << rem::endl << token->mark();
                return nullptr;
            }
            rem::push_debug() << t0->text() << "::input(" << token->text() << "):" << rem::endl << token->mark();
            return (this->*fn)(a);
        }
    }

    rem::push_info(source_fnl) << t0->text()
        << "::input(" << token->text() << ") invalid relational operands at "
        << token->location() << " - unexpected token : "
        << rem::endl << token->mark();
    rem::push_output() << t0->details() << " || " << token->details() << rem::endl << "Returning nullptr";

    return nullptr;
}

xio* xio::_binary(xio* a)
{
    header(a, source_fnl);

    if (t0->is_leaf())
    {
        if (a->t0->c == mnemonic::k_open_par)
            SyntaxError(a);
    }

    if (t0->c == mnemonic::k_open_par)
        return to_left(a);

    if (t0->is_binary())
    {
        if (!rhs) SyntaxError(a);

        if (a->t0->d < t0->d)
            return to_right(a);
        if (op)
        {
            auto fn = query(this, a);
            if (!fn)
                xio::Error(rem::unexpected, this, a);

            return (op->*fn)(a);
        }

        return a->to_left(this);
    }

    if (op) {
        auto fn = query(this, a);
        if (!fn)
            xio::Error(rem::unexpected, this, a);
        return (op->*fn)(a);

    }
    a->to_left(this);
    return a;
}

xio* xio::_close_pair(xio* a)
{
    header(a, source_fnl);
    xio* x = xio::pop_par();
    if (!x)
    {
        rem::push_error() << "Unmatched left paranthese:" << rem::endl << a->t0->mark();
        return nullptr;
    }
    a->op = x->op;
    a->lhs = x->lhs;
    if (a->lhs)
    {
        if (a->lhs->op)
            a->lhs->op = a;
    }
    if (a->op)
        a->op->rhs = a; // oops!!

    rem::push_debug()
        << "new input vertex:["
        << color::Yellow
        << a->t0->text()
        << color::Reset
        << "]" << rem::endl
        << a->t0->mark();

    return a;
}

xio* xio::_close_par(xio* a)
{
    header(a, source_fnl);

    xio* x = xio::pop_par();
    if (!x)
    {
        rem::push_error() << "Unmatched left paranthese." << a->t0->mark();
        return nullptr;
    }
    a->op = x->op;
    a->lhs = x->lhs;
    if (a->lhs)
    {
        if (a->lhs->op)
            a->lhs->op = a;
    }
    if (a->op)
        a->op->rhs = a; // oops!!

    rem::push_output()
        << "new input vertex:["
        << color::Yellow
        << a->t0->text()
        << color::Reset
        << "]" << rem::endl
        << a->t0->mark();

    return a;
}

xio* xio::collapse_par_pair(xio* a)
{
    header(a, source_fnl);
    xio* v = lhs;

    // Collapse lhs

    v->op = op;
    if (op) op->rhs = v;

    // discard();

    if (v->op) {
        rem::push_output()
            << color::Yellow << v->op->attribute() << color::CornflowerBlue
            << " <-- "
            << color::Yellow << a->attribute();

        auto p_fn = query(v->op, a);
        if (!p_fn)
            xio::SyntaxError(a);

        return (v->op->*p_fn)(a);
    }

    v->op = a;
    a->lhs = v;
    return a;
}

xio* xio::op_to_right(xio* a)
{
    header(a, source_fnl);
    if (!op) {
        a->lhs = this;
        op = a;
        return a;
    }
    return op->to_right(a);
}


int xio::push_par(xio* a)
{
    xio::pars.push(a);
    return static_cast<int>(xio::pars.size());
}

xio* xio::pop_par()
{
    if (xio::pars.empty()) return nullptr;
    xio* x = xio::pars.top();
    xio::pars.pop(); // Tabarnak!
    return x;
}


xio* xio::begin(xio* parent_, token_data* token)
{
    if (!token->_flags.V)
    {
        return nullptr;
    }
    xio* a = new xio(parent_, token);
    if (a->t0->c == mnemonic::k_open_par) push_par(a);
    return a;
}

xio* xio::tree_close()
{
    header(this, source_fnl);

    if (t0->c == mnemonic::k_open_par)
    {
        rem::push_error() << " unexpected end of file.";
        return nullptr;
    }

    if (!xio::pars.empty())
    {
        xio* x = xio::pars.top();
        xio::pars.pop();
        rem::push_error() << " umatched closing parenthese from:" << rem::endl << x->t0->mark();
        return nullptr;
    }


    if (t0->c == mnemonic::k_close_par) {
        rem::push_debug() << "Closing the tree on close parenthese:";
        if (lhs)
        {
            xio* x = lhs;
            rem::push_debug() << "left hand side operand: " << lhs->t0->details() << ":" << rem::endl << lhs->t0->mark();

            lhs->op = op;

            if (op)
            {
                op->rhs = lhs;
                xio::trace_connect_binary_operands(op);
            }

            // discard();
            return x->tree_root(false);
        }
    }
    return tree_root(false);
}

xio* xio::tree_root(bool skip_syntax)
{
    rem::push_debug(source_fnl) << "query tree ins from xio node:" << rem::endl << t0->mark();
    xio* x = this;
    xio* p = x;
    do {
        x = p;
        if (!skip_syntax)
        {
            switch (x->t0->t) {
            case type::assign_t:
            case type::binary_t:
                if (!x->lhs)
                {
                    rem::push_error() << "Syntax error: binary operator is missing its left operand." << rem::endl << x->t0->mark();
                    return nullptr;
                }
                if (!x->rhs)
                {
                    rem::push_error() << "Syntax error: binary operator is missing its right operand." << rem::endl << x->t0->mark();
                    return nullptr;
                }
            case type::prefix_t:
                if (!x->rhs)
                {
                    rem::push_error() << "Syntax error: prefix unary operator is missing its (right) operand." << rem::endl << x->t0->mark();
                    return nullptr;
                }
                break;
            case type::postfix_t:
                if (!x->lhs)
                {
                    rem::push_error() << "Syntax error: postfix unary operator is missing its (left) operand." << rem::endl << x->t0->mark();
                    return nullptr;
                }
                break;
            }
        }
        p = p->op;
    } while (p);

    rem::push_debug(source_fnl) << "query tree ins returning node: " << x->t0->details() << rem::endl << x->t0->mark();
    return  x;
}

xio* xio::to_right(xio* in_rhs)
{
    header(in_rhs, source_fnl);

    // Temporary hack....
    if (in_rhs->t0->c == mnemonic::k_open_par)
        xio::push_par(in_rhs);

    if (rhs) {
        /*
           this
              \
               x <- next_token
              /
            rhs
        */
        rem::push_debug() << t0->text() << " -> " << rhs->t0->text()
            << color::Lime << "tree_set_right "
            << color::White << " <- "
            << color::Yellow << in_rhs->t0->text();
        rhs->op = in_rhs;
        in_rhs->lhs = rhs;
    }
    rhs = in_rhs;
    in_rhs->op = this;
    if (t0->is_binary())
    {
        rem::push_debug(source_fnl) << xio::trace_connect_binary_operands(this);
    }
    return in_rhs;
}

xio* xio::to_left(xio* in_lhs)
{
    header(in_lhs, source_fnl);
    /*
    (; [; {   // Appliqué sur aucun autre type de token car l'appel de tree_set_left ne se fait qu'� partir de tree_input qui r�soud l'associativit�.
        /
            x < -next_token
            /
            lhs
    */

    if (lhs) {
        // here we are supposed to be the openning par/index/bracket. so the interior will become right hand side of the parent op of this.
        lhs->op = in_lhs;
        in_lhs->lhs = lhs;
    }
    in_lhs->op = this;

    lhs = in_lhs;
    return in_lhs;
}


#pragma region TRIGRAPH

void xio::dot_tree_start(stracc& a_out, const stracc& Title)
{
    a_out << "digraph arithmetic_expression_tree {\n";
    a_out << "ratio=compress; ranksep=.55; size = \"6.5,6.5\";\n";
    a_out << "    node [fontname=\"Source Code Pro\", fontsize=12];\n";
    a_out << "    label=\"Arithmetic Expr Tree:[" << Title() << "]\"; fontsize = 10;\n";
}

void xio::dot_node(xio* A, stracc& a_out)
{
    //static int nullcount = 0;
    if (!A) return;
    if ((!A->lhs) && (!A->rhs)) return;

    if (A->lhs) {
        a_out << "    xioNode" << A << " -> xioNode" << A->lhs << ";\n";
        xio::dot_node(A->lhs, a_out);
    }
    //     else
    //         t_node::trace_null_node(A, nullcount++, a_stream);

    if (A->rhs) {
        a_out << "    xioNode" << A << " -> xioNode" << A->rhs << ";\n";
        xio::dot_node(A->rhs, a_out);
    }
    //     else
    //         trace_null_node(A, nullcount++, a_stream);
}

void xio::dot_null_node(xio*, int, stracc&)
{

}

void xio::dot_tree(xio* a_root, stracc& a_out)
{
    a_root->dot_attr(a_out);
    xio::dot_node(a_root, a_out);
}

void xio::dot_tree_close(stracc& a_out)
{
    a_out << "}\n";
}

void xio::dot_attr(stracc& a_out)
{
    stracc attr;
    attr << t0->text();
    stracc Shape;
    if (t0->t & type::text_t)
        Shape << "none";
    else
        if (t0->t & type::assign_t)
            Shape << "none";
        else
            Shape << "none";

    a_out << "    xioNode" << this << " [shape=%s, label=\"%s\"]\n";
    a_out << Shape() << attr();

    if (lhs)
        lhs->dot_attr(a_out);
    if (rhs)
        rhs->dot_attr(a_out);
}

std::string xio::trace_connect_binary_operands(xio* x)
{
    // assume this binary operator already has its lhs rhs operands !!
    //stracc str;
    auto lw = x->lhs->attribute().length();
    auto rw = x->rhs->attribute().length();
    auto ow = x->attribute().length();
    auto w = lw + rw + 3; // total width
    w -= lw % 2 == 0;


    auto m_lhs = lw - (lw > 1 ? 1 : 0);

    point oper_xy = point(static_cast<int>(m_lhs) + 1, 0);
    oper_xy.x -= ow % 2 == 0 ? 1 : 0;

    winbuffer area;
    area.set_geometry(static_cast<int>(w), 3); // pour l'instant c'est hardcodé.
    area.gotoxy(oper_xy.x, 0);
    area << x->attribute();
    area << point{ static_cast<int>(m_lhs),1 } << "/ \\";

    area.gotoxy(0, 2);
    area << x->lhs->attribute();

    area.gotoxy(static_cast<int>(m_lhs) + 2 + (rw <= 1 ? 1 : 0), 2);
    area << x->rhs->attribute();
    return area;
}



#pragma endregion TRIGRAPH





}


