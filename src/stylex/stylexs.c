/*
 * stylexs.c
 *
 *  Created on: 15.06.2012
 *      Author: wladt
 */
#include <cwt/stylex/stylex.h>
#include <string.h>
#include <ctype.h>
#include <cwt/string.h>
#include <cwt/strutils.h>

static BOOL __cwtStylexParseColor(const CWT_CHAR *str, void *pval);        /* Cwt_Color */
static BOOL __cwtStylexParseUINT32(const CWT_CHAR *str, void *pval);         /* UINT32 */
static BOOL __cwtStylexParseINT32(const CWT_CHAR *str, void *pval);          /* INT32 */
static BOOL __cwtStylexParseBorderStyle(const CWT_CHAR *str, void *pval);  /* Cwt_BorderStyle */
static BOOL __cwtStylexParseString(const CWT_CHAR *str, void *pval);       /* string */
static BOOL __cwtStylexParseFontStyle(const CWT_CHAR *str, void *pval);    /* Cwt_FontStyle */
static BOOL __cwtStylexParseFontWeight(const CWT_CHAR *str, void *pval);   /* Cwt_FontWeight */
static BOOL __cwtStylexParseOpacity(const CWT_CHAR *str, void *pval);      /* float: [0.0, 1.0] */
static BOOL __cwtStylexParsePosition(const CWT_CHAR *str, void *pval);     /* Cwt_Position */
static BOOL __cwtStylexParseVAlign(const CWT_CHAR *str, void *pval); 	  /* Cwt_Style_VAlign */
static BOOL __cwtStylexParseHAlign(const CWT_CHAR *str, void *pval);       /* Cwt_Style_HAlign */

