#pragma once

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
