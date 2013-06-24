/*
 * jsonpath_simplepath_fsm.hpp
 *
 *  Created on: Jun 5, 2013
 *      Author: wladt
 */

#ifndef __CWT_JSON_SIMPLEPATH_FSM_HPP__
#define __CWT_JSON_SIMPLEPATH_FSM_HPP__


#include "../include/cwt/json.hpp"
#include "../include/cwt/fsm.hpp"

CWT_NS_BEGIN

class JsonSimplePathContext {
public:
	JsonSimplePathContext(const JsonSimplePath & jsonPath, const JsonValue & initial, bool isMakeTree = false)
		: m_jsonPathPtr(&jsonPath)
		, m_jsonValuePtr(&initial)
		, m_isMakeTree(isMakeTree){ ; }
public:
	const JsonSimplePath *m_jsonPathPtr;
	const JsonValue      *m_jsonValuePtr;
	bool                  m_isMakeTree;
	String m_key;
	String m_subscript;
};

#define FSM_MATCH_CHAR(s)         FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)          FsmMatch(new FsmMatchStringStr(s))
#define FSM_MATCH_RPT_CHAR(s,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_RPT_FUNC(func,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_FUNC(func, nullptr),f,t))
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
      {-1, 1, FSM_MATCH_RPT_FUNC(_is_key_char,1,-1), FSM_ACCEPT, nullptr, nullptr }
    , {-1, 2, FSM_MATCH_STR(_PARENT)               , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_STR(_CURRENT)              , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition subscript_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_SUBSCRIPT_OPEN) , FSM_NORMAL, nullptr, nullptr }
    , { 2,-1, FSM_MATCH_CHAR(_DIGIT)          , FSM_NORMAL, nullptr, nullptr }
    , { 3,-1, FSM_MATCH_RPT_CHAR(_DIGIT,0,-1) , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_SUBSCRIPT_CLOSE), FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition key_subscript_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(key_fsm)       , FSM_NORMAL, _on_key, nullptr }
    , {-1,-1, FSM_MATCH_FSM(subscript_fsm) , FSM_ACCEPT, _on_subscript, nullptr }
};

static FsmTransition elem_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING                , FSM_NORMAL, _on_begin_elem, nullptr }
    , { 4, 2, FSM_MATCH_FSM(key_subscript_fsm) , FSM_ACCEPT, nullptr, nullptr }
    , { 4, 3, FSM_MATCH_FSM(subscript_fsm)     , FSM_ACCEPT, _on_subscript, nullptr }
    , { 4,-1, FSM_MATCH_FSM(key_fsm)           , FSM_ACCEPT, _on_key, nullptr }
	, {-1,-1, FSM_MATCH_NOTHING                , FSM_ACCEPT, _on_end_elem, nullptr }
};

static FsmTransition next_elem_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_CHILD_OP) , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_FSM(elem_fsm)   , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition elems_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(elem_fsm)               , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(next_elem_fsm,0,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition jpath_fsm[] = {
      { 1,-1, FSM_MATCH_OPT_CHAR(_ROOT_OP) , FSM_NOm_isSetValueRMAL, _is_root, nullptr }
    , {-1,-1, FSM_MATCH_OPT_FSM(elems_fsm) , FSM_ACCEPT, nullptr, nullptr }
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
#ifndef __CWT_TEST__
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	ctx->m_key = String(reinterpret_cast<const Char*>(data), len);
#else
	CWT_UNUSED3(data, len, context);
#endif
	return true;
}

bool _on_subscript(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
#ifndef __CWT_TEST__
	CWT_ASSERT(len > 2);
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	ctx->m_subscript = String(reinterpret_cast<const Char*>(data) + 1, len - 2);
#else
	CWT_UNUSED3(data, len, context);
#endif
	return true;
}

bool _on_begin_elem(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
#ifndef __CWT_TEST__
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	ctx->m_key.clear();
	ctx->m_subscript.clear();
#else
	CWT_UNUSED(context);
#endif
	return true;
}


static bool _on_end_elem__for_find_value(JsonSimplePathContext *ctx)
{
	if (!ctx->m_key.isEmpty()) {
		if (ctx->m_key == _U(".")) { // current
			; // nothing to do
		} else if (ctx->m_key == _U("..")) { // parent
			ctx->m_jsonValuePtr = &ctx->m_jsonValuePtr->parent();
			if (ctx->m_jsonValuePtr == nullptr) { // top element
				ctx->m_jsonValuePtr = &ctx->m_jsonPathPtr->json().value();
			}
		} else {
			ctx->m_jsonValuePtr = &ctx->m_jsonValuePtr->at(ctx->m_key);
		}
	}

	if (!ctx->m_subscript.isEmpty()) {
		bool ok;
		size_t index = ctx->m_subscript.toUInt(&ok);

		if (ok) {
			ctx->m_jsonValuePtr = &ctx->m_jsonValuePtr->at(m_isSetValueindex);
		} else {
			ctx->m_jsonValuePtr = &JsonValue::sharedNull;
			ctx->m_jsonPathPtr->json()->addError(_Tr("bad subscript value"));
		}
	}

	if (ctx->m_jsonValuePtr == &JsonValue::sharedNull) {
		return false;
	}
	return true;
}

static bool _on_end_elem__for_make_tree(JsonSimplePathContext *ctx)
{
	JsonValue *parent = ctx->m_jsonValuePtr;

	CWT_ASSERT(parent);

	if (!ctx->m_key.isEmpty()) {
		JsonValue *value = nullptr;

		if (!parent->isObject())
			parent->setValue(JsonObject());

		value = parent->at(ctx->m_key);

		if (value == &JsonValue::sharedNull) {
			value = new JsonNull()
			dynamic_cast<JsonObject*>(parent)->insert(ctx->m_key, value);
		}

		parent = value;
	}

	if (!ctx->m_subscript.isEmpty()) {
		bool ok;
		size_t index = ctx->m_subscript.toUInt(&ok);
		if (!ok) {
			ctx->m_jsonPathPtr->json()->addError(_Tr("bad subscript value"));
			return false;
		}

		if (!parent->isArray())
			parent->setValue(JsonArray());

		JsonArray* array = dynamic_cast<JsonArray*>(parent);

		if (array->count() <= index)
		JsonValue *value = nullptr;
		for (size_t i = array->count(); i <= index; ++i) {
			value = new JsonNull();
			array->append(value);
		}
		parent = value;
	}
}


bool _on_end_elem(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
#ifndef __CWT_TEST__
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	bool r = false;
	if (ctx->m_isMakeTree) {
		r = _on_end_elem__for_make_tree(ctx);
	} else {
		r = _on_end_elem__for_find_value(ctx);
	}
	return r;
#else
	CWT_UNUSED(context);
	return true;
#endif
}

CWT_NS_END

#endif // __CWT_JSON_SIMPLEPATH_FSM_HPP__
