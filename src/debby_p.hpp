/*
 * debby_p.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_P_HPP__
#define __CWT_DEBBY_P_HPP__

#include "../include/cwt/debby.hpp"
#include <cwt/json.hpp>

CWT_NS_BEGIN

class DebbyFieldSpec
{
public:
	DebbyFieldSpec()
		: name()
		, type(Debby::TypeUnknown)
		, pk(false)
		, autoinc(false)
		, notnull(true)
		, index(false)
		, unique(false)
		, ref()
	{
		opt1.min = 0;
		opt2.max = 0;
	}

	DebbyFieldSpec(const DebbyFieldSpec & spec)
		: name(spec.name)
		, type(spec.type)
		, pk(spec.pk)
		, autoinc(spec.autoinc)
		, notnull(spec.notnull)
		, index(spec.index)
		, unique(spec.unique)
		, ref(spec.ref)
	{
		opt1.min = spec.opt1.min;
		opt2.max = spec.opt2.max;
	}

	String name;
	Debby::TypeEnum type;
	bool pk;
	bool autoinc;
	bool notnull;
	bool index;
	bool unique;
	String ref; // entity name referenced to (foreigh key)

	union { long_t  min; long_t  prec; }  opt1;
	union { ulong_t max; ulong_t scale; } opt2;
};

class Debby::Impl : public Json
{
public:
	Impl() : Json() {  }
	~Impl() { }

	bool parseAll();

private:
	bool parseEntity(const String & name, const JsonValue::object_type & entity) const;
	bool parseField(const JsonValue * field, DebbyFieldSpec & spec) const;
	bool parseType(const String & type, DebbyFieldSpec & spec) const;
	bool parseNumericExtra(const Vector<String> & extra, DebbyFieldSpec & spec) const;

	bool refSpec(const String & refEntityName, DebbyFieldSpec & refSpec) const;
	String generateCxxEntity(const String & name, const Vector<DebbyFieldSpec> & specs) const;
	String generateCxxScheme(const String & name) const;
	bool onEntity(const String & name, const Vector<DebbyFieldSpec> & specs) const;

	static bool unbrace(const String & s, Vector<String> & r);
	static String comment(const DebbyFieldSpec & spec);

private:
	const JsonValue * m_jvSchemeObject;
};

CWT_NS_END

#endif /* __CWT_DEBBY_P_HPP__ */
