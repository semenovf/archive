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
#include <ctime>

CWT_NS_BEGIN

struct PatternSpec {
	Char spec_char;
	bool left_justify;
	size_t min_width;
	size_t max_width;
	String fspec; /* format specifier */
};

struct LoggerPatternContext {
	Logger::Priority priority;
	String result;
	const String *msg;
	PatternSpec pspec;
};

const char *__priority_str[] = {
	  "Trace"
	, "Debug"
	, "Info"
	, "Warn"
	, "Error"
	, "Fatal"
};

#define FSM_MATCH_CHAR(s)         FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)          FsmMatch(new FsmMatchStringStr(s))
#define FSM_MATCH_RPT_CHAR(s,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_OPT_CHAR(s)     FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),0,1))

static bool begin_spec(const void *data, size_t len, void *context, void *action_args);
static bool end_spec(const void *data, size_t len, void *context, void *action_args);
static bool append_plain_char(const void *data, size_t len, void *context, void *action_args);
static bool set_left_justify(const void *data, size_t len, void *context, void *action_args);
static bool set_min_width(const void *data, size_t len, void *context, void *action_args);
static bool set_max_width(const void *data, size_t len, void *context, void *action_args);
static bool set_spec_char(const void *data, size_t len, void *context, void *action_args);
static bool set_format_spec(const void *data, size_t len, void *context, void *action_args);

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
 */

static String _LOGGER_ALPHA(_U("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
static String _LOGGER_DIGIT(_U("0123456789"));
static String _LOGGER_WS(_U(" \t"));

/* exclude '%' (0x25) */
Char plain_char[] = {
	  0x20, 0x24
	, 0x26, 0x10FFFF
};
static FsmTransition plain_char_fsm[] = {
      {-1, 1, FSM_MATCH_RANGE(&plain_char[0], &plain_char[1]) , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_RANGE(&plain_char[2], &plain_char[3]) , FSM_ACCEPT, NULL, NULL }
};

/* format-mod = [ "-" ] *2DIGIT [ "." *2DIGIT ] */
static FsmTransition dot_digit_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("."))          , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_LOGGER_DIGIT, 0, 2) , FSM_ACCEPT, set_max_width, NULL }
};
static FsmTransition format_mod_fsm[] = {
	  { 1,-1, FSM_MATCH_OPT_CHAR(_U("-"))      , FSM_NORMAL, set_left_justify, NULL }
	, { 2,-1, FSM_MATCH_RPT_CHAR(_LOGGER_DIGIT, 0, 2) , FSM_NORMAL, set_min_width, NULL }
	, {-1,-1, FSM_MATCH_OPT_FSM(dot_digit_fsm) , FSM_ACCEPT, NULL, NULL }
};

/* format-spec = "{" *( <exclude '{' (0x7B) and '}' (0x7D) > ) "}" */
Char format_spec_char[] = {
	  0x20, 0x7A
	, 0x7C, 0x7C
	, 0x7E, 0x10FFFF
};
static FsmTransition format_spec_char_fsm[] = {
	  {-1, 1, FSM_MATCH_RANGE(&format_spec_char[0], &format_spec_char[1]) , FSM_ACCEPT, NULL, NULL }
	, {-1, 2, FSM_MATCH_RANGE(&format_spec_char[2], &format_spec_char[3]) , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_RANGE(&format_spec_char[4], &format_spec_char[5]) , FSM_ACCEPT, NULL, NULL }
};
static FsmTransition format_spec_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_U("{"))     , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_RPT_FSM(format_spec_char_fsm, 0,256) , FSM_NORMAL, set_format_spec, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_U("}"))     , FSM_ACCEPT, NULL, NULL }
};

/* spec = "%" [ format-mod ] ( "m" / "d" / "p" ) [ format-spec ]*/
static FsmTransition spec_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_U("%"))            , FSM_NORMAL, begin_spec, NULL }
    , { 2,-1, FSM_MATCH_OPT_FSM(format_mod_fsm)  , FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_CHAR(_U("mdp"))          , FSM_NORMAL, set_spec_char, NULL }
    , {-1,-1, FSM_MATCH_OPT_FSM(format_spec_fsm) , FSM_ACCEPT, end_spec, NULL }
};

/* pattern = *( spec-fsm / plain-char ) */
/* spec-fsm / plain-char */
static FsmTransition pattern_unit_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(spec_fsm)       , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(plain_char_fsm) , FSM_ACCEPT, append_plain_char, NULL }
};

