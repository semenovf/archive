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


static void __fsm_unused_commons(void);
void (*fsm_unused_commons_fn)(void) = __fsm_unused_commons;


static ssize_t __fsm_CHAR(struct _CwtFsm *fsm, void *fn_context, const void *data, size_t len)
{
	CWT_UNUSED(fsm);
	CWT_UNUSED(fn_context);

	if( len > 0 ) {
		const CWT_CHAR *ptr = (const CWT_CHAR*)data;

		return ( *ptr >= (CWT_CHAR)0x01 && *ptr < (CWT_CHAR)0x7F )
				|| *ptr == (CWT_CHAR)0x7F /* to avoid GCC warning: comparison is always true due to limited range of data type for 'char' type*/
				? 1 : 0;
	}

	return len == 0 ? 0 : -1;
}

/* %x00-1F / %x7F  ; controls */
static ssize_t __fsm_CTL(struct _CwtFsm *fsm, void *fn_context, const void *data, size_t len)
{
	CWT_UNUSED(fsm);
	CWT_UNUSED(fn_context);

	if( len > 0 ) {
		const CWT_CHAR *ptr = (const CWT_CHAR*)data;

		return ( *ptr >= (CWT_CHAR)0x00 && *ptr <= (CWT_CHAR)0x1F )
				|| *ptr == (CWT_CHAR)0x7F
				? 1 : 0;
	}

	return len == 0 ? 0 : -1;
}

/* %x00-FF ; 8 bits of data */
static ssize_t __fsm_OCTET(struct _CwtFsm *fsm, void *fn_context, const void *data, size_t len)
{
	CWT_UNUSED(fsm);
	CWT_UNUSED(fn_context);

	if( len > 0 ) {
		const CWT_CHAR *ptr = (const CWT_CHAR*)data;

		return ( *ptr >= (CWT_CHAR)0x00 && *ptr <= (CWT_CHAR)0xFF )
				? 1 : 0;
	}

	return len == 0 ? 0 : -1;
}


/* %x21-7E ; visible (printing) characters */
static ssize_t __fsm_VCHAR(struct _CwtFsm *fsm, void *fn_context, const void *data, size_t len)
{
	CWT_UNUSED(fsm);
	CWT_UNUSED(fn_context);

	if( len > 0 ) {
		const CWT_CHAR *ptr = (const CWT_CHAR*)data;

		return ( *ptr >= (CWT_CHAR)0x21 && *ptr <= (CWT_CHAR)0x7E )
				? 1 : 0;
	}

	return len == 0 ? 0 : -1;
}

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

 */


/* %x41-5A / %x61-7A   ; A-Z / a-z */
static CwtFsmTransition ALPHA_FSM[] = {
	  { -1,-1, FSM_MATCH_CHAR(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), 52), FSM_ACCEPT, NULL, NULL }
 };

/* "0" / "1" */
static CwtFsmTransition BIT_FSM[] = {
	{ -1,-1, FSM_MATCH_CHAR(_T("01"), 2), FSM_ACCEPT, NULL, NULL }
};

/*
 %x01-7F ; any 7-bit US-ASCII character, excluding NUL
*/
static CwtFsmTransition CHAR_FSM[] = {
	{ -1,-1, FSM_MATCH_FUNC(__fsm_CHAR, NULL), FSM_ACCEPT, NULL, NULL }
};


/* %x0D ; carriage return */
static CwtFsmTransition CR_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T("\x0D"), 1), FSM_ACCEPT, NULL, NULL }
};

/* %x0A ; linefeed */
static CwtFsmTransition LF_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T("\x0A"), 1), FSM_ACCEPT, NULL, NULL }
};

/* CR LF ; Internet standard newline */
static CwtFsmTransition CRLF_FSM[] = {
	  {-1,-1, FSM_MATCH_STR(_T("\x0D\x0A"), 2), FSM_ACCEPT, NULL, NULL }
};


/* %x00-1F / %x7F  ; controls */
static CwtFsmTransition CTL_FSM[] = {
	{ -1,-1, FSM_MATCH_FUNC(__fsm_CTL, NULL), FSM_ACCEPT, NULL, NULL }
};


/* %x30-39 ; 0-9 */
static CwtFsmTransition DIGIT_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T("0123456789"), 10), FSM_ACCEPT, NULL, NULL }
};


/* %x22 ; " (Double Quote) */
static CwtFsmTransition DQUOTE_FSM[] = {
	{-1,-1, FSM_MATCH_CHAR(_T("\""), 1), FSM_ACCEPT, NULL, NULL }
};


/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static CwtFsmTransition HEXDIG_FSM[] = {
	  {-1, 1, FSM_MATCH_FSM(DIGIT_FSM),               FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_T("ABCDEFabcdef"), 12), FSM_ACCEPT, NULL, NULL }
};

/* %x09 ; horizontal tab */
static CwtFsmTransition HTAB_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T("\x09"), 1), FSM_ACCEPT, NULL, NULL }
};

/* %x00-FF ; 8 bits of data */
static CwtFsmTransition OCTET_FSM[] = {
	{ -1,-1, FSM_MATCH_FUNC(__fsm_OCTET, NULL), FSM_ACCEPT, NULL, NULL }
};

/* %x20  ; space */
static CwtFsmTransition SP_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T("\x20"), 1), FSM_ACCEPT, NULL, NULL }
};

/* %x21-7E ; visible (printing) characters */
static CwtFsmTransition VCHAR_FSM[] = {
	{ -1,-1, FSM_MATCH_FUNC(__fsm_VCHAR, NULL), FSM_ACCEPT, NULL, NULL }
};

/* SP / HTAB ; white space */
static CwtFsmTransition WSP_FSM[] = {
	{-1,-1, FSM_MATCH_CHAR(_T("\x20\x09"), 2), FSM_ACCEPT, NULL, NULL }
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
	  { 0, 1, FSM_MATCH_CHAR(_T("\x20\x09"), 2),    FSM_NORMAL, NULL, NULL }
	, { 0, 2, FSM_MATCH_STR(_T("\x0D\x0A\x20"), 3), FSM_NORMAL, NULL, NULL }
	, { 0, 3, FSM_MATCH_STR(_T("\x0D\x0A\x09"), 3), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,                    FSM_ACCEPT, NULL, NULL }
};


static void __fsm_unused_commons(void)
{
	CWT_UNUSED3( ALPHA_FSM,  BIT_FSM,   CHAR_FSM);
	CWT_UNUSED3( CR_FSM,     LF_FSM,    CRLF_FSM);
	CWT_UNUSED3( CTL_FSM,    DIGIT_FSM, DQUOTE_FSM);
	CWT_UNUSED3( HEXDIG_FSM, HTAB_FSM,  LWSP_FSM);
	CWT_UNUSED3( OCTET_FSM,  SP_FSM,    VCHAR_FSM);
	CWT_UNUSED ( WSP_FSM);
}

#endif /* __CWT_FSM_COMMON_H__ */
