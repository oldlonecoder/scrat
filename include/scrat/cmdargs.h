#pragma once


/*
Standalone commandline options and arguments header only;

Copyright (C) 2011;2022  oldlonecoder/lussier.serge@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/




#include <list>
#include <map>
#include <string>
#include <sstream>
#include <vector>


// trace ouputs for dev phase - remove when finished
#include <iostream>
using std::cout;
using std::cerr;
// -------------------------------------------------

namespace scrat
{
    namespace option
    {

        class token_t {
        public:
            std::vector<std::string> s_tokens;
            char sep;

            token_t() {};
            token_t(char sep);

            ~token_t() { s_tokens.clear(); }
            int input(const std::string& data);
            std::string operator [] (int);

        };


        int token_t::input(const std::string& data)
        {
            std::string token = "";
            std::string::const_iterator s, e, i;
            i = s = data.cbegin();
            e = data.end();
            s_tokens.clear();
            for (; i != e; i++) {
                if (*i == sep) {
                    if (!token.empty())
                        s_tokens.push_back(token);
                    token = "";
                    continue;
                }
                token = token + *i;
            }
            if (!token.empty()) s_tokens.push_back(token);
            return s_tokens.size();
        }



        std::string token_t::operator[](int i)
        {

            if (i < 0 || i >= s_tokens.size()) return "";
            return s_tokens[i];

        }

        token_t::token_t(char sep)
        {
            this->sep = sep;
        }


        /*!
            * @brief Simple support for command-line arguments processing template class
            *
            * I've started to get curious about command-line args processing, and this class is what I think can be interresting to write a simple
            * command-line args with callbacks, in a single header file.
            * 
            *
            * @author  &copy; 2012, Serge Lussier (lussier.serge@gmail.com)
            *
            *
            *
            */

        template <typename T> class ccoption
        {
            /// The object of class T that will process the arguments
            T* m_processing_object;
            /// Command-line switch ( and arg-value pairs ) -- subject to change as this class evolves
            std::list<std::string> m_cmdline_data_raw;
            std::list<std::string> m_orhan_values;
            bool         m_collect_orphans;

            std::string       m_swerr;

            ccoption() {}
        public:

            enum class error :int {
                Argument,
                Switch,
                Value
            };

            struct error_data {
                std::string message;
                int    err_int;
            };
            /// (kind-of) functor as callback defined in the class T
            typedef bool (T::* _switch)(const std::string&);

            error_data make_error(int e, const std::string& msg) {
                error_data ed;
                ed.err_int = e;
                ed.message = msg;
                return ed;
            }
            /*!
                * @brief A definition node describing the switch, and the functor to be executed on the definition
                *
                * @code
                *    m_option << new ccoption<myClass>::Definition("-d", "--declare", &MyClass::mem_fun, valid_values);
                * @endcode
                */
            struct arg_switch {
                std::string m_value;
                bool m_requires_value;
                ccoption<T>::_switch m_memfun_ptr;
                bool m_used;
                arg_switch(ccoption<T>::_switch sw, bool requires_value = false);

                arg_switch() {
                    m_memfun_ptr = (ccoption<T>::_switch) 0;
                    m_requires_value = false;
                    m_used = false;
                }

                ~arg_switch() {
                    // m_value.clear();
                }
                bool require() { return m_requires_value; }
                void set_value(const std::string& v) { m_value = v; }
                std::string value() { return m_value; }
                bool& used() { return m_used; }
            };


            typedef std::map<std::string, arg_switch> switch_dictionary;
            arg_switch* m_psw;
            /// Only available Constructor
            ccoption(T* proc_obj, int argc, char** argv) {
                m_processing_object = proc_obj;
                m_collect_orphans = true;
                m_psw = 0;
                if (argc < 2) return;
                // Save everythings from argv
                for (int c = 1; c < argc; c++) m_cmdline_data_raw.push_back(argv[c]);
            }
            ~ccoption();
            int         process() throw(ccoption<T>::Error);
            int         initialize_data();
            int         add(const std::string& swKey, ccoption<T>::_switch sw_functor, bool rv = false);
            bool& collect_orphans() { return m_collect_orphans; }
            arg_switch* switch_ptr() { return m_psw; }
            std::string sw() { return m_swerr; }
            void clear();

        private:

            switch_dictionary m_switches;
        };



        template <typename T>  void ccoption<T>::clear() {
            cerr << __PRETTY_FUNCTION__ << "\n";
            if (!m_switches.empty()) m_switches.clear();
            if (!m_cmdline_data_raw.empty()) m_cmdline_data_raw.clear();
            if (!m_orhan_values.empty()) m_orhan_values.clear();
        }


        template <typename T>  ccoption<T>::~ccoption()
        {
            clear();
        }

        /*!
            * @brief builds the std::list of arguments that matches the switches dictionary
            *
            * Whatever does not matches any of the defined Switches, it causes this function to throw an exception of type `std::string` .
            */
        template <typename T> int ccoption<T>::initialize_data()
        {
            /// Iterators on m_cmdline_data_raw.
            std::list<std::string>::iterator cb, ce, ck;
            /// char iterator on ck
            std::string::const_iterator  pk, nk;
            std::string  sw_key = "", tmp;
            token_t tokens('=');
            if (!m_switches.size()) return 0;
            cb = m_cmdline_data_raw.begin();
            try {
                for (; cb != m_cmdline_data_raw.end(); cb++) {
                    // Must determine the switch against the Switches dictionary
                    tmp = *cb;
                    pk = tmp.begin();
                    switch (*pk) {
                    case '-': // Maybe a short switch name
                        nk = std::next(pk);
                        if (*nk == '-') {
                            // This is a long switch
                            sw_key = tmp;
                            tokens.sep = '=';
                            tokens.input(sw_key);
                            sw_key = tokens[0];
                            tmp = tokens[1];// get empty contents if nomore tokens
                            if (!tmp.empty())
                                //insert into the std::strings std::list to be processed on the next iteration
                                m_cmdline_data_raw.insert(std::next(cb), tmp);

                            if (m_switches.find(sw_key) != m_switches.end()) {
                                m_psw = &m_switches[sw_key];
                                m_psw->used() = true;
                            }
                            else {
                            getout:
                                m_psw = 0;
                                m_swerr = sw_key;
                                throw ccoption<T>::Error::Switch;
                            }
                        }
                        else {
                            // short switch
                            sw_key = std::string(pk, next(nk));
                            if (m_switches.find(sw_key) != m_switches.end()) {
                                m_psw = &m_switches[sw_key];
                                m_psw->used() = true;
                            }
                            else goto getout;
                            // is value not separated ?
                            ++nk;
                            if (nk != tmp.end()) {
                                --nk;
                                // push value token into the std::list, the next iteration will have it
                                tmp = std::string(next(nk), tmp.cend());
                                m_cmdline_data_raw.insert(std::next(cb), tmp);
                                break;
                            }
                        }
                        break;
                        //case '/': // upcoming '/' support 
                        //    break;
                    default:
                        if (m_psw) {
                            if (m_psw->require()) {
                                m_psw->set_value(tmp);
                                m_psw = 0;
                                break;
                            }
                        }
                        //cout << tmp << ": it is a value as token not bound to a switch ( orphan value ) :\n ";
                        // Landed here because this token is not a switch.
                        //cout <<  (collect_orphans() ? " collecting" : "dismissing") << " the value" << "\n";
                        if (collect_orphans()) {
                            //cout << "collecting value `" << tmp << "` into the collection\n";
                            m_orhan_values.push_back(tmp);
                        }
                        break;
                    }
                }
            }
            catch (std::exception e) {
                cerr << e.what() << "\n";
                throw std::string(e.what()) + " aborted in ccoption\n";
            }
            return 0;
        }



        template <typename T> int ccoption<T>::add(const std::string& swKey, ccoption< T >::_switch sw_functor, bool rv)
        {

            m_switches[swKey] = ccoption<T>::arg_switch(sw_functor, rv);
            return m_switches.size();
        }


        template<typename T> ccoption<T>::arg_switch::arg_switch(ccoption<T>::_switch sw, bool r)
        {
            m_memfun_ptr = sw;
            m_requires_value = r;
            m_used = false;
        }


        template<typename T> int ccoption<T>::process() throw (ccoption<T>::Error)
        {
            if (m_cmdline_data_raw.empty() || m_switches.empty()) {
                return 0;
            }
            typename switch_dictionary::iterator it = m_switches.begin();
            typename switch_dictionary::iterator et = m_switches.end();

            for (; it != et; it++) {
                ccoption<T>::arg_switch s = it->second;
                if (!s.used()) continue;
                if (s.m_memfun_ptr) {
                    if (!(m_processing_object->*(s.m_memfun_ptr))(s.value())) {
                        m_psw = &m_switches[it->first];
                        m_swerr = it->first;
                        throw Error::Value;
                    }
                }
            }
            return m_switches.size();
        }




        template<typename V> V toType(std::string v) throw (std::string)
        {
            V dest;
            std::istd::stringstream in(v);
            try {
                in >> dest;
                if (in.fail()) {
                    throw " (part of ) value `" + v + "` cannot be converted by :\n" + __PRETTY_FUNCTION__;
                }
                return dest;
            }
            catch (std::exception e) {
                throw e.what();
            }
        }
    } // namespace option

} // namespace tea

