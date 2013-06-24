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
	domCtx->containers.push(new JsonArray(NULL));
	//CWT_TRACE("Begin JSON");
	return true;
}

bool on_end_json(void *userContext, bool status)
{
	if (status) {
		JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
		CWT_ASSERT(domCtx->containers.size() == 1);
		domCtx->rootContainer = const_cast<JsonValue*>(&domCtx->containers.top()->at(0));
		//CWT_TRACE("End JSON: OK");
	} else {
		//CWT_TRACE("End JSON: FAILED");
	}
	return true;
}

bool on_begin_object(void *userContext, const String &name, JsonObject *object)
{
	CWT_UNUSED(name);
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.push(dynamic_cast<JsonValue*>(object));
	//CWT_TRACE(String().sprintf("Begin object: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_end_object(void *userContext, const String &name, JsonObject *object)
{
	CWT_UNUSED2(name, object);
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.pop();
	//CWT_TRACE(String().sprintf("End object: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_begin_array(void *userContext, const String &name, JsonArray *array)
{
	CWT_UNUSED(name);
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.push(dynamic_cast<JsonValue*>(array));
	//CWT_TRACE(String().sprintf("Begin array: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_end_array(void *userContext, const String &name, JsonArray *array)
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
		dynamic_cast<JsonArray*>(activeContainer)->append(value);
		break;
	case JsonValue::JsonValue_Object:
		dynamic_cast<JsonObject*>(activeContainer)->insert(name, value);
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
		, JsonMethod2<const String&, JsonObject*>(saj->onBeginObject)
		, JsonMethod2<const String&, JsonObject*>(saj->onEndObject)
		, JsonMethod2<const String&, JsonArray*>(saj->onBeginArray)
		, JsonMethod2<const String&, JsonArray*>(saj->onEndArray)
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

bool Json::isEmpty() const
{
	return m_root
		? m_root->type() == JsonValue::JsonValue_Object
		  	  ? dynamic_cast<JsonObject*>(m_root)->isEmpty()
		  	  : dynamic_cast<JsonArray*>(m_root)->isEmpty()
		: false;
}


String Json::toString() const
{
	String r;

	return r;
}

inline void JsonValue::setValue(const JsonValue &value)
{
	switch(m_type) {
	case JsonValue_Object:
		dynamic_cast<JsonObject*>(this)->destroy();
		break;
	case JsonValue_Array:
		dynamic_cast<JsonArray*>(this)->destroy();
		break;
	default:
		break;
	}
	m_type = value.m_type;
	m_value = value;
}

void JsonArray::destroy()
{
	array_type* siblings = arrayPtr();
	Vector<JsonValue*>::iterator it;
	Vector<JsonValue*>::iterator end = siblings->end();
	for (it = siblings->begin(); it != end; it++) {
		delete *it;
	}
}

JsonObject::destroy()
{
	object_type* siblings = objectPtr();
	Hash<String, JsonValue*>::iterator it;
	Hash<String, JsonValue*>::iterator end = siblings->end();
	for (it = siblings->begin(); it != end; it++) {
		delete *it;
	}
}

CWT_NS_END
