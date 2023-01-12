#pragma once

#include <scrat/textattr>
#include <scrat/result>
#include <scrat/object>
#include <scrat/io/listener.h>

class test
{
	scrat::object* _object = nullptr;
public:
	test() = default;
	~test();

	void run();
	void texattr();
	void test_dimension();
	void test_text();
	void put_colors();
	void test_interpret();
	void test_console();
	void test_alu();

    void test_iolistener();

	scrat::result<scrat::object*> test_result();
    scrat::result<> key_in(scrat::io::ifd& ifd);


};

