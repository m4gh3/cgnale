#include "include/cgnale.hpp"

macro_handler_decl(cgnale_macro_def)
{
	if( macro_args.size() < 2 )
	{
		std::cerr << "Not sufficient macro args" << std::endl;
		exit(EXIT_FAILURE);
	}
	if( macro_states["cgnale.macro"] == NULL )
	{
		macro_states["cgnale.macro"] = new std::vector<std::string>();
	}
	std::vector<std::string > &func_names = *(std::vector<std::string> *) macro_states["cgnale.macro"];
	func_names.push_back(macro_args[1]);
	ostr << "macro_handler_decl(f" << func_names.size()-1 << ")";
}

macro_handler_decl(cgnale_create_get_macros)
{
	auto cgnale_macro = macro_states.find("cgnale.macro");
	if( cgnale_macro != macro_states.end() )
	{

		std::vector<std::string > &func_names = *(std::vector<std::string> *) cgnale_macro->second;
		ostr << "extern \"C\" std::vector<std::pair<std::string,macro_handler_t *>> cgnale_get_macros()" << std::endl;
		ostr << "{ return {{\"" << func_names[0] << "\", " << 'f' << 0 << "}";
		for(int i=1; i < func_names.size(); i++ )
			ostr << ", {\"" << func_names[i] << "\", " << 'f' << i << "}";

		ostr << "}; }" << std::endl;

	}
}

extern "C" std::vector<std::pair<std::string,macro_handler_t *>> cgnale_get_macros()
{ return {{"cgnale.macro",cgnale_macro_def}, {"cgnale.macros.get.create", cgnale_create_get_macros}}; }
