/*
 * alignof.h
 *
 *  Created on: Jan 30, 2015
 *      Author: wladt
 */

#ifndef __PFS_PP_ALIGNOF_H__
#define __PFS_PP_ALIGNOF_H__

/*
 * See http://www.wambold.com/Martin/writings/alignof.html
 */
#if defined(__cplusplus) && __cplusplus >= 201103L // >= C++11
	/* aleady have alignof */
#	define PFS_HAVE_ALIGNOF 1
#else
#	if defined (__GNUC__) && (__GNUC__ >= 2)
#		define alignof(type) __alignof__(type)
#		define PFS_HAVE_ALIGNOF 1
#	else
#		include <pfs/bits/alignof.h>
#		define alignof(type) ALIGNOF(type)
#	endif
#endif

#endif /* __PFS_PP_ALIGNOF_H__ */
