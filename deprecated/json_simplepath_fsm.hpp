/*
 * jsonpath_simplepath_fsm.hpp
 *
 *  Created on: Jun 5, 2013
 *      Author: wladt
 */

#include "../include/cwt/json.hpp"
#include "../include/cwt/fsm.hpp"

CWT_NS_BEGIN

class JsonSimplePathContext {
public:
	JsonSimplePathContext(const JsonSimplePath & jsonPath, const JsonValue & initial)
		: m_jsonPathPtr(&jsonPath)
		, m_jsonValuePtr(&initial) { ; }
public:
	const JsonSimplePath *m_jsonPathPtr;
	const JsonValue      *m_jsonValuePtr;

	String m_key;
	String m_subscript;
};

#define FSM_MATCH_CHAR(s)         FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)          FsmMatch(new FsmMatchStringStr(s))
#define FSM_MATCH_RPT_CHAR(s,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_RPT_FUNC(func,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_FUNC(func, NULL),f,t))
#define FSM_MATCH_OPT_CHAR(s)     FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),0,1))

static ssize_t _is_key_char(FsmContext *fsm, void *fn_context, const void *data, size_t len);

static String _ROOT_OP(_U("/"));
static String _CHILD_OP(_U("/"));
static String _PARENT(_U(".."));
static String _CURRENT(_U("."));
static String _SUBSCRIPT_OPEN(_U("["));
static String _SUBSCRIPT_CLOSE(_U("]"));
//static String _DIGIT1_9(_U("123456789"));
static String _DIGIT(_U("0123456789"));

static bool _is_root(const void *, size_t, void *, void *);
static bool _on_begin_elem(const void *, size_t, void *, void *);
static bool _on_end_elem(const void *, size_t, void *, void *);
static bool _on_key(const void *, size_t, void *, void *);
static bool _on_subscript(const void *, size_t, void *, void *);

//static bool _trace(const void *data, size_t len, void *context, void *action_args);

/* Examples
 *
 * jpath = [ '/' ] elems
 * elems = elem *( '/' elem )
 * elem  = ( key subscript ) | key | subscript
 * key   = <string excluding '[', ']' and '/'>
 * subscript = '[' DIGIT1_9 *DIGIT ']'
 *
 * /store/book[0]/title
 * /[1]/book[0]/title
 * [1]/[2]/[3]/title
 *
 */

//static String _TRACE(_U("TRACE"));

static FsmTransition key_fsm[] = {
      {-1, 1, FSM_MATCH_RPT_FUNC(_is_key_char,1,-1), FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_STR(_PARENT)               , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_STR(_CURRENT)              , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition subscript_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_SUBSCRIPT_OPEN) , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_DIGIT)          , FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_RPT_CHAR(_DIGIT,0,-1) , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_SUBSCRIPT_CLOSE), FSM_ACCEPT, NULL, NULL }
};

static FsmTransition key_subscript_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(key_fsm)       , FSM_NORMAL, _on_key, NULL }
    , {-1,-1, FSM_MATCH_FSM(subscript_fsm) , FSM_ACCEPT, _on_subscript, NULL }
};

static FsmTransition elem_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING                , FSM_NORMAL, _on_begin_elem, NULL }
    , { 4, 2, FSM_MATCH_FSM(key_subscript_fsm) , FSM_ACCEPT, NULL, NULL }
    , { 4, 3, FSM_MATCH_FSM(subscript_fsm)     , FSM_ACCEPT, _on_subscript, NULL }
    , { 4,-1, FSM_MATCH_FSM(key_fsm)           , FSM_ACCEPT, _on_key, NULL }
	, {-1,-1, FSM_MATCH_NOTHING                , FSM_ACCEPT, _on_end_elem, NULL }
};

static FsmTransition next_elem_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_CHILD_OP) , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(elem_fsm)   , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition elems_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(elem_fsm)               , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_RPT_FSM(next_elem_fsm,0,-1) , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition jpath_fsm[] = {
      { 1,-1, FSM_MATCH_OPT_CHAR(_ROOT_OP) , FSM_NORMAL, _is_root, NULL }
    , {-1,-1, FSM_MATCH_OPT_FSM(elems_fsm) , FSM_ACCEPT, NULL, NULL }
};

/*
bool _trace(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(context);
	if (action_args) {
		CWT_TRACE(String().sprintf("%s: [%s]"
				, reinterpret_cast<String*>(action_args)->toUtf8().data()
				, String((const Char*)data, len).toUtf8().data()).toUtf8().data());
	}
	return true;
}
*/

ssize_t _is_key_char(FsmContext *fsm, void *fn_context, const void *data, size_t len)
{
	CWT_UNUSED2(fsm, fn_context);
	if (len > 0) {
		Char ch = *reinterpret_cast<const Char*>(data);
		if ( !(ch == '/' || ch == '[' || ch == ']') )
			return ssize_t(1);
	}
	return ssize_t(-1);
}

bool _is_root(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED2(data, action_args);
#ifndef __CWT_TEST__
	if (len > 0) {
		JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
		ctx->m_jsonValuePtr = &ctx->m_jsonPathPtr->json().value();
	}
#else
	CWT_UNUSED2(len, context);
#endif
	return true;
}


bool _on_key(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	ctx->m_key = String(reinterpret_cast<const Char*>(data), len);
	return true;
}

bool _on_subscript(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	CWT_ASSERT(len > 2);
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	ctx->m_subscript = String(reinterpret_cast<const Char*>(data) + 1, len - 2);
	return true;
}

bool _on_begin_elem(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	ctx->m_key.clear();
	ctx->m_subscript.clear();
	return true;
}

bool _on_end_elem(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);

	if (!ctx->m_key.isEmpty()) {
		if (ctx->m_key == _U(".")) { // current
			; // nothing to do
		} else if (ctx->m_key == _U("..")) { // parent
			ctx->m_jsonValuePtr = &ctx->m_jsonValuePtr->parent();
			if (ctx->m_jsonValuePtr == NULL) { // top element
				ctx->m_jsonValuePtr = &ctx->m_jsonPathPtr->json().value();
			}
		} else {
			ctx->m_jsonValuePtr = &ctx->m_jsonValuePtr->at(ctx->m_key);
		}
	}

	if (!ctx->m_subscript.isEmpty()) {
		bool ok;
		size_t index = ctx->m_subscript.toUInt(&ok);

		if (ok)
			ctx->m_jsonValuePtr = &ctx->m_jsonValuePtr->at(index);
		else
			ctx->m_jsonValuePtr = &JsonValue::sharedNull;
	}

	if (ctx->m_jsonValuePtr == &JsonValue::sharedNull)
		return false;
	return true;

}

CWT_NS_END
