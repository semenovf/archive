/*
 * abnf.hpp
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */

#ifndef __CWT_ABNF_HPP__
#define __CWT_ABNF_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/callback.hpp>
#include <cwt/vector.hpp>
#include <cwt/hash.hpp>
#include <cwt/unitype.hpp>

//#define CWT_TRACE_ENABLE
//#include <cwt/trace.hpp>

CWT_NS_BEGIN

class Abnf;
class AbnfGenContext;
class AbnfRule;
class AbnfAltern;
class AbnfConcat;
class AbnfRpt;
class AbnfRuleRef;
class AbnfCharVal;
class AbnfNumVal;
class AbnfProseVal;
class AbnfComment;

#ifdef __COMMENT__
struct AbnfSimpleApi {
	Callback1<void *> beginDocument;
	Callback2<bool, void *> endDocument;
	Callback3<const String &, bool, void *> beginRule; // callback(rulename, incremental, abnfContext)
	Callback1<void *> endRule;
	Callback1<void *> beginAlternation;
	Callback1<void *> endAlternation;
	Callback1<void *> beginConcatenation;
	Callback1<void *> endConcatenation;
	Callback3<int, int, void *> beginRepetition; // callback(from, to, abnfContext)
	Callback1<void *> endRepetition;
	Callback1<void *> beginOption;
	Callback1<void *> endOption;
	Callback1<void *> beginGroup;
	Callback1<void *> endGroup;
	Callback2<const String &, void *> ruleRef;
	Callback2<const String &, void *> charVal;
	Callback2<const String &, void *> numVal;
	Callback2<const String &, void *> proseVal;
};
#endif

enum AbnfElementType {
	  Abnf_RuleSet
	, Abnf_Rule
	, Abnf_Altern
	, Abnf_Concat
	, Abnf_Rpt
	, Abnf_RuleRef
	, Abnf_CharVal
	, Abnf_NumVal
	, Abnf_ProseVal
	, Abnf_Comment
};


class AbnfElement
{
public:
	virtual ~AbnfElement () {}
	virtual String toString () const = 0;
	virtual String toFsmMatchString () const { return String(); } // not applicable

	AbnfElementType type() const { return m_type; }
	virtual bool isScalar() const { return false; }

protected:
	AbnfElement (AbnfElementType type) : m_type(type), m_parent(nullptr) {}
	AbnfElementType m_type;
	AbnfElement * m_parent;

	friend class Abnf;
	friend class AbnfContainer;
};

class AbnfContainer : public AbnfElement
{
public:
	virtual ~AbnfContainer();

	AbnfContainer & add (AbnfElement & element)
	{
		m_elements.append(& element);
		element.m_parent = this;
		return *this;
	}

	AbnfContainer & add (const String & ruleref);
	AbnfContainer & addComment (const String & comment = String());
	const Vector<AbnfElement *> & elements() const { return m_elements; }

	virtual String toString (const String & separator = String(1, ' ')) const;

	AbnfAltern & newAltern ();
	AbnfConcat & newConcat ();
	AbnfRpt &    newRpt    (int from = -1, int to = -1);
	AbnfRpt &    newRpt    (const String & ruleref, int from = -1, int to = -1);

protected:
	AbnfContainer (AbnfElementType type) : AbnfElement(type) {}
	Vector<AbnfElement *> m_elements;

	friend class AbnfRuleList;
};

class DLL_API AbnfRule : public AbnfContainer
{
	CWT_DENY_COPY(AbnfRule);
public:
	AbnfRule (const String & name) : AbnfContainer(Abnf_Rule), m_name(name) {}

	const String & name () const { return m_name; }
	virtual String toString () const { return m_name + String(" = ") + AbnfContainer::toString(); }

private:
	String m_name;
};

class DLL_API AbnfAltern : public AbnfContainer
{
	CWT_DENY_COPY(AbnfAltern);
public:
	AbnfAltern() : AbnfContainer(Abnf_Altern) {}
	virtual String toString () const;
};

class DLL_API AbnfConcat : public AbnfContainer
{
	CWT_DENY_COPY(AbnfConcat);
public:
	AbnfConcat() : AbnfContainer(Abnf_Concat) {}
	virtual String toString () const;
};

class DLL_API AbnfRpt : public AbnfContainer
{
	CWT_DENY_COPY(AbnfRpt);
public:
	AbnfRpt () : AbnfContainer(Abnf_Rpt), m_from(-1), m_to(-1) {}
	AbnfRpt (int from, int to) : AbnfContainer(Abnf_Rpt), m_from(from), m_to(to) {}
	void setBounds (int from, int to) { m_from = from; m_to = to; }
	void setFrom (int from) { m_from = from; }
	void setTo (int to)     { m_to = to; }
	int from () const       { return m_from; }
	int to () const         { return m_to; }

