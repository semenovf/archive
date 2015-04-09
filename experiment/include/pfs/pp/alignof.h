/*
 * alignof.h
 *
 *  Created on: Jan 30, 2015
 *      Author: wladt
 */

#ifndef __PFS_PP_ALIGNOF_H__
#define __PFS_PP_ALIGNOF_H__

#ifndef __PFS_H__
#	error Do not need to include this file directly
#endif

/*
 * See http://www.wambold.com/Martin/writings/alignof.html
 */
#if defined(__cplusplus) && __cplusplus >= 201103L // >= C++11
	/* already have alignof */
#	define PFS_HAVE_ALIGNOF 1
#else
#	if defined (__GNUC__) && (__GNUC__ >= 2)
#		define alignof(type) __alignof__(type)
#		define PFS_HAVE_ALIGNOF 1
#	elif defined(PFS_CC_MSVC_VERSION) && && PFS_CC_MSVC_VERSION >= 1600
#		define alignof(type) __alignof(type)
#		define PFS_HAVE_ALIGNOF 1
#	else
#		include <pfs/bits/alignof.h>       /* TODO DEPRICATED */
#		define alignof(type) ALIGNOF(type) /* TODO DEPRICATED */
#	endif
#endif

#ifdef __cplusplus
	template <class T>
	struct alignment_of
	{
#if __cplusplus >= 201103L // >= C++11
		/* already have alignment_of */
#else
		struct helper
		{
			char a;
			T b;
		};

		enum { value = sizeof(helper) - sizeof(T) };
#endif
	};
#endif

#endif /* __PFS_PP_ALIGNOF_H__ */
