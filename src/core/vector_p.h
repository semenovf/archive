/**
 * @file   vector_p.h
 * @author wladt
 * @date   Dec 24, 2012 9:06:09 AM
 *
 * @brief
 */

#ifndef __CWT_VECTOR_P_H__
#define __CWT_VECTOR_P_H__

#include "cwt/vector.h"

typedef struct _DataShared {
	int     sizeof_item;
	int     refs;
	size_t  capacity;
	size_t  max_capacity;
	char   *buffer;
} DataShared;

typedef struct _CwtVectorImpl
{
	CwtVector __base;
	size_t  head;
    size_t  count;
    DataShared *d;
} CwtVectorImpl;


extern void        v_deref_shared  (DataShared *d);
extern void        v_init_shared   (DataShared *d, int sizeof_item, size_t initial_size, size_t max_size);
extern void        v_copy_shared   (DataShared *to, DataShared *from, size_t off, size_t count);
extern DataShared* v_clone_shared  (DataShared *d, size_t off, size_t count);
extern BOOL        v_right_reserve_shared (CwtVectorImpl *v);
extern void        v_mreserve_shared   (CwtVectorImpl *v);

#endif /* __CWT_VECTOR_P_H__ */
