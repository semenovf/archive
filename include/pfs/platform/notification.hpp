/*
 * notification.hpp
 *
 *  Created on: Nov 8, 2015
 *      Author: wladt
 */

#ifndef __PFS_PLATFORM_NOTIFICATION_HPP__
#define __PFS_PLATFORM_NOTIFICATION_HPP__

#include <pfs/notification.hpp>
#include <pfs/platform/string.hpp>

namespace pfs { namespace platform {

typedef pfs::basic_notification<string_type> notification_type;

}} // pfs::platform

#endif /* __PFS_PLATFORM_NOTIFICATION_HPP__ */
