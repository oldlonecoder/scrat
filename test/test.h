#pragma once

#include <scrat/textattr>
#include <scrat/result>
#include <scrat/object>

class test
{
	scrat::object* _object = nullptr;
public:
	test();
	~test();

	void run();
	void texattr();
	void test_dimension();
	void test_text();

	scrat::result<scrat::object*> test_result();
};

