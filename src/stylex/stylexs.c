/*
 * stylexs.c
 *
 *  Created on: 15.06.2012
 *      Author: wladt
 */
#include <cwt/stylex/stylex.h>
#include <ctype.h>
#include <cwt/stdio.h>
#include <cwt/string.h>
#include <cwt/str.h>

static BOOL __parseColor(const CWT_CHAR *str, void *pval);        /* Cwt_Color */
static BOOL __parseUINT32(const CWT_CHAR *str, void *pval);       /* UINT32 */
static BOOL __parseINT32(const CWT_CHAR *str, void *pval);        /* INT32 */
static BOOL __parseBorderStyle(const CWT_CHAR *str, void *pval);  /* Cwt_BorderStyle */
static BOOL __parseString(const CWT_CHAR *str, void *pval);       /* string */
static BOOL __parseFontStyle(const CWT_CHAR *str, void *pval);    /* Cwt_FontStyle */
static BOOL __parseFontWeight(const CWT_CHAR *str, void *pval);   /* Cwt_FontWeight */
static BOOL __parseOpacity(const CWT_CHAR *str, void *pval);      /* float: [0.0, 1.0] */
static BOOL __parsePosition(const CWT_CHAR *str, void *pval);     /* Cwt_Position */
static BOOL __parseVAlign(const CWT_CHAR *str, void *pval); 	  /* Cwt_Style_VAlign */
static BOOL __parseHAlign(const CWT_CHAR *str, void *pval);       /* Cwt_Style_HAlign */

struct _cwt_stylex_helper {
	Cwt_StyleAttrType attrType;
	BOOL (*validator)(const CWT_CHAR *str, void *pval);
} __cwt_stylex_helpers[Cwt_StyleAttr_Count] = {
	  { Cwt_StyleAttr_BackgroundColor,   __parseColor }	     /* Cwt_Color */
	, { Cwt_StyleAttr_BorderLeftWidth,   __parseUINT32 }     /* unsigned integer */
	, { Cwt_StyleAttr_BorderLeftColor,   __parseColor }      /* Cwt_Color */
	, { Cwt_StyleAttr_BorderLeftStyle,   __parseBorderStyle} /* Cwt_BorderStyle */
	, { Cwt_StyleAttr_BorderTopWidth,    __parseUINT32 }     /* unsigned integer */
	, { Cwt_StyleAttr_BorderTopColor,    __parseColor }      /* Cwt_Color */
	, { Cwt_StyleAttr_BorderTopStyle,    __parseBorderStyle} /* Cwt_BorderStyle */
	, { Cwt_StyleAttr_BorderRightWidth,  __parseUINT32 }     /* unsigned integer */
	, { Cwt_StyleAttr_BorderRightColor,  __parseColor }      /* Cwt_Color */
	, { Cwt_StyleAttr_BorderRightStyle,  __parseBorderStyle} /* Cwt_BorderStyle */
	, { Cwt_StyleAttr_BorderBottomWidth, __parseUINT32 }     /* unsigned integer */
	, { Cwt_StyleAttr_BorderBottomColor, __parseColor }      /* Cwt_Color */
	, { Cwt_StyleAttr_BorderBottomStyle, __parseBorderStyle} /* Cwt_BorderStyle */
	, { Cwt_StyleAttr_Left,              __parseINT32 }      /* signed int */
	, { Cwt_StyleAttr_Top,               __parseINT32 }      /* signed int */
	, { Cwt_StyleAttr_Width,             __parseINT32 }      /* signed int */
	, { Cwt_StyleAttr_Height,            __parseINT32 }      /* signed int */
	, { Cwt_StyleAttr_Color,             __parseColor }      /* Cwt_Color */
	, { Cwt_StyleAttr_FontFamily,        __parseString }     /* string */
	, { Cwt_StyleAttr_FontSize,          __parseUINT32 }     /* unsigned integer */
	, { Cwt_StyleAttr_FontStyle,         __parseFontStyle }  /* Cwt_FontStyle */
	, { Cwt_StyleAttr_FontWeight,        __parseFontWeight } /* Cwt_FontWeight */
	, { Cwt_StyleAttr_MarginLeft,        __parseINT32 }      /* signed integer */
	, { Cwt_StyleAttr_MarginTop,         __parseINT32 }      /* signed integer */
	, { Cwt_StyleAttr_MarginRight,       __parseINT32 }      /* signed integer */
	, { Cwt_StyleAttr_MarginBottom,      __parseINT32 }      /* signed integer */
	, { Cwt_StyleAttr_PaddingLeft,       __parseUINT32}      /* unsigned integer */
	, { Cwt_StyleAttr_PaddingTop,        __parseUINT32}      /* unsigned integer */
	, { Cwt_StyleAttr_PaddingRight,      __parseUINT32}      /* unsigned integer */
	, { Cwt_StyleAttr_PaddingBottom,     __parseUINT32}      /* unsigned integer */
	, { Cwt_StyleAttr_MaxWidth,          __parseUINT32}	     /* unsigned integer */
	, { Cwt_StyleAttr_MaxHeight,         __parseUINT32}		 /* unsigned integer */
	, { Cwt_StyleAttr_MinWidth,          __parseUINT32}		 /* unsigned integer */
	, { Cwt_StyleAttr_MinHeight,         __parseUINT32}		 /* unsigned integer */
	, { Cwt_StyleAttr_Opacity,           __parseOpacity }    /* float: [0.0, 1.0] */
	, { Cwt_StyleAttr_Position,          __parsePosition }	 /* Cwt_Position */
	, { Cwt_StyleAttr_TextIndent,        __parseINT32 }      /* signed integer */
	, { Cwt_StyleAttr_TextAlign,         __parseHAlign }     /* Cwt_Style_TextAlign */
	, { Cwt_StyleAttr_VAlign,            __parseVAlign }     /* Cwt_Style_VAlign */
	, { Cwt_StyleAttr_HAlign,            __parseHAlign } 	 /* Cwt_Style_HAlign */
};



