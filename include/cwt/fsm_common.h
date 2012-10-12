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

static CwtFsmTransition ALPHA_FSM[] = {
	{ -1, FSM_MATCH_CHAR(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), 52), FSM_TERM, NULL, NULL }
 };

static CwtFsmTransition DIGIT_FSM[] = {
	{ -1, FSM_MATCH_CHAR(_T("0123456789"), 10), FSM_TERM, NULL, NULL }
};

static CwtFsmTransition HEXDIG_FSM[] = {
      {-1, FSM_MATCH_FSM(DIGIT_FSM), FSM_CHAINED, NULL, NULL  }
    , {-1, FSM_MATCH_CHAR(_T("ABCDEFabcdef"), 12), FSM_TERM, NULL, NULL }
};

/*
  RFC 2234

     ALPHA          =  %x41-5A / %x61-7A   ; A-Z / a-z

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

        DIGIT          =  %x30-39
                               ; 0-9

        DQUOTE         =  %x22
                               ; " (Double Quote)

        HEXDIG         =  DIGIT / "A" / "B" / "C" / "D" / "E" / "F"

        HTAB           =  %x09
                               ; horizontal tab

        LF             =  %x0A
                               ; linefeed

        LWSP           =  *(WSP / CRLF WSP)
                               ; linear white space (past newline)

        OCTET          =  %x00-FF
                               ; 8 bits of data

        SP             =  %x20
                               ; space

        VCHAR          =  %x21-7E
                               ; visible (printing) characters

        WSP            =  SP / HTAB
                               ; white space
 */


#endif /* __CWT_FSM_COMMON_H__ */
