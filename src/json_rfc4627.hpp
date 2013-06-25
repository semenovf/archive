/*
 * Copyright (c) 2012, Fedor Semenov
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * uri-rfc4627.hpp
 *
 *  Created on: 30.01.2012
 *      Author: Fedor Semenov
 */


#ifndef __CWT_JSON_RFC4627_HPP__
#define __CWT_JSON_RFC4627_HPP__

#include "../include/cwt/fsm.hpp"
#include "json_p.hpp"

CWT_NS_BEGIN

static bool __begin_json(const void *data, size_t len, void *context, void *action_args);
static bool __end_json(const void *data, size_t len, void *context, void *action_args);
static bool __begin_member(const void *data, size_t len, void *context, void *action_args);
static bool __end_member(const void *data, size_t len, void *context, void *action_args);
static bool __begin_object(const void *data, size_t len, void *context, void *action_args);
static bool __end_object(const void *data, size_t len, void *context, void *action_args);
static bool __begin_array(const void *data, size_t len, void *context, void *action_args);
static bool __end_array(const void *data, size_t len, void *context, void *action_args);
static bool __scalar_value(const void *data, size_t len, void *context, void *action_args);

static const int _JSON_VALUE_NULL   = 1;
static const int _JSON_VALUE_FALSE  = 2;
static const int _JSON_VALUE_TRUE   = 3;
static const int _JSON_VALUE_NUMBER = 4;
static const int _JSON_VALUE_STRING = 5;
/*
static const int _JSON_VALUE_OBJECT = 6;
static const int _JSON_VALUE_ARRAY  = 7;
*/

/*
   RFC 4627:  The application/json Media Type for JavaScript Object Notation (JSON).
   URL: http://www.ietf.org/rfc/rfc4627.txt
  -----------------------------------------------------------------------

	A JSON text is a sequence of tokens.  The set of tokens includes six
	structural characters, strings, numbers, and three literal names.

	A JSON text is a serialized object or array.

	JSON-text = object / array

	ws = *(
			%x20 /              ; Space
			%x09 /              ; Horizontal tab
			%x0A /              ; Line feed or New line
			%x0D                ; Carriage return
		)

	begin-array     = ws %x5B ws  ; [ left square bracket
	begin-object    = ws %x7B ws  ; { left curly bracket
	end-array       = ws %x5D ws  ; ] right square bracket
	end-object      = ws %x7D ws  ; } right curly bracket
	name-separator  = ws %x3A ws  ; : colon
	value-separator = ws %x2C ws  ; , comma

	Values.
	------------------------------------------------------------------
	A JSON value MUST be an object, array, number, or string, or one of
	the following three literal names:

		false null true

	The literal names MUST be lowercase.  No other literal names are
	allowed.

	value = false / null / true / object / array / number / string
	false = %x66.61.6c.73.65   ; false
	null  = %x6e.75.6c.6c      ; null
	true  = %x74.72.75.65      ; true


	Objects.
	------------------------------------------------------------------
	object = begin-object [ member *( value-separator member ) ] end-object

	member = string name-separator value

	Arrays.
	------------------------------------------------------------------
	array = begin-array [ value *( value-separator value ) ] end-array


	Numbers.
	------------------------------------------------------------------
    number = [ minus ] int [ frac ] [ exp ]
    decimal-point = %x2E       ; .
	digit1-9 = %x31-39         ; 1-9
	e = %x65 / %x45            ; e E
	exp = e [ minus / plus ] 1*DIGIT
	minus = %x2D               ; -
	plus = %x2B                ; +
	zero = %x30                ; 0
	frac = decimal-point 1*DIGIT
	int = zero / ( digit1-9 *DIGIT )

	Strings.
	------------------------------------------------------------------
	string = quotation-mark *char quotation-mark

	char = unescaped /
		escape (
			%x22 /          ; "    quotation mark  U+0022
			%x5C /          ; \    reverse solidus U+005C
			%x2F /          ; /    solidus         U+002F
			%x62 /          ; b    backspace       U+0008
			%x66 /          ; f    form feed       U+000C
			%x6E /          ; n    line feed       U+000A
			%x72 /          ; r    carriage return U+000D
			%x74 /          ; t    tab             U+0009
			%x75 4HEXDIG )  ; uXXXX                U+XXXX

	escape = %x5C              ; \

	quotation-mark = %x22      ; "

	unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
  */

#define FSM_MATCH_CHAR(s)         FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)          FsmMatch(new FsmMatchStringStr(s))
#define FSM_MATCH_RPT_CHAR(s,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_OPT_CHAR(s)     FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),0,1))

