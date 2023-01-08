#pragma once
#include<scrat/rem.h>


namespace scrat
{
	template<typename T = rem::code>  class  result final
	{
		rem* fail = nullptr;
        T value;
		bool good = false;


	public:
		result()
		{
			good = false;
			fail = nullptr;
		}

		~result() {}

		result(rem& msg)
		{
			good = false;
			fail = &msg;
		}

		result(T value_):value(value_),good(true) {}

		result(result&& r) noexcept
		{
			good = std::move(r.good);
			if (!good)
				fail = std::move(r.fail);
			else
				value = std::move(r.value);
		}

		result(const result& r)
		{
			good = r.good;
			if (!good)
				fail = r.fail;
			else
				value = r.value;
		}


		result& operator=(result&& r) noexcept
		{
			good = std::move(r.good);
			if (!good)
				fail = std::move(r.fail);
			else
				value = std::move(r.value);
			return *this;
		}

		result& operator=(const result& r)
		{
			good = r.good;
			if (!good)
				fail = r.fail;
			else
				value = r.value;

		}

		result operator=(rem& r)
		{
			good = false;
			fail = &r;
			return* this;
		}

		operator bool() { return good; }

		rem& operator()()
		{
			if (good)
				throw rem(rem::except) < " invalid call to result::operator '()', instance is in good state.";
			return *fail;
		}

		T operator *()
		{
			if(!good)
				throw rem(rem::except) < " invalid call to result::operator '*', instance is in fail state.";
			return value;
		}

		auto operator->() -> T
		{
			if(!good)
				throw rem(rem::except) < " invalid call to result::operator '->', instance is in fail state.";
			return value;
		}
	};
}
