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

class JsonSimplePathContext;

static bool __end_elem__for_find_value(JsonSimplePathContext *ctx);
static bool __end_elem__for_make_tree(JsonSimplePathContext *ctx);

class JsonSimplePathContext {
public:
	JsonSimplePathContext(Json* json, JsonValue* initial, bool isMakeTree = false)
		: m_json(json)
		, m_jsonTmpValue(initial)
	{
		end_elem_helper = isMakeTree
				? &__end_elem__for_make_tree
				: &__end_elem__for_find_value;
	}

	bool (*end_elem_helper)(JsonSimplePathContext *ctx);

public:
	Json      *m_json;
	JsonValue *m_jsonTmpValue;
	String     m_key;
	String     m_subscript;
};

#define FSM_MATCH_CHAR(s)            FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)             FsmMatch(new FsmMatchStringStr(s))
#define FSM_MATCH_RPT_CHAR(s,f,t)    FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_RPT_FUNC(func,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_FUNC(func, nullptr),f,t))
#define FSM_MATCH_OPT_CHAR(s)        FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),0,1))

static ssize_t __is_key_char(FsmContext *fsm, void *fn_context, const void *data, size_t len);

static String _JSONP_ROOT_OP(_U("/"));
static String _JSONP_CHILD_OP(_U("/"));
static String _JSONP_PARENT(_U(".."));
static String _JSONP_CURRENT(_U("."));
static String _JSONP_SUBSCRIPT_OPEN(_U("["));
static String _JSONP_SUBSCRIPT_CLOSE(_U("]"));
static String _JSONP_DIGIT(_U("0123456789"));

static bool __is_root(const void *, size_t, void *, void *);
static bool __begin_elem(const void *, size_t, void *, void *);
static bool __end_elem(const void *, size_t, void *, void *);
static bool __key(const void *, size_t, void *, void *);
static bool __subscript(const void *, size_t, void *, void *);

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
      {-1, 1, FSM_MATCH_RPT_FUNC(__is_key_char,1,-1), FSM_ACCEPT, nullptr, nullptr }
    , {-1, 2, FSM_MATCH_STR(_JSONP_PARENT)               , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_STR(_JSONP_CURRENT)              , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition subscript_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_JSONP_SUBSCRIPT_OPEN) , FSM_NORMAL, nullptr, nullptr }
    , { 2,-1, FSM_MATCH_CHAR(_JSONP_DIGIT)          , FSM_NORMAL, nullptr, nullptr }
    , { 3,-1, FSM_MATCH_RPT_CHAR(_JSONP_DIGIT,0,-1) , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_JSONP_SUBSCRIPT_CLOSE), FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition key_subscript_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(key_fsm)       , FSM_NORMAL, __key, nullptr }
    , {-1,-1, FSM_MATCH_FSM(subscript_fsm) , FSM_ACCEPT, __subscript, nullptr }
};

static FsmTransition elem_fsm[] = {
	  { 1,-1, FSM_MATCH_NOTHING                , FSM_NORMAL, __begin_elem, nullptr }
    , { 4, 2, FSM_MATCH_FSM(key_subscript_fsm) , FSM_ACCEPT, nullptr, nullptr }
    , { 4, 3, FSM_MATCH_FSM(subscript_fsm)     , FSM_ACCEPT, __subscript, nullptr }
    , { 4,-1, FSM_MATCH_FSM(key_fsm)           , FSM_ACCEPT, __key, nullptr }
	, {-1,-1, FSM_MATCH_NOTHING                , FSM_ACCEPT, __end_elem, nullptr }
};

