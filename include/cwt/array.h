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

typedef struct _CwtArray {
	int     sizeof_item;
	size_t  capacity;
	size_t  max_capacity;
	char   *buffer;
} CwtArray;

typedef struct _CwtVectorNS {
	void          (*init)          (CwtVector *v, int sizeof_item);
	void          (*initSized)     (CwtVector *v, int sizeof_item, size_t initial_size, size_t max_size);
} CwtArrayNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtArrayNS* cwt_array_ns(void);
EXTERN_C_END

#endif /* __CWT_ARRAY_H__ */
