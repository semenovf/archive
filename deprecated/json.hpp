#ifndef __CWT_JSON_HPP__
#define __CWT_JSON_HPP__
 
#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/errorable.hpp>
#include <cwt/hash.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

class JsonValue
{
public:
	enum TypeEnum {
		  JsonValue_Null
		, JsonValue_Boolean
		, JsonValue_Number
		, JsonValue_String
		, JsonValue_Object
		, JsonValue_Array
	};

	static JsonValue sharedNull;

protected:
	JsonValue() : m_parent(NULL) {}

public:
	JsonValue(JsonValue *parent) : m_parent(parent) {}
	virtual ~JsonValue() {}

	virtual TypeEnum type() const { return JsonValue_Null; }
	//virtual String toString() const { return _U("null"); }
	bool isNull() const { return type() == JsonValue_Null ? true : false; }

	JsonValue& parent() { return *m_parent; }
	const JsonValue& parent() const { return *m_parent; }

	virtual JsonValue& at(size_t i) { CWT_UNUSED(i); return sharedNull; }
	virtual JsonValue& at(const String &key) { CWT_UNUSED(key); return sharedNull; }
	virtual const JsonValue& at(size_t i) const { CWT_UNUSED(i); return sharedNull; }
	virtual const JsonValue& at(const String &key) const { CWT_UNUSED(key); return sharedNull; }

	virtual JsonValue& operator[](size_t i) { CWT_UNUSED(i); return sharedNull; }
	virtual JsonValue& operator[](const String &key) { CWT_UNUSED(key); return sharedNull; }
	virtual const JsonValue& operator[](size_t i) const { CWT_UNUSED(i); return sharedNull; }
	virtual const JsonValue& operator[](const String &key) const { CWT_UNUSED(key); return sharedNull; }

	virtual bool boolean() const { return false; }
	virtual double number() const { return double(0); }
	virtual String string() const { return String(); }
protected:
	JsonValue *m_parent;
};

class JsonBoolean : public JsonValue
{
private:
	JsonBoolean() : m_value(false) {}
public:
	JsonBoolean(JsonValue *parent, bool v) : JsonValue(parent), m_value(v) {}
	virtual TypeEnum type() const { return JsonValue_Boolean; }
	//virtual String toString() const { return m_value ? _U("true") : _U("false"); }

	virtual bool boolean() const { return m_value; }
	virtual double number() const { return m_value ? double(1) : double(0); }
	virtual String string() const { return m_value ? _U("true") : _U("false"); }

private:
	bool m_value;
};

class JsonNumber : public JsonValue
{
private:
	JsonNumber() : m_value(0.0) {}
public:
	JsonNumber(JsonValue *parent, double v) : JsonValue(parent), m_value(v) {}
	virtual TypeEnum type() const { return JsonValue_Number; }
	//virtual String toString() const { return String().number(m_value); }

	virtual bool boolean() const { return m_value == double(0) ? false : true; }
	virtual double number() const { return m_value; }
	virtual String string() const { String s; s.setNumber(m_value); return s; }
/*
	double value() const { return m_value; }
	virtual double number() const { return value(); }
*/
private:
	double m_value;
};

class JsonString : public JsonValue
{
private:
	JsonString() : m_value() {}
public:
	JsonString(JsonValue *parent, const String &s) : JsonValue(parent), m_value(s) {}
	JsonString(JsonValue *parent, const Char *s, int size = -1) : JsonValue(parent), m_value(s, size) {}
	virtual TypeEnum type() const { return JsonValue_String; }
	//virtual String toString() const { return m_value; }

	virtual bool boolean() const { return m_value.isEmpty() ? false : true; }
	virtual double number() const { return m_value.toDouble(); }
	virtual String string() const { return m_value; }

/*
	String value() const { return m_value; }
	virtual String string() const { return value(); }
*/
private:
	String m_value;
};


class JsonContainer : public JsonValue
{
public:
	JsonContainer(JsonValue *parent) : JsonValue(parent) {}
	virtual ~JsonContainer() {}
	virtual void add(const String &key, JsonValue *value) = 0;
};

class JsonArray : public JsonContainer
{
public:
	JsonArray(JsonValue *parent) : JsonContainer(parent), m_siblings() {}
	virtual ~JsonArray();
	virtual TypeEnum type() const { return JsonValue_Array; }
	virtual void add(const String &key, JsonValue *value) { CWT_UNUSED(key); append(value); }

	virtual JsonValue& at(size_t i) {
		if (i >= m_siblings.size())
			return JsonValue::sharedNull;
		return *m_siblings.at(i);
	}
	virtual const JsonValue& at(size_t i) const {
		if (i >= m_siblings.size())
			return JsonValue::sharedNull;
		return *m_siblings.at(i);
	}
	virtual JsonValue& operator[](size_t i) { return at(i); }
	virtual const JsonValue& operator[](size_t i) const { return at(i); }

	void append(JsonValue *v) { m_siblings.append(v); }
	int count() const         { return size(); }
	bool isEmpty() const      { return m_siblings.isEmpty(); }
	int size() const          { return m_siblings.size(); }
	//virtual String toString() const { return _U("Array"); }
private:
	Vector<JsonValue*> m_siblings;
};

