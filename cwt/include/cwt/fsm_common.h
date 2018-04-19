/**
 * @file   fsm_common.h
 * @author wladt
 * @date   Oct 12, 2012 1:11:57 PM
 *
 * @brief Contains common transitions for FSM
 */

#ifndef __CWT_FSM_COMMON_H__
#define __CWT_FSM_COMMON_H__

#ifndef __CWT_FSM_H__
#	error include <cwt/fsm.h> before using this header file
#endif

/*
RFC-5234 Core Rules

	ALPHA          =  %x41-5A / %x61-7A   ; A-Z / a-z

	BIT            =  "0" / "1"

	CHAR           =  %x01-7F
						; any 7-bit US-ASCII character,
						;  excluding NUL

	CR             =  %x0D
						; carriage return

	LF             =  %x0A
						; linefeed

	CRLF           =  CR LF
						; Internet standard newline

	CTL            =  %x00-1F / %x7F
						; controls

	DIGIT          =  %x30-39
						; 0-9

	DQUOTE         =  %x22
						; " (Double Quote)

	QUOTE		   =  %x27
						; ' (Quote)

	HEXDIG         =  DIGIT / "A" / "B" / "C" / "D" / "E" / "F"

	HTAB           =  %x09
						; horizontal tab

	LWSP           =  *(WSP / CRLF WSP)
						; Use of this linear-white-space rule
						;  permits lines containing only white
						;  space that are no longer legal in
						;  mail headers and have caused
						;  interoperability problems in other
						;  contexts.
						; Do not use when defining mail
						;  headers and use with caution in
						;  other contexts.

	OCTET          =  %x00-FF
						; 8 bits of data

	SP             =  %x20

	VCHAR          =  %x21-7E
						; visible (printing) characters

	WSP            =  SP / HTAB
						; white space

	NL             =  \r\n ; Windows/DOS
	NL             =/ \n\r ; MAC OS 9
	NL             =/ \n   ; Unix

 */


/* %x41-5A / %x61-7A   ; A-Z / a-z */
#define ALPHA_FSM_INL  FSM_MATCH_CHAR(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), 52)

/* "0" / "1" */
#define BIT_FSM_INL    FSM_MATCH_CHAR(_T("01"), 2)
static CwtFsmTransition BIT_FSM[] = {
	{ -1,-1, FSM_MATCH_INLINE(BIT_FSM_INL), FSM_ACCEPT, NULL, NULL }
};


/* %x01-7F ; any 7-bit US-ASCII character, excluding NUL */
static CWT_CHAR CHAR_RG[2] = {(CWT_CHAR)0x01, (CWT_CHAR)0x7F };
#define CHAR_FSM_INL   FSM_MATCH_RANGE(&CHAR_RG[0], &CHAR_RG[1])

/* %x0D ; carriage return */
#define CR_FSM_INL     FSM_MATCH_STR(_T("\r"), 1)

/* %x0A ; linefeed */
#define LF_FSM_INL     FSM_MATCH_STR(_T("\n"), 1)

/* CR LF ; Internet standard newline */
#define CRLF_FSM_INL   FSM_MATCH_STR(_T("\r\n"), 2)


/*
static CwtFsmTransition NL_FSM[] = {
	  {-1,-1, FSM_MATCH_INLINE(NL_FSM_INL), FSM_ACCEPT, NULL, NULL }
};
*/

/* %x30-39 ; 0-9 */
#define DIGIT_FSM_INL  FSM_MATCH_CHAR(_T("0123456789"), 10)
static CwtFsmTransition DIGIT_FSM[] = {
	  {-1,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_ACCEPT, NULL, NULL }
};

/* %x22 ; " (Double Quote) */
#define DQUOTE_FSM_INL FSM_MATCH_STR(_T("\""), 1)

/* %x09 ; horizontal tab */
#define HTAB_FSM_INL   FSM_MATCH_STR(_T("\t"), 1)

/* %x00-FF ; 8 bits of data */
static CWT_CHAR OCTET_RG[2] = {(CWT_CHAR)0x00, (CWT_CHAR)0xFF };
#define OCTET_FSM_INL  FSM_MATCH_RANGE(&OCTET_RG[0], &OCTET_RG[0])

/* %x20  ; space */
#define SP_FSM_INL     FSM_MATCH_STR(_T(" "), 1)

/* %x21-7E ; visible (printing) characters */
static CWT_CHAR VCHAR_RG[2] = {(CWT_CHAR)0x21, (CWT_CHAR)0x7E };
#define VCHAR_FSM_INL  FSM_MATCH_RANGE(&VCHAR_RG[0], &VCHAR_RG[1])

/* SP / HTAB ; white space */
#define WSP_FSM_INL    FSM_MATCH_CHAR(_T(" \t"), 2)


/*
static CwtFsmTransition ALPHA_FSM[] = {
	  { -1,-1, FSM_MATCH_CHAR(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), 52), FSM_ACCEPT, NULL, NULL }
 };
*/


