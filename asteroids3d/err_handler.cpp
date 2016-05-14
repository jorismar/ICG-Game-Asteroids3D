#include "err_handler.h"

// ******************* GLOBAL ERR VAR ********************

_Err err_code = 0;
_Err err_line = 0;

//************************************************************************************

void regerr(_Err code, size_t nline) {
	err_code = code;
	err_line = nline;
}

//************************************************************************************

void printErr(const std::string msg) {
	std::string str_err;

	if (err_code == FILE_NOT_FOUND) str_err = "The requested file not found.";
	else if (err_code == FILE_READ_ERROR) str_err = "File read error.";
	else if (err_code == IL_WRONG_VERSION) str_err = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
	else if (err_code == IL_CONVERT_IMG_FAIL) str_err = "Couldn't convert image";
	else if (err_code == IL_LOAD_IMG_FAIL) str_err = "";
	else if (err_code == ASSIMP_IMPORTER_ERROR) str_err = "";
	else if (err_code == ASSIMP_NOT_HAS_TEXTURE) str_err = "";
	//	else if (err_code == ) str_err = ;
	else return;

	std::cout << (msg.length() > 0 ? msg + " :: " : "") << str_err << std::endl;
}

//************************************************************************************

void printDbgErr(const std::string msg) {
	printErr("\t[ERROR:ln " + std::to_string(err_line) + "] " + msg);
}
