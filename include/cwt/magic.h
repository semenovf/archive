/**
 * @file   magic.h
 * @author wladt
 * @date   Dec 24, 2012 8:55:46 AM
 *
 * @brief Contains magic numbers for CWT classes
 */

#ifndef __CWT_MAGIC_H__
#define __CWT_MAGIC_H__

#define CWT_MAGIC_VECTOR 0xDF0001FD

#ifdef __COMMENT__
#ifndef CWT_NDEBUG
#	include <cwt/magic.h>
#	define _ASSERT_MAGIC(v) CWT_ASSERT(_CAST_IMPL(v)->__base.magic == CWT_MAGIC_VECTOR)
#	define _EQ_MAGIC(v)     (_CAST_IMPL(v)->__base.magic == CWT_MAGIC_VECTOR)
#	define _SET_MAGIC(v)    _CAST_IMPL(v)->__base.magic = CWT_MAGIC_VECTOR
#	define _RESET_MAGIC(v)  _CAST_IMPL(v)->__base.magic = 0
#else
#	define _ASSERT_MAGIC(v)
#	define _EQ_MAGIC(v)
#	define _SET_MAGIC(v)
#	define _RESET_MAGIC(v)
#endif
#endif


#endif /* __CWT_MAGIC_H__ */
