/**
 * @file   platform.hpp
 * @author wladt
 * @date   Dec 31, 2013 2:34:11 PM
 *
 * @brief
 */

#ifndef __PFS_PLATFORM_HPP__
#define __PFS_PLATFORM_HPP__

#include <pfs/time.hpp>
#include <pfs/date.hpp>
#include <pfs/datetime.hpp>
#include <pfs/error_code.hpp>

namespace pfs { namespace platform {

DLL_API pfs::time current_time ();
DLL_API pfs::date current_date ();
DLL_API pfs::datetime current_datetime ();

DLL_API string to_string (const error_code & ex);

/**
 * @brief Format date and time.
 *
 * @param max Maximum size of resulting string in code units.
 * @param format Format specification (@see strftime(3))
 * @param tm Time specification.
 *
 * @return Resulting string.
 *
 * @note This function is a wrapper around ANSI C strftime() or wcsftime()
 *       (depends on pfs::string implementation).
 */
pfs::string strftime (const pfs::string & format, const struct tm & tm, size_t max = 128);


struct display_data
{
	int pixelsWidth;
	int pixelsHeight;
	int mmWidth;  // width in millimeters
	int mmHeight; // height in millimeters
};

DLL_API void displayParameters (display_data * d);

}} // pfs::platform

//#ifndef NDEBUG
//namespace pfs { namespace platform {
//struct __verify_errno
//{
//	int operator () (const char * file, int lineno, const char * exprtext, int errn) const
//	{
//		pfs::string errstr;
//		lexical_cast(pfs::error_code(errn), errstr);
//
//		if (0 != errn)
//			fprintf(stderr, "ERROR (%s[%d]): %s: %s [errno=%d]\n"
//					, file, lineno, exprtext, errstr.c_str(), errn);
//		return errn;
//	}
//
//	bool operator () (bool predicate, const char * file, int lineno, const char * exprtext, int errn) const
//	{
//		pfs::string errstr;
//		lexical_cast(pfs::error_code(errn), errstr);
//
//		if (! predicate)
//			fprintf(stderr, "ERROR (%s[%d]): %s: %s [errno=%d]\n"
//					, file, lineno, exprtext, errstr.c_str(), errn);
//		return predicate;
//	}
//};
//}}
//#	define PFS_VERIFY_ERRNO(errn) platform::__verify_errno()(__TFILE__, __LINE__, #errn, (errn))
//#	define PFS_VERIFY_ERRNO_X(expr,errn) platform::__verify_errno()((expr), __TFILE__, __LINE__, #expr, (errn))
//#else /* !NDEBUG */
//#	define PFS_VERIFY_ERRNO(errn) (errn)
//#	define PFS_VERIFY_ERRNO_X(expr,errn) (expr)
//#endif

#endif /* __PFS_PLATFORM_HPP__ */
