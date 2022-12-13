#include <scrat/rem.h>
//#include "rem.h"

#include <scrat/text>


namespace scrat
{




	/*
	https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
	#include <chrono>
	#include <format>

	std::string get_current_time_and_date()
	{
		auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
		return std::format("{:%Y-%m-%d %X}", time);
	}

	*/
	rem::list rem::_application_message_q;

	attribute_list rem::colors =
	{
		"Application Messages Queu",
		{
		{"default",		{color::Reset,			color::Reset}},
		{"err",			{color::Red4,			color::Reset}},
		{"separator",	{color::White,			color::Reset}},
		{"warning",		{color::Yellow,			color::Reset}},
		{"fatal",		{color::OrangeRed1,		color::Reset}},
		{"exception",   {color::DodgerBlue1,	color::Reset}},
		{"stamp",       {color::GreenYellow,	color::Reset}},
		{"function",    {color::LightCyan3,		color::Reset}},
		{"filename",    {color::LighcoreateBlue,color::Reset}},
		{"line",		{color::LightGoldenrod4,color::Reset}},
		{"debug",		{color::LightSteelBlue3,color::Reset}},
		{"output",		{color::Reset,			color::Reset}},
		{"message",		{color::White,			color::Reset}},
		{"syntax",		{color::SeaGreen1,		color::Reset}},
		{"status",		{color::Grey42,			color::Reset}},
		{"test",		{color::Yellow,			color::Reset}},
		{"info",		{color::Blue4,			color::Reset}},
		{"comment",		{color::White,			color::Reset}}
		}

	};


	static std::unordered_map<rem::ctype, std::string_view> attr_map = {
		{rem::err,     "err"},
		{rem::warning, "warning"},
		{rem::fatal,   "fatal"},
		{rem::except,  "exception"},
		{rem::message, "message"},
		{rem::output,  "output"},
		{rem::debug,   "debug"},
		{rem::info,    "info"},
		{rem::comment, "comment"},
		{rem::syntax,  "syntax"},
		{rem::status,  "status"},
		{rem::test,    "test"},
	};


	static std::unordered_map<rem::ctype, std::string_view> _typenames_ =
	{
		{rem::err,		"err"},
		{rem::warning,  "warning"},
		{rem::fatal,	"fatal"},
		{rem::except,	"except"},
		{rem::message,  "message"},
		{rem::output,   "output"},
		{rem::debug,    "debug"},
		{rem::info,     "info"},
		{rem::comment,  "comment"},
		{rem::syntax,   "syntax"},
		{rem::status,   "status"},
		{rem::test,     "test"}
	};

	static std::unordered_map<rem::code, std::string_view> _codes_ =
	{
		{rem::ok,				"ok"},
		{rem::accepted,			"accepted"},
		{rem::success,			"success"},
		{rem::rejected,			"rejected"},
		{rem::failed,			"failed"},
		{rem::empty,			"empty"},
		{rem::full,				"full"},
		{rem::notempty,			"not empty"},
		{rem::implemented,		"already implemented"},
		{rem::notimplemented,	"not implemented"},
		{rem::untested,			"untested"},
		{rem::eof,				"end of file"},
		{rem::eos,				"end of string or stream"},
		{rem::null_ptr,		    "null pointer"},
		{rem::endl,				"\n"},
		{rem::enter,			"enter"},
		{rem::leave,			"leave"},
		{rem::begin,			"begin"},
		{rem::end,				"end"},
		{rem::notexist,			"does not exist"},
		{rem::unexpected,    	"unexpected"},
		{rem::expected,			"expected"},
	};


	std::string rem::cc()
	{
		// Build the prefix with the
		_text.clear();
		// Check time stamp :
		if (_mcode == rem::stamp)
		{

			_text += rem::colors["stamp"]();
			_text += rem::now();
			_text += rem::colors["default"]();
			_text += ':';
		}

		if(!_src._file.empty())
		{
			_text += rem::colors["filename"]();
			text_tokenizer::token_t::list words;
			text_tokenizer tokenizer  = text_tokenizer(_src._file);
			(void) tokenizer(words, "/",false);

			_text += words.back()();
			_text += rem::colors["default"]();
			_text += ':';
		}
		if (!_src._fn.empty())
		{
			_text += rem::colors["function"]();
			_text += _src._fn;
			_text += rem::colors["default"]();
			_text += ':';
		}
		if(_src.line !=0)
		{
			_text += rem::colors["line"]();
			_text += _src.line;
			_text += rem::colors["default"]();
			_text += ' ';
		}
		if (_mclass != rem::nulltype)
		{
			auto n = _typenames_[_mclass];
			_text += rem::colors[n.data()]();
			_text +=  _typenames_[_mclass].data();
			_text += rem::colors["default"]();
			_text += ':';
		}

		for (auto item : _components) _text += item;

		return _text();
	}

	scrat::rem::~rem()
	{
	}

	rem::rem(const rem& r):
		_src(r._src),
		_components(r._components),
		_text(r._text),
		_mclass(r._mclass),
		_mcode(r._mcode)
	{
	}


	rem::rem(rem::ctype ty, source_location src_):
		_src(src_)
	{
	}

	rem& rem::operator=(const rem& m)
	{
		_src = m._src;
		_components = m._components;
		_text = m._text;
		_mclass = m._mclass;
		_mcode = m._mcode;

		return *this;
	}
	rem& rem::operator<(rem::code c_)
	{
		if(c_ == rem::endl)
		{
			_components.push_back(_codes_[c_].data());
			return *this;
		}

		if (_mcode != rem::nullcode)
			_components.push_back(_codes_[c_].data());
		else
			_mcode = c_;

		return *this;
	}
	rem& rem::operator<(color::type c_)
	{
		_components.push_back(attr<textattr::format::ansi256>::fg(c_));
		return *this;
	}

	rem& rem::operator<(textattr::pair cf)
	{
		_components.push_back(cf());

		return *this;
	}

	rem& rem::operator<(const std::string& arg_)
	{
		_components.push_back(arg_);
		return *this;
	}

	rem& rem::operator<(const char* arg_)
	{
		_components.push_back(arg_);
		return *this;
	}

    std::string rem::now(std::string_view fmt)
    {

		auto here = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(here);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), fmt.data());
		return ss.str();
    }


    rem &rem::push_error(source_location src_)
    {
		rem::_application_message_q.emplace_back(rem{ rem::err, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_warning(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::warning, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_fatal(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::fatal, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_exception(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::except, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_message(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::message, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_output(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::output, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_debug(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::debug, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_info(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::info, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_comment(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::comment, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_syntax(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::syntax, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_status(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::status, src_ });
		return rem::_application_message_q.back();
	}
	rem& rem::push_test(source_location src_)
	{
		rem::_application_message_q.emplace_back(rem{ rem::test, src_ });
		return rem::_application_message_q.back();
	}
	rem::code rem::clear(std::function<void(rem&)> fn_)
	{
		for (auto& r : rem::_application_message_q)
		{
			if (fn_)
				fn_(r);
		}
		rem::_application_message_q.clear();
		return rem::ok;
	}
}
