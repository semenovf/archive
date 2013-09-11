/*
 * debby_p.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: wladt
 */

#include "debby_p.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

static const String __INDENT(4, UChar(' '));

static const String __DEBBY_JV_SCHEME ("-scheme");

static const String __DEBBY_BOOL      ("bool");
static const String __DEBBY_STRING    ("string");
static const String __DEBBY_INT       ("int");
static const String __DEBBY_FLOAT     ("float");
static const String __DEBBY_DOUBLE    ("double");
static const String __DEBBY_DATE      ("date");
static const String __DEBBY_TIME      ("time");
static const String __DEBBY_DATETIME  ("datetime");
static const String __DEBBY_TIMESTAMP ("timestamp");
static const String __DEBBY_REF       ("ref");

static const String __CXX_BOOL        ("bool");
static const String __CXX_STRING      ("String");
static const String __CXX_INT         ("long_t");
static const String __CXX_INT8        ("int8_t");
static const String __CXX_INT16       ("int16_t");
static const String __CXX_INT32       ("int32_t");
static const String __CXX_INT64       ("int64_t");
static const String __CXX_UINT8       ("uint8_t");
static const String __CXX_UINT16      ("uint16_t");
static const String __CXX_UINT32      ("uint32_t");
static const String __CXX_UINT64      ("uint64_t");
static const String __CXX_DOUBLE      ("double");
static const String __CXX_TIME        ("ulong_t");
static const String __CXX_DATE        (__CXX_TIME);
static const String __CXX_DATETIME    (__CXX_TIME);
static const String __CXX_TIMESTAMP   (__CXX_TIME);

Debby::Debby() : pimpl(new Debby::Impl)
{
}

bool Debby::parse(const String & str)
{
	return pimpl->parse(str);
}

 bool Debby::Impl::unbrace(const String & s, Vector<String> & r)
{
	String::const_iterator openBrace  = s.find(String("("));
	String::const_iterator closeBrace = s.find(String(")"));
	String::const_iterator itEnd = s.cend();

	if (openBrace == itEnd && closeBrace == itEnd)
		return true;

	if (openBrace != itEnd && closeBrace != itEnd && openBrace < closeBrace) {
		String tmp = s.substr(openBrace + 1, closeBrace);
		r = tmp.split(String(","), true);
		return true;
	}

	return false;
}

String Debby::Impl::comment(const DebbyFieldSpec & spec)
{
	String r;

	CWT_ASSERT(spec.type != Debby::TypeNull);

	if (spec.ref.isEmpty()) {

		switch(spec.type) {
		case Debby::TypeBool:
			r.append(__DEBBY_BOOL);
			break;
		case Debby::TypeString:
			r.append(__DEBBY_STRING);
			break;
		case Debby::TypeInteger:
			r.append(__DEBBY_INT);
			if (spec.opt1.min || spec.opt2.max)
				r.append(_F("(%d, %u)") % spec.opt1.min % spec.opt2.max);
			break;
		case Debby::TypeFloat:
			r.append(__DEBBY_FLOAT);
			if (spec.opt1.prec || spec.opt2.scale)
				r.append(_F("(%d, %u)") % spec.opt1.prec % spec.opt2.scale);
			break;
		case Debby::TypeDouble:
			r.append(__DEBBY_DOUBLE);
			if (spec.opt1.prec || spec.opt2.scale)
				r.append(_F("(%d, %u)") % spec.opt1.prec % spec.opt2.scale);
			break;
		case Debby::TypeDate:
			r.append(__DEBBY_DATE);
			break;
		case Debby::TypeTime:
			r.append(__DEBBY_TIME);
			break;
		case Debby::TypeDateTime:
			r.append(__DEBBY_DATETIME);
			break;
		case Debby::TypeTimeStamp:
			r.append(__DEBBY_TIMESTAMP);
			break;
		default:
			break;
		}
	} else { // Ref
		r.append(__DEBBY_REF);
		r.append(1, UChar('('));
		r.append(spec.ref);
		r.append(1, UChar(')'));
	}

	if (spec.pk)       r.append(" pk");
	if (spec.autoinc)  r.append(" autoinc");
	if (!spec.notnull) r.append(" null");
	if (spec.index)    r.append(" index");
	if (spec.unique)   r.append(" unique");

	return r;
}


