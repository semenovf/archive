/**
 * @file   array.h
 * @author wladt
 * @date   Dec 24, 2012 5:08:06 PM
 *
 * @brief
 */

#ifndef __CWT_ARRAY_H__
#define __CWT_ARRAY_H__

#include <cwt/types.h>

#define CWT_STATIC_ARRAY_BEGIN(arr,T,n)                      \
	struct {                                                 \
		UINT32  sizeof_item : 31;                            \
		UINT32  alloc : 1;                                   \
		size_t  capacity;                                    \
		T       buffer[n];                                 	 \
	} __static_##arr = { sizeof(T), 0, n, {

#define CWT_STATIC_ARRAY_END(arr) }}; CwtArray *arr = (CwtArray *)&__static_##arr;

/*
#define CWT_STATIC_ARRAY(arr,T,n)                            \
	struct {                                                 \
		UINT32  sizeof_item : 31;                            \
		UINT32  alloc : 1;                                   \
		size_t  capacity;                                    \
		T       buffer[n];                                 	 \
	} __static_##arr = { sizeof(T), 0, n, }};                \
	CwtArray *arr = (CwtArray *)&__static_##arr;
*/


#ifdef __COMMENT__
/* Used variadic macros */
#define CWT_STATIC_ARRAY_DEF(arr,T,n,...)                    \
	struct {                                                 \
		UINT32  sizeof_item : 31;                            \
		UINT32  alloc : 1;                                   \
		size_t  capacity;                                    \
		T       buffer[n];                                 	 \
	} __static_##arr = { sizeof(T), 0, n, { __VA_ARGS__ } }; \
	CwtArray *arr = (CwtArray *)&__static_##arr;
#endif

typedef struct _CwtArray {
	UINT32  sizeof_item : 31;
	UINT32  alloc : 1; /* == 1 if array allocated */
	size_t  capacity;
	char    buffer[1];
} CwtArray;

typedef struct _CwtArrayNS {
	CwtArray*     (*alloc)    (int sizeof_item, size_t capacity);
	CwtArray*     (*allocz)   (int sizeof_item, size_t capacity);
	CwtArray*     (*realloc)  (CwtArray *, size_t new_capacity);
	void          (*free)     (CwtArray *);
	CwtArray*     (*clone)    (CwtArray *);
	size_t        (*copy)     (CwtArray *, CwtArray *, size_t off_to, size_t off_from, size_t n);
	size_t        (*move)     (CwtArray *, size_t off_to, size_t off_from, size_t n);
	void          (*bzero)    (CwtArray *);
	void*         (*data)     (CwtArray *);
	const void*   (*at)       (CwtArray *, size_t index);
	void*         (*put)      (CwtArray *, size_t index, const void *item);
	size_t        (*size)     (CwtArray *);
	BOOL          (*eq)       (CwtArray *, CwtArray *);

	CwtArray*     (*allocChar)     (size_t capacity);
	CwtArray*     (*allocShort)    (size_t capacity);
	CwtArray*     (*allocInt)      (size_t capacity);
	CwtArray*     (*allocLong)     (size_t capacity);
	CwtArray*     (*allocLongLong) (size_t capacity);
} CwtArrayNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtArrayNS* cwt_array_ns(void);
EXTERN_C_END

#endif /* __CWT_ARRAY_H__ */
