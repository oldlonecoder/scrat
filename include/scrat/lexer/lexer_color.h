#pragma once

#include <scrat/result.h>
#include <scrat/textattr.h>
#include <scrat/lexer/tokendata.h>
#include <map>


namespace scrat::script
{

	class SCRAT_API lexer_color
	{
		token_data::collection tokens;
		textattr::format _formatormat = textattr::format::ansi256; ///< Not used yet

		// Color::Type or Color::Set ...
		//static std::map<type::T, Color::Type> SemanticTypes; // Addresing token_data::s
		static std::map<type::T, color::type> Types;   // Addressing token_data::t
		std::string _product_data;

	public:
		  lexer_color();
		  ~lexer_color();

		result<> operator << (const std::string& aSource);
		std::string Product() {
			return _product_data;
		}
		//...
	};



}
