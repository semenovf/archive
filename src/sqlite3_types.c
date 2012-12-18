/**
 * @file   sqlite3_types.c
 * @author wladt
 * @date   Nov 28, 2012 10:00:31 AM
 *
 * @brief
 */

#include <cwt/str.h>

CWT_CHAR* s3_stringify_bool_type(void)
{
	return cwt_str_ns()->strDup(_T("INTEGER"));
}


CWT_CHAR* s3_stringify_int_type(LONGLONG min, ULONGLONG max)
{
	CWT_UNUSED2(min, max);
	return cwt_str_ns()->strDup(_T("INTEGER"));
}

CWT_CHAR* s3_stringify_float_type(CwtTypeEnum type, UINT prec, UINT scale)
{
	CWT_UNUSED3(type, prec, scale);
	return cwt_str_ns()->strDup(_T("REAL"));
}

CWT_CHAR* s3_stringify_text_type(ULONGLONG maxlen)
{
	CWT_UNUSED(maxlen);
	return cwt_str_ns()->strDup(_T("TEXT"));
}

CWT_CHAR* s3_stringify_blob_type(ULONGLONG maxlen)
{
	CWT_UNUSED(maxlen);
	return cwt_str_ns()->strDup(_T("TEXT"));
}

CWT_CHAR* s3_stringify_time_type(CwtTypeEnum time_type, BOOL stamp)
{
	CWT_UNUSED2(time_type, stamp);
	return cwt_str_ns()->strDup(_T("INTEGER"));
}
