#include <scrat/rem.h>



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
		{"default",		{color::Reset,color::Reset}},
		{"err",			{color::Reset,color::Red4}},
		{"separator",	{color::Reset,color::White}},
		{"warning",		{color::Reset,color::Yellow}},
		{"fatal",		{color::Reset,color::OrangeRed1}},
		{"exception",   {color::Reset,color::DodgerBlue1}},
		{"stamp",       {color::Reset,color::GreenYellow}},
		{"function",    {color::Reset,color::LightCyan3}},
		{"filename",    {color::Reset,color::LighcoreateBlue}},
		{"line",		{color::Reset,color::LightGoldenrod4}},
		{"debug",		{color::Reset,color::LightSteelBlue3}},
		{"output",		{color::Reset,color::Reset}},
		{"message",		{color::Reset,color::White}},
		{"syntax",		{color::Reset,color::SeaGreen1}},
		{"status",		{color::Reset,color::Grey42}},
		{"test",		{color::Reset,color::Yellow}},
		{"info",		{color::Reset,color::Blue4}},
		{"comment",		{color::Reset,color::White}}
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
	};


	std::string rem::cc()
	{
		// Build the prefix with the 
		_text.clear();
		// Check time stamp :
		if (_mcode == rem::stamp)
		{
			auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
			_text += rem::colors["stamp"]();
			_text += std::format("{:%Y/%m/%d %X}", time);
			_text += rem::colors["default"]();
			_text += ' ';
		}

		if(!_src._file.empty())
		{
			_text += rem::colors["filename"]();
			_text += _src._file;
			_text += rem::colors["default"]();
			_text += ' ';
		}
		if (!_src._fn.empty())
		{
			_text += rem::colors["function"]();
			_text += _src._fn;
			_text += rem::colors["default"]();
			_text += ' ';
		}

		if (_mclass != rem::nulltype)
		{
			_text += _typenames_[_mclass];
			_text += ' ';
		}
		
		for (auto item : _components) _text += item;
		
		return _text;
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

	rem& rem::push_error(source_location src_)
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
