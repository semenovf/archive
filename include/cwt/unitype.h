/*
 * unitype.h
 *
 *  Created on: 02.08.2012
 *      Author: wladt
 */

#ifndef __CWT_UNITYPE_H__
#define __CWT_UNITYPE_H__

#include <cwt/types.h>

typedef struct _CwtUniType {
	union {
/*
		char      char_val;
        SBYTE     sbyte_val;
        BYTE      byte_val;
        CWT_CHAR  cwt_char_val;
        SHORT     short_val;
        USHORT    ushort_val;
        INT       int_val;
        UINT      uint_val;
        LONG      long_val;
        ULONG     ulong_val;
*/
		LONGLONG  llong_val;
/*        ULONGLONG ullong_val;*/
/*        BOOL      bool_val;*/
        double    double_val;
/*        float     float_val;*/
        void     *ptr;
	} value;
    CwtTypeEnum type;
    UINT is_null : 1;
    size_t sz;     /* size of buffer pointed by ptr */
    size_t length; /* length in chars for CwtType_TEXT and CwtType_CWT_TEXT */
} CwtUniType;

typedef struct _CwtUniTypeNS {
	CwtUniType* (*create)        (void);
	void        (*free)          (CwtUniType *ut);
	CwtTypeEnum (*type)          (CwtUniType *ut);
/*	BOOL        (*canCast)       (CwtUniType *ut, CwtTypeEnum type);*/
	BOOL        (*set)           (CwtUniType *ut, CwtTypeEnum type, const void *copy, size_t sz);
	BOOL        (*setFromString) (CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
	BOOL        (*setBool)       (CwtUniType *ut, BOOL b);
	BOOL        (*setChar)       (CwtUniType *ut, char ch);
	BOOL        (*setCwtChar)    (CwtUniType *ut, CWT_CHAR ch);
	BOOL        (*setSByte)      (CwtUniType *ut, SBYTE n);
	BOOL        (*setByte)       (CwtUniType *ut, BYTE n);
	BOOL        (*setShort)      (CwtUniType *ut, SHORT n);
	BOOL        (*setUShort)     (CwtUniType *ut, USHORT n);
	BOOL        (*setInt)        (CwtUniType *ut, INT n);
	BOOL        (*setUInt)       (CwtUniType *ut, UINT n);
	BOOL        (*setLong)       (CwtUniType *ut, LONG n);
	BOOL        (*setULong)      (CwtUniType *ut, ULONG n);
	BOOL        (*setLongLong)   (CwtUniType *ut, LONGLONG n);
	BOOL        (*setULongLong)  (CwtUniType *ut, ULONGLONG n);
	BOOL        (*setFloat)      (CwtUniType *ut, float n);
	BOOL        (*setDouble)     (CwtUniType *ut, double n);
	BOOL        (*setText)       (CwtUniType *ut, const char *p, size_t length);
	BOOL        (*setCwtText)    (CwtUniType *ut, const CWT_CHAR *p, size_t length);
	BOOL        (*setBlob)       (CwtUniType *ut, const void *p, size_t sz);
	BOOL        (*setTime)       (CwtUniType *ut, const CWT_TIME *p, size_t sz);
	BOOL        (*setDate)       (CwtUniType *ut, const CWT_TIME *p, size_t sz);
	BOOL        (*setDateTime)   (CwtUniType *ut, const CWT_TIME *p, size_t sz);

	BOOL        (*toBool)        (CwtUniType*);
	char        (*toChar)        (CwtUniType*);
	CWT_CHAR    (*toCwtChar)     (CwtUniType*);
	SBYTE       (*toSByte)       (CwtUniType*);
	BYTE        (*toByte)        (CwtUniType*);
	SHORT       (*toShort)       (CwtUniType*);
	USHORT      (*toUShort)      (CwtUniType*);
	INT         (*toInt)         (CwtUniType*);
	UINT        (*toUInt)        (CwtUniType*);
	LONG        (*toLong)        (CwtUniType*);
	ULONG       (*toULong)       (CwtUniType*);
	LONGLONG    (*toLongLong)    (CwtUniType*);
	ULONGLONG   (*toULongLong)   (CwtUniType*);
	float       (*toFloat)       (CwtUniType*);
	double      (*toDouble)      (CwtUniType*);
	char*       (*toText)        (CwtUniType*);
	CWT_CHAR*   (*toCwtText)     (CwtUniType*);
	void*       (*toBlob)        (CwtUniType*);
	CWT_TIME*   (*toTime)        (CwtUniType*);
	CWT_TIME*   (*toDate)        (CwtUniType*);
	CWT_TIME*   (*toDateTime)    (CwtUniType*);
} CwtUniTypeNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtUniTypeNS* cwtUniTypeNS(void);
EXTERN_C_END

#endif /* __CWT_UNITYPE_H__ */
