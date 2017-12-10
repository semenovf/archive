/**
 * @file   logger_p.hpp
 * @author wladt
 * @date   Feb 13, 2013 3:46:59 PM
 *
 * @brief
 */

#ifndef __PFS_LOGGER_P_HPP__
#define __PFS_LOGGER_P_HPP__

#include <ctime>
#include <pfs/datetime.hpp>
#include <pfs/fsm.hpp>
#include <pfs/fsm/aliases.hpp>

#include "pfs/logger.hpp"

namespace pfs {

struct pattern_spec
{
	string::value_type spec_char;
	bool        left_justify;
	size_t      min_width;
	size_t      max_width;
	string      fspec; /* format specifier */
};

struct pattern_context
{
    logger_appender * appender;
	int            level;
	string         result;
	string const * msg;
	pattern_spec   pspec;
};

static bool begin_spec        (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool end_spec          (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool append_plain_char (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_left_justify  (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_min_width     (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_max_width     (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_spec_char     (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);
static bool set_format_spec   (string::const_iterator begin, string::const_iterator end, void * context, void * action_args);

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
 * p    Priority (Trace, Debug, Info etc.).
 * n    new line
 * t    horizontal tab
 */

typedef pfs::fsm::m<pfs::string> M;

//static string _LOGGER_ALPHA("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
//static string _LOGGER_WS(" \t");
static string LOGGER_DIGIT("0123456789");

/* exclude '%' (0x25) */
string::value_type plain_char[] = {
	  string::value_type(0x20u), string::value_type(0x24u)
	, string::value_type(0x26u), string::value_type(0x10FFFFu)
};

static fsm::transition<string> plain_char_fsm[] = {
      {-1, 1, M::range(plain_char[0], plain_char[1]).m , FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::range(plain_char[2], plain_char[3]).m , FSM_ACCEPT, 0, 0 }
};

/* format-mod = [ "-" ] *2DIGIT [ "." *2DIGIT ] */
static fsm::transition<string> dot_digit_fsm[] = {
	  { 1,-1, M::chr(_u8(".")).m               , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::rpt_chr(LOGGER_DIGIT, 0, 2).m , FSM_ACCEPT, set_max_width, 0 }
};

static fsm::transition<string> format_mod_fsm[] = {
	  { 1,-1, M::opt_chr(_u8("-")).m           , FSM_NORMAL, set_left_justify, 0 }
	, { 2,-1, M::rpt_chr(LOGGER_DIGIT, 0, 2).m , FSM_NORMAL, set_min_width, 0 }
	, {-1,-1, M::opt_tr(dot_digit_fsm).m       , FSM_ACCEPT, 0, 0 }
};

/* format-spec = "{" *( <exclude '{' (0x7B) and '}' (0x7D) > ) "}" */
string::value_type format_spec_char[] = {
	  string::value_type(0x20u), string::value_type(0x7Au)
	, string::value_type(0x7Cu), string::value_type(0x7Cu)
	, string::value_type(0x7Eu), string::value_type(0x10FFFFu)
};
static fsm::transition<string> format_spec_char_fsm[] = {
	  {-1, 1, M::range(format_spec_char[0], format_spec_char[1]).m , FSM_ACCEPT, 0, 0 }
	, {-1, 2, M::range(format_spec_char[2], format_spec_char[3]).m , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::range(format_spec_char[4], format_spec_char[5]).m , FSM_ACCEPT, 0, 0 }
};
static fsm::transition<string> format_spec_fsm[] = {
      { 1,-1, M::chr(_u8("{")).m                       , FSM_NORMAL, 0, 0 }
    , { 2,-1, M::rpt_tr(format_spec_char_fsm, 0,256).m , FSM_NORMAL, set_format_spec, 0 }
    , {-1,-1, M::chr(_u8("}")).m                       , FSM_ACCEPT, 0, 0 }
};

/* spec = "%" [ format-mod ] ( "m" / "d" / "p" ) [ format-spec ]*/
static fsm::transition<string> spec_fsm[] = {
      { 1,-1, M::chr(_u8("%")).m           , FSM_NORMAL, begin_spec, 0 }
    , { 2,-1, M::opt_tr(format_mod_fsm).m  , FSM_NORMAL, 0, 0 }
    , { 3,-1, M::chr(_u8("mdpnt")).m       , FSM_NORMAL, set_spec_char, 0 }
    , {-1,-1, M::opt_tr(format_spec_fsm).m , FSM_ACCEPT, end_spec, 0 }
};

/* pattern = *( spec-fsm / plain-char ) */
/* spec-fsm / plain-char */
static fsm::transition<string> pattern_unit_fsm[] = {
      {-1, 1, M::tr(spec_fsm).m       , FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::tr(plain_char_fsm).m , FSM_ACCEPT, append_plain_char, 0 }
};

static fsm::transition<string> pattern_fsm[] = {
	{-1,-1, M::rpt_tr(pattern_unit_fsm, 0,-1).m , FSM_ACCEPT, 0, 0 }
};


static bool begin_spec (string::const_iterator, string::const_iterator,  void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	ctx->pspec.spec_char = string::value_type();
	ctx->pspec.left_justify = false;
	ctx->pspec.min_width = 0;
	ctx->pspec.max_width = 0;
	ctx->pspec.fspec.clear();
	return true;
}

static bool end_spec (string::const_iterator, string::const_iterator, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	string result;

	switch(lexical_cast<char>(ctx->pspec.spec_char)) {
	case 'n':
		result.push_back('\n');
		break;
	case 't':
		result.push_back('\t');
		break;
	case 'p':
		result = ctx->appender->priority_text(ctx->level);
		break;
	case 'm':
		PFS_ASSERT(ctx->msg);
		result = *ctx->msg;
		break;

	case 'd': {
        pfs::datetime dt = pfs::current_datetime();

		if (ctx->pspec.fspec == string("ABSOLUTE")) {
            result = pfs::to_string(dt, string("%H:%M:%S.%Q"));
		} else if (ctx->pspec.fspec == string("DATE")) {
            result = pfs::to_string(dt, string("%d %b %Y %H:%M:%S.%Q"));
		} else if(ctx->pspec.fspec == string("ISO8601")) {
			result = pfs::to_string(dt, string("%Y-%m-%d %H:%M:%S.%Q"));
		} else {
			result = pfs::to_string(dt, ctx->pspec.fspec);
		}

		break;
	}

	default:
		break;
	}

	/* truncate */
	if (ctx->pspec.max_width > 0 && result.length() > ctx->pspec.max_width) {
		result = result.substr(0, ctx->pspec.max_width);
	}

	/* pad */
	if (ctx->pspec.min_width > 0 && result.length() < ctx->pspec.min_width) {
		string padding(ctx->pspec.min_width - result.length(), string::value_type(' '));
		if (ctx->pspec.left_justify) {
			result.append(padding);
		} else {
			result.prepend(padding);
		}
	}

	ctx->result.append(result);

	return true;
}

static bool set_spec_char (string::const_iterator begin, string::const_iterator end, void * context, void *)
{
	PFS_ASSERT(begin < end);
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	ctx->pspec.spec_char = *begin;
	return true;
}

static bool set_format_spec (string::const_iterator begin, string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	ctx->pspec.fspec = string(begin, end);
	return true;
}


static bool append_plain_char (string::const_iterator begin, string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	ctx->result.append(string(begin, end));
	return true;
}

static bool set_left_justify (string::const_iterator begin, string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	std::advance(begin, 1);
	if (begin == end) {
		ctx->pspec.left_justify = true;
	}
	return true;
}

static bool set_min_width (string::const_iterator begin, string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	if (begin < end) {
		bool ok;
		string n(begin, end);
		ctx->pspec.min_width = lexical_cast<size_t>(n, & ok);
		PFS_ASSERT_X(ok, "Bad padding value in Logger pattern");
	}
	return true;
}

static bool set_max_width (string::const_iterator begin, string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	if (begin < end) {
		bool ok;
		string n(begin, end);
		ctx->pspec.max_width = lexical_cast<size_t>(n, & ok);
		PFS_ASSERT_X(ok, "Bad truncation value in Logger pattern");
	}
	return true;
}

} // pfs

#endif /* __PFS_LOGGER_P_HPP__ */