static String _ALPHA(_U("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
static String _HEXDIGIT(_U("0123456789ABCDEFabcdef")); /* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static String _DIGIT(_U("0123456789"));
static String _DEC_POINT(_U("."));
static String _DIGIT1_9(_U("123456789"));
static String _E(_U("eE"));
static String _MINUS(_U("-"));
static String _PLUS(_U("+"));
static String _MINUSPLUS(_U("-+"));
static String _ZERO(_U("0"));
static String _WS(_U(" \t\n\r"));
static String _ESC(_U("\\"));
static String _DQUOTE(_U("\""));


/* exp = e [ minus / plus ] 1*DIGIT */
static FsmTransition exp_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_E)               , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_OPT_CHAR(_MINUSPLUS)   , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_DIGIT, 1,-1) , FSM_ACCEPT, NULL, NULL }
};

/* frac = decimal-point 1*DIGIT */
static FsmTransition frac_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_DEC_POINT)       , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_DIGIT, 1,-1) , FSM_ACCEPT, NULL, NULL }
};

/* decimal_number_fsm = digit1-9 *DIGIT */
static FsmTransition decimal_num_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_DIGIT1_9)        , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_DIGIT,-1,-1) , FSM_ACCEPT, NULL, NULL }
};

/* int = zero / ( digit1-9 *DIGIT ) */
static FsmTransition int_fsm[] = {
      {-1, 1, FSM_MATCH_CHAR(_ZERO)          , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(decimal_num_fsm) , FSM_ACCEPT, NULL, NULL }
};

/* number = [ minus ] int [ frac ] [ exp ] */
static FsmTransition number_fsm[] = {
      { 1, 1, FSM_MATCH_OPT_CHAR(_MINUS) , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_FSM(int_fsm)     , FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_OPT_FSM(frac_fsm), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_OPT_FSM(exp_fsm) , FSM_ACCEPT, NULL, NULL }
};

/* begin-array     = ws %x5B ws  ; [ left square bracket */
static FsmTransition begin_array_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_U("["))       , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_ACCEPT, NULL, NULL }
};

/* end-array       = ws %x5D ws  ; ] right square bracket */
static FsmTransition end_array_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_U("]"))       , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_ACCEPT, NULL, NULL }
};

/* begin-object    = ws %x7B ws  ; { left curly bracket */
static FsmTransition begin_object_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_U("{"))       , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_ACCEPT, NULL, NULL }
};

/* end-object      = ws %x7D ws  ; } right curly bracket */
static FsmTransition end_object_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_U("}"))       , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_ACCEPT, NULL, NULL }
};

/* name-separator  = ws %x3A ws  ; : colon */
static FsmTransition name_separator_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_U(":"))       , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_ACCEPT, NULL, NULL }
};

/* value-separator = ws %x2C ws  ; , comma */
static FsmTransition value_separator_fsm[] = {
	  { 1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_U(","))       , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_WS, 0,-1) , FSM_ACCEPT, NULL, NULL }
};


/* === Strings === */

/* unescaped = %x20-21 / %x23-5B / %x5D-10FFFF */
Char unescaped_char[] = { 0x20, 0x21
	, 0x23, 0x5B
	, 0x5D, 0x10FFFF
};
static FsmTransition unescaped_char_fsm[] = {
      {-1, 1, FSM_MATCH_RANGE(&unescaped_char[0], &unescaped_char[1]) , FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_RANGE(&unescaped_char[2], &unescaped_char[3]) , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_RANGE(&unescaped_char[4], &unescaped_char[5]) , FSM_ACCEPT, NULL, NULL }
};

/* %x75 4HEXDIG ; uXXXX */
static FsmTransition unicode_char_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("uU"))            , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_CHAR(_HEXDIGIT, 4, 4) , FSM_ACCEPT, NULL, NULL }
};


/*
	%x22 /          ; "    quotation mark  U+0022
	%x5C /          ; \    reverse solidus U+005C
	%x2F /          ; /    solidus         U+002F
	%x62 /          ; b    backspace       U+0008
	%x66 /          ; f    form feed       U+000C
	%x6E /          ; n    line feed       U+000A
	%x72 /          ; r    carriage return U+000D
	%x74 /          ; t    tab             U+0009
	%x75 4HEXDIG    ; uXXXX                U+XXXX
*/
static FsmTransition escaped_char_fsm[] = {
	  {-1, 1, FSM_MATCH_CHAR(_U("\"\\/bfnrt")) , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(unicode_char_fsm)  , FSM_ACCEPT, NULL, NULL }
};

/* escape escaped_char */
static FsmTransition escaped_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_ESC)            , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(escaped_char_fsm) , FSM_ACCEPT, NULL, NULL }
};