class JsonObject : public JsonContainer
{
public:
	JsonObject(JsonValue *parent) : JsonContainer(parent), m_siblings() {}
	virtual ~JsonObject();
	virtual TypeEnum type() const { return JsonValue_Object; }
	virtual void add(const String &key, JsonValue *value) { insert(key, value); }

	virtual JsonValue& at(const String &key) {
		Hash<String, JsonValue*>::iterator it = m_siblings.find(key);
		if (it == m_siblings.end())
			return JsonValue::sharedNull;
		return *it.value();
	}
	virtual const JsonValue& at(const String &key) const {
		Hash<String, JsonValue*>::const_iterator it = m_siblings.find(key);
		if (it == m_siblings.end())
			return JsonValue::sharedNull;
		return *it.value();
	}
	virtual JsonValue& operator[](const String &key) { return at(key); }
	virtual const JsonValue& operator[](const String &key) const { return at(key); }

	int count() const { return size(); }
	void insert(const String &key, JsonValue *value) { m_siblings.insert(key, value); }
	bool isEmpty() const { return m_siblings.isEmpty(); }
	int size() const { return m_siblings.size(); }
	//virtual String toString() const { return _U("Object"); }
private:
	Hash<String, JsonValue*> m_siblings;
};


/* This struct provides the Simple API for JSON */
struct JsonSAJ {
	bool (*onBeginJson)(void *userContext);
	bool (*onEndJson)(void *userContext, bool status);
	bool (*onBeginObject)(void *userContext, const String &name, JsonObject *object);
	bool (*onEndObject)(void *userContext, const String &name, JsonObject *object);
	bool (*onBeginArray)(void *userContext, const String &name, JsonArray *object);
	bool (*onEndArray)(void *userContext, const String &name, JsonArray *object);
	bool (*onValue)(void *userContext, const String &name, JsonValue *value);
};


class DLL_API Json : public Errorable {
	CWT_DENY_COPY(Json);
public:
	Json() : Errorable(), m_root(NULL) {}
	Json(const String &json) : Errorable(), m_root(NULL) { parse(json); }
	~Json() { if (m_root) delete m_root; }

	bool parse(const String &json);
	bool parseFromFile(const char *path);
	bool parse(const String &json, const JsonSAJ *saj, void *userContext);

	bool isArray() const { return m_root ? m_root->type() == JsonValue::JsonValue_Array ? true : false : false; }
	bool isEmpty() const;
	bool isObject() const { return m_root ? m_root->type() == JsonValue::JsonValue_Object ? true : false : false; }
	bool isNull() const { return m_root ? false : true; }

	virtual JsonValue& at(size_t i) { return m_root ? m_root->at(i) : JsonValue::sharedNull; }
	virtual JsonValue& at(const String &key) { return m_root ? m_root->at(key) : JsonValue::sharedNull; }
	virtual const JsonValue& at(size_t i) const { return m_root ? m_root->at(i) : JsonValue::sharedNull; }
	virtual const JsonValue& at(const String &key) const { return m_root ? m_root->at(key) : JsonValue::sharedNull; }

	virtual JsonValue& operator[](size_t i) { return at(i); }
	virtual JsonValue& operator[](const String &key) { return at(key); }
	virtual const JsonValue& operator[](size_t i) const { return at(i); }
	virtual const JsonValue& operator[](const String &key) const { return at(key); }

	JsonObject& object() { CWT_ASSERT(m_root && m_root->type() == JsonValue::JsonValue_Object); return *dynamic_cast<JsonObject*>(m_root); }
	JsonArray& array() { CWT_ASSERT(m_root && m_root->type() == JsonValue::JsonValue_Array); return *dynamic_cast<JsonArray*>(m_root); }
	const JsonObject& object() const { CWT_ASSERT(m_root && m_root->type() == JsonValue::JsonValue_Object); return *dynamic_cast<JsonObject*>(m_root); }
	const JsonArray& array() const { CWT_ASSERT(m_root && m_root->type() == JsonValue::JsonValue_Array); return *dynamic_cast<JsonArray*>(m_root); }

	JsonValue& value() { return *m_root; }
	const JsonValue& value() const { return *m_root; }

private:
	JsonValue *m_root;
};


/*
 * Syntax (according to suggestions from http://goessner.net/articles/JsonPath/):
 * /            root object/element
 * /            child operator
 * .            current object/element
 * ..           parent operator
 * []           subscript operator
 */
class DLL_API JsonSimplePath {
public:
	JsonSimplePath(Json &json) : m_json(&json), m_jsonRootValue(&json.value()) { ; }

	Json& json() { return *m_json; }
	const Json& json() const { return *m_json; }
	void setJson(Json &json) { m_json = &json; m_jsonRootValue = &json.value(); }

	bool changeRoot(const String &jpath);
	JsonValue& find (const String &jpath) { return const_cast<JsonValue&>(findValue(jpath)); }
	const JsonValue& find (const String &jpath) const { return findValue(jpath); }
	bool contains(const String &jpath) const;

	JsonValue& operator [] (const String &jpath) { return const_cast<JsonValue&>(findValue(jpath)); }
	const JsonValue& operator [] (const String &jpath) const { return findValue(jpath); }

protected:
	const JsonValue& findValue (const String &jpath) const;

private:
	Json *m_json;
	const JsonValue *m_jsonRootValue;
};

CWT_NS_END

#endif /* __CWT_JSON_HPP__ */
