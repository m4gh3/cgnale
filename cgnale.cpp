/*
 * This file is part of cgnale

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

#include "include/cgnale.hpp"
#include <sstream>
#include <fstream>
#include <getopt.h>
#include <dlfcn.h>

/*typedef void macro_handler_t(std::ostream &ostr, std::vector<std::string> &macro_args, std::map<std::string, void *> &global_states );
#define macro_handler_decl(name) void name(std::ostream &ostr, std::vector<std::string> &macro_args, std::map<std::string, void *> &global_states )*/

std::map<std::string, macro_wrapper_base *> macro_handlers;
std::map<std::string, void *> global_states;

void check_eof(int c)
{
	if( c == EOF )
	{
		std::cerr << "Error unexpected EOF in stream" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int subst_escape(int c)
{
	switch(c)
	{
		case 'n':
			return '\n';
		case 't':
			return '\t';
		default:
			return c;
	}
}

std::string str_escape(std::string s)
{
	std::ostringstream ostr;
	int i=0;
	for(; i < s.size()-1; i++ )
		if(s[i] != '\\')
			ostr << s[i];
		else
			ostr << (char)subst_escape(s[++i]);
	if( i < s.size() )
		ostr << s[i];
	return ostr.str();
}

std::string str_escape(char *s)
{ return str_escape(std::string(s)); }


void macro_parse(std::istream &istr, std::ostream &ostr )
{

	std::vector<std::string> macro_args;
	std::ostringstream ostringstr;

	int c = istr.get();
	while(1)
	{

		check_eof(c);
		
		if( c == '\\' )
		{
			check_eof( c = istr.get() );
			ostringstr << (char)subst_escape(c);
		}
		else if( c == ':' )
		{

			macro_args.push_back(ostringstr.str());
			ostringstr.str("");
			//ostringstr.clear();

			check_eof( c = istr.get() );
			
			if( c == '}' )
				break;
			else	
				continue;

		}
		else
			ostringstr << (char)c;

		c = istr.get();

	}

	auto handler = macro_handlers.find(macro_args[0]);

	if( handler != macro_handlers.end() )
	{
		(*(handler->second))(ostr, macro_args, global_states );
	}
	else
	{
		std::cerr << "Unknown macro name \"" << macro_args[0] << '\"' << std::endl;
		exit(EXIT_FAILURE);
	}

}

void normal_parse(std::istream &istr, std::ostream &ostr, bool genmode )
{
	int c = istr.get();
	std::ostringstream macro_ostr;
	int retval = 0;
	for(; c != EOF; )
	{
		if( c == '{' )
		{
			c = istr.get();
			if( c == ':' )
			{
				c = istr.get();
				if( c == ':' )
				{
					if(genmode)
						ostr << "{:";
				}
				else
				{
					istr.putback(c);
					macro_parse(istr, macro_ostr );
					std::istringstream sub_istr(macro_ostr.str());
					std::ostringstream sub_ostr;
					normal_parse(sub_istr, sub_ostr, 1 );
					ostr << sub_ostr.str();
					macro_ostr.str("");
				}
			}
			else
			{
				if(genmode)
					ostr << '{';
				continue;
			}
		}
		else
			if(genmode)
				ostr << (char)c;
		c = istr.get();
	}
}

void show_help()
{
	exit(EXIT_FAILURE);
}

void load_lib(char *path)
{

	void *libhandle = dlopen(path, RTLD_LAZY );
	if(!libhandle)
	{
		fprintf(stderr, "%s\n", dlerror() );
        	exit(EXIT_FAILURE);
	}
	
	dlerror();
	
	std::vector<std::pair<std::string, macro_wrapper_base * >> (*cgnale_get_macros)();

	*(void **) (&cgnale_get_macros) = dlsym(libhandle, "cgnale_get_macros" );

	for(auto [first,second] : cgnale_get_macros() )
		macro_handlers[first] = second;

}

int main(int argc, char **argv )
{

	std::vector<std::istream *> genstreams;
	std::vector<bool> genmodes;
	int mode;

	for(int option; (option = getopt(argc, argv, "c:f:l:s:" ) ) != -1; )
	{
		switch(option)
		{
			case 's':
				genstreams.push_back(new std::istringstream(str_escape(optarg)));
				genmodes.push_back(1);
				break;
			case 'c':
				genstreams.push_back(new std::ifstream(optarg));
				genmodes.push_back(1);
				break;
			case 'f':
				genstreams.push_back(new std::ifstream(optarg));
				genmodes.push_back(0);
				break;
			case 'l':
				load_lib(optarg);
				break;
		}
	}

	for(int i=0; i < genstreams.size(); i++ )
		normal_parse(*genstreams[i], std::cout, genmodes[i] );

	return 0;

}
