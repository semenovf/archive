/*
 * json_dump.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: wladt
 */

#include "../include/cwt/json.hpp"


CWT_NS_BEGIN

inline static void __indent(String& r, int indent, int baseIndent)
{
	while (indent--) {
		for (int i = 0; i < baseIndent; ++i)
			r += Char(' ');
	}
}

static void __to_string (String& r, JsonValue* jvalue, int *indent, int baseIndent)
{
	switch (jvalue->type()) {
	case JsonValue::JsonValue_Object:
		r += Char('{');
		r += Char('\n');
		++(*indent);


		object_type* siblings = objectPtr();
		Hash<String, JsonValue*>::iterator it;
		Hash<String, JsonValue*>::iterator end = siblings->end();
		for (it = siblings->begin(); it != end; it++) {
			delete *it;
		}

/*
		for( MapNode::const_iterator itm = dynamic_cast<MapNode*>(node)->begin();
				itm != dynamic_cast<MapNode*>(node)->end(); itm++ ) {
			__indent(out, *indent);
			out += Char('"')
			out += itm->first;
			out += Char(':');
			__to_string( out, itm->second, ind );
		}
*/

		--(*indent);
		__indent(r, *indent, baseIndent);
		r += Char('}');
		r += Char('\n');
		break;

	case JsonValue::JsonValue_Array:
		size_t i = 0;
		r += Char('[');
		r += Char('\n');
		++(*indent);

		array_type* siblings = arrayPtr();
		Vector<JsonValue*>::iterator it;
		Vector<JsonValue*>::iterator end = siblings->end();
		for (it = siblings->begin(); it != end; it++) {
			delete *it;
		}

/*
		for( VectorNode::const_iterator itv = dynamic_cast<VectorNode*>(node)->begin();
				itv != dynamic_cast<VectorNode*>(node)->end(); i++, itv++ ) {
			__indent(out, *indent);
			out << _T('[') << i << _T("] => ");
			__to_string(out, *itv, indent);
		}
*/

		--(*indent);
		__indent(r, *indent, baseIndent);
		r += Char(']');
		r += Char('\n');
		break;

	default:
		r += Char('"');
		r += jvalue->string();
		r += Char('"');
		r += Char('\n');
		break;
	}
}


String Json::toString(int baseIndent) const
{
	String r;
	int indent = 0;
	__to_string(r, m_root, &indent, baseIndent);
	return r;
}

CWT_NS_END
