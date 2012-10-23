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

/* %x41-5A / %x61-7A   ; A-Z / a-z */
static CwtFsmTransition ALPHA_FSM[] = {
	  { -1,-1, FSM_MATCH_CHAR(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), 52), FSM_ACCEPT, NULL, NULL }
 };

/* %x30-39 ; 0-9 */
static CwtFsmTransition DIGIT_FSM[] = {
	  {-1,-1, FSM_MATCH_CHAR(_T("0123456789"), 10), FSM_ACCEPT, NULL, NULL }
};

/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static CwtFsmTransition HEXDIG_FSM[] = {
	  {-1, 1, FSM_MATCH_FSM(DIGIT_FSM),               FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_T("ABCDEFabcdef"), 12), FSM_ACCEPT, NULL, NULL }
};


/* %x20  ; space */
static CwtFsmTransition SP_FSM[] = {
	  {-1,-1, FSM_MATCH_STR(_T(" "), 1), FSM_ACCEPT, NULL, NULL }
};


/*
  RFC 2234

        BIT            =  "0" / "1"

        CHAR           =  %x01-7F
                               ; any 7-bit US-ASCII character,
                                  excluding NUL

        CR             =  %x0D
                               ; carriage return

        CRLF           =  CR LF
                               ; Internet standard newline

        CTL            =  %x00-1F / %x7F
                               ; controls


        DQUOTE         =  %x22
                               ; " (Double Quote)

        HTAB           =  %x09
                               ; horizontal tab

        LF             =  %x0A
                               ; linefeed

        LWSP           =  *(WSP / CRLF WSP)
                               ; linear white space (past newline)

        OCTET          =  %x00-FF
                               ; 8 bits of data

        VCHAR          =  %x21-7E
                               ; visible (printing) characters

        WSP            =  SP / HTAB
                               ; white space
 */


#endif /* __CWT_FSM_COMMON_H__ */
