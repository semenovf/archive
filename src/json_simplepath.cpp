/*
 * jsonpath.cpp
 *
 *  Created on: Jun 5, 2013
 *      Author: wladt
 */

#include "../include/cwt/json.hpp"
#include "../include/cwt/fsm.hpp"

CWT_NS_BEGIN

class JsonSimplePathContext {
public:
	JsonSimplePathContext(const JsonSimplePath *jsonPath)
		: m_jsonPath(jsonPath), m_jsonValue(&JsonValue::sharedNull) {}
public:
	const JsonSimplePath *m_jsonPath;
	JsonValue      *m_jsonValue;
};

#define FSM_MATCH_CHAR(s)         FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)          FsmMatch(new FsmMatchStringStr(s))
#define FSM_MATCH_RPT_CHAR(s,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_RPT_FUNC(func,f,t) FsmMatch(new FsmMatchRpt(FSM_MATCH_FUNC(func, NULL),f,t))
#define FSM_MATCH_OPT_CHAR(s)     FsmMatch(new FsmMatchRpt(FSM_MATCH_CHAR(s),0,1))

static ssize_t is_key_char(FsmContext *fsm, void *fn_context, const void *data, size_t len);

static String _ROOT_OP(_U("/"));
static String _CHILD_OP(_U("/"));
static String _PARENT_OP(_U(".."));
static String _SUBSCRIPT_OPEN(_U("["));
static String _SUBSCRIPT_CLOSE(_U("]"));
static String _DIGIT1_9(_U("123456789"));
static String _DIGIT(_U("0123456789"));

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

static FsmTransition key_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FUNC(is_key_char,1,-1), FSM_ACCEPT, NULL, NULL }
};

static FsmTransition subscript_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_SUBSCRIPT_OPEN) , FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_CHAR(_DIGIT1_9)       , FSM_NORMAL, NULL, NULL }
    , { 3,-1, FSM_MATCH_RPT_CHAR(_DIGIT,0,-1) , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_SUBSCRIPT_CLOSE), FSM_ACCEPT, NULL, NULL }
};

static FsmTransition key_subscript_fsm[] = {
      { 1,-1, FSM_MATCH_FSM(key_fsm)       , FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(subscript_fsm) , FSM_ACCEPT, NULL, NULL }
};

static FsmTransition elem_fsm[] = {
      {-1, 1, FSM_MATCH_FSM(key_subscript_fsm) , FSM_ACCEPT, NULL, NULL }
    , {-1, 2, FSM_MATCH_FSM(subscript_fsm)     , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(key_fsm)           , FSM_ACCEPT, NULL, NULL }
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
      { 1,-1, FSM_MATCH_OPT_CHAR(_ROOT_OP), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(elems_fsm)    , FSM_ACCEPT, NULL, NULL }
};


ssize_t is_key_char(FsmContext *fsm, void *fn_context, const void *data, size_t len)
{
	CWT_UNUSED2(fsm, fn_context);
	//JsonSimplePathContext *ctx = reinterpret_cast<JsonSimplePathContext*>(fsm->context);
	if (len > 0) {
		Char ch = *reinterpret_cast<const Char*>(data);
		if ( !(ch == '/' || ch == '[' || ch == ']') )
			return ssize_t(1);
	}
	return ssize_t(-1);
}

JsonValue& JsonSimplePath::find(const String &jpath)
{
	JsonSimplePathContext ctx(this);
	Fsm<Char> fsm(jpath_fsm, &ctx);
	ssize_t result = fsm.exec(0, jpath.data(), jpath.length());

	if (result > 0 && size_t(result) == jpath.length()) {
		return *ctx.m_jsonValue;
	}

	return JsonValue::sharedNull;
}

const JsonValue& JsonSimplePath::find (const String &jpath) const
{
	return (const JsonValue&)find(jpath);
}


bool JsonSimplePath::contains(const String &jpath) const
{
	const JsonValue *p = & find(jpath);
	if (p == &JsonValue::sharedNull)
		return false;
	return true;
}

CWT_NS_END

