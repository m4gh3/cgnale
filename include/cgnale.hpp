#pragma once

/*This file is part of cgnale.

    cgnale is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cgnale is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cgnale.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <map>
#include <vector>

typedef void macro_handler_t(std::ostream &ostr, std::vector<std::string> &args, std::map<std::string, void *> &gloabal_states );
//#define macro_handler_decl(name) static void name(std::ostream &ostr, std::vector<std::string> &args, std::map<std::string, void *> &global_states )
#define macro_handler_decl(name) void name(std::ostream &ostr, std::vector<std::string> &args, std::map<std::string, void *> &global_states )


struct macro_wrapper_base
{
	void *data;
	virtual void operator() (std::ostream &ostr, std::vector<std::string> &args, std::map<std::string, void *> &global_states ) = 0;
};

template<typename T> struct macro_wrapper : macro_wrapper_base
{

	macro_handler_decl((T::*macro));

	virtual void operator() (std::ostream &ostr, std::vector<std::string> &args, std::map<std::string, void *> &global_states )
	{ (((T*)data)->*macro)(ostr, args, global_states ); }

	macro_wrapper(T *data_,  macro_handler_decl((T::*macro_)) )
	{
		macro = macro_;
		data = data_;
	}

};
