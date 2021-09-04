#include "include/cgnale.hpp"

struct cgnalec
{

	std::vector<std::string> func_names;

	macro_handler_decl(cgnale_macros_dataclass)
	{ ostr << "struct macro_data\n{" << std::endl; }

	macro_handler_decl(cgnale_macro_def)
	{
		if( args.size() < 2 )
		{
			std::cerr << "Not sufficient macro args" << std::endl;
			exit(EXIT_FAILURE);
		}
		func_names.push_back(args[1]);
		ostr << "macro_handler_decl(f" << func_names.size()-1 << ")";
	}

	macro_handler_decl(cgnale_create_get_macros)
	{
		ostr <<	"};\n"
			"extern \"C\" std::vector<std::pair<std::string,macro_wrapper_base *>> cgnale_get_macros()\n"
			"{\n"
			"\tmacro_data *data_obj = new macro_data;\n"
			"\tmacro_wrapper<macro_data> *wrappers_array = new macro_wrapper<macro_data>[" << func_names.size() << "]{ {data_obj, &macro_data::f0}";
		for(int i=1; i < func_names.size(); i++ )
			ostr << ",  {data_obj, &macro_data::f" << i << "}";
		ostr << " };\n"
		"\treturn {{\"" << func_names[0] << "\",wrappers_array}";
		for(int i=1; i < func_names.size(); i++ )
			ostr << ", {\"" << func_names[i] << "\",wrappers_array+" << i << '}';
		ostr << "};\n}\n";
		/*ostr << "{ return {{\"" << func_names[0] << "\", " << 'f' << 0 << "}";
		for(int i=1; i < func_names.size(); i++ )
		ostr << ", {\"" << func_names[i] << "\", " << 'f' << i << "}";
		ostr << "}; }" << std::endl;*/
	}
	
};

extern "C" std::vector<std::pair<std::string, macro_wrapper_base * >> cgnale_get_macros()
{
	cgnalec *data_obj = new cgnalec;
	macro_wrapper<cgnalec> *wrappers_array = new macro_wrapper<cgnalec>[3]{ {data_obj, &cgnalec::cgnale_macros_dataclass}, {data_obj, &cgnalec::cgnale_macro_def},  {data_obj, &cgnalec::cgnale_create_get_macros} };
	return {{"cgnale.macros.dataclass",wrappers_array}, {"cgnale.macro",wrappers_array+1}, {"cgnale.macros.get.create", wrappers_array+2}};
}
