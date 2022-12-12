#include "test.h"


using scrat::rem;
using scrat::color;


auto main() -> int {

	test test;

	//...
	test.run();
	scrat::rem::clear([](rem& r)
		{
			std::cout << r.cc() << '\n';
		}
	);
	return 0;
}

test::test()
{
}

test::~test()
{
	delete _object;
}

void test::run()
{
	_object = new scrat::object;
	rem::push_debug(source_aaa) < rem::stamp < " testing! " < scrat::color::Yellow < "Allo";

	try {
		std::cout << _object->class_name() << '\n';
		auto r = test_result();
		test_dimension();
	}
	catch (std::exception e)
	{
		std::cout << " execption: '" << e.what() << '\n';
	}
}

void test::texattr()
{
	scrat::attr<scrat::textattr::format::ansi256> attr;


}

void test::test_dimension()
{
	using scrat::dim;

	dim area = { {100,100}, 5,5 };
	rem::push_debug(source_pfnl) < "dim<>:: area:" < std::format("{}*{}={}", area.w, area.h, area.w * area.h);

	scrat::rect r;
	r.assign({ 5, 6 }, scrat::dim{ {100, 100}, 25, 25 });

	rem::push_output() < "r = " < (std::string)r;

}

scrat::result<scrat::object*> test::test_result()
{
	using color = scrat::color;
	_object = new scrat::object();
	scrat::result<scrat::object*> r = _object;

	rem::push_debug(source_pfnl) < " object class name: " < color::Yellow < r->class_name() < "";
	return r;
}