struct _cwt_stylex_helper {
	Cwt_StyleAttrType attrType;
	BOOL (*validator)(const CWT_CHAR *str, void *pval);
} __cwt_stylex_helpers[Cwt_StyleAttr_Count] = {
	  { Cwt_StyleAttr_BackgroundColor,   __cwtStylexParseColor }	/* Cwt_Color */
	, { Cwt_StyleAttr_BorderLeftWidth,   __cwtStylexParseUINT32 } /* unsigned integer */
	, { Cwt_StyleAttr_BorderLeftColor,   __cwtStylexParseColor } /* Cwt_Color */
	, { Cwt_StyleAttr_BorderLeftStyle,   __cwtStylexParseBorderStyle} /* Cwt_BorderStyle */
	, { Cwt_StyleAttr_BorderTopWidth,    __cwtStylexParseUINT32 } /* unsigned integer */
	, { Cwt_StyleAttr_BorderTopColor,    __cwtStylexParseColor } /* Cwt_Color */
	, { Cwt_StyleAttr_BorderTopStyle,    __cwtStylexParseBorderStyle} /* Cwt_BorderStyle */
	, { Cwt_StyleAttr_BorderRightWidth,  __cwtStylexParseUINT32 } /* unsigned integer */
	, { Cwt_StyleAttr_BorderRightColor,  __cwtStylexParseColor } /* Cwt_Color */
	, { Cwt_StyleAttr_BorderRightStyle,  __cwtStylexParseBorderStyle} /* Cwt_BorderStyle */
	, { Cwt_StyleAttr_BorderBottomWidth, __cwtStylexParseUINT32 } /* unsigned integer */
	, { Cwt_StyleAttr_BorderBottomColor, __cwtStylexParseColor } /* Cwt_Color */
	, { Cwt_StyleAttr_BorderBottomStyle, __cwtStylexParseBorderStyle} /* Cwt_BorderStyle */
	, { Cwt_StyleAttr_Left,              __cwtStylexParseINT32 }            /* signed int */
	, { Cwt_StyleAttr_Top,               __cwtStylexParseINT32 }             /* signed int */
	, { Cwt_StyleAttr_Width,             __cwtStylexParseINT32 }           /* signed int */
	, { Cwt_StyleAttr_Height,            __cwtStylexParseINT32 }          /* signed int */
	, { Cwt_StyleAttr_Color,             __cwtStylexParseColor }           /* Cwt_Color */
	, { Cwt_StyleAttr_FontFamily,        __cwtStylexParseString }      /* string */
	, { Cwt_StyleAttr_FontSize,          __cwtStylexParseUINT32 }        /* unsigned integer */
	, { Cwt_StyleAttr_FontStyle,         __cwtStylexParseFontStyle }       /* Cwt_FontStyle */
	, { Cwt_StyleAttr_FontWeight,        __cwtStylexParseFontWeight }      /* Cwt_FontWeight */
	, { Cwt_StyleAttr_MarginLeft,        __cwtStylexParseINT32 }      /* signed integer */
	, { Cwt_StyleAttr_MarginTop,         __cwtStylexParseINT32 }       /* signed integer */
	, { Cwt_StyleAttr_MarginRight,       __cwtStylexParseINT32 }     /* signed integer */
	, { Cwt_StyleAttr_MarginBottom,      __cwtStylexParseINT32 }    /* signed integer */
	, { Cwt_StyleAttr_PaddingLeft,       __cwtStylexParseUINT32}     /* unsigned integer */
	, { Cwt_StyleAttr_PaddingTop,        __cwtStylexParseUINT32}      /* unsigned integer */
	, { Cwt_StyleAttr_PaddingRight,      __cwtStylexParseUINT32}    /* unsigned integer */
	, { Cwt_StyleAttr_PaddingBottom,     __cwtStylexParseUINT32}   /* unsigned integer */
	, { Cwt_StyleAttr_MaxWidth,          __cwtStylexParseUINT32}		/* unsigned integer */
	, { Cwt_StyleAttr_MaxHeight,         __cwtStylexParseUINT32}		/* unsigned integer */
	, { Cwt_StyleAttr_MinWidth,          __cwtStylexParseUINT32}		/* unsigned integer */
	, { Cwt_StyleAttr_MinHeight,         __cwtStylexParseUINT32}		/* unsigned integer */
	, { Cwt_StyleAttr_Opacity,           __cwtStylexParseOpacity }         /* float: [0.0, 1.0] */
	, { Cwt_StyleAttr_Position,          __cwtStylexParsePosition }		/* Cwt_Position */
	, { Cwt_StyleAttr_TextIndent,        __cwtStylexParseINT32 }    	/* signed integer */
	, { Cwt_StyleAttr_TextAlign,         __cwtStylexParseHAlign }     	/* Cwt_Style_TextAlign */
	, { Cwt_StyleAttr_VAlign,            __cwtStylexParseVAlign } 			/* Cwt_Style_VAlign */
	, { Cwt_StyleAttr_HAlign,            __cwtStylexParseHAlign } 			/* Cwt_Style_HAlign */
};



static BOOL __cwtStylexParseColor(const CWT_CHAR *str, void *pval)
{
	return cwt_stylex_parseColor(str, (Cwt_Color*)pval);
}

static BOOL __cwtStylexParseUINT32(const CWT_CHAR *str, void *pval)
{
	BOOL ok = FALSE;
	UINT n;

	n = cwtStrToUINT32(str, 0, &ok);
	if( ok )
		*((UINT*)pval) = n;
	return ok;
}

static BOOL __cwtStylexParseINT32(const CWT_CHAR *str, void *pval)
{
	BOOL ok = FALSE;
	INT n;

	n = cwtStrToINT32(str, 0, &ok);
	if( ok )
		*((INT*)pval) = n;
	return ok;
}

