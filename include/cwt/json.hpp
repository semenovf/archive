#ifndef __CWT_JSON_HPP__
#define __CWT_JSON_HPP__
 
#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/errorable.hpp>
#include <cwt/hash.hpp>
#include <cwt/vector.hpp>
#include <cwt/unitype.hpp>

CWT_NS_BEGIN

class DLL_API JsonValue
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

private:
	typedef Hash<String, JsonValue*> object_type;
	object_type* objectPtr()             { CWT_ASSERT(m_type == JsonValue_Object); return &m_value.objectRef<object_type>(); }
	const object_type* objectPtr() const { CWT_ASSERT(m_type == JsonValue_Object); return &m_value.objectRef<object_type>(); }

	typedef Vector<JsonValue*> array_type;
	array_type* arrayPtr()               { CWT_ASSERT(m_type == JsonValue_Array); return &m_value.objectRef<array_type>(); }
	const array_type* arrayPtr() const   { CWT_ASSERT(m_type == JsonValue_Array); return &m_value.objectRef<array_type>(); }

	static void destroyScalar (JsonValue & jvalue) { CWT_UNUSED(jvalue); }
	static void destroyObject (JsonValue & jvalue);
	static void destroyArray  (JsonValue & jvalue);

	static String nullToString    (const JsonValue & jvalue)  { CWT_UNUSED(jvalue); return _U("null"); }
	static String booleanToString (const JsonValue & jvalue)  { return jvalue.boolean() ? _U("true") : _U("false"); }
	static String numberToString  (const JsonValue & jvalue)  { return jvalue.string(); }
	static String stringToString  (const JsonValue & jvalue)  { String r(Char('"')); r += jvalue.string(); r += Char('"'); return r; }
	static String objectToString  (const JsonValue & jvalue);
	static String arrayToString   (const JsonValue & jvalue);

//	JsonValue(TypeEnum type) : m_type(type), m_value(), m_parent(nullptr) {}
	void destroy();

public:
	JsonValue() : m_type(JsonValue_Null), m_value(), m_parent(nullptr) {}
	JsonValue(const JsonValue &value) { setValue(value); }

	JsonValue(bool value)    : m_type(JsonValue_Boolean), m_value(value), m_parent(nullptr)  { }
	JsonValue(double value)  : m_type(JsonValue_Number) , m_value(value), m_parent(nullptr)  { }
	JsonValue(float value)   : m_type(JsonValue_Number) , m_value(value), m_parent(nullptr)  { }
	JsonValue(long_t value)  : m_type(JsonValue_Number) , m_value(value), m_parent(nullptr)  { }
	JsonValue(ulong_t value) : m_type(JsonValue_Number) , m_value(value), m_parent(nullptr)  { }
	JsonValue(int_t value)   : m_type(JsonValue_Number) , m_value(value), m_parent(nullptr)  { }
	JsonValue(uint_t value)  : m_type(JsonValue_Number) , m_value(value), m_parent(nullptr)  { }
	JsonValue(const String &s)   : m_type(JsonValue_String), m_value(s), m_parent(nullptr)   { }
	JsonValue(const Char *chars) : m_type(JsonValue_String), m_value(String(chars)), m_parent(nullptr) { }
	JsonValue(const Char *chars, size_t size) : m_type(JsonValue_String), m_value(String(chars, size)), m_parent(nullptr) { }

	static JsonValue* createObject();
	static JsonValue* createArray();

	~JsonValue() { destroy(); }
	JsonValue& operator = (const JsonValue &value) { setValue(value); return *this; }

	bool setValue(const JsonValue &value);

	TypeEnum type()  const { return m_type; }
	bool isInvalid() const { return this   == &sharedNull; }
	bool isScalar()  const { return isNull() || isBoolean() || isNumber() || isString(); }
	bool isNull()    const { return m_type == JsonValue_Null; }
	bool isBoolean() const { return m_type == JsonValue_Boolean; }
	bool isNumber()  const { return m_type == JsonValue_Number; }
	bool isString()  const { return m_type == JsonValue_String; }
	bool isObject()  const { return m_type == JsonValue_Object; }
	bool isArray()   const { return m_type == JsonValue_Array; }

	JsonValue& parent()             { return *m_parent; }
	const JsonValue& parent() const { return *m_parent; }

	bool    boolean() const  { return m_value.toBool(); }
	long_t  integer() const  { return m_value.toLong(); }
	double  number() const   { return m_value.toDouble(); }
	String  string() const   { return m_value.toString(); }
	UniType value() const    { return m_value; }

	void remove(size_t i);
	void remove(const String &key);
	void append(JsonValue *jvalue);
	void insert(const String &key, JsonValue *jvalue); // { objectPtr()->insert(key, value); value->m_parent = this; }
	//void insert(size_t i, JsonValue *value); // TODO add this method after implementing Vector::insert method

	JsonValue&       at (size_t i);
	JsonValue&       at (const String &key);
	const JsonValue& at (size_t i) const;
	const JsonValue& at (const String &key) const;

	JsonValue&       operator [] (size_t i)                { return at(i); }
	const JsonValue& operator [] (size_t i) const          { return at(i); }
	JsonValue&       operator [] (const String &key)       { return at(key); }
	const JsonValue& operator [] (const String &key) const { return at(key); }

	size_t count() const      { return size(); }
	size_t size() const {
		return m_type == JsonValue_Object
				? objectPtr()->size()
				: m_type == JsonValue_Array ? arrayPtr()->size() : 1;
	}

	String toString() const;

