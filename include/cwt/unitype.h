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
        LONGLONG  llong_val;
        ULONGLONG ullong_val;
        BOOL      bool_val;
        double    double_val;
        float     float_val;
        void     *ptr;
	} value;
    CwtTypeEnum type;
    UINT is_null : 1;
    size_t length; /* for pointer (in bytes) */
} CwtUniType;

typedef struct _CwtUniTypeNS {
	void      (*init)          (CwtUniType *ut);
	void      (*set)           (CwtUniType *ut, CwtTypeEnum type, void *copy);
	BOOL      (*setFromString) (CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
	void      (*setBool)       (CwtUniType *ut, BOOL b);
	void      (*setChar)       (CwtUniType *ut, char ch);
	void      (*setCwtChar)    (CwtUniType *ut, CWT_CHAR ch);
	void      (*setSByte)      (CwtUniType *ut, SBYTE n);
	void      (*setByte)       (CwtUniType *ut, BYTE n);
	void      (*setShort)      (CwtUniType *ut, SHORT n);
	void      (*setUShort)     (CwtUniType *ut, USHORT n);
	void      (*setInt)        (CwtUniType *ut, INT n);
	void      (*setUInt)       (CwtUniType *ut, UINT n);
	void      (*setLong)       (CwtUniType *ut, LONG n);
	void      (*setULong)      (CwtUniType *ut, ULONG n);
	void      (*setLongLong)   (CwtUniType *ut, LONGLONG n);
	void      (*setULongLong)  (CwtUniType *ut, ULONGLONG n);
	void      (*setFloat)      (CwtUniType *ut, float n);
	void      (*setDouble)     (CwtUniType *ut, double n);
	void      (*setText)       (CwtUniType *ut, char *s);
	void      (*setTextSized)  (CwtUniType *ut, char *s, size_t sz);
	void      (*setCwtText)     (CwtUniType *ut, CWT_CHAR *s);
	void      (*setCwtTextSized)(CwtUniType *ut, CWT_CHAR *s, size_t sz);
	void      (*setBlob)       (CwtUniType *ut, void *p, size_t sz);
	void      (*setTime)       (CwtUniType *ut, CWT_TIME *tm);
	void      (*setDate)       (CwtUniType *ut, CWT_TIME *tm);
	void      (*setDateTime)   (CwtUniType *ut, CWT_TIME *tm);

	BOOL      (*toBool)        (CwtUniType*);
	char      (*toChar)        (CwtUniType*);
	CWT_CHAR  (*toCwtChar)     (CwtUniType*);
	SBYTE     (*toSByte)       (CwtUniType*);
	BYTE      (*toByte)        (CwtUniType*);
	SHORT     (*toShort)       (CwtUniType*);
	USHORT    (*toUShort)      (CwtUniType*);
	INT       (*toInt)         (CwtUniType*);
	UINT      (*toUInt)        (CwtUniType*);
	LONG      (*toLong)        (CwtUniType*);
	ULONG     (*toULong)       (CwtUniType*);
	LONGLONG  (*toLongLong)    (CwtUniType*);
	ULONGLONG (*toULongLong)   (CwtUniType*);
	float     (*toFloat)       (CwtUniType*);
	double    (*toDouble)      (CwtUniType*);
	char*     (*toText)        (CwtUniType*);
	CWT_CHAR* (*toCwtText)     (CwtUniType*);
	void*     (*toBlob)        (CwtUniType*);
	CWT_TIME* (*toTime)        (CwtUniType*);
	CWT_TIME* (*toDate)        (CwtUniType*);
	CWT_TIME* (*toDateTime)    (CwtUniType*);
/*	CWT_CHAR* (*toCwtText)     (CwtUniType*);*/
} CwtUniTypeNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtUniTypeNS* cwtUniTypeNS(void);
EXTERN_C_END

#endif /* __CWT_UNITYPE_H__ */