static FsmTransition pattern_fsm[] = {
	{-1,-1, FSM_MATCH_RPT_FSM(pattern_unit_fsm, 0,-1) , FSM_ACCEPT, NULL, NULL }
};


static bool begin_spec(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	LoggerPatternContext *ctx = reinterpret_cast<LoggerPatternContext *>(context);
	ctx->pspec.spec_char = 0;
	ctx->pspec.left_justify = false;
	ctx->pspec.min_width = 0;
	ctx->pspec.max_width = 0;
	ctx->pspec.fspec.clear();
	return true;
}


static bool end_spec(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	LoggerPatternContext *ctx = reinterpret_cast<LoggerPatternContext *>(context);
	char spec_char = ctx->pspec.spec_char.toLatin1();
	String result;

	switch((char)spec_char) {
	case 'p':
		result = String().fromUtf8(__priority_str[ctx->priority]);
		break;
	case 'm':
		CWT_ASSERT(ctx->msg);
		result = *ctx->msg;
		break;
	case 'd': {
		/* FIXME replace code with reentrant time functions or use mutex */
		time_t t;
		struct tm *tm;
		char buf[128];
		t = time(NULL);
		tm = localtime(&t);

		if (ctx->pspec.fspec == _U("ABSOLUTE")) {
			strftime(buf, sizeof(buf), "%H:%M:%S", tm);
		} else if (ctx->pspec.fspec == _U("DATE")) {
			strftime(buf, sizeof(buf), "%d %b %Y %H:%M:%S", tm);
		} else if(ctx->pspec.fspec == _U("ISO8601")) {
			strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
		} else {
			strftime(buf, sizeof(buf), ctx->pspec.fspec.toUtf8().data(), tm);
		}
		result = String().fromUtf8(buf);
	}
		break;
	default:
		break;
	}

	/* truncate */
	if (ctx->pspec.max_width > 0 && result.length() > ctx->pspec.max_width) {
		result.truncate(ctx->pspec.max_width);
	}

	/* pad */
	if (ctx->pspec.min_width > 0 && result.length() < ctx->pspec.min_width) {
		String padding(ctx->pspec.min_width - result.length(), Char(0x20));
		if (ctx->pspec.left_justify) {
			result.append(padding);
		} else {
			result.prepend(padding);
		}
	}

	ctx->result.append(result);

	return true;
}

static bool set_spec_char(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	CWT_ASSERT(len ==1);
	LoggerPatternContext *ctx = reinterpret_cast<LoggerPatternContext *>(context);
	const Char* spec_char = reinterpret_cast<const Char*>(data);
	ctx->pspec.spec_char = spec_char[0];
	return true;
}

static bool set_format_spec(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	LoggerPatternContext *ctx = reinterpret_cast<LoggerPatternContext *>(context);
	ctx->pspec.fspec = String(reinterpret_cast<const Char*>(data), len);
	return true;
}

static bool append_plain_char(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	String ch(reinterpret_cast<const Char*>(data), len);
	LoggerPatternContext *ctx = reinterpret_cast<LoggerPatternContext *>(context);
	ctx->result.append(ch);
	return true;
}

static bool set_left_justify(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED2(data, action_args);
	LoggerPatternContext *ctx = reinterpret_cast<LoggerPatternContext *>(context);
	if (len == 1) {
		ctx->pspec.left_justify = true;
	}
	return true;
}

static bool set_min_width(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED2(data, action_args);
	LoggerPatternContext *ctx = reinterpret_cast<LoggerPatternContext *>(context);
	if (len > 0) {
		bool ok;
		String n(reinterpret_cast<const Char*>(data), len);
		ctx->pspec.min_width = n.toUInt(&ok);
		if (!ok) {
			fprintf(stderr, _Tr("[<!LOGGER PATTERN: BAD PADDING VALUE!>]"));
		}
	}
	return true;
}

static bool set_max_width(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED2(data, action_args);
	LoggerPatternContext *ctx = reinterpret_cast<LoggerPatternContext *>(context);
	if (len > 0) {
		bool ok;
		String n(reinterpret_cast<const Char*>(data), len);
		ctx->pspec.max_width = n.toUInt(&ok);
		if (!ok) {
			fprintf(stderr, _Tr("[<!LOGGER PATTERN: BAD TRUNCATION VALUE!>]"));
		}
	}
	return true;
}

CWT_NS_END

#endif /* __CWT_LOGGER_P_HPP__ */