static BOOL __parseColor(const CWT_CHAR *str, void *pval)
{
	return cwt_stylex_parseColor(str, (Cwt_Color*)pval);
}

static BOOL __parseUINT32(const CWT_CHAR *str, void *pval)
{
	BOOL ok = FALSE;
	UINT32 n;

	n = cwtStrNS()->toUINT32(str, 0, &ok);
	if( ok )
		*((UINT*)pval) = n;
	return ok;
}

static BOOL __parseINT32(const CWT_CHAR *str, void *pval)
{
	BOOL ok = FALSE;
	INT n;

	n = cwtStrNS()->toINT32(str, 0, &ok);
	if( ok )
		*((INT*)pval) = n;
	return ok;
}

static BOOL __parseBorderStyle(const CWT_CHAR *str, void *pval)
{
	CwtStrNS *strNS = cwtStrNS();
	CWT_CHAR *borderStyleStr = strNS->strDup(str);
	int borderStyle = -1;

	strNS->toLowerStr(borderStyleStr, borderStyleStr, strNS->strLen(borderStyleStr));

	if( strNS->strEq(_T("inherit"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Inherit;
	} else if( strNS->strEq(_T("none"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_None;
	} else if( strNS->strEq(_T("hidden"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Hidden;
	} else if( strNS->strEq(_T("dotted"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Dotted;
	} else if( strNS->strEq(_T("dashed"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Dashed;
	} else if( strNS->strEq(_T("solid"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Solid;
	} else if( strNS->strEq(_T("double"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Double;
	} else if( strNS->strEq(_T("groove"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Groove;
	} else if( strNS->strEq(_T("ridge"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Ridge;
	} else if( strNS->strEq(_T("inset"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Inset;
	} else if( strNS->strEq(_T("outset"), borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Outset;
	}

	CWT_FREE(borderStyleStr);

	if( borderStyle < 0 )
		return FALSE;

	if( pval ) {
		*((Cwt_BorderStyle*)pval) = (Cwt_BorderStyle)borderStyle;
	}

	return TRUE;
}

static BOOL __parseString(const CWT_CHAR *str, void *pval)
{
	if( !str )
		return FALSE;

	if( pval )
		*((CWT_CHAR**)pval) = cwtStrNS()->strDup(str);

	return TRUE;
}

static BOOL __parseFontStyle(const CWT_CHAR *str, void *pval)
{
	/* Cwt_FontStyle */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __parseFontWeight(const CWT_CHAR *str, void *pval)
{
	/* Cwt_FontWeight */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __parseOpacity(const CWT_CHAR *str, void *pval)
{
	/* float: [0.0, 1.0] */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __parsePosition(const CWT_CHAR *str, void *pval)
{
	/* Cwt_Position */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __parseVAlign(const CWT_CHAR *str, void *pval)
{
	/* Cwt_Style_VAlign */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __parseHAlign(const CWT_CHAR *str, void *pval)
{
	/* Cwt_Style_HAlign */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

#ifdef CWT_USE_TESTS
#include <cwt/test.h>
void cwt_stylex_test_parse_attrs(void)
{
	CwtStdioNS *stdioNS = cwtStdioNS();
	CWT_CHAR buf[32];
	ULONG ulongNum;

	CWT_TEST_OK(__parseColor(_T("#123456"), NULL));
	CWT_TEST_OK(__parseColor(_T("white"), NULL));
	CWT_TEST_NOK(__parseColor(_T("abracadabra"), NULL));


	/* parse uint */
	ulongNum = CWT_UINT_MAX;
	stdioNS->sprintf(buf, _T("%lu"), ulongNum);
	stdioNS->printf(_Tr("Parse UINT MAX = %lu\n"), ulongNum);
	CWT_TEST_OK(__parseUINT32(buf, NULL));

	ulongNum = 0L;
	stdioNS->sprintf(buf, _T("%lu"), ulongNum);
	stdioNS->printf(_Tr("Parse UINT MIN = %lu\n"), ulongNum);
	CWT_TEST_OK(__parseUINT32(buf, NULL));

	stdioNS->sprintf(buf, _T("4294967296"), ulongNum);
	stdioNS->printf(_Tr("Parse greater than UINT32 MAX = %s\n"), buf);
	CWT_TEST_NOK(__parseUINT32(buf, NULL));

	stdioNS->sprintf(buf, _T("-1"));
	stdioNS->printf(_Tr("Parse less than UINT MIN = %s\n"), buf);
	CWT_TEST_NOK(__parseUINT32(buf, NULL));

	/* parse int */
	stdioNS->sprintf(buf, _T("2147483647"));
	stdioNS->printf(_Tr("Parse INT32 MAX = %s\n"), buf);
	CWT_TEST_OK(__parseINT32(buf, NULL));

	stdioNS->sprintf(buf, _T("-2147483648"));
	stdioNS->printf(_Tr("Parse INT32 MIN = %s\n"), buf);
	CWT_TEST_OK(__parseINT32(buf, NULL));

	stdioNS->sprintf(buf, _T("2147483648"));
	stdioNS->printf(_Tr("Parse greater than INT32 MAX = %s\n"), buf);
	CWT_TEST_NOK(__parseINT32(buf, NULL));

	stdioNS->sprintf(buf, _T("-2147483649"));
	stdioNS->printf(_Tr("Parse less than INT MIN = %�\n"), buf);
	CWT_TEST_NOK(__parseINT32(buf, NULL));


	/* parse border style */
	CWT_TEST_OK(__parseBorderStyle(_T("Inherit"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("none"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("hidden"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("Dotted"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("Dashed"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("Solid"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("Double"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("Groove"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("Ridge"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("Inset"), NULL));
	CWT_TEST_OK(__parseBorderStyle(_T("Outset"), NULL));
	CWT_TEST_NOK(__parseBorderStyle(_T("abracadabra"), NULL));

/*

	longNum = CWT_INT_MAX;
	sprintf(buf, "%ld", longNum);
	CWT_TEST(_cwt_parse_int(buf, NULL));

	longNum = CWT_INT_MIN;
	sprintf(buf, "%ld", longNum);
	CWT_TEST(_cwt_parse_int(buf, NULL));

	longNum = CWT_INT_MAX + 1;
	sprintf(buf, "%ld", longNum);

	longNum = CWT_INT_MIN - 1;
	sprintf(buf, "%ld", longNum);


	CWT_TEST(_cwt_parse_int("-9999", NULL));
*/

}
#endif