/* %x01-7F ; any 7-bit US-ASCII character, excluding NUL */
/*
static CWT_CHAR __CHAR_RG[2] = {(CWT_CHAR)0x01, (CWT_CHAR)0x7F };
static CwtFsmTransition CHAR_FSM[] = {
	{ -1,-1, FSM_MATCH_RANGE(&__CHAR_RG[0], &__CHAR_RG[1]), FSM_ACCEPT, NULL, NULL }
};
*/


/* %x0D ; carriage return */
/*
static CwtFsmTransition CR_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T("\r"), 1), FSM_ACCEPT, NULL, NULL }
};
*/

/* %x0A ; linefeed */
/*
static CwtFsmTransition LF_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T("\n"), 1), FSM_ACCEPT, NULL, NULL }
};
*/

/* CR LF ; Internet standard newline */
/*
static CwtFsmTransition CRLF_FSM[] = {
	  {-1,-1, FSM_MATCH_STR(_T("\r\n"), 2), FSM_ACCEPT, NULL, NULL }
};
*/



/* %x22 ; " (Double Quote) */
/*
static CwtFsmTransition DQUOTE_FSM[] = {
	{-1,-1, FSM_MATCH_CHAR(_T("\""), 1), FSM_ACCEPT, NULL, NULL }
};
*/

/* %x09 ; horizontal tab */
/*
static CwtFsmTransition HTAB_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T("\t"), 1), FSM_ACCEPT, NULL, NULL }
};
*/

/* %x00-FF ; 8 bits of data */
/*
static CWT_CHAR OCTET_RG[2] = {(CWT_CHAR)0x00, (CWT_CHAR)0xFF };
static CwtFsmTransition OCTET_FSM[] = {
	{ -1,-1, FSM_MATCH_RANGE(&OCTET_RG[0], &OCTET_RG[0]), FSM_ACCEPT, NULL, NULL }
};
*/

/* %x20  ; space */
/*
static CwtFsmTransition SP_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T(" "), 1), FSM_ACCEPT, NULL, NULL }
};
*/

/* %x21-7E ; visible (printing) characters */
/*
static CWT_CHAR VCHAR_RG[2] = {(CWT_CHAR)0x21, (CWT_CHAR)0x7E };
static CwtFsmTransition VCHAR_FSM[] = {
	{ -1,-1, FSM_MATCH_RANGE(&VCHAR_RG[0], &VCHAR_RG[1]), FSM_ACCEPT, NULL, NULL }
};
*/

/* SP / HTAB ; white space */
/*
static CwtFsmTransition WSP_FSM[] = {
	{-1,-1, FSM_MATCH_CHAR(_T(" \t"), 2), FSM_ACCEPT, NULL, NULL }
};
*/

/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static CwtFsmTransition HEXDIG_FSM[] = {
	  {-1, 1, FSM_MATCH_INLINE(DIGIT_FSM_INL),        FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_T("ABCDEFabcdef"), 12), FSM_ACCEPT, NULL, NULL }
};

/* %x00-1F / %x7F  ; controls */
static CWT_CHAR CTL_RG[2] = {(CWT_CHAR)0x00, (CWT_CHAR)0x1F };
static CwtFsmTransition CTL_FSM[] = {
	  { -1, 1, FSM_MATCH_RANGE(&CTL_RG[0], &CTL_RG[1]), FSM_ACCEPT, NULL, NULL }
	, { -1,-1, FSM_MATCH_CHAR(_T("\0x7F"), 1),          FSM_ACCEPT, NULL, NULL }
};

/* *(WSP / CRLF WSP)
					; Use of this linear-white-space rule
					;  permits lines containing only white
					;  space that are no longer legal in
					;  mail headers and have caused
					;  interoperability problems in other
					;  contexts.
					; Do not use when defining mail
					;  headers and use with caution in
					;  other contexts.
*/
static CwtFsmTransition LWSP_FSM[] = {
	  { 0, 1, FSM_MATCH_INLINE(CRLF_FSM_INL),  FSM_NORMAL, NULL, NULL }
	, { 0, 2, FSM_MATCH_INLINE(WSP_FSM_INL),   FSM_ACCEPT, NULL, NULL }
	, { 0,-1, FSM_MATCH_INLINE(WSP_FSM_INL),   FSM_ACCEPT, NULL, NULL }
};




/* BELOW CODE IS FOR SUPPRESS WARNING 'defined but not used' ONLY */
static void __fsm_unused_commons(CwtFsm *fsm, void *fn_context, const void *data, size_t len);

static CwtFsmTransition UNUSED_FSM[] = {
	  {-1,-1, FSM_MATCH_FUNC(__fsm_unused_commons, NULL),  FSM_NORMAL, NULL, NULL }
};

static void __fsm_unused_commons(CwtFsm *fsm, void *fn_context, const void *data, size_t len)
{
	CWT_UNUSED2(fsm, fn_context);
	CWT_UNUSED2(data, len);
	CWT_UNUSED(UNUSED_FSM);
	CWT_UNUSED2(BIT_FSM, DIGIT_FSM);
	CWT_UNUSED3(CHAR_RG, OCTET_RG, VCHAR_RG);
	CWT_UNUSED3(HEXDIG_FSM, CTL_FSM, LWSP_FSM);
}

#endif /* __CWT_FSM_COMMON_H__ */
