/**
 * @file   config.h
 * @author wladt
 * @date   Nov 30, 2012 8:08:31 AM
 *
 * @brief
 */

#ifndef __CWT_CONFIG_H__
#define __CWT_CONFIG_H__

#if _POSIX_C_SOURCE >= 199309L
#	define CWT_HAVE_NANOSLEEP
#endif

#if _POSIX_C_SOURCE >= 200112L
#	define CWT_HAVE_POLL   1
#	define CWT_HAVE_SELECT 1
#endif

#if _POSIX_C_SOURCE >= 1 || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#	define CWT_HAVE_SIGACTION 1
#endif

#endif /* __CWT_CONFIG_H__ */