	virtual String toString () const;
	virtual String toFsmMatchString() const;
private:
	int m_from, m_to;
};


class AbnfScalar : public AbnfElement
{
public:
	virtual bool isScalar() const { return true; }
	void setValue (const String & v) { m_value = v;	}
	const String & value () const { return m_value; }

protected:
	AbnfScalar (AbnfElementType type) : AbnfElement(type), m_value() {}
	AbnfScalar (AbnfElementType type, const String & v) : AbnfElement (type), m_value(v) {}
	String m_value;
};

class DLL_API AbnfRuleRef : public AbnfScalar
{
	CWT_DENY_COPY(AbnfRuleRef);
public:
	AbnfRuleRef () : AbnfScalar (Abnf_RuleRef) {}
	AbnfRuleRef (const String & rulename) : AbnfScalar (Abnf_RuleRef, rulename) {}

	const String & name () const { return value(); }

	virtual String toString () const { return m_value; }
	virtual String toFsmMatchString() const;
};

class DLL_API AbnfCharVal : public AbnfScalar
{
	CWT_DENY_COPY(AbnfCharVal);
public:
	AbnfCharVal () : AbnfScalar (Abnf_CharVal) {}
	AbnfCharVal (const String & v) : AbnfScalar (Abnf_CharVal, v) {}
	virtual String toString () const { return String(1, '"') + m_value + String(1, '"'); }
	virtual String toFsmMatchString() const;
};

class DLL_API AbnfNumVal : public AbnfScalar
{
	CWT_DENY_COPY(AbnfNumVal);
public:
	AbnfNumVal () : AbnfScalar (Abnf_NumVal)
		, m_base(10)
		, m_min(0)
		, m_max(0)
	{}
	AbnfNumVal (ulong_t single, int base = 10)
		: AbnfScalar (Abnf_NumVal, String::number(single, base, String::NumberUppercase))
		, m_base(base)
		, m_min(single)
		, m_max(single)
	{
		CWT_ASSERT(base == 2 || base == 10 || base == 16);
	}

	AbnfNumVal (ulong_t min, ulong_t max, int base = 10)
		: AbnfScalar (Abnf_NumVal, String())
		, m_base(base)
		, m_min(min)
		, m_max(max)
	{
		CWT_ASSERT(base == 2 || base == 10 || base == 16);
		CWT_ASSERT(min <= max);
		m_value = String::number(min, base, String::NumberUppercase) + String(1, '-') + String::number(max, base, String::NumberUppercase);
	}

	ulong_t min () const { return m_min; }
	ulong_t max () const { return m_max; }
	bool isRange () const { return m_min != m_max; }

	AbnfNumVal & add (ulong_t single, int base = 10);
	virtual String toString () const;
	virtual String toFsmMatchString() const;

private:
	int m_base;
	ulong_t m_min;
	ulong_t m_max;
};

class DLL_API AbnfProseVal : public AbnfScalar
{
	CWT_DENY_COPY(AbnfProseVal);
public:
	AbnfProseVal () : AbnfScalar (Abnf_ProseVal) {}
	AbnfProseVal (const String & v) : AbnfScalar (Abnf_ProseVal, v) {}
	virtual String toString () const { return String(1, '<') + m_value + String(1, '>'); }
	virtual String toFsmMatchString() const; //{ return String(); } // TODO need to implement
};

class DLL_API AbnfComment : public AbnfScalar
{
	CWT_DENY_COPY(AbnfComment);
public:
	AbnfComment () : AbnfScalar (Abnf_Comment, String::EndOfLine) {}
	AbnfComment (const String & v) : AbnfScalar (Abnf_Comment, v) {}
	virtual String toString () const
	{
		return (m_value.isEmpty() || m_value == String::EndOfLine)
				? String::EndOfLine
				: String("\t\t; ") + m_value + String::EndOfLine;
	}
};

class DLL_API AbnfRuleList : public AbnfContainer
{
	CWT_DENY_COPY(AbnfRuleList);

public:
	AbnfRuleList() :  AbnfContainer(Abnf_RuleSet), m_rulesIndices() { }
	~AbnfRuleList() {}

	AbnfRule & newRule (const String & name);

