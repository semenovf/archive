#include "../include/cwt/json.hpp"
#include "../include/cwt/fsm.hpp"
#include "../include/cwt/textstream.hpp"
#include "../include/cwt/file.hpp"
#include "json_p.hpp"
#include "json_rfc4627.hpp"

CWT_NS_BEGIN

JsonValue JsonValue::sharedNull;

bool on_begin_json(void *userContext)
{
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.push(JsonValue::createArray());
	return true;
}

bool on_end_json(void *userContext, bool status)
{
	if (status) {
		JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
		CWT_ASSERT(domCtx->containers.size() == 1);
		domCtx->rootContainer = /*const_cast<JsonValue*>(*/&domCtx->containers.top()->at(0)/*)*/;
	} else {
		;
	}
	return true;
}

bool on_begin_object(void *userContext, const String &name, JsonValue *object)
{
	CWT_UNUSED(name);
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.push(object);
	//CWT_TRACE(String().sprintf("Begin object: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_end_object(void *userContext, const String &name, JsonValue *object)
{
	CWT_UNUSED2(name, object);
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.pop();
	//CWT_TRACE(String().sprintf("End object: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_begin_array(void *userContext, const String &name, JsonValue *array)
{
	CWT_UNUSED(name);
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.push(array);
	//CWT_TRACE(String().sprintf("Begin array: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_end_array(void *userContext, const String &name, JsonValue *array)
{
	CWT_UNUSED2(name, array);
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.pop();
	//CWT_TRACE(String().sprintf("End array: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_value(void *userContext, const String &name, JsonValue *value)
{
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	JsonValue *activeContainer = domCtx->containers.top();

	CWT_ASSERT(activeContainer != NULL);
	switch (activeContainer->type()) {
	case JsonValue::JsonValue_Array:
		activeContainer->append(value);
		break;
	case JsonValue::JsonValue_Object:
		activeContainer->insert(name, value);
		break;
	default:
		break;
	}

/*
	String trace;
	trace.sprintf("Value: '%ls' => %ls", name.unicode(), value->string().unicode());
	CWT_TRACE(trace.toUtf8().data());
*/
	return true;
}

static JsonSAJ JSON_DOM_SAJ = {
	  on_begin_json
	, on_end_json
	, on_begin_object
	, on_end_object
	, on_begin_array
	, on_end_array
	, on_value
};

bool Json::parse(const String &json_str, const JsonSAJ *saj, void *userContext)
{
	JsonParseContext ctx = {
		  userContext
		, this
		, String()
		, Stack<JsonNamedValue>()
		, Stack<JsonNamedValue>()
		, JsonMethod<>(saj->onBeginJson)
		, JsonMethod1<bool>(saj->onEndJson)
		, JsonMethod2<const String&, JsonValue*>(saj->onBeginObject)
		, JsonMethod2<const String&, JsonValue*>(saj->onEndObject)
		, JsonMethod2<const String&, JsonValue*>(saj->onBeginArray)
		, JsonMethod2<const String&, JsonValue*>(saj->onEndArray)
		, JsonMethod2<const String&, JsonValue*>(saj->onValue)
	};

	JsonDomContext domContext;

	if (saj == &JSON_DOM_SAJ)
		ctx.userContext = &domContext;

	Fsm<Char> fsm(json_fsm, &ctx);

	clearErrors();
	ssize_t result = fsm.exec(0, json_str.data(), json_str.length());

	if (result > 0 && size_t(result) == json_str.length()) {
		if (isGood()) {
			if (saj == &JSON_DOM_SAJ) {
				m_root = domContext.rootContainer;
			}
			return true;
		}
	}

	return false;
}

bool Json::parse(const String &json_str)
{
	return parse(json_str, &JSON_DOM_SAJ, NULL);
}


bool Json::parseFromFile(const char *path)
{
	File file;
	if (!file.open(path, IODevice::ReadOnly))
		return false;

    TextInputStream ts(&file);
    Decoder *utf8Decoder = Decoder::forName("utf8");
    ts.setDecoder(utf8Decoder);
    String json_str = ts.readAll();
    file.close();
    return parse(json_str);
}

/*
bool Json::isEmpty() const
{
	return m_root
		? m_root->type() == JsonValue::JsonValue_Object
		  	  ? dynamic_cast<JsonObject*>(m_root)->isEmpty()
		  	  : dynamic_cast<JsonArray*>(m_root)->isEmpty()
		: false;
}
*/


typedef void (*destroyer_f)(JsonValue &);

void JsonValue::destroy()
{
	static destroyer_f __vtable_destroy [] = {
		  & JsonValue::destroyScalar // null
		, & JsonValue::destroyScalar // boolean
		, & JsonValue::destroyScalar // number
		, & JsonValue::destroyScalar // string
		, & JsonValue::destroyObject
		, & JsonValue::destroyArray
	};
	__vtable_destroy[m_type](*this);
}

bool JsonValue::setValue(const JsonValue &value)
{
	destroy();
	m_type = value.m_type;
	m_value = value.m_value;
	return true;
}


void JsonValue::destroyObject(JsonValue & jvalue)
{
	object_type* siblings = jvalue.objectPtr();
	Hash<String, JsonValue*>::iterator it = siblings->begin();
	Hash<String, JsonValue*>::iterator end = siblings->end();
	for (; it != end; it++) {
		delete *it;
	}
}

void JsonValue::destroyArray(JsonValue & jvalue)
{
	array_type* siblings = jvalue.arrayPtr();
	Vector<JsonValue*>::iterator it;
	Vector<JsonValue*>::iterator end = siblings->end();
	for (it = siblings->begin(); it != end; it++) {
		delete *it;
	}
}

JsonValue& JsonValue::at(const String &key)
{
	if (m_type == JsonValue::JsonValue_Object) {
		object_type* siblings = objectPtr();
		Hash<String, JsonValue*>::iterator it = siblings->find(key);
		if (it != siblings->end())
			return *it.value();
	}
	return JsonValue::sharedNull;
}

const JsonValue& JsonValue::at(const String &key) const
{
	if (m_type == JsonValue::JsonValue_Object) {
		const object_type* siblings = objectPtr();
		Hash<String, JsonValue*>::const_iterator it = siblings->find(key);
		if (it != siblings->end())
			return *it.value();
	}
	return JsonValue::sharedNull;
}

JsonValue& JsonValue::at(size_t i)
{
	if (m_type == JsonValue::JsonValue_Array) {
		array_type* siblings = arrayPtr();
		if (i < siblings->size())
			return *siblings->at(i);
	}
	return JsonValue::sharedNull;
}

const JsonValue& JsonValue::at(size_t i) const
{
	if (m_type == JsonValue::JsonValue_Array) {
		const array_type* siblings = arrayPtr();
		if (i < siblings->size())
			return *siblings->at(i);
	}
	return JsonValue::sharedNull;
}

void JsonValue::append(JsonValue *jvalue)
{
	if (m_type != JsonValue_Array) {
		destroy();
		m_type = JsonValue_Array;
		m_value = UniType::make_object<array_type>(array_type());
	}

	jvalue->m_parent = this;
	arrayPtr()->append(jvalue);
}

void JsonValue::insert(const String &key, JsonValue *jvalue)
{
	if (m_type != JsonValue_Object) {
		destroy();
		m_type = JsonValue_Object;
		m_value = UniType::make_object<object_type>(object_type());
	}

	jvalue->m_parent = this;
	objectPtr()->insert(key, jvalue);
}

void JsonValue::remove(const String &key)
{
	if (m_type == JsonValue::JsonValue_Object) {
		object_type* siblings = objectPtr();
		JsonValue *value = siblings->at(key);
		if (value) {
			siblings->remove(key);
			delete value;
		}
	}
}

void JsonValue::remove(size_t i)
{
	if (m_type == JsonValue::JsonValue_Array) {
		array_type* siblings = arrayPtr();
		if (i < siblings->size()) {
			JsonValue *value = siblings->at(i);
			siblings->remove(i);
			delete value;
		}
	}
}


String JsonValue::objectToString(const JsonValue & jvalue)
{
	String r;
	const object_type* siblings = jvalue.objectPtr();
	Hash<String, JsonValue*>::const_iterator it = siblings->begin();
	Hash<String, JsonValue*>::const_iterator end = siblings->end();

	r += Char('{');

	if (it != end) {
		r += Char('"');
		r += it.key();
		r += Char('"');
		r += Char(':');
		r += it.value()->toString();
	}
	++it;

	for (; it != end; it++) {
		r += Char('"');
		r += it.key();
		r += Char('"');
		r += Char(':');
		r += it.value()->toString();
	}

	r += Char('}');
	return r;
}

String JsonValue::arrayToString(const JsonValue & jvalue)
{
	String r;

	const array_type* siblings = jvalue.arrayPtr();
	Vector<JsonValue*>::const_iterator it = siblings->begin();
	Vector<JsonValue*>::const_iterator end = siblings->end();

	r += Char('[');

	if (it != end)
		r += (*it)->toString();
	++it;

	for (; it != end; it++) {
		r += Char(',');
		r += (*it)->toString();
	}
	r += Char(']');

	return r;
}


typedef String (*to_string_f)(const JsonValue &);

String JsonValue::toString() const
{
	static to_string_f __vtable_to_string [] = {
		  & JsonValue::nullToString    // null
		, & JsonValue::booleanToString // boolean
		, & JsonValue::numberToString  // number
		, & JsonValue::stringToString  // string
		, & JsonValue::objectToString
		, & JsonValue::arrayToString
	};
	return __vtable_to_string[m_type](*this);
}


CWT_NS_END
