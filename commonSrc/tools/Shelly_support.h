#ifndef __SHELLY_MACROS_H
#define __SHELLY_MACROS_H

#include <sstream> // ostringstream
#include <string>
#include <vector>
#include <stdexcept> // logic_error
#include <assert.h>
#include "tools/error_code.h"

///////////////////////////////////////////////////////////
#if NAVIPACK_WIN
#define SHELLY_OS_WINDOWS
#else

#define __int64  long long
#endif
///////////////////////////////////////////////////////////

using namespace std;

namespace shelly{
	string format(const char *fmt, ...);
	//unsigned __int64 GetCurrentTick();
	//unsigned __int64 time_tToTimestamp(const double &t);
	bool vectorToBinaryFile(const vector<char> &vec, const std::string &fileName);
	unsigned __int64 getFileSize(const std::string &fileName);
	std::string getTempFileName();
	std::string getTempFileNameD();
}

/** A macro for obtaining the name of the current function:  */
#if defined(__BORLANDC__)
#define	__CURRENT_FUNCTION_NAME__	__FUNC__
#elif defined(NAVIPACK_WIN) && (_MSC_VER>=1300)
#define	__CURRENT_FUNCTION_NAME__	__FUNCTION__
#else
#define	__CURRENT_FUNCTION_NAME__	__PRETTY_FUNCTION__
#endif

/** \def THROW_EXCEPTION(msg)
* \param msg This can be a char*, a std::string, or a literal string.
* Defines a unified way of reporting exceptions
* \sa MRPT_TRY_START, MRPT_TRY_END, THROW_EXCEPTION_CUSTOM_MSG1
*/
#define THROW_EXCEPTION(msg)	\
	{\
	std::ostringstream auxCompStr; \
	auxCompStr << "\n\n =============== SHELLY EXCEPTION =============\n"; \
	auxCompStr << __CURRENT_FUNCTION_NAME__ << ", line " << __LINE__ << ":\n"; \
	auxCompStr << msg << std::endl; \
	throw std::logic_error(auxCompStr.str()); \
	}\

/** \def THROW_STACKED_EXCEPTION
* \sa MRPT_TRY_START, MRPT_TRY_END
*/
#define THROW_STACKED_EXCEPTION(e)	\
	{\
	std::string str(e.what()); \
	if (str.find("MRPT stack trace") == std::string::npos) \
		{ \
		str += __CURRENT_FUNCTION_NAME__; \
		str += shelly::format(", line %i:\n", __LINE__); \
		throw std::logic_error(str); \
		} \
		else throw std::logic_error(e.what()); \
	}\

#define SHELLY_TRY_START	\
	try {
	\

/** \def THROW_EXCEPTION_CUSTOM_MSG1
* \param e The caught exception.
*	\param msg Is a char* or literal string.
*/
#define THROW_EXCEPTION_CUSTOM_MSG1(msg,param1)	\
	{\
		std::ostringstream auxCompStr; \
		auxCompStr << "\n\n =============== MRPT EXCEPTION =============\n"; \
		auxCompStr << __CURRENT_FUNCTION_NAME__ << ", line " << __LINE__ << ":\n"; \
		auxCompStr << shelly::format(msg, param1) << std::endl; \
		throw std::logic_error(auxCompStr.str()); \
	}\

/** The end of a standard MRPT "try...catch()" block that allows tracing throw the call stack after an exception.
* \sa MRPT_TRY_START,MRPT_TRY_END_WITH_CLEAN_UP
*/
#define SHELLY_TRY_END	\
	} \
	catch (std::bad_alloc &) \
	{ throw; } \
	catch (std::exception &e) \
	{ \
		THROW_STACKED_EXCEPTION(e); \
	} \
		catch (...) \
	{ \
		THROW_EXCEPTION("Unexpected runtime error!"); \
	} \


#endif