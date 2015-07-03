/*
 * addrinfo_unix.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#include "cwt/net/hostinfo.hpp"
#include <cwt/logger.hpp>
#include <pfs/safeformat.hpp>
#include <netdb.h>

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1) \
		|| defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#	define CWT_HAVE_GETADDRINFO 1
#else
#	include <cwt/mt.hpp>

//Since glibc 2.8: _BSD_SOURCE || _SVID_SOURCE || _GNU_SOURCE
#	if defined(__GNUC_PREREQ) && __GNUC_PREREQ(2,8) && (defined(_BSD_SOURCE) || defined(_SVID_SOURCE) || defined(_GNU_SOURCE)))
#		define CWT_HAVE_HSTRERROR 1
#	endif
#endif


namespace cwt { namespace net {




}} // cwt::net
