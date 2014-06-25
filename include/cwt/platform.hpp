/**
 * @file   platform.hpp
 * @author wladt
 * @date   Dec 31, 2013 2:34:11 PM
 *
 * @brief
 */

#ifndef __CWT_PLATFORM_HPP__
#define __CWT_PLATFORM_HPP__

#include <pfs/time.hpp>
#include <pfs/date.hpp>
#include <pfs/datetime.hpp>

namespace cwt { namespace platform {

DLL_API pfs::time currentTime ();
DLL_API pfs::date currentDate ();
DLL_API pfs::datetime currentDateTime ();
DLL_API pfs::string strerror (int errn);

struct display_data
{
	int pixelsWidth;
	int pixelsHeight;
	int mmWidth;  // width in millimeters
	int mmHeight; // height in millimeters
};

DLL_API void displayParameters (display_data * d);

}} // platform::cwt

#ifndef NDEBUG
namespace cwt { namespace platform {
struct __verify_errno
{
	int operator () (const char * file, int lineno, const char * exprtext, int errn) const
	{
		if (0 != errn)
			fprintf(stderr, "ERROR (%s[%d]): %s: %s [errno=%d]\n"
					, file, lineno, exprtext, cwt::platform::strerror(errn).c_str(), errn);
		return errn;
	}

	bool operator () (bool predicate, const char * file, int lineno, const char * exprtext, int errn) const
	{
		if (! predicate)
			fprintf(stderr, "ERROR (%s[%d]): %s: %s [errno=%d]\n"
					, file, lineno, exprtext, cwt::platform::strerror(errn).c_str(), errn);
		return predicate;
	}
};
}}
#	define CWT_VERIFY_ERRNO(errn) cwt::platform::__verify_errno()(__TFILE__, __LINE__, #errn, (errn))
#	define CWT_VERIFY_ERRNO_X(expr,errn) cwt::platform::__verify_errno()((expr), __TFILE__, __LINE__, #expr, (errn))
#else /* !NDEBUG */
#	define CWT_VERIFY_ERRNO(errn) (errn)
#	define CWT_VERIFY_ERRNO_X(expr,errn) (expr)
#endif

#endif /* __CWT_PLATFORM_HPP__ */
