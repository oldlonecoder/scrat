#include <scrat/lexer/tokendata.h>
#include <scrat/Icons.h>
#include <map>


static const char* fmt = "'%s'(%d,%d) %s[%s]";

namespace scrat
{
namespace script
{

static std::map<mnemonic, script::type::T> key_to_type = { {mnemonic::k_i8,   script::type::i8_t},
														  {mnemonic::k_i16,  script::type::i16_t},
														  {mnemonic::k_i32,  script::type::i32_t},
														  {mnemonic::k_i64,  script::type::i64_t},
														  {mnemonic::k_u8,   script::type::u8_t},
														  {mnemonic::k_u16,  script::type::u16_t},
														  {mnemonic::k_u32,  script::type::u32_t},
														  {mnemonic::k_u64,  script::type::u64_t},
														  {mnemonic::k_float,script::type::float_t}

};

std::string token_data::location_data::operator()() const
{
	// line y, col x: `attribute`
	return text();
}

std::string token_data::location_data::text() const
{
	if (begin == nullptr)
		return std::string();


	std::string str;
	const char* ptr = begin;
	str += *ptr++;
	if (!end)
	{
		while (*ptr)
			str += *ptr++;
		return str;
	}
	while (ptr <= end)
		str += *ptr++;
	//str += '\'';
	return str;
}

std::string token_data::location_data::position() const
{
	stracc Str = "%d,%d";
	Str << linenum << colnum;
	return Str();
}

token_data SCRAT_API token_data::mNull = token_data();

token_data::token_data(mnemonic aCode, script::type::T aType, script::type::T aSem, script::type::distance::T aDelta, lexem::T aLexem, uint8_t V_Flag)
{
	c = aCode;
	t = aType;
	s = aSem;
	d = aDelta;
	mLoc = { aLexem, 0, 0, 0, -1 };
	_flags = { V_Flag, 0, 0 };

}

token_data::token_data(mnemonic aCode, script::type::T aType, script::type::T aSem, script::type::distance::T aDelta, token_data::location_data aLoc, token_data::Flag aFlag, void* aPtr)
{
	c = aCode;
	t = aType;
	s = aSem;
	d = aDelta;
	mLoc = aLoc;
	_flags = aFlag;
	vdata = aPtr;
}


token_data::token_data(const token_data& aToken) // Consider using "default" ...
{
	c = aToken.c;
	t = aToken.t;
	s = aToken.s;
	d = aToken.d;
	mLoc = aToken.mLoc;
	_flags = aToken._flags;
	vdata = aToken.vdata;
}

token_data::token_data(token_data&& aToken) noexcept
{
	std::swap(c, aToken.c);
	std::swap(t, aToken.t);
	std::swap(s, aToken.s);
	std::swap(d, aToken.d);
	std::swap(mLoc, aToken.mLoc);
	std::swap(_flags, aToken._flags);
	std::swap(vdata, aToken.vdata);

}

token_data& token_data::operator=(token_data&& aToken) noexcept
{
	std::swap(c, aToken.c);
	std::swap(t, aToken.t);
	std::swap(s, aToken.s);
	std::swap(d, aToken.d);
	std::swap(mLoc, aToken.mLoc);
	std::swap(_flags, aToken._flags);
	std::swap(vdata, aToken.vdata);
	return *this;
}

token_data& token_data::operator=(const token_data& aToken)
{
	c = aToken.c;
	t = aToken.t;
	s = aToken.s;
	d = aToken.d;
	mLoc = aToken.mLoc;
	_flags = aToken._flags;
	vdata = aToken.vdata;
	return *this;
}

std::string token_data::mark() const
{
	stracc str;
	const char* B = mLoc.begin - mLoc.offset;
	//std::string str;
	int l = 1;
	const char* cc = mLoc.begin;
	// localiser le debut de la ligne;
	while (*cc && (cc > B) && (*cc != '\n') && (*cc != '\r'))
		--cc;
	// debut de la ligne ou de la source:
	if (cc >= B)
	{
		if ((*cc == '\n') || (*cc == '\r'))
			++cc;
		while (*cc && (*cc != '\n') && (*cc != '\r'))
			str += *cc++;
	}
	stracc tstr;

	tstr << str() << '\n';
	for (int x = 1; x < mLoc.colnum; x++)
		tstr << ' ';
	tstr << Icon::CArrowUp;

	return tstr();
}

std::string token_data::location() const
{
	stracc Str = "Line :%d, Col:%d";
	Str << mLoc.linenum << mLoc.colnum;
	return Str();
}


std::string token_data::semantic_types() const
{
	std::string Str;
	return script::type::name(s);
}

std::string token_data::type_name() const
{
	std::string Str;
	return script::type::name(t);
}


/*!
	@note !IMPORTANT : Decompress token_data::s values! 
*/
static token_data::collection tokens_table =
{
{mnemonic::k_null,              type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_null,        1},
{mnemonic::k_left_shift,        type::binary_t,     0x000C00000, type::distance::shift,      lexem::k_left_shift,  1},
{mnemonic::k_unshadow,          type::prefix_t,     0x000580000, type::distance::scope,      lexem::k_unshadow,    1},
{mnemonic::k_open_abs_op,       type::open_pair_t,  0x080C00000, type::distance::paranthese, lexem::k_abs_begin,   0},
{mnemonic::k_close_abs_op,      type::close_pair_t, 0x080400000, type::distance::paranthese, lexem::k_abs_end,     0},
{mnemonic::k_radical,           type::binary_t,     0x000C00000, type::distance::exponent,   lexem::k_radical,     1},
{mnemonic::k_exponent,          type::binary_t,     0x000C00000, type::distance::exponent,   lexem::k_exponent,    1},
{mnemonic::k_right_shift,       type::binary_t,     0x000400000, type::distance::shift,      lexem::k_right_shift, 1},
{mnemonic::k_binary_xor,        type::binary_t,     0x000C00000, type::distance::bits,       lexem::k_xor,         1},
{mnemonic::k_decr,              type::unary_t,      0x000580000, type::distance::unary,      lexem::k_decr,        1},
{mnemonic::k_incr,              type::unary_t,      0x000580000, type::distance::unary,      lexem::k_incr,        1},
{mnemonic::k_assign_add,        type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_add,  1},
{mnemonic::k_assign_sub,        type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_sub,  1},
{mnemonic::k_assign_mul,        type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_mul,  1},
{mnemonic::k_assign_div,        type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_div,  1},
{mnemonic::k_assign_mod,        type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_mod,  1},
{mnemonic::k_assign_and,        type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_and,  1},
{mnemonic::k_assign_or,         type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_or,   1},
{mnemonic::k_assign_xor,        type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_xor,  1},
{mnemonic::k_assign_c1,         type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_c1,     1},
{mnemonic::k_assign_left_shift, type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_lshift, 1},
{mnemonic::k_assign_right_shift,type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign_rshift, 1},
{mnemonic::k_deref,             type::prefix_t,     0x000580000, type::distance::deref,      lexem::k_deref        , 1},
{mnemonic::k_less_eq,           type::binary_t,     0x200C00000, type::distance::equality,   lexem::k_less_equal,    1},
{mnemonic::k_greater_eq,        type::binary_t,     0x200C00000, type::distance::equality,   lexem::k_greater_equal, 1},
{mnemonic::k_equal,             type::binary_t,     0x200C00000, type::distance::equality,   lexem::k_equal,        1},
{mnemonic::k_not_equal,         type::binary_t,     0x200C00000, type::distance::equality,   lexem::k_not_equal,    1},
{mnemonic::k_not_equal,         type::binary_t,     0x200C00000, type::distance::equality,   "<>",                      1},
{mnemonic::k_add,               type::binary_t,     0x000C00000, type::distance::addition,   lexem::k_addition,     1},
{mnemonic::k_sub,               type::binary_t,     0x000C00000, type::distance::addition,   lexem::k_sub,          1},
{mnemonic::k_mul,               type::binary_t,     0x000C00000, type::distance::product,    lexem::k_multiply,     1},
{mnemonic::k_comment_cpp,       type::punc_t,       0x000400000, type::distance::noop_,      lexem::k_comment_cpp,  0},
{mnemonic::k_modulo,            type::binary_t,     0x000C00000, type::distance::product,    lexem::k_modulo,       1},
{mnemonic::k_less_than,         type::binary_t,     0x200C00000, type::distance::equality,   lexem::k_less_than,    1},
{mnemonic::k_greater_than,      type::binary_t,     0x200C00000, type::distance::equality,   lexem::k_greater_than, 1},
{mnemonic::k_assign,            type::assign_t,     0x100C40000, type::distance::assign,     lexem::k_assign,     1},
{mnemonic::k_binary_and,        type::binary_t,     0x000C00000, type::distance::bits,       lexem::k_binary_and,  1},
{mnemonic::k_binary_or,         type::binary_t,     0x000C00000, type::distance::bits,       lexem::k_binary_or,   1},
{mnemonic::k_c2,                type::prefix_t,     0x000580000, type::distance::prefix,     lexem::k_c2,          1},
{mnemonic::k_c1,                type::prefix_t,     0x000580000, type::distance::prefix,     lexem::k_c1,            1},
{mnemonic::k_binary_not,        type::unary_t,      0x000580000, type::distance::prefix,     lexem::k_not,           1},
{mnemonic::k_logical_and,       type::binary_t,     0x200C00000, type::distance::bool_and,   lexem::k_bool_and,      1},
{mnemonic::k_logical_or,        type::binary_t,     0x200C00000, type::distance::bool_or,    lexem::k_bool_or,       1},
{mnemonic::k_open_par,          type::open_pair_t,  0x0880400000, type::distance::paranthese, lexem::k_open_par,     1},
{mnemonic::k_close_par,         type::close_pair_t, 0x1080400000,type::distance::paranthese, lexem::k_close_par,     1},
{mnemonic::Openindex,           type::binary_t,     0x2080C00000,type::distance::paranthese, lexem::k_open_index,    1},
{mnemonic::Closeindex,          type::close_pair_t, 0x4080400000,type::distance::paranthese, lexem::k_close_index,   1},
{mnemonic::Openbrace,           type::binary_t,     0x0880C00000, type::distance::paranthese, lexem::k_brace_begin,  1},
{mnemonic::Closebrace,          type::close_pair_t, 0x1080400000,type::distance::paranthese, lexem::k_brace_end,     1},
{mnemonic::k_begin_comment,     type::punc_t,       0x00000,     type::distance::noop_,      lexem::k_comment_begin, 0},
{mnemonic::k_end_comment,       type::punc_t,       0x00000,     type::distance::noop_,      lexem::k_comment_end, 0},
{mnemonic::k_div,               type::binary_t,     0x000C00000, type::distance::product,    lexem::k_division,  1},
{mnemonic::k_comma,             type::binary_t,     0x080C00000, type::distance::comma,      lexem::k_comma,     1},
{mnemonic::k_scope,             type::binary_t,     0x080C00000, type::distance::scope,      lexem::k_scope,     1},
{mnemonic::k_semicolon,         type::punc_t,       0x080000000, type::distance::noop_,      lexem::k_semicolon, 0},
{mnemonic::k_colon,             type::binary_t,     0x080C00000, type::distance::noop_,      lexem::k_colon,     0},
{mnemonic::k_range,             type::binary_t,     0x080C00000, type::distance::scope,      lexem::k_range,     1},
{mnemonic::k_factorial,         type::postfix_t,    0x000580000, type::distance::unary,      lexem::k_factorial, 1},
{mnemonic::k_positive,          type::prefix_t,     0x000580020, type::distance::unary,      lexem::k_positive, 1},
{mnemonic::k_negative,          type::prefix_t,     0x000580020, type::distance::unary,      lexem::k_negative, 1},
{mnemonic::k_squote,            type::text_t,       0x000400012, type::distance::identifier, lexem::k_squote,  1},
{mnemonic::k_dquote,            type::text_t,       0x000400012, type::distance::identifier, lexem::k_dquote,  1},
{mnemonic::k_ternary,           type::keyword_t,    0x000C40000, type::distance::unary,      lexem::k_ternary, 1},
{mnemonic::k_hash,              type::prefix_t,     0x000580000, type::distance::unary,      lexem::k_hash,   1},
{mnemonic::k_eos,               type::prefix_t,     0x000580000, type::distance::unary,      lexem::k_eos,    1},
{mnemonic::k_dot,               type::punc_t,       0x080400000, type::distance::scope,      lexem::k_dot,    1},
{mnemonic::k_return,            type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_return, 0},
{mnemonic::k_if,                type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_if,     0},
{mnemonic::k_pi,                type::number_t,     0x2000044011, type::distance::identifier, lexem::k_pi,     1},
{mnemonic::k_number,            type::prefix_t,     0x000540001, type::distance::identifier, lexem::k_number, 1},
{mnemonic::k_u8,                type::prefix_t,     0x000540400, type::distance::identifier, lexem::k_u8,     1},
{mnemonic::k_u16,               type::prefix_t,     0x000540800, type::distance::identifier, lexem::k_u16,    1},
{mnemonic::k_u32,               type::prefix_t,     0x000541000, type::distance::identifier, lexem::k_u32,    1},
{mnemonic::k_u64,               type::prefix_t,     0x000542000, type::distance::identifier, lexem::k_u64,     1},
{mnemonic::k_i8,                type::prefix_t,     0x000540040, type::distance::identifier, lexem::k_i8,      1},
{mnemonic::k_i16,               type::prefix_t,     0x000540080, type::distance::identifier, lexem::k_i16,     1},
{mnemonic::k_i32,               type::prefix_t,     0x000540100, type::distance::identifier, lexem::k_i32,     1},
{mnemonic::k_i64,               type::prefix_t,     0x000540200, type::distance::identifier, lexem::k_i64,     1},
{mnemonic::k_float,             type::prefix_t,     0x000544000, type::distance::identifier, lexem::k_float,   1},
{mnemonic::k_string,            type::prefix_t,     0x000540002, type::distance::identifier, lexem::k_string,  1},
{mnemonic::k_then,              type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_then,    0},
{mnemonic::k_else,              type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_else,    0},
{mnemonic::k_const,             type::keyword_t,    0x2000040000,type::distance::identifier, lexem::k_const,   0},
{mnemonic::k_include,           type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_include, 0},
{mnemonic::k_module,            type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_module, 0},
{mnemonic::k_at,                type::prefix_t,     0x000580000, type::distance::unary,      lexem::k_at,      1},
{mnemonic::k_prime,             type::prefix_t,     0x000580000, type::distance::unary,      lexem::k_prime,   1},
{mnemonic::k_do,                type::keyword_t,    0x000040000, type::distance::noop_,      lexem::k_do,      0},
{mnemonic::k_while,             type::keyword_t,    0x000040000, type::distance::noop_,      lexem::k_while,       0},
{mnemonic::k_for,               type::keyword_t,    0x000040000, type::distance::noop_,      lexem::k_for,         0},
{mnemonic::k_until,             type::keyword_t,    0x000040000, type::distance::noop_,      lexem::k_until,       0},
{mnemonic::k_repeat,            type::keyword_t,    0x000040000, type::distance::noop_,      lexem::k_repeat,      0},
{mnemonic::k_switch,            type::keyword_t,    0x000040000, type::distance::noop_,      lexem::k_switch,      0},
{mnemonic::k_case,              type::keyword_t,    0x000040000, type::distance::noop_,      lexem::k_case,        0},
{mnemonic::k_type,              type::keyword_t,    0x000040000, type::distance::noop_,      lexem::k_type,        0},
{mnemonic::k_low_hex,           type::hex_t,        0x400440011, type::distance::noop_,      lexem::k_low_hex,     1},
{mnemonic::k_up_hex,            type::hex_t,        0x400440011, type::distance::noop_,      lexem::k_up_hex,      1},
{mnemonic::k_cosinus,           type::prefix_t,     0x000580000, type::distance::unary,      lexem::k_cosinus,     1},
{mnemonic::k_arc_cosinus,       type::prefix_t,     0x0005C0000, type::distance::unary,      lexem::k_arc_cosinus, 1},
{mnemonic::k_tangent,           type::prefix_t,     0x0005C0000, type::distance::unary,      lexem::k_tangent,     1},
{mnemonic::k_arc_tangent,       type::prefix_t,     0x0005C0000, type::distance::unary,      lexem::k_arc_tangent, 1},
{mnemonic::k_sinus,             type::prefix_t,     0x0005C0000, type::distance::unary,      lexem::k_sinus,       1},
{mnemonic::k_arc_sinus,         type::prefix_t,     0x0005C0000, type::distance::unary,      lexem::k_arc_sinus,   1},
{mnemonic::k_object,            type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_object,      0},
{mnemonic::k_static,            type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_static,      0},
{mnemonic::k_this,              type::keyword_t,    0x000040000, type::distance::identifier, lexem::k_this,        1}
};

// It was the right one ... duh...
token_data token_data::scan(const char* C_)
{
	int      unicode = 0;
	if (!*C_)
		return token_data::mNull;

	for (auto token : tokens_table)
	{
		const char* crs = C_;
		const char* rtxt = token.mLoc.begin;
		unicode = 0; // oops...
		std::size_t sz = std::strlen(rtxt);

		if (*crs != *token.mLoc.begin) continue;

		while ((*crs && *rtxt) && (*crs == *rtxt))
		{
			if (*crs < 0)
				++unicode; ///< @note oui/ yes; Soon/Bientot je supporte quelques symboles UTF-8 (pi, xor,...) 
			++crs;
			++rtxt;
		}
		if (*rtxt == 0)
		{
			if (*crs && !isspace(*crs))
			{
				if ((isalnum(*crs) || (*crs == '_')) && !token.is_operator())
					continue;
			}

			token.mLoc.begin = C_;
			token.mLoc.end = crs - 1;
			return token;
		}
	}
	return token_data::mNull;

}

script::type::T token_data::ktype(mnemonic m)
{
	return key_to_type[m];
}

token_data token_data::operator[](mnemonic CC)
{
	for (token_data T : tokens_table)
	{
		if (CC == T.c)
			return T;
	}
	return token_data::mNull;
}

std::string token_data::details(bool Mark_) const
{
	stracc Str = "'%s'[%s]: offset: %d line:%d, col:%d, %s/{%s}";
	Str << text() << mnemonic_name(c) << mLoc.offset << mLoc.linenum << mLoc.colnum << type_name() << semantic_types();
	if (Mark_)
		Str << '\n' << mark();
	return Str();
}
}

}
