#pragma once

#ifdef _WIN32
# pragma warning(disable : 4996)
# pragma warning(disable : 4251)

# define _CRT_SECURE_NO_WARNINGS
# define _SCL_SECURE_NO_WARNINGS

#ifdef SCRAT_EXPORT
	# define SCRAT_API __declspec(dllexport)
#else 
	# define SCRAT_API __declspec(dllimport)
#endif 


#include <Windows.h>
#include <thread>
#include <cstdint>  ///< uintxx_t

#define __PRETTY_FUNCTION__ __FUNCSIG__

#endif 

# define _decl_objname \
	public:\
		virtual const char* class_name() const noexcept;\
    private:

# define _object_name(CLASS)\
	const char* CLASS::class_name() const noexcept{ return #CLASS; }
