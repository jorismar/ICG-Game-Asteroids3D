#ifndef JB_ERR_HANDLER_H
#define JB_ERR_HANDLER_H

#include <iostream>
#include <string>

typedef unsigned int _Err;

// ********************* ERROR CODES *********************

#define	FILE_NOT_FOUND			0x001
#define FILE_READ_ERROR			0x002

#define ASSIMP_IMPORTER_ERROR	0x064
#define ASSIMP_NOT_HAS_TEXTURE	0x065

#define IL_WRONG_VERSION		0x0C8
#define IL_LOAD_IMG_FAIL		0x0C9
#define IL_CONVERT_IMG_FAIL		0x0CA

// ********************* ERROR FUNC **********************

#define ERR(cond, err, ret)		\
	if(cond) {					\
		regerr(err, __LINE__);	\
		return ret;				\
	}

// ****************** USER FUNCTIONS *********************

/*********************************************************
 \brief	Record the error code and the line number.
 \param	code		Error code.
 \param	nline		Line number where the error occurred.
**********************************************************/
void regerr(_Err code, size_t nline);

/*********************************************************
 \brief	Print the current error mensage.
 \param	msg		Custom user mensage.
**********************************************************/
void printErr(const std::string msg);

/*********************************************************
 \brief	Print the current error mensage in debug mode.
 \param	msg		Custom user mensage.
**********************************************************/
void printDbgErr(const std::string msg);

#endif // !JB_ERR_HANDLER_H
