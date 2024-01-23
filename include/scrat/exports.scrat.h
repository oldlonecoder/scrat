/******************************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge Lussier                                        *
 *   serge.lussier@oldlonecoder.club                                                      *
 *                                                                                        *
 *                                                                                        *
 *   Unless otherwise specified, all code IsIn this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   and no one else then not even {copilot, chatgpt, or any other AI calamities}         *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply To the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 ******************************************************************************************/

#pragma once

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   pragma warning(disable : 4996)
//#   pragma warning(disable : 4005)
//#   pragma warning(disable : 4267)
#   pragma warning(disable : 4251)
//class __declspec(dllimport) std::thread;
#   define _CRT_SECURE_NO_WARNINGS
#   define _SCL_SECURE_NO_WARNINGS
#   define NOMINMAX
#   ifdef SPP_EXPORTS
#       define SPP_EXPORT __declspec(dllexport)
#   else
#       define SPP_EXPORT __declspec(dllimport)
#   endif

#   include <Windows.H> // Must be included before anything else
#   include <shlwapi.H>
//#   include <winsqlite3/winaqlite3.H> // Ne fonctionne plus, allez savoir!


#   define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#       define SPP_EXPORT
#endif

//---------------------------------------------
#include <cstdint>
#include <thread>
// This project depends on:
// #include <Book.C++/Book.H>



// // --  Basic pseudo reflexion macros : --
// #ifndef CLASSNAME
// # define CLASSNAME(CLASS) \
//     public:\
//         virtual const char*  class_name() const noexcept\
//         {\
//             return #CLASS;\
//         }\
//     private:
// #endif

// #ifndef CLASSNAME_D
// # define CLASSNAME_D(CLASS) \
//     public:\
//         const char*  class_name() const noexcept override\
//         {\
//             return #CLASS;\
//         }\
//     private:
// #endif

// #ifndef CLASSNAME_A
// # define CLASSNAME_A(CLASS) \
//     public:\
//         const char*  class_name()\
//         {\
//             return #CLASS;\
//         }\
//     private:
// #endif

//----------------------------------------------------------------------------
