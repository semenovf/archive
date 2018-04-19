/*
 * global.c
 *
 *  Created on: 17.08.2012
 *      Author: user
 */

#include <cwt/types.h>

DLL_API_EXPORT size_t cwt_sizeof(CwtTypeEnum type)
{
	static size_t __size_of_array[] = {
		  0
		, sizeof_CwtType_BOOL
		, sizeof_CwtType_CHAR
		, sizeof_CwtType_SBYTE
		, sizeof_CwtType_BYTE
		, sizeof_CwtType_SHORT
		, sizeof_CwtType_USHORT
		, sizeof_CwtType_INT
		, sizeof_CwtType_UINT
		, sizeof_CwtType_LONG
		, sizeof_CwtType_ULONG
		, sizeof_CwtType_LONGLONG
		, sizeof_CwtType_ULONGLONG
		, sizeof_CwtType_FLOAT
		, sizeof_CwtType_DOUBLE
		, sizeof_CwtType_TEXT
		, sizeof_CwtType_BLOB
		, sizeof_CwtType_TIME
		, sizeof_CwtType_DATE
		, sizeof_CwtType_DATETIME
	};

	return __size_of_array[type];
}


