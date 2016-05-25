#include "err_handler.h"

// ******************* GLOBAL ERR VAR ********************

_Err err_line = 0;
std::string err_file = "";
std::string err_msg  = "";

//************************************************************************************

void regerr(std::string file, size_t nline, std::string msg) {
	err_line = nline;
	err_msg	 = msg;

	size_t pos = file.find_last_of("\\/");
	err_file = (std::string::npos == pos) ? "" : file.substr(pos + 1, file.length() - 1);
}

//************************************************************************************

void printErr(const std::string msg) {
	if(err_msg.length() > 0)
		std::cout << "\nError:\n   " + msg + (msg.length() > 0 ? ": " : "") << err_msg << std::endl;
}

//************************************************************************************

void printDbgErr(const std::string msg) {
	size_t pos = err_file.find_last_of("\\/");

	printErr("[" + err_file + "][ln:" + std::to_string(err_line) + "] " + msg);
}
