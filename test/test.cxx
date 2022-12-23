#include "test.h"
#include <scrat/text>
#include <scrat/interpret/interpret.h>


using scrat::rem;
using scrat::color;


auto main() -> int {

	test test;

	test.put_colors();
	//...
	test.run();
	scrat::rem::clear([](rem& r) { std::cout << r.cc() << '\n'; } );
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
		test_text();
		test_interpret();
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
	scrat::stracc str = "[{%d,%d}:[%d]]";
	str <<  area.w, area.h,  (area.w * area.h);
	rem::push_debug(source_pfnl) < "dim<>:: area:" < str();

	scrat::rect r;
	r.assign({ 5, 6 }, scrat::dim{ {100, 100}, 25, 25 });

	rem::push_output() < "r = " < (std::string)r;
}

void test::test_text()
{
	scrat::text Txt = " <Icon: ArrowRight;> Allo <Color: Yellow, BlueViolet;> &agrave; <Color : Reset> vous <Fg:Yellow;>t<Fg:BlueViolet;>o<Fg:Green;>u<Fg:White;>s<Fg:DeepPink2;>!<Color:Reset;>  ";
    Txt.compile();
    std::string Str;
    Txt >> Str;
    rem::push_info(source_fl) < Str;
}

void test::put_colors()
{
    int C = 0;
    scrat::stracc Str, Line;
    for (int l = 0; l < 16; l++)
    {
        Line = "%3d";
        Line << C;
        for (int cn = 0; cn < 16; cn++)
        {
            Str = "%s      ";
            Str << scrat::attr<scrat::textattr::format::ansi256>::bg(static_cast<scrat::color::type>(C));
            Line += Str();
            C++;
        }
        rem::push_output() < Line() < color::Reset;
        Line.clear();
    }
}



scrat::result<scrat::object*> test::test_result()
{
	using color = scrat::color;
	_object = new scrat::object();
	scrat::result<scrat::object*> r = _object;

	rem::push_debug(source_pfnl) < " object class name: " < color::Yellow < r->class_name() < "";
	return r;
}

void test::test_interpret()
{
	using scrat::script::interpret;
	try{
		interpret::set_location("/home/oldlonecoder/app/dev/c++/scrat/resources");
	}
	catch(rem& e)
	{
		std::cout << " Oh! rem failure here?\n";
	}

}