/*
char = unescaped /
	escape (
		%x22 /          ; "    quotation mark  U+0022
		%x5C /          ; \    reverse solidus U+005C
		%x2F /          ; /    solidus         U+002F
		%x62 /          ; b    backspace       U+0008
		%x66 /          ; f    form feed       U+000C
		%x6E /          ; n    line feed       U+000A
		%x72 /          ; r    carriage return U+000D
		%x74 /          ; t    tab             U+0009
		%x75 4HEXDIG )  ; uXXXX                U+XXXX
*/
static FsmTransition char_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(unescaped_char_fsm) , FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(escaped_fsm)        , FSM_ACCEPT, NULL, NULL }
};

/* string = quotation-mark *char quotation-mark */
static FsmTransition string_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_DQUOTE)           , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT_FSM(char_fsm, 0,-1) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_CHAR(_DQUOTE)           , FSM_ACCEPT, NULL, NULL }
};


extern FsmTransition value_fsm[];

/* === Objects ===*/

/* member = string name-separator value */
static FsmTransition member_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(string_fsm)         , FSM_NORMAL, __begin_member, NULL }
	, { 2,-1, FSM_MATCH_FSM(name_separator_fsm) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(value_fsm)          , FSM_ACCEPT, __end_member, NULL }
};

/* next-member = value-separator member */
static FsmTransition next_member_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(value_separator_fsm), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(member_fsm)         , FSM_ACCEPT, NULL, NULL }
};

/* object-body = member *next-member */
static FsmTransition object_body_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(member_fsm)                , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(next_member_fsm, 0,-1) , FSM_ACCEPT, NULL, NULL }
};

/* object = begin-object [ member *( value-separator member ) ] end-object */
/* object = begin-object [ object-body ] end-object */
static FsmTransition object_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(begin_object_fsm)    , FSM_NORMAL, __begin_object, NULL }
	, { 2,-1, FSM_MATCH_OPT_FSM(object_body_fsm) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(end_object_fsm)      , FSM_ACCEPT, __end_object, NULL }
};

/* === Arrays === */
/* next-value = value-separator value  */
static FsmTransition next_value_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(value_separator_fsm), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(value_fsm)          , FSM_ACCEPT, NULL, NULL }
};

/* array-body = value *next-value */
static FsmTransition array_body_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(value_fsm)                , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(next_value_fsm, 0,-1) , FSM_ACCEPT, NULL, NULL }
};

/* array = begin-array [ value *( value-separator value ) ] end-array */
/* array = begin-array [ array-body ] end-array */
static FsmTransition array_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(begin_array_fsm)    , FSM_NORMAL, __begin_array, NULL }
	, { 2,-1, FSM_MATCH_OPT_FSM(array_body_fsm) , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(end_array_fsm)      , FSM_ACCEPT, __end_array, NULL }
};


/* === Values === */
static String _FALSE(_U("false"));
static String _NULL(_U("null"));
static String _TRUE(_U("true"));

/* value = false / null / true / object / array / number / string */
FsmTransition value_fsm[] = {
	  {-1, 1, FSM_MATCH_STR(_FALSE)     , FSM_ACCEPT, __scalar_value, (void*)&_JSON_VALUE_FALSE }
	, {-1, 2, FSM_MATCH_STR(_NULL)      , FSM_ACCEPT, __scalar_value, (void*)&_JSON_VALUE_NULL }
	, {-1, 3, FSM_MATCH_STR(_TRUE)      , FSM_ACCEPT, __scalar_value, (void*)&_JSON_VALUE_TRUE }
	, {-1, 4, FSM_MATCH_FSM(number_fsm) , FSM_ACCEPT, __scalar_value, (void*)&_JSON_VALUE_NUMBER }
	, {-1, 5, FSM_MATCH_FSM(string_fsm) , FSM_ACCEPT, __scalar_value, (void*)&_JSON_VALUE_STRING }
	, {-1, 6, FSM_MATCH_FSM(object_fsm) , FSM_ACCEPT, NULL, NULL }//value, (void*)&_JSON_VALUE_OBJECT }
	, {-1,-1, FSM_MATCH_FSM(array_fsm)  , FSM_ACCEPT, NULL, NULL }//value, (void*)&_JSON_VALUE_ARRAY }
};

/* JSON-text = object / array */
const bool _STATUS_OK = true;
const bool _STATUS_FAIL = false;
static FsmTransition json_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING         , FSM_NORMAL, __begin_json, NULL }
	, { 3, 2, FSM_MATCH_FSM(object_fsm) , FSM_NORMAL, NULL, NULL }
	, { 3, 4, FSM_MATCH_FSM(array_fsm)  , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING         , FSM_ACCEPT, __end_json, (void*)&_STATUS_OK }
	, {-1,-1, FSM_MATCH_NOTHING         , FSM_REJECT, __end_json, (void*)&_STATUS_FAIL }
};


inline void unescape_chars(String &s)
{
	s.replace(_U("\\\""), _U("\""));
	// TODO unescape hexdigits in form: \uXXXX
}

