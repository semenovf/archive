/**
 * @file   logger_p.hpp
 * @author wladt
 * @date   Feb 13, 2013 3:46:59 PM
 *
 * @brief
 */

#ifndef __CWT_LOGGER_P_HPP__
#define __CWT_LOGGER_P_HPP__

#include <cwt/fsm.hpp>
#include <cwt/logger.hpp>

CWT_NS_BEGIN

#define FSM_MATCH_CHAR(s)         FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)          FsmMatch(new FsmMatchStringStr(s))
#define FSM_MATCH_RPT_CHAR(s,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_OPT_CHAR(s)     FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),0,1))

/**
 * Each conversion specifier starts with a percent sign (%)
 * and is followed by optional format modifiers and a conversion character.
 *
 * Format modifier 	left justify 	minimum width 	maximum width 	comment
 * %20m 	        false        	20 				none 			Left pad with spaces if the message is less than 20 characters long.
 * %-20m 			true 			20 				none 			Right pad with spaces if the message is less than 20 characters long.
 * %.30m 			NA 				none 			30 				Truncate from the beginning if the message is longer than 30 characters.
 * %20.30m 			false 			20 				30 				Left pad with spaces if the message is shorter than 20 characters. However, if message is longer than 30 characters, then truncate from the beginning.
 * %-20.30m 		true 			20 				30 				Right pad with spaces if the message is shorter than 20 characters. However, if message is longer than 30 characters, then truncate from the beginning.
 *
 * Conversation characters
 * m 	message associated with the logging event.
 * d	date of the logging event. The date conversion specifier may be followed
 * 		by a date format specifier enclosed between braces.
 * 		For example, %d{HH:mm:ss,SSS} or %d{dd MMM yyyy HH:mm:ss,SSS}.
 * 		If no date format specifier is given then ISO8601 format is assumed.
 */

static String _ALPHA(_U("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
static String _DIGIT(_U("0123456789"));
static String _WS(_U(" \t"));

/* format-mod = [ "-" ] *2DIGIT [ "." *2DIGIT ] */
static FsmTransition dot_digit_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("."))          , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_DIGIT, 0, 2) , FSM_ACCEPT, NULL, NULL }
};
static FsmTransition format_mod_fsm[] = {
	  { 1,-1, FSM_MATCH_OPT_CHAR(_U("-"))      , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_DIGIT, 0, 2) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_OPT_FSM(dot_digit_fsm) , FSM_ACCEPT, NULL, NULL }
};

/* format-spec = "{" *( ALPHA / DIGIT / "." / "," / ":" / "-" / "_" / WS ) "}" */
static FsmTransition format_spec_char_fsm[] = {
      {-1, 1, FSM_MATCH_CHAR(_ALPHA)      , FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_CHAR(_DIGIT)      , FSM_ACCEPT, NULL, NULL }
    , {-1, 3, FSM_MATCH_CHAR(_WS)         , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_U(".,:-_")) , FSM_ACCEPT, NULL, NULL }
};
static FsmTransition format_spec_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_U("{"))     , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_DIGIT)      , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_U("}"))     , FSM_ACCEPT, NULL, NULL }
};

/* spec = "%" [ format-mod ] ( "m" / "d" ) [ ] */
static FsmTransition spec_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_U("%"))            , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_OPT_FSM(format_mod_fsm)  , FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_CHAR(_U("md"))           , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_OPT_FSM(format_spec_fsm) , FSM_NORMAL, NULL, NULL }
};

/* pattern = spec-fsm  */
static FsmTransition pattern_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_U("%"))            , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_OPT_FSM(format_mod_fsm)  , FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_CHAR(_U("md"))           , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_OPT_FSM(format_spec_fsm) , FSM_NORMAL, NULL, NULL }
};


CWT_NS_END

#endif /* __CWT_LOGGER_P_HPP__ */
