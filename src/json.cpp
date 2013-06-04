#include <cwt/json.hpp>
#include <cwt/fsm.hpp>
#include <cwt/textstream.hpp>
#include <cwt/file.hpp>
#include "json_p.hpp"
#include "json_rfc4627.hpp"

CWT_NS_BEGIN

const JsonValue JsonValue::Null;

bool on_begin_json(void *userContext)
{
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.push(new JsonArray(NULL));
	CWT_TRACE("Begin JSON");
	return true;
}

bool on_end_json(void *userContext, bool status)
{
	if (status) {
		JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
		CWT_ASSERT(domCtx->containers.size() == 1);
		domCtx->rootContainer = const_cast<JsonValue*>(&domCtx->containers.top()->at(0));
		CWT_TRACE("End JSON: OK");
	} else {
		CWT_TRACE("End JSON: FAILED");
	}
	return true;
}

bool on_begin_object(void *userContext, const String &name, JsonObject *object)
{
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.push(dynamic_cast<JsonValue*>(object));
	CWT_TRACE(String().sprintf("Begin object: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_end_object(void *userContext, const String &name, JsonObject *object)
{
	CWT_UNUSED(object);
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.pop();
	CWT_TRACE(String().sprintf("End object: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_begin_array(void *userContext, const String &name, JsonArray *array)
{
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.push(dynamic_cast<JsonValue*>(array));
	CWT_TRACE(String().sprintf("Begin array: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_end_array(void *userContext, const String &name, JsonArray *array)
{
	CWT_UNUSED(array);
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	domCtx->containers.pop();
	CWT_TRACE(String().sprintf("End array: %ls", name.data()).toUtf8().data());
	return true;
}

bool on_value(void *userContext, const String &name, JsonValue *value)
{
	JsonDomContext *domCtx = reinterpret_cast<JsonDomContext*>(userContext);
	JsonValue *activeContainer = domCtx->containers.top();

	CWT_ASSERT(activeContainer != NULL);
	dynamic_cast<JsonContainer*>(activeContainer)->add(name, value);
	CWT_TRACE(String().sprintf("Value: '%ls' => %ls", name.unicode(), value->toString().unicode()).toUtf8().data());
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

JsonArray::~JsonArray()
{
	Vector<JsonValue*>::iterator it;
	Vector<JsonValue*>::iterator end = m_siblings.end();
	for (it = m_siblings.begin(); it != end; it++) {
		delete *it;
	}
}

JsonObject::~JsonObject()
{
	Hash<String, JsonValue*>::iterator it;
	Hash<String, JsonValue*>::iterator end = m_siblings.end();
	for (it = m_siblings.begin(); it != end; it++) {
		delete *it;
	}
}


CWT_NS_END
