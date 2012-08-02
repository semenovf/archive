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
        SBYTE     sbyte_val;
        BYTE      byte_val;
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
    UINT type    : 31;
    UINT is_null : 1;
    ULONGLONG length; /* for pointer */
} CwtUniType;


#endif /* __CWT_UNITYPE_H__ */
