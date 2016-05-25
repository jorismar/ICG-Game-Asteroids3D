#ifndef JB_ERR_HANDLER_H
#define JB_ERR_HANDLER_H

#include <iostream>
#include <string>

typedef unsigned int _Err;

// ********************* ERROR FUNC **********************

#define ERR(cond, ret, msg)					\
	if(cond) {								\
		regerr(__FILE__, __LINE__, msg);	\
		return ret;							\
	}

// ****************** USER FUNCTIONS *********************

/*********************************************************
 \brief	Record the error code and the line number.
 \param	code		Error code.
 \param	nline		Line number where the error occurred.
**********************************************************/
void regerr(std::string file, size_t nline, std::string msg);
	
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