static BOOL __cwtStylexParseBorderStyle(const CWT_CHAR *str, void *pval)
{
	CWT_CHAR *borderStyleStr = cwtStrDup(str);
	int borderStyle = -1;

	str_tolower(borderStyleStr, borderStyleStr, cwtStrLen(borderStyleStr));

	if( cwtStrEq("inherit", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Inherit;
	} else if( cwtStrEq("none", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_None;
	} else if( cwtStrEq("hidden", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Hidden;
	} else if( cwtStrEq("dotted", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Dotted;
	} else if( cwtStrEq("dashed", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Dashed;
	} else if( cwtStrEq("solid", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Solid;
	} else if( cwtStrEq("double", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Double;
	} else if( cwtStrEq("groove", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Groove;
	} else if( cwtStrEq("ridge", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Ridge;
	} else if( cwtStrEq("inset", borderStyleStr) ) {
		borderStyle = Cwt_BorderStyle_Inset;
	} else if( cwtStrEq("outset", borderStyleStr) ) {
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

static BOOL __cwtStylexParseString(const CWT_CHAR *str, void *pval)
{
	if( !str )
		return FALSE;

	if( pval )
		(CWT_CHAR*)pval = cwtStrDup(str);

	return TRUE;
}

static BOOL __cwtStylexParseFontStyle(const CWT_CHAR *str, void *pval)
{
	/* Cwt_FontStyle */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __cwtStylexParseFontWeight(const CWT_CHAR *str, void *pval)
{
	/* Cwt_FontWeight */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __cwtStylexParseOpacity(const CWT_CHAR *str, void *pval)
{
	/* float: [0.0, 1.0] */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __cwtStylexParsePosition(const CWT_CHAR *str, void *pval)
{
	/* Cwt_Position */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __cwtStylexParseVAlign(const CWT_CHAR *str, void *pval)
{
	/* Cwt_Style_VAlign */
	CWT_UNUSED(str);
	CWT_UNUSED(pval);
	return TRUE;
}

static BOOL __cwtStylexParseHAlign(const CWT_CHAR *str, void *pval)
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
	CWT_CHAR buf[32];
	ULONG ulongNum;

	CWT_TEST_OK(__cwtStylexParseColor("#123456", NULL));
	CWT_TEST_OK(__cwtStylexParseColor("white", NULL));
	CWT_TEST_NOK(__cwtStylexParseColor("abracadabra", NULL));


	/* parse uint */
	ulongNum = CWT_UINT_MAX;
	sprintf(buf, "%lu", ulongNum);
	printf("Parse UINT MAX = %lu\n", ulongNum);
	CWT_TEST_OK(__cwtStylexParseUINT32(buf, NULL));

	ulongNum = 0L;
	sprintf(buf, "%lu", ulongNum);
	printf("Parse UINT MIN = %lu\n", ulongNum);
	CWT_TEST_OK(__cwtStylexParseUINT32(buf, NULL));

	sprintf(buf, "4294967296", ulongNum);
	printf("Parse greater than UINT32 MAX = %s\n", buf);
	CWT_TEST_NOK(__cwtStylexParseUINT32(buf, NULL));

	sprintf(buf, "-1");
	printf("Parse less than UINT MIN = %s\n", buf);
	CWT_TEST_NOK(__cwtStylexParseUINT32(buf, NULL));

	/* parse int */
	sprintf(buf, "2147483647");
	printf("Parse INT32 MAX = %s\n", buf);
	CWT_TEST_OK(__cwtStylexParseINT32(buf, NULL));

	sprintf(buf, "-2147483648");
	printf("Parse INT32 MIN = %s\n", buf);
	CWT_TEST_OK(__cwtStylexParseINT32(buf, NULL));

	sprintf(buf, "2147483648");
	printf("Parse greater than INT32 MAX = %s\n", buf);
	CWT_TEST_NOK(__cwtStylexParseINT32(buf, NULL));

	sprintf(buf, "-2147483649");
	printf("Parse less than INT MIN = %ë\n", buf);
	CWT_TEST_NOK(__cwtStylexParseINT32(buf, NULL));


	/* parse border style */
	CWT_TEST_OK(__cwtStylexParseBorderStyle("Inherit", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("none", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("hidden", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("Dotted", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("Dashed", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("Solid", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("Double", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("Groove", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("Ridge", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("Inset", NULL));
	CWT_TEST_OK(__cwtStylexParseBorderStyle("Outset", NULL));
	CWT_TEST_NOK(__cwtStylexParseBorderStyle("abracadabra", NULL));

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

