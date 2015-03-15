/**
 * @file   logger_p.hpp
 * @author wladt
 * @date   Feb 13, 2013 3:46:59 PM
 *
 * @brief
 */

#ifndef __PFS_LOGGER_P_HPP__
#define __PFS_LOGGER_P_HPP__

#include "pfs/logger.hpp"
#include <pfs/fsm.hpp>
#include <ctime>

namespace pfs {

struct pattern_spec
{
	pfs::ucchar spec_char;
	bool        left_justify;
	size_t      min_width;
	size_t      max_width;
	pfs::string fspec; /* format specifier */
};

struct pattern_context
{
	pfs::log::priority  level;
	pfs::string         result;
	const pfs::string * msg;
	pattern_spec        pspec;
};

const pfs::string __priority_str[] = {
	  _u8("Trace")
	, _u8("Debug")
	, _u8("Info")
	, _u8("Warn")
	, _u8("Error")
	, _u8("Fatal")
};

static bool begin_spec        (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void * action_args);
static bool end_spec          (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void * action_args);
static bool append_plain_char (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void * action_args);
static bool set_left_justify  (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void * action_args);
static bool set_min_width     (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void * action_args);
static bool set_max_width     (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void * action_args);
static bool set_spec_char     (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void * action_args);
static bool set_format_spec   (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void * action_args);

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

//static pfs::string _LOGGER_ALPHA("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
//static pfs::string _LOGGER_WS(" \t");
static pfs::string LOGGER_DIGIT("0123456789");

/* exclude '%' (0x25) */
pfs::ucchar plain_char[] = {
	  pfs::ucchar(0x20u), pfs::ucchar(0x24u)
	, pfs::ucchar(0x26u), pfs::ucchar(0x10FFFFu)
};
static pfs::fsm::transition<pfs::string> plain_char_fsm[] = {
      {-1, 1, FSM_MATCH_RANGE(plain_char[0], plain_char[1]) , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RANGE(plain_char[2], plain_char[3]) , FSM_ACCEPT, nullptr, nullptr }
};

/* format-mod = [ "-" ] *2DIGIT [ "." *2DIGIT ] */
static pfs::fsm::transition<pfs::string> dot_digit_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_u8("."))               , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_CHAR(LOGGER_DIGIT, 0, 2) , FSM_ACCEPT, set_max_width, nullptr }
};

static pfs::fsm::transition<pfs::string> format_mod_fsm[] = {
	  { 1,-1, FSM_MATCH_OPT_CHAR(_u8("-"))           , FSM_NORMAL, set_left_justify, nullptr }
	, { 2,-1, FSM_MATCH_RPT_CHAR(LOGGER_DIGIT, 0, 2) , FSM_NORMAL, set_min_width, nullptr }
	, {-1,-1, FSM_MATCH_OPT_FSM(dot_digit_fsm)       , FSM_ACCEPT, nullptr, nullptr }
};

/* format-spec = "{" *( <exclude '{' (0x7B) and '}' (0x7D) > ) "}" */
pfs::ucchar format_spec_char[] = {
	  pfs::ucchar(0x20u), pfs::ucchar(0x7Au)
	, pfs::ucchar(0x7Cu), pfs::ucchar(0x7Cu)
	, pfs::ucchar(0x7Eu), pfs::ucchar(0x10FFFFu)
};
static pfs::fsm::transition<pfs::string> format_spec_char_fsm[] = {
	  {-1, 1, FSM_MATCH_RANGE(format_spec_char[0], format_spec_char[1]) , FSM_ACCEPT, nullptr, nullptr }
	, {-1, 2, FSM_MATCH_RANGE(format_spec_char[2], format_spec_char[3]) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RANGE(format_spec_char[4], format_spec_char[5]) , FSM_ACCEPT, nullptr, nullptr }
};
static pfs::fsm::transition<pfs::string> format_spec_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_u8("{"))                       , FSM_NORMAL, nullptr, nullptr }
    , { 2,-1, FSM_MATCH_RPT_FSM(format_spec_char_fsm, 0,256) , FSM_NORMAL, set_format_spec, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_u8("}"))                       , FSM_ACCEPT, nullptr, nullptr }
};

