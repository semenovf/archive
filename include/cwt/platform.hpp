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

namespace cwt {

class DLL_API platform
{
	platform (); // disable constructor

public:
    static pfs::time currentTime ();
	static pfs::date currentDate ();
	static pfs::datetime currentDateTime ();
};

} // cwt

#endif /* __CWT_PLATFORM_HPP__ */
