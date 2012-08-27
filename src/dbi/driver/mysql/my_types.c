/*
 * my_types.c
 *
 *  Created on: 02.08.2012
 *      Author: wladt
 */


#include <cwt/dbi/dbi.h>
#include <cwt/str.h>
#include <cwt/stdio.h>
#include <cwt/logger.h>


/**
 * BOOLEAN, BOOL and TINYINT(1) are equivalents
 *
 * @return
 */
CWT_CHAR* __stringifyBoolType(void)
{
	return cwtStrNS()->strdup(_T("BOOLEAN"));
}


CWT_CHAR* __stringifyIntType(LONGLONG min, ULONGLONG max)
{
	CwtStrNS *strNS = cwtStrNS();
	CWT_CHAR *s = NULL;
	BOOL isUnsigned = FALSE;

	if( max > CWT_LONGLONG_MAX && ! (min >= 0) ) {
		print_error(_Tr("bounds for integer is illegal"));
		return NULL;
	}

	if( min >= 0 ) { /* unsigned */
		if( max <= (ULONGLONG)CWT_UINT8_MAX )
			s = strNS->strdup(_T("TINYINT UNSIGNED"));
		else if( max <= (ULONGLONG)CWT_UINT16_MAX )
			s = strNS->strdup(_T("SMALLINT UNSIGNED"));
		else if( max <= (ULONGLONG)CWT_UINT24_MAX )
			s = strNS->strdup(_T("MEDIUMINT UNSIGNED"));
		else if( max <= (ULONGLONG)CWT_UINT32_MAX )
			s = strNS->strdup(_T("INT UNSIGNED"));
		else
			s = strNS->strdup(_T("BIGINT UNSIGNED"));
	} else {
		if( min >= (LONGLONG)CWT_INT8_MIN && max <= (ULONGLONG)CWT_INT8_MAX )
			s = strNS->strdup(_T("TINYINT"));
		else if( min >= (LONGLONG)CWT_INT16_MIN && max <= (ULONGLONG)CWT_INT16_MAX )
			s = strNS->strdup(_T("SMALLINT"));
		else if( min >= (LONGLONG)CWT_INT24_MIN && max <= (ULONGLONG)CWT_INT24_MAX )
			s = strNS->strdup(_T("MEDIUMINT"));
		else if( min >= (LONGLONG)CWT_INT32_MIN && max <= (ULONGLONG)CWT_INT32_MAX )
			s = strNS->strdup(_T("INT"));
		else
			s = strNS->strdup(_T("BIGINT"));
	}

	return s;
}

/*
 * A precision from 0 to 23 results in a 4-byte single-precision FLOAT column.
 * A precision from 24 to 53 results in an 8-byte double-precision DOUBLE column.
 */
CWT_CHAR* __stringifyFloatType(CwtTypeEnum type, UINT prec, UINT scale)
{
	CwtStrNS *strNS = cwtStrNS();
	CWT_CHAR *fstr = NULL;

	CWT_ASSERT(type == CwtType_FLOAT || type == CwtType_DOUBLE);

	if( prec == 0 && scale == 0 ) {
		switch(type) {
		case CwtType_FLOAT:
			fstr = strNS->strdup(_T("FLOAT"));
			break;
		case CwtType_DOUBLE:
		default:
			fstr = strNS->strdup(_T("DOUBLE"));
			break;
		}
	} else {
		CWT_CHAR buf[64];
		strNS->bzero(buf, 64 * sizeof(CWT_CHAR));

		CWT_ASSERT(cwtStdioNS()->snprintf(buf, 31, _T("DECIMAL(%d,%d)"), prec, scale) > 0);
		fstr = strNS->strdup(buf);
	}

	return fstr;
}

CWT_CHAR* __stringifyTextType(ULONGLONG maxlen)
{
	CwtStrNS *strNS = cwtStrNS();
	CWT_CHAR *s = NULL;

	if( maxlen == 0ULL ) {
		s = strNS->strdup(_T("TEXT"));
	} else if( maxlen < 64 ) {
		CWT_CHAR buf[64];
		CWT_ASSERT(cwtStdioNS()->snprintf(buf, 63, _T("CHAR(%lu)"), maxlen) > 0);
		s = strNS->strdup(buf);
	} else if( maxlen < 256 ) {
		CWT_CHAR buf[64];
		CWT_ASSERT(cwtStdioNS()->snprintf(buf, 63, _T("VARCHAR(%lu)"), maxlen) > 0);
		s = strNS->strdup(buf);
	} else if( maxlen <= CWT_UINT16_MAX ) {
		s = strNS->strdup(_T("TEXT"));
	} else if( maxlen <= CWT_UINT24_MAX ) {
		s = strNS->strdup(_T("MEDIUMTEXT"));
	}  else if( maxlen <= CWT_UINT32_MAX ) {
		s = strNS->strdup(_T("LONGTEXT"));
	}

	return s;
}

CWT_CHAR* __stringifyBlobType(ULONGLONG maxlen)
{
	CwtStrNS *strNS = cwtStrNS();
	CWT_CHAR *s = NULL;

	if( maxlen == 0ULL ) {
		s = strNS->strdup(_T("BLOB"));
	} else if( maxlen < 256 ) {
		s = strNS->strdup(_T("TINYBLOB"));
	} else if( maxlen <= CWT_UINT16_MAX ) {
		s = strNS->strdup(_T("BLOB"));
	} else if( maxlen <= CWT_UINT24_MAX ) {
		s = strNS->strdup(_T("MEDIUMBLOB"));
	}  else if( maxlen <= CWT_UINT32_MAX ) {
		s = strNS->strdup(_T("LONGBLOB"));
	}

	return s;
}

CWT_CHAR* __stringifyTimeType(CwtTypeEnum time_type, BOOL stamp)
{
	CwtStrNS *strNS = cwtStrNS();

	if( stamp ) {
		return strNS->strdup(_T("TIMESTAMP DEFAULT CURRENT_TIMESTAMP"));
	}

	switch(time_type) {
	case CwtType_TIME:
		return strNS->strdup(_T("TIME"));
	case CwtType_DATE:
		return strNS->strdup(_T("DATE"));
	case CwtType_DATETIME:
		return strNS->strdup(_T("DATETIME"));
	default:
		break;
	}

	return NULL;
}
