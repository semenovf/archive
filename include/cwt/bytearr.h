/*
 * bytearr.h
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */

#ifndef __CWT_BYTEARR_H__
#define __CWT_BYTEARR_H__

#include <cwt/vector.h>

typedef CwtVector CwtByteArray;

typedef struct _CwtByteArrayNS {
	_CWT_VECTOR_COMMON_NS
	void          (*init)          (CwtByteArray *ba);
	void          (*initSized)     (CwtByteArray *ba, size_t initial_size, size_t max_capacity);
	CwtVector*    (*create)        (void);
	CwtVector*    (*createSized)   (size_t initial_size, size_t max_capacity);
	BYTE          (*at)            (CwtByteArray *ba, size_t index);
	BYTE          (*first)         (CwtByteArray *ba);
	BYTE          (*last)          (CwtByteArray *ba);
	BYTE*         (*data)          (CwtByteArray *ba);
	const BYTE*   (*constData)     (CwtByteArray *ba);
	void          (*append)        (CwtByteArray *ba, BYTE b);
	void          (*appendBytes)   (CwtByteArray *ba, const BYTE *bytes, size_t n);
	void          (*pushBack)      (CwtByteArray *ba, BYTE b);
	void          (*pushFront)     (CwtByteArray *ba, BYTE b);
	void          (*insert)        (CwtByteArray *ba, BYTE b, size_t pos);
	void          (*insertRep)     (CwtByteArray *ba, BYTE b, size_t pos, size_t n);
	void          (*fill)          (CwtByteArray *ba, BYTE b, size_t n);
	BOOL          (*indexOf)       (CwtByteArray *ba, BYTE b, size_t off_from, size_t *index);
	BOOL          (*replace)       (CwtByteArray *ba, BYTE b, size_t pos);
	const char*   (*cstr)          (CwtByteArray *ba);
	void          (*ltrim)         (CwtByteArray *ba);
	void          (*rtrim)         (CwtByteArray *ba);
	void          (*trim)          (CwtByteArray *ba);
} CwtByteArrayNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtByteArrayNS* cwt_bytearray_ns(void);
EXTERN_C_END


#endif /* __CWT_BYTEARR_H__ */