const JsonValue * Debby::Impl::schemeObject() const
{
	const JsonValue::object_type & root = this->object();

	if (!root.contains(__DEBBY_JV_SCHEME)) {
		Logger::error("Debby's source (JSON) must contain '-scheme' entry as object");
		return nullptr;
	}

	const JsonValue * r = root.at(__DEBBY_JV_SCHEME);

	if (!r->isObject()) {
		Logger::error("'-scheme' entry must be an object");
		return nullptr;
	}

	return r;
}

bool Debby::Impl::refSpec(const String & refName, DebbyFieldSpec & refSpec) const
{
	const JsonValue * scheme = this->schemeObject();

	if (!scheme)
		return false;

	const JsonValue * jv = & scheme->at(refName);

	if (jv->isInvalid()) {
		Logger::error(_Fr("Table '%s' not found") % refName);
		return false;
	}

	CWT_ASSERT(jv);

	if (!jv->isObject()) {
		Logger::error(_Fr("Debby's entity '%s' must be an object") % refName);
		return false;
	}

	JsonValue::object_type::const_iterator it = jv->object().cbegin();
	JsonValue::object_type::const_iterator itEnd = jv->object().cend();

	while (it != itEnd) {
		if (!parseField(*it, refSpec))
			return false;
		if (refSpec.pk)
			break;
		++it;
	}

	if (it == itEnd) {
		Logger::error(_Fr("Primary key field not found in entity '%s'") % refName);
		return false;
	}

	return true;
}


bool Debby::Impl::parseNumericExtra(const Vector<String> & extra, DebbyFieldSpec & spec) const
{
	if (extra.isEmpty() ) {
		 spec.opt1.min = 0;
		 spec.opt2.max = 0;
		 return true;
	}

	if (extra.size() == 2) {
		bool ok1 = true;
		bool ok2 = true;
		spec.opt1.min  = extra[0].isEmpty() ? 0 : extra[0].trim().toLong(&ok1); // prec for double/float
		spec.opt2.max = extra[1].isEmpty() ? 0 : extra[1].trim().toULong(&ok2); // scale for double/float

		if (ok1 && ok2)
			return true;
	}

	Logger::error(_Fr("Invalid parameter(s) for numeric field '%s'") % spec.name);
	return false;
}

bool Debby::Impl::parseType(const String & type, DebbyFieldSpec & spec) const
{
	Vector<String> parms;

	if (!unbrace(type, parms)) {
		Logger::error(_Fr("Field specification contains unbalanced braces for field '%s'") % spec.name);
		return false;
	}

	if (type == __DEBBY_BOOL) {

		spec.type = Debby::TypeBool;

	} else if (type.startsWith(__DEBBY_STRING)) {

		spec.type = Debby::TypeString;

		if (parms.isEmpty()) {
			spec.opt2.max = 0;
		} else if  (parms.size() == 1) {
			bool ok = true;
			spec.opt2.max = parms[0].trim().toULong(&ok);

			if (!ok) {
				Logger::error(_Fr("Invalid parameter for string field '%s'") % spec.name);
				return false;
			}
		} else {
			Logger::error(_Fr("Field '%s' must contain referenced entity in braces") % spec.name);
			return false;
		}

	} else if (type.startsWith(__DEBBY_INT)) {

		spec.type = Debby::TypeInteger;
		return parseNumericExtra(parms, spec);

	} else if (type.startsWith(__DEBBY_FLOAT)) {

		spec.type = Debby::TypeFloat;
		return parseNumericExtra(parms, spec);

	} else if (type.startsWith(__DEBBY_DOUBLE)) {

		spec.type = Debby::TypeDouble;
		return parseNumericExtra(parms, spec);

	} else if (type == __DEBBY_DATE) {

		spec.type = Debby::TypeDate;

	} else if (type == __DEBBY_TIME) {

		spec.type = Debby::TypeTime;

	} else if (type == __DEBBY_DATETIME) {

		spec.type = Debby::TypeDateTime;

	} else if (type == __DEBBY_TIMESTAMP) {

		spec.type = Debby::TypeTimeStamp;

	} else if (type.startsWith(__DEBBY_REF)) {

		if (parms.size() == 1) {
			DebbyFieldSpec tmp_spec;
			spec.ref = parms[0].trim();

			if (!refSpec(spec.ref, tmp_spec))
				return false;

			spec.type = tmp_spec.type;
		} else {
			Logger::error(_Fr("Ref must contain referenced entity in braces for field '%s'") % spec.name);
			return false;
		}
		// fieldSpecForCxx(const JsonValue * v, DebbyFieldSpec & spec)
	}

	return true;
}

