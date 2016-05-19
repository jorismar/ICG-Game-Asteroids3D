#include "err_handler.h"

// ******************* GLOBAL ERR VAR ********************

_Err err_code = 0;
_Err err_line = 0;
std::string err_file = "";

//************************************************************************************

void regerr(_Err code, std::string file, size_t nline) {
	err_code = code;
	err_line = nline;

	size_t pos = file.find_last_of("\\/");
	err_file = (std::string::npos == pos) ? "" : file.substr(pos + 1, file.length() - 1);
}

//************************************************************************************

void printErr(const std::string msg) {
	std::string str_err;

	if (err_code == FILE_NOT_FOUND) str_err = "The requested file not found.";
	else if (err_code == FILE_READ_ERROR) str_err = "File read error.";
	else if (err_code == IL_WRONG_VERSION) str_err = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
	else if (err_code == IL_CONVERT_IMG_FAIL) str_err = "Couldn't convert image.";
	else if (err_code == IL_LOAD_IMG_FAIL) str_err = "Couldn't load texture image.";
	else if (err_code == ASSIMP_IMPORTER_ERROR) str_err = "Importer error.";
	else if (err_code == ASSIMP_NOT_HAS_TEXTURE) str_err = "Support for meshes with embedded textures is not implemented.";
	//	else if (err_code == ) str_err = ;
	else return;

	std::cout << "\nError:\n   " + msg + (msg.length() > 0 ? ": " : "") << str_err << std::endl;
}

//************************************************************************************

void printDbgErr(const std::string msg) {
	size_t pos = err_file.find_last_of("\\/");

	printErr("[" + err_file + "][ln:" + std::to_string(err_line) + "] " + msg);
}