	virtual String toString () const;
	AbnfRule * find (const String & name);
	const AbnfRule * find (const String & name) const;
	const Vector<AbnfRule *> & rules () const { return reinterpret_cast<const Vector<AbnfRule *> &>(m_elements); }

	bool normalize ();

private:
	Vector<AbnfRule *> & rules () { return reinterpret_cast<Vector<AbnfRule *> &>(m_elements); }
	bool normalizeRule (AbnfRule & rule);
	bool normalizeElement (AbnfElement & element, AbnfRule & rule, int & uniqn);

private:
	Hash<String, size_t>  m_rulesIndices;

	friend class Abnf;
	friend class AbnfGenContext;
};


// Context for generation of FSM data
class DLL_API AbnfGenContext
{
public:
	struct Options {
		bool compactCharValues;
	};
public:
	AbnfGenContext (const String & srcDataType, AbnfRuleList & ruleset)
		: m_sourceDataType(srcDataType)
		, m_transitionType(String("FsmTransition<") + m_sourceDataType + String(1, '>'))
		, m_rulelist(ruleset)
	{
		m_options.compactCharValues = false;
	}
	String generate () const;
	void setCompactCharValues(bool compact) { m_options.compactCharValues = compact; }

private:
/*
	String generateTransitionTables () const;
	String generateTransitionTable (const AbnfRule & rule) const;
*/

	const String & sourceDataType () const { return m_sourceDataType; }
	const String & transitionType () const { return m_transitionType; }

private:
	String         m_sourceDataType;
	String         m_transitionType;
	AbnfRuleList & m_rulelist;

	// Options for generate transitions
	Options        m_options;
};

class DLL_API Abnf
{
public:
	Abnf() {}
	~Abnf() {}

	bool parse (const String & abnf, AbnfRuleList & ruleset);

	static AbnfAltern &   newAltern ()   { return * new AbnfAltern; }
	static AbnfConcat &   newConcat ()   { return * new AbnfConcat; }
	static AbnfRpt &      newRpt    (int from = -1, int to = -1) { { return * new AbnfRpt(from, to); } }
	static AbnfRpt &      newRpt    (const String & ruleref, int from = -1, int to = -1)
	{
		AbnfRpt * r = new AbnfRpt(from, to);
		r->add(Abnf::newRuleRef(ruleref));
		return *r;
	}

	static AbnfRuleRef &  newRuleRef  (const String & name)  { return * new AbnfRuleRef(name); }
	static AbnfCharVal &  newCharVal  (const String & v) { return * new AbnfCharVal(v); }
	static AbnfNumVal &   newNumVal   (ulong_t single, int base = 10) { return * new AbnfNumVal(single, base); }
	static AbnfNumVal &   newNumVal   (ulong_t from, ulong_t to, int base = 10) { return * new AbnfNumVal(from, to, base); }
	static AbnfProseVal & newProseVal (const String & v) { return * new AbnfProseVal(v); }
	static AbnfComment &  newComment  (const String & v = String()) { return * new AbnfComment(v); }
};

inline AbnfContainer & AbnfContainer::add (const String & ruleref)
{
	return add(Abnf::newRuleRef(ruleref));
}

inline AbnfContainer & AbnfContainer::addComment (const String & comment)
{
	return add(Abnf::newComment(comment));
}

inline String AbnfAltern::toString () const
{
	String r;
	r << (m_parent->type() == Abnf_Rule || m_parent->type() == Abnf_Rpt ? "" : "( ")
	  << AbnfContainer::toString(" / ")
	  << (m_parent->type() == Abnf_Rule || m_parent->type() == Abnf_Rpt ? "" : " )");
	return r;
}

inline String AbnfConcat::toString () const
{
	String r;
	r << (m_parent->type() == Abnf_Rule || m_parent->type() == Abnf_Rpt ? "" : "( ")
	  << AbnfContainer::toString(" ")
	  << (m_parent->type() == Abnf_Rule || m_parent->type() == Abnf_Rpt ? "" : " )");
	return r;
}

inline AbnfNumVal & AbnfNumVal::add (ulong_t single, int base)
{
	CWT_ASSERT(base == 2 || base == 10 || base == 16);
	m_value += m_value.isEmpty()
			? String::number(single, base)
			: String(1, '.') + String::number(single, base);
	return *this;
}

inline String AbnfNumVal::toString () const
{
	return String(1, '%')
			+ (m_base == 10
				? String(1, 'd')
				: m_base == 16
				  	  ? String(1, 'x')
				  	  : String(1, 'b'))
	        + m_value;
}


CWT_NS_END

#endif /* __CWT_ABNF_HPP__ */