bool Debby::Impl::parseField(const JsonValue * v, DebbyFieldSpec & spec) const
{
	String type;

	if (v->isScalar()) {
		type = v->string();
	} else if (v->isObject()) {
		if (!v->at("type").isInvalid()) {
			type = v->at("type").string();

			spec.pk      = v->at("pk")     .boolean(false);
			spec.autoinc = v->at("autoinc").boolean(false);
			spec.notnull = v->at("notnull").boolean(true);
			spec.index   = v->at("index")  .boolean(false);
			spec.unique  = v->at("unique") .boolean(false);

		} else {
			Logger::error(_Fr("Field specification must contain 'type' attribute for field '%s'") % spec.name);
			return false;
		}
	} else {
		Logger::error(_Fr("Field specification must be an object or string for field '%s'") % spec.name);
		return false;
	}

	CWT_ASSERT(!type.isEmpty());

	return parseType(type, spec);
}

bool Debby::Impl::parseTable(const String & name, const JsonValue::object_type & entity) const
{
	JsonValue::object_type::const_iterator it = entity.cbegin();
	JsonValue::object_type::const_iterator itEnd = entity.cend();

	Vector<DebbyFieldSpec> specs;

	while (it != itEnd) {
		DebbyFieldSpec fieldSpec;
		fieldSpec.name = it.key();

		if (!parseField(it.value(), fieldSpec)) {
			Logger::error(_Fr("Error(s) occured while process entity: '%s'") % name);
			return false;
		}
		specs.append(fieldSpec);

		++it;
	}

	onEntity(name, specs);

	return true;
}

String Debby::Impl::generateCxxTable(const String & name, const Vector<DebbyFieldSpec> & specs) const
{
	String r;

	Vector<DebbyFieldSpec>::const_iterator it = specs.cbegin();
	Vector<DebbyFieldSpec>::const_iterator itEnd = specs.cend();

	r.append(_F("struct %s \n{\n") % name);
	while (it != itEnd) {
		String type;

		CWT_ASSERT(it->type != Debby::TypeNull);

		switch (it->type) {
		case Debby::TypeBool:
			type = __CXX_BOOL;
			break;
		case Debby::TypeString:
			type = __CXX_STRING;
			break;
		case Debby::TypeInteger:
			type = __CXX_INT;
			break;
		case Debby::TypeFloat:
		case Debby::TypeDouble:
			type = __CXX_DOUBLE;
			break;
		case Debby::TypeDate:
			type = __CXX_DATE;
			break;
		case Debby::TypeTime:
			type = __CXX_TIME;
			break;
		case Debby::TypeDateTime:
			type = __CXX_DATETIME;
			break;
		case Debby::TypeTimeStamp:
			type = __CXX_TIMESTAMP;
			break;
		default:
			break;
		}

		r.append(_F("%s%s %s; // %s\n")
			% __INDENT
			% type
			% it->name
			% comment(*it));
		++it;
	}
	r.append(String("}\n\n"));

	return r;
}

String Debby::Impl::generateCxxScheme(const String & name) const
{
	String classDecl = SafeFormat(
			"class %s\n"
			"{\n"
			"%sbool deploy() const;\n"
			"%sbool recall() const;\n"
			"};\n\n")
		% name
		% __INDENT
		% __INDENT;

	return classDecl;
}

bool Debby::Impl::onEntity(const String & name, const Vector<DebbyFieldSpec> & specs) const
{
	String cxx = generateCxxTable(name, specs);
	if (cxx.isEmpty())
		return false;

	puts(cxx.c_str());

	return true;
}


bool Debby::Impl::parseAll()
{
	if (!this->isObject()) {
		Logger::error("Debby's source (JSON) must be an object");
		return false;
	}

	const JsonValue * schemeObject = this->schemeObject();

	JsonValue::object_type::const_iterator it = schemeObject->object().cbegin();
	JsonValue::object_type::const_iterator itEnd = schemeObject->object().cend();

	// traverse through entities
	while (it != itEnd) {
		String entityName = it.key();
		if (!it.value()->isObject()) {
			Logger::error(_Fr("Debby's entity '%s' must be an object") % entityName);
			return false;
		}

		JsonValue::object_type entity = it.value()->object();
		if (!parseTable(entityName, entity)) {
			return false;
		}
		++it;
	}

	String cxxScheme = generateCxxScheme(String("Scheme"));
	if (cxxScheme.isEmpty())
		return false;

	puts(cxxScheme.c_str());

	return true;
}

CWT_NS_END
