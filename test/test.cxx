#include "test.h"
#include <scrat/text>
#include <scrat/interpret/interpret.h>
#include <scrat/ui/console.h>
#include <scrat/ui/elements/label.h>
#include <scrat/ui/elements/icon.h>
#include <scrat/io/listener.h>
#include <scrat/ui/elements/text_input.h>
using scrat::rem;
using scrat::color;
void signal_int(int s)
{
    scrat::rem::push_info(source_ffl) < "interrupted by user signal " < color::LightCyan3 < "int";
    scrat::ui::console::terminate();
    scrat::rem::clear([](rem& r) { std::cout << r.cc() << '\n'; } );
    exit(s);
}



auto main() -> int {

	test test;
    ::signal(SIGINT, signal_int);
	test.put_colors();
	//...
	test.run();
	scrat::rem::clear([](rem& r) { std::cout << r.cc() << '\n'; } );
	return 0;
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
		test_console();
		test_alu();

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

	dim area = { {100,100},{1,1}, 5,5 };
	scrat::stracc str = "[{%d,%d}:[%d]]";
	str <<  area.w, area.h,  (area.w * area.h);
	rem::push_debug(source_pfnl) < "dim<>:: area:" < str();

	scrat::rect r;
	r.assign({ 5, 6 }, scrat::dim{ {100, 100}, {1,1}, 25, 25 });

	rem::push_output() < "r = " < (std::string)r;
}

void test::test_text()
{
	scrat::text Txt = " <Icon: ArrowRight;> Allo <Color: Yellow, BlueViolet;> &agrave; <Color : Reset> vous <Fg:Yellow;>t<Fg:BlueViolet;>o<Fg:Green;>u<Fg:White;>s<Fg:DeepPink2;>!<Color:Reset;>  ";
    Txt.compile();
    std::string Str;
    Txt >> Str;
    rem::push_info(source_fnl) < Str;
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
	catch(rem& )
	{
		std::cout << " Oh! rem failure here?\n";
	}

}

void test::test_console()
{
	using namespace scrat::ui;
	console::init();
	//console::me() << " Test ...\n\n\n\n";

	widget w = widget(nullptr, wclass::TopLevel);
	w.set_geometry({1,1,50,4});
	w.set_location({1,1});

    auto *lbl = new label(&w, "Hello, scrat::ui!!!");
    lbl->set_geometry({0,0, 40,1});
	lbl->set_location({1,1});

	auto* icn = new scrat::ui::icon(lbl, scrat::Icon::PencilDr);
	icn->set_location({1,0});

    auto* in = new scrat::ui::input_field(&w, {scrat::Icon::PencilDr, scrat::Icon::Success, 0, "Press any key then enter to quit"});
    in->set_geometry({{},{}, w.width()-2,1});
    in->set_location({1,2});
    in->set_state(scrat::ui::State::Active);
    in->set_state(scrat::ui::State::Success);

	try{
		w.draw();
		w.update();
	}
	catch(rem e)
	{
		;
	}
    //...
    char c;
	console::me().gotoxy({1, 6});
	console::me() << scrat::Icon::PencilDr <<  "Enter anykey + " << scrat::Icon::KeyEnter << " :";
    std::cin >> c;
    test_iolistener();
	console::terminate();
	delete lbl;
	delete icn;
}

void test::test_alu()
{
	using scrat::script::alu;
	alu a = -1;
	alu b = 30;
	alu c = b-a;
	rem::push_info() < "b{" < b() < "} - a{" < a() < "} = c{" < c() <"};";

	rem::push_info() < "1/3=" < (int)1/(int)3;
}

void test::test_iolistener()
{
    using namespace scrat;
    io::listener l(nullptr, 500);
    l.init();
    auto R = l.add_ifd(STDIN_FILENO, io::ifd::O_READ|io::ifd::I_AUTOFILL);
    auto i = l.query_fd(0);
    i->read_signal.connect(this, &test::key_in);
    std::cout << " Starting the listener:\n";
    R = l.start();
    // Exited the loop.
    l.shutdown(); // No effect on descriptor lower than 3.
}

scrat::result<> test::key_in(scrat::io::ifd &ifd)
{
    rem::push_debug(source_ffl) < color::Yellow  < ifd.pksize < " bytes in terminal input queu:";
    scrat::stracc str = "key_in[";
    str << ifd.pksize << "] : ";
    for(int x = 0; x<= ifd.pksize; x++)
    {
        auto c = *(ifd.internal_buffer + x);
        if(c == 'q') return rem::push_status(source_ffl) < "returning a rem message for quitting";
        str << "[%d]" << *(ifd.internal_buffer + x);
    }
    rem::push_info(source_ffl) < str();
    rem::push_debug(source_ffl) < color::LightCyan3 < str() < color::Reset < " Ending brutally: shutting down the listener: ";
    return rem::end;
}