static bool __begin_member(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	if (!context)
		return true;

	JsonParseContext *ctx = _CAST_CTX(context);

	const Char *str = (const Char*)data;
	CWT_ASSERT(len >= 2);
	CWT_ASSERT(str[0] == _DQUOTE[0]);
	CWT_ASSERT(str[len-1] == _DQUOTE[0]);

	ctx->memberName = String(str + 1, len - 2);
	unescape_chars(ctx->memberName);
	return true;
}

static bool __end_member(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	if (!context)
		return true;

	JsonParseContext *ctx = _CAST_CTX(context);
	ctx->memberName.clear();
	return true;
}

static bool __begin_json(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	if (!context)
		return true;

	JsonParseContext *ctx = _CAST_CTX(context);
	return ctx->onBeginJson(ctx->userContext);
}

static bool __end_json(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED2(data, len);
	if (!context)
		return true;

	const bool *pstatus = (const bool*)action_args;
	JsonParseContext *ctx = _CAST_CTX(context);
	if (!*pstatus) {
		ctx->status->addError(_Tr("bad or unexpected end of json"));
	}
	return ctx->onEndJson(ctx->userContext, *pstatus);
}

static bool __begin_object(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	if (!context)
		return true;

	JsonParseContext *ctx = _CAST_CTX(context);
	JsonValue *object = JsonValue::createObject();
	JsonNamedValue namedValue(ctx->memberName, object);
	ctx->objects.push(namedValue);
	ctx->memberName.clear();

	return ctx->onBeginObject(ctx->userContext, namedValue.name, namedValue.value);
}

static bool __end_object(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	if (!context)
		return true;

	JsonParseContext *ctx = _CAST_CTX(context);
	CWT_ASSERT(ctx->objects.size() > 0);
	JsonNamedValue namedValue(ctx->objects.pop());

	return ctx->onEndObject(ctx->userContext, namedValue.name, namedValue.value)
			&& ctx->onValue(ctx->userContext, namedValue.name, namedValue.value);
}


static bool __begin_array(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	if (!context)
		return true;

	JsonParseContext *ctx = _CAST_CTX(context);
	JsonValue *array = JsonValue::createArray();
	JsonNamedValue namedValue(ctx->memberName, array);
	ctx->arrays.push(namedValue);
	ctx->memberName.clear();

	return ctx->onBeginArray(ctx->userContext, namedValue.name, namedValue.value);
}

static bool __end_array(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	if (!context)
		return true;

	JsonParseContext *ctx = _CAST_CTX(context);
	CWT_ASSERT(ctx->arrays.size() > 0);
	JsonNamedValue namedValue(ctx->arrays.pop());

	return ctx->onEndArray(ctx->userContext, namedValue.name, namedValue.value)
			&& ctx->onValue(ctx->userContext, namedValue.name, namedValue.value);
}

static bool __scalar_value(const void *data, size_t len, void *context, void *action_args)
{
	CWT_ASSERT(action_args);
	if (!context)
		return true;

	JsonParseContext *ctx = _CAST_CTX(context);
	JsonValue *value;
	String name(ctx->memberName);
	const int *pflag = (const int*)action_args;

	CWT_ASSERT(*pflag == _JSON_VALUE_NULL
			|| *pflag == _JSON_VALUE_FALSE
			|| *pflag == _JSON_VALUE_TRUE
			|| *pflag == _JSON_VALUE_NUMBER
			|| *pflag == _JSON_VALUE_STRING);

	switch (*pflag) {
	case _JSON_VALUE_NULL:
		value = new JsonValue();
		break;
	case _JSON_VALUE_FALSE:
		value = new JsonValue(false);
		break;
	case _JSON_VALUE_TRUE:
		value = new JsonValue(true);
		break;
	case _JSON_VALUE_NUMBER: {
		String number((const Char*)data, len);
		bool ok;

		double d = number.toDouble(&ok);
		if (!ok) {
			ctx->status->addError(_Tr("invalid number near %ls"), String((const Char*)data, CWT_MIN(10,len)).unicode());
			return false;
		}
		value = new JsonValue(d);
	}
		break;
	case _JSON_VALUE_STRING: {
		const Char *str = (const Char*)data;
		CWT_ASSERT(len >= 2);
		CWT_ASSERT(str[0] == _DQUOTE[0]);
		CWT_ASSERT(str[len-1] == _DQUOTE[0]);
		String value_str(str + 1, len - 2);
		unescape_chars(value_str);
		value = new JsonValue(value_str);
		break;
	}
	default:
		break;
	}

	ctx->memberName.clear();
	return ctx->onValue(ctx->userContext, name, value);
}

CWT_NS_END

#endif /* __CWT_JSON_RFC4627_HPP__ */
