#ifndef __CWT_JSON_HPP__
#define __CWT_JSON_HPP__
 
#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/errorable.hpp>
#include <cwt/hash.hpp>
#include <cwt/vector.hpp>
#include <cwt/unitype.hpp>

CWT_NS_BEGIN

class JsonArray;
class JsonObject;

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
	JsonValue() : m_type(JsonValue_Null), m_value(), m_parent(NULL) {}
	JsonValue(TypeEnum type, JsonValue *parent) : m_type(type), m_value(), m_parent(parent) {}

public:
	JsonValue(JsonValue *parent) : m_type(JsonValue_Null), m_value(), m_parent(parent) {}
	virtual ~JsonValue() {}

	TypeEnum type() const { return m_type; }
	bool isNull() const { return type() == JsonValue_Null ? true : false; }
	JsonValue& parent() { return *m_parent; }
	const JsonValue& parent() const { return *m_parent; }
	bool boolean() const { return m_value.toBool(); }
	double number() const { return m_value.toDouble(); }
	String string() const { return m_value.toString(); }

	virtual JsonValue& at(size_t i) { CWT_UNUSED(i); return sharedNull; }
	virtual JsonValue& at(const String &key) { CWT_UNUSED(key); return sharedNull; }
	virtual const JsonValue& at(size_t i) const { CWT_UNUSED(i); return sharedNull; }
	virtual const JsonValue& at(const String &key) const { CWT_UNUSED(key); return sharedNull; }

	JsonValue& operator[](size_t i) { return at(i); }
	JsonValue& operator[](const String &key) { return at(key); }
	const JsonValue& operator[](size_t i) const { return at(i); }
	const JsonValue& operator[](const String &key) const { return at(key); }

protected:
	TypeEnum m_type;
	UniType  m_value;
	JsonValue *m_parent;

	friend class JsonArray; // for access to protected members, see void JsonArray::append(JsonValue *value)
	friend class JsonObject;
};

typedef JsonValue JsonNull;

class JsonBoolean : public JsonValue
{
public:
	explicit JsonBoolean(bool v, JsonValue *parent = nullptr) : JsonValue(JsonValue_Boolean, parent) { m_value.setBool(v); }
};

class JsonNumber : public JsonValue
{
public:
	explicit JsonNumber(double v, JsonValue *parent = nullptr) : JsonValue(JsonValue_Number, parent) { m_value.setDouble(v); }
};

class JsonString : public JsonValue
{
public:
	explicit JsonString(const String &s, JsonValue *parent = nullptr) : JsonValue(JsonValue_String, parent) { m_value.setString(s); }
	explicit JsonString(const Char *chars, JsonValue *parent = nullptr) : JsonValue(JsonValue_String, parent) { m_value.setString(String(chars)); }
	explicit JsonString(const Char *chars, size_t size, JsonValue *parent = nullptr) : JsonValue(JsonValue_String, parent) { m_value.setString(String(chars, size)); }
};

class JsonArray : public JsonValue
{
	typedef Vector<JsonValue*> array_type;
	array_type* arrayPtr() { return &m_value.objectRef<array_type>(); }
	const array_type* arrayPtr() const { return &m_value.objectRef<array_type>(); }

public:
	explicit JsonArray(JsonValue *parent = nullptr);
	virtual ~JsonArray();

	virtual JsonValue& at(size_t i);
	virtual const JsonValue& at(size_t i) const;

	void append(JsonValue *value) { arrayPtr()->append(value); value->m_parent = this; }
	size_t count() const      { return arrayPtr()->size(); }
	bool isEmpty() const      { return arrayPtr()->isEmpty(); }
	size_t size() const       { return arrayPtr()->size(); }
};

inline JsonArray::JsonArray(JsonValue *parent)
	: JsonValue(JsonValue_Array, parent)
{
	m_value = UniType::make_object<array_type>(array_type());
}

inline JsonValue& JsonArray::at(size_t i)
{
	array_type* siblings = arrayPtr();
	if (i >= siblings->size())
		return JsonValue::sharedNull;
	return *siblings->at(i);
}

inline const JsonValue& JsonArray::at(size_t i) const
{
	const array_type* siblings = arrayPtr();
	if (i >= siblings->size())
		return JsonValue::sharedNull;
	return *siblings->at(i);
}

class JsonObject : public JsonValue
{
	typedef Hash<String, JsonValue*> object_type;
	object_type* objectPtr() { return &m_value.objectRef<object_type>(); }
	const object_type* objectPtr() const { return &m_value.objectRef<object_type>(); }

public:
	explicit JsonObject(JsonValue *parent = nullptr);
	virtual ~JsonObject();

	virtual JsonValue& at(const String &key);
	virtual const JsonValue& at(const String &key) const;

	size_t count() const                             { return objectPtr()->size(); }
	void insert(const String &key, JsonValue *value) { objectPtr()->insert(key, value); value->m_parent = this; }
	bool isEmpty() const                             { return objectPtr()->isEmpty(); }
	int size() const                                 { return objectPtr()->size(); }
};

inline JsonObject::JsonObject(JsonValue *parent)
	: JsonValue(JsonValue_Object, parent)
{
	m_value = UniType::make_object<object_type>(object_type());
}

inline JsonValue& JsonObject::at(const String &key)
{
	object_type* siblings = objectPtr();
	Hash<String, JsonValue*>::iterator it = siblings->find(key);
	if (it == siblings->end())
		return JsonValue::sharedNull;
	return *it.value();
}

inline const JsonValue& JsonObject::at(const String &key) const
{
	const object_type* siblings = objectPtr();
	Hash<String, JsonValue*>::const_iterator it = siblings->find(key);
	if (it == siblings->end())
		return JsonValue::sharedNull;
	return *it.value();
}


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
protected:
	Json() : Errorable(), m_root(NULL) {}

public:
	Json(JsonObject *object) : Errorable(), m_root(object) { }
	Json(JsonArray *array) : Errorable(), m_root(array) { }
	Json(const String &json) : Errorable(), m_root(NULL) { parse(json); }
	~Json() {
		if (m_root)
			delete m_root;
		m_root = nullptr;
	}

	bool parse(const String &json);
	bool parseFromFile(const char *path);
	bool parse(const String &json, const JsonSAJ *saj, void *userContext);

	bool isArray() const { return m_root ? m_root->type() == JsonValue::JsonValue_Array ? true : false : false; }
	bool isEmpty() const;
	bool isObject() const { return m_root ? m_root->type() == JsonValue::JsonValue_Object ? true : false : false; }
	bool isNull() const { return m_root ? false : true; }

	JsonValue& at(size_t i) { return m_root ? m_root->at(i) : JsonValue::sharedNull; }
	JsonValue& at(const String &key) { return m_root ? m_root->at(key) : JsonValue::sharedNull; }
	const JsonValue& at(size_t i) const { return m_root ? m_root->at(i) : JsonValue::sharedNull; }
	const JsonValue& at(const String &key) const { return m_root ? m_root->at(key) : JsonValue::sharedNull; }

	JsonValue& operator[](size_t i) { return at(i); }
	JsonValue& operator[](const String &key) { return at(key); }
	const JsonValue& operator[](size_t i) const { return at(i); }
	const JsonValue& operator[](const String &key) const { return at(key); }

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
