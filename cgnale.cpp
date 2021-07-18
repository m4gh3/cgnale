#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>
#include <getopt.h>
#include <dlfcn.h>

typedef void macro_handler_t(std::ostream &ostr, std::vector<std::string> &macro_args, std::map<std::string, void *> &macro_states );
#define macro_handler_decl(name) void name(std::ostream &ostr, std::vector<std::string> &macro_args, std::map<std::string, void *> &macro_states )

std::map<std::string, macro_handler_t *> macro_handlers;
std::map<std::string, void *> macro_states;

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
			ostr << (char)subst_escape(c);
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
		(handler->second)(ostr, macro_args, macro_states );
	}
	else
	{
		std::cerr << "Unknown macro name" << std::endl;
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
	
	std::vector<std::pair<std::string,macro_handler_t *>> (*cgnale_get_macros)();

	*(void **) (&cgnale_get_macros) = dlsym(libhandle, "cgnale_get_macros" );

	for(auto [first,second] : cgnale_get_macros() )
		macro_handlers[first] = second;

}

int main(int argc, char **argv )
{

	std::vector<std::ifstream> genfiles;
	std::vector<bool> genmodes;
	int mode;

	for(int option; (option = getopt(argc, argv, "c:f:l:" ) ) != -1; )
	{
		switch(option)
		{
			case 'c':
				genfiles.push_back(std::ifstream(optarg));
				genmodes.push_back(1);
				break;
			case 'f':
				genfiles.push_back(std::ifstream(optarg));
				genmodes.push_back(0);
				break;
			case 'l':
				load_lib(optarg);
				break;
		}
	}

	for(int i=0; i < genfiles.size(); i++ )
		normal_parse(genfiles[i], std::cout, genmodes[i] );

	return 0;

}