protected:
	TypeEnum   m_type;
	UniType    m_value;
	JsonValue *m_parent;
};

inline JsonValue* JsonValue::createObject()
{
	JsonValue* jvalue = new JsonValue();
	jvalue->m_value = UniType::make_object<object_type>(object_type());
	jvalue->m_type = JsonValue_Object;
	return jvalue;
}

inline JsonValue* JsonValue::createArray()
{
	JsonValue* jvalue = new JsonValue();
	jvalue->m_value = UniType::make_object<array_type>(array_type());
	jvalue->m_type = JsonValue_Array;
	return jvalue;
}



/* This struct provides the Simple API for JSON */
struct JsonSAJ
{
	bool (*onBeginJson)(void *userContext);
	bool (*onEndJson)(void *userContext, bool status);
	bool (*onBeginObject)(void *userContext, const String &name, JsonValue *object);
	bool (*onEndObject)(void *userContext, const String &name, JsonValue *object);
	bool (*onBeginArray)(void *userContext, const String &name, JsonValue *array);
	bool (*onEndArray)(void *userContext, const String &name, JsonValue *array);
	bool (*onValue)(void *userContext, const String &name, JsonValue *value);
};


class DLL_API Json : public Errorable
{
	CWT_DENY_COPY(Json);

public:
	Json() : Errorable(), m_root(JsonValue::createObject()) {}
	Json(const String & text) : Errorable(), m_root(JsonValue::createObject()) { parse(text); }
	~Json();

	bool parse(const String &json);
	bool parseFromFile(const char *path);
	bool parse(const String &json, const JsonSAJ *saj, void *userContext);

	bool isArray() const  { CWT_ASSERT(m_root); return m_root->type() == JsonValue::JsonValue_Array; }
	bool isObject() const { CWT_ASSERT(m_root); return m_root->type() == JsonValue::JsonValue_Object; }

	JsonValue&       at(size_t i)                { CWT_ASSERT(m_root); return m_root->at(i); }
	JsonValue&       at(const String &key)       { CWT_ASSERT(m_root); return m_root->at(key); }
	const JsonValue& at(size_t i) const          { CWT_ASSERT(m_root); return m_root->at(i); }
	const JsonValue& at(const String &key) const { CWT_ASSERT(m_root); return m_root->at(key); }

	JsonValue& operator       [] (size_t i)                { return at(i); }
	JsonValue& operator       [] (const String &key)       { return at(key); }
	const JsonValue& operator [] (size_t i) const          { return at(i); }
	const JsonValue& operator [] (const String &key) const { return at(key); }

	JsonValue& value() { CWT_ASSERT(m_root); return *m_root; }
	const JsonValue& value() const { CWT_ASSERT(m_root); return *m_root; }

	size_t size() const      { CWT_ASSERT(m_root); return m_root->size(); }
	String toString() const { CWT_ASSERT(m_root); return m_root->toString(); }
private:
	JsonValue *m_root;
};


inline Json::~Json()
{
	if (m_root) delete m_root;
	m_root = nullptr;
}


/*
 * Syntax (according to suggestions from http://goessner.net/articles/JsonPath/):
 * /            root object/element
 * /            child operator
 * .            current object/element
 * ..           parent operator
 * []           subscript operator
 */
class DLL_API JsonSimplePath
{
public:
	JsonSimplePath(Json &json);

	void  attachJson(Json &json);
	Json& json() const { return *m_json; }

	bool changeRoot(const String &jpath);
	bool chroot(const String &jpath) { return changeRoot(jpath); }

	JsonValue&       find (const String &jpath) { return const_cast<JsonValue&>(findValue(jpath)); }
	const JsonValue& find (const String &jpath) const { return findValue(jpath); }
	bool contains(const String &jpath) const;
	bool setValue(const String& jpath, const JsonValue &value);

	JsonValue&       operator [] (const String &jpath) { return const_cast<JsonValue&>(findValue(jpath)); }
	const JsonValue& operator [] (const String &jpath) const { return findValue(jpath); }

protected:
	const JsonValue& findValue (const String &jpath) const;
	JsonValue&       makeTree  (const String &jpath);

private:
	Json      * m_json;
	JsonValue * m_jsonRootValue;
};

inline JsonSimplePath::JsonSimplePath(Json &json)
	: m_json(&json)
	, m_jsonRootValue(& json.value())
{
}

inline void JsonSimplePath::attachJson(Json &json)
{
	m_json = &json;
	m_jsonRootValue = & json.value();
}

CWT_NS_END

#endif /* __CWT_JSON_HPP__ */