/* spec = "%" [ format-mod ] ( "m" / "d" / "p" ) [ format-spec ]*/
static pfs::fsm::transition<pfs::string> spec_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_u8("%"))           , FSM_NORMAL, begin_spec, nullptr }
    , { 2,-1, FSM_MATCH_OPT_FSM(format_mod_fsm)  , FSM_NORMAL, nullptr, nullptr }
    , { 3,-1, FSM_MATCH_CHAR(_u8("mdpnt"))       , FSM_NORMAL, set_spec_char, nullptr }
    , {-1,-1, FSM_MATCH_OPT_FSM(format_spec_fsm) , FSM_ACCEPT, end_spec, nullptr }
};

/* pattern = *( spec-fsm / plain-char ) */
/* spec-fsm / plain-char */
static pfs::fsm::transition<pfs::string> pattern_unit_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(spec_fsm)       , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_FSM(plain_char_fsm) , FSM_ACCEPT, append_plain_char, nullptr }
};

static pfs::fsm::transition<pfs::string> pattern_fsm[] = {
	{-1,-1, FSM_MATCH_RPT_FSM(pattern_unit_fsm, 0,-1) , FSM_ACCEPT, nullptr, nullptr }
};


static bool begin_spec (pfs::string::const_iterator, pfs::string::const_iterator,  void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	ctx->pspec.spec_char = pfs::ucchar();
	ctx->pspec.left_justify = false;
	ctx->pspec.min_width = 0;
	ctx->pspec.max_width = 0;
	ctx->pspec.fspec.clear();
	return true;
}

static bool end_spec (pfs::string::const_iterator, pfs::string::const_iterator, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	char spec_char = char(ctx->pspec.spec_char);
	pfs::string result;

	switch((char)spec_char) {
	case 'n':
		result << pfs::ucchar('\n');
		break;
	case 't':
		result << pfs::ucchar('\t');
		break;
	case 'p':
		result = __priority_str[ctx->level];
		break;
	case 'm':
		PFS_ASSERT(ctx->msg);
		result = *ctx->msg;
		break;
	case 'd': {
		/* FIXME replace code with reentrant time functions or use mutex */
		time_t t;
		struct tm *tm;
		char buf[128];
		t = time(nullptr);
		tm = localtime(&t);

		if (ctx->pspec.fspec == pfs::string("ABSOLUTE")) {
			strftime(buf, sizeof(buf), "%H:%M:%S", tm);
		} else if (ctx->pspec.fspec == pfs::string("DATE")) {
			strftime(buf, sizeof(buf), "%d %b %Y %H:%M:%S", tm);
		} else if(ctx->pspec.fspec == pfs::string("ISO8601")) {
			strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
		} else {
			strftime(buf, sizeof(buf), ctx->pspec.fspec.c_str(), tm);
		}
		result = pfs::string().fromUtf8(buf);
	}
		break;
	default:
		break;
	}

	/* truncate */
	if (ctx->pspec.max_width > 0 && result.length() > ctx->pspec.max_width) {
		result = result.substr(0, ctx->pspec.max_width);
	}

	/* pad */
	if (ctx->pspec.min_width > 0 && result.length() < ctx->pspec.min_width) {
		pfs::string padding(ctx->pspec.min_width - result.length(), pfs::ucchar(' '));
		if (ctx->pspec.left_justify) {
			result.append(padding);
		} else {
			result.prepend(padding);
		}
	}

	ctx->result.append(result);

	return true;
}

static bool set_spec_char (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void *)
{
	PFS_ASSERT(begin < end);
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	ctx->pspec.spec_char = *begin;
	return true;
}

static bool set_format_spec (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	ctx->pspec.fspec = pfs::string(begin, end);
	return true;
}


static bool append_plain_char (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	ctx->result.append(pfs::string(begin, end));
	return true;
}

static bool set_left_justify (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	if (begin + 1 == end) {
		ctx->pspec.left_justify = true;
	}
	return true;
}

static bool set_min_width (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	if (begin < end) {
		bool ok;
		pfs::string n(begin, end);
		ctx->pspec.min_width = n.toUInt(&ok);
		PFS_VERIFY_X(ok, _Tr("Bad padding value in Logger pattern"));
	}
	return true;
}

static bool set_max_width (pfs::string::const_iterator begin, pfs::string::const_iterator end, void * context, void *)
{
	pattern_context * ctx = reinterpret_cast<pattern_context *>(context);
	if (begin < end) {
		bool ok;
		pfs::string n(begin, end);
		ctx->pspec.max_width = n.toUInt(&ok);
		PFS_VERIFY_X(ok, _Tr("Bad truncation value in Logger pattern"));
	}
	return true;
}

} // pfs

#endif /* __PFS_LOGGER_P_HPP__ */
