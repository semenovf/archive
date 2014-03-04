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


struct display_data
{
	int pixelsWidth;
	int pixelsHeight;
	int mmWidth;  // width in millimeters
	int mmHeight; // height in millimeters
};

DLL_API void displayParameters (display_data * d);

}} // platform::cwt

#endif /* __CWT_PLATFORM_HPP__ */
