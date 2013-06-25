/**
 * @file   json_p.hpp
 * @author wladt
 * @date   Feb 6, 2013 10:52:01 AM
 *
 * @brief For internal use
 */

#ifndef __CWT_JSON_P_HPP__
#define __CWT_JSON_P_HPP__

#include <cwt/stack.hpp>
#include "../include/cwt/json.hpp"

CWT_NS_BEGIN

#define _CAST_CTX(ctx) reinterpret_cast<JsonParseContext*>(ctx)

template <typename cb = bool (*)(void*)>
struct JsonMethod
{
	JsonMethod(cb func = NULL) : callback(func ? func : &default_callback) {}
	bool operator ()(void *userContext) { return callback(userContext); }
	cb callback;
	static bool default_callback(void*) {return true;}
};


template <typename arg1, typename cb = bool (*)(void*, arg1)>
struct JsonMethod1
{
	JsonMethod1(cb func = NULL) : callback(func ? func : &default_callback) {}
	bool operator ()(void *userContext, arg1 a1) { return callback(userContext, a1); }
	cb callback;
	static bool default_callback(void*, arg1) {return true;}
};

template <typename arg1, typename arg2, typename cb = bool (*)(void*, arg1, arg2)>
struct JsonMethod2
{
	JsonMethod2(cb func = NULL) : callback(func ? func : &default_callback) {}
	bool operator ()(void *userContext, arg1 a1, arg2 a2) { return callback(userContext, a1, a2); }
	cb callback;
	static bool default_callback(void*, arg1, arg2) {return true;}
};

struct JsonNamedValue {
	String     name;
	JsonValue *value;

	JsonNamedValue() : name(), value(NULL) {}
	JsonNamedValue(const String n, JsonValue *v) : name(n), value(v) {}
	JsonNamedValue(const JsonNamedValue &other) : name(other.name), value(other.value) {}
	JsonNamedValue& operator =(const JsonNamedValue &other) { name = other.name; value = other.value; return *this; }
};

struct JsonDomContext {
	JsonValue        *rootContainer;
	Stack<JsonValue*> containers;
};

struct JsonParseContext {
	void          *userContext;
	Errorable     *status;
	String         memberName;
	Stack<JsonNamedValue> objects;
	Stack<JsonNamedValue> arrays;
	JsonMethod<>      onBeginJson;
	JsonMethod1<bool> onEndJson;
	JsonMethod2<const String&, JsonValue*> onBeginObject;
	JsonMethod2<const String&, JsonValue*> onEndObject;
	JsonMethod2<const String&, JsonValue*> onBeginArray;
	JsonMethod2<const String&, JsonValue*> onEndArray;
	JsonMethod2<const String&, JsonValue*> onValue;
};

CWT_NS_END

#endif /* __CWT_JSON_P_HPP__ */
