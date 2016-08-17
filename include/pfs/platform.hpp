/**
 * @file   platform.hpp
 * @author wladt
 * @date   Dec 31, 2013 2:34:11 PM
 *
 * @brief
 */

#ifndef __PFS_PLATFORM_HPP__
#define __PFS_PLATFORM_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace platform {

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
 * 
 * @note DEPRECATED
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


#endif /* __PFS_PLATFORM_HPP__ */
