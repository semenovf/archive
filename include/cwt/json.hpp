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
	typedef enum {
		  JsonValue_Null
		, JsonValue_Boolean
		, JsonValue_Number
		, JsonValue_String
		, JsonValue_Object
		, JsonValue_Array
	} type_enum;

	static const JsonValue Null;

protected:
	JsonValue() : m_parent(NULL) {}

public:
	JsonValue(JsonValue *parent) : m_parent(parent) {}
	virtual ~JsonValue() {}

	virtual type_enum type() const { return JsonValue_Null; }
	virtual String toString() const { return _U("null"); }
	bool isNull() const { return type() == JsonValue_Null ? true : false; }

	virtual const JsonValue& at(size_t i) const { CWT_UNUSED(i); return Null; }
	virtual const JsonValue& operator[](size_t i) const { CWT_UNUSED(i); return Null; }

	virtual const JsonValue& at(const String &key) const { CWT_UNUSED(key); return Null; }
	virtual const JsonValue& operator[](const String &key) const { CWT_UNUSED(key); return Null; }

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
	virtual type_enum type() const { return JsonValue_Boolean; }
	virtual String toString() const { return m_value ? _U("true") : _U("false"); }

	bool value() const { return m_value; }
	virtual bool boolean() const { return value(); }
private:
	bool m_value;
};

class JsonNumber : public JsonValue
{
private:
	JsonNumber() : m_value(0.0) {}
public:
	JsonNumber(JsonValue *parent, double v) : JsonValue(parent), m_value(v) {}
	virtual type_enum type() const { return JsonValue_Number; }
	virtual String toString() const { return String().number(m_value); }

	double value() const { return m_value; }
	virtual double number() const { return value(); }
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
	virtual type_enum type() const { return JsonValue_String; }
	virtual String toString() const { return m_value; }

	String value() const { return m_value; }
	virtual String string() const { return value(); }
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
	~JsonArray();
	virtual type_enum type() const { return JsonValue_Array; }
	virtual void add(const String &key, JsonValue *value) { CWT_UNUSED(key); append(value); }

	virtual const JsonValue& at(size_t i) const {
		if (i >= m_siblings.size())
			return JsonValue::Null;
		return *m_siblings.at(i);
	}
	virtual const JsonValue& operator[](size_t i) const { return at(i); }

	void append(JsonValue *v) { m_siblings.append(v); }
	int count() const { return size(); }
	bool isEmpty() const { return m_siblings.isEmpty(); }
	int size() const { return m_siblings.size(); }
	virtual String toString() const { return _U("Array"); }
private:
	Vector<JsonValue*> m_siblings;
};

class JsonObject : public JsonContainer
{
public:
	JsonObject(JsonValue *parent) : JsonContainer(parent), m_siblings() {}
	~JsonObject();
	virtual type_enum type() const { return JsonValue_Object; }
	virtual void add(const String &key, JsonValue *value) { insert(key, value); }

	virtual const JsonValue& at(const String &key) const {
		Hash<String, JsonValue*>::const_iterator it = m_siblings.find(key);
		if (it == m_siblings.end())
			return JsonValue::Null;
		return *it.value();
	}
	virtual const JsonValue& operator[](const String &key) const { return at(key); }

	int count() const { return size(); }
	void insert(const String &key, JsonValue *value) { m_siblings.insert(key, value); }
	bool isEmpty() const { return m_siblings.isEmpty(); }
	int size() const { return m_siblings.size(); }
	virtual String toString() const { return _U("Object"); }
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

	virtual const JsonValue& at(size_t i) const { return m_root ? m_root->at(i) : JsonValue::Null; }
	virtual const JsonValue& operator[](size_t i) const { return at(i); }
	virtual const JsonValue& at(const String &key) const { return m_root ? m_root->at(key) : JsonValue::Null; }
	virtual const JsonValue& operator[](const String &key) const { return at(key); }

	JsonObject& object() { CWT_ASSERT(m_root && m_root->type() == JsonValue::JsonValue_Object); return *dynamic_cast<JsonObject*>(m_root); }
	JsonArray& array() { CWT_ASSERT(m_root && m_root->type() == JsonValue::JsonValue_Array); return *dynamic_cast<JsonArray*>(m_root); }
private:
	JsonValue *m_root;
};

CWT_NS_END

#endif /* __CWT_JSON_HPP__ */
