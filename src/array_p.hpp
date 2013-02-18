/**
 * @file   array_p.hpp
 * @author wladt
 * @date   Feb 18, 2013 4:25:32 PM
 *
 * @brief
 */

#ifndef __CWT_ARRAY_P_HPP__
#define __CWT_ARRAY_P_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

template <typename _T>
struct Array {
	size_t capacity;
	_T buffer[1];

	static Array*   alloc   (int sizeof_item, size_t capacity);
	static Array*   allocz  (int sizeof_item, size_t capacity);
	static Array*   realloc (Array *a, size_t new_capacity);
	static void     free    (Array *a);
	static Array*   clone   (const Array &a);
	static size_t   copy    (Array *, Array *, size_t off_to, size_t off_from, size_t n);
	static size_t   move    (Array *a, size_t off_to, size_t off_from, size_t n);
	static void     bzero   (Array &a);
	static const _T at      (const Array &a, size_t index);
	static _T       at      (Array &a, size_t index);
	static _T       data    (Array &a);
	static const _T data    (const Array &a);
	static void     put     (Array &a, size_t index, _T item);
	static size_t   size    (const Array &a);
	static bool     eq      (const Array &a1, const Array &a2);
} Array;

CWT_NS_END

#endif /* __CWT_ARRAY_P_HPP__ */
