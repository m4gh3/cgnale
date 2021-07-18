#pragma once

#include <iostream>
#include <map>
#include <vector>

typedef void macro_handler_t(std::ostream &ostr, std::vector<std::string> &macro_args, std::map<std::string, void *> &macro_states );
#define macro_handler_decl(name) static void name(std::ostream &ostr, std::vector<std::string> &macro_args, std::map<std::string, void *> &macro_states )