static FsmTransition next_elem_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_JSONP_CHILD_OP) , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_FSM(elem_fsm)   , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition elems_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(elem_fsm)               , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_RPT_FSM(next_elem_fsm,0,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition jpath_fsm[] = {
      { 1,-1, FSM_MATCH_OPT_CHAR(_JSONP_ROOT_OP) , FSM_NORMAL, __is_root, nullptr }
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

ssize_t __is_key_char(FsmContext *fsm, void *fn_context, const void *data, size_t len)
{
	CWT_UNUSED2(fsm, fn_context);
	if (len > 0) {
		Char ch = *reinterpret_cast<const Char*>(data);
		if ( !(ch == '/' || ch == '[' || ch == ']') )
			return ssize_t(1);
	}
	return ssize_t(-1);
}

#ifdef __CWT_TEST__
bool __is_root(const void *, size_t , void *, void *)
{
	return true;
}
#else
bool __is_root(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED2(data, action_args);

	if (len > 0) {
		JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
		ctx->m_jsonTmpValue = &ctx->m_json->value();
	}
	return true;
}
#endif

#ifdef __CWT_TEST__
bool __key(const void *, size_t , void *, void *)
{
	return true;
}
#else
bool __key(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	ctx->m_key = String(reinterpret_cast<const Char*>(data), len);
	return true;
}
#endif

#ifdef __CWT_TEST__
bool __subscript(const void *, size_t, void *, void *)
{
	return true;
}
#else
bool __subscript(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(action_args);
	CWT_ASSERT(len > 2);
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	ctx->m_subscript = String(reinterpret_cast<const Char*>(data) + 1, len - 2);
	return true;
}
#endif

#ifdef __CWT_TEST__
bool __begin_elem(const void *, size_t, void *, void *)
{
	return true;
}
#else
bool __begin_elem(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	ctx->m_key.clear();
	ctx->m_subscript.clear();
	return true;
}
#endif

bool __end_elem__for_find_value(JsonSimplePathContext *ctx)
{
	if (!ctx->m_key.isEmpty()) {
		if (ctx->m_key == _U(".")) { // current
			; // nothing to do
		} else if (ctx->m_key == _U("..")) { // parent
			ctx->m_jsonTmpValue = &ctx->m_jsonTmpValue->parent();
			if (ctx->m_jsonTmpValue == nullptr) { // top element
				ctx->m_jsonTmpValue = &ctx->m_json->value();
			}
		} else {
			ctx->m_jsonTmpValue = &ctx->m_jsonTmpValue->at(ctx->m_key);
		}
	}

	if (!ctx->m_subscript.isEmpty()) {
		bool ok;
		size_t index = ctx->m_subscript.toUInt(&ok);

		if (ok) {
			ctx->m_jsonTmpValue = &ctx->m_jsonTmpValue->at(index);
		} else {
			ctx->m_jsonTmpValue = &JsonValue::sharedNull;
			ctx->m_json->addError(_Tr("bad subscript value"));
		}
	}

	if (ctx->m_jsonTmpValue == &JsonValue::sharedNull) {
		return false;
	}
	return true;
}

bool __end_elem__for_make_tree(JsonSimplePathContext *ctx)
{
	JsonValue *parent = ctx->m_jsonTmpValue;

	CWT_ASSERT(parent);

	if (!ctx->m_key.isEmpty()) {

		if (!parent->isObject()) {
			shared_ptr<JsonValue> o(JsonValue::createObject());
			parent->setValue(*o);
		}

		if (parent->at(ctx->m_key).isInvalid()) {
			parent->insert(ctx->m_key, new JsonValue());
		}

		parent = & parent->at(ctx->m_key);
		CWT_ASSERT(!parent->isInvalid());
	}

	if (!ctx->m_subscript.isEmpty()) {
		bool ok;
		size_t index = ctx->m_subscript.toUInt(&ok);
		if (!ok) {
			ctx->m_json->addError(_Tr("bad subscript value"));
			return false;
		}

		if (!parent->isArray()) {
			shared_ptr<JsonValue> a(JsonValue::createArray());
			parent->setValue(*a);
		}

		if (parent->count() <= index) {
			for (size_t i = parent->count(); i <= index; ++i) {
				parent->append(new JsonValue());
			}
		}

		parent = & parent->at(index);
		CWT_ASSERT(!parent->isInvalid());
	}

	ctx->m_jsonTmpValue = parent;
	return true;
}


#ifdef __CWT_TEST__
bool __end_elem(const void *, size_t, void *, void *)
{
	return true;
}
#else
bool __end_elem(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED3(data, len, action_args);
	JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(context);
	return ctx->end_elem_helper(ctx);
}
#endif

CWT_NS_END

#endif // __CWT_JSON_SIMPLEPATH_FSM_HPP__
