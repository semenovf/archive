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
#include <cwt/trace.hpp>

CWT_NS_BEGIN

class Abnf;
class AbnfRule;
class AbnfAltern;
class AbnfConcat;
class AbnfRpt;
class AbnfGroup;
class AbnfOption;
class AbnfRuleRef;
class AbnfCharVal;
class AbnfNumVal;
class AbnfProseVal;
class AbnfComment;

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


enum AbnfElementType {
	  Abnf_Rule
	, Abnf_Altern
	, Abnf_Concat
	, Abnf_Rpt
	, Abnf_Group
	, Abnf_Option
	, Abnf_RuleRef
	, Abnf_CharVal
	, Abnf_NumVal
	, Abnf_ProseVal
	, Abnf_Comment
};


class AbnfAbstractElement
{
	//CWT_DENY_COPY(AbnfAbstractElement);

public:
	virtual ~AbnfAbstractElement() {}
	virtual String toString() const = 0;

	AbnfElementType type() const { return m_type; }
	virtual bool isScalar() const { return false; }

protected:
	AbnfAbstractElement (AbnfElementType type) : m_type(type) {}
	AbnfElementType m_type;

	friend class Abnf;
};

class AbnfAbstractContainer : public AbnfAbstractElement
{
	//CWT_DENY_COPY(AbnfAbstractContainer);
public:
	virtual ~AbnfAbstractContainer();

	AbnfAbstractContainer & add (AbnfAbstractElement & element)
	{
		m_elements.append(& element);
		return *this;
	}

	AbnfAbstractContainer & add (const String & ruleref);
	AbnfAbstractContainer & addComment (const String & comment = String());
	const Vector<AbnfAbstractElement *> & elements() const { return m_elements; }

	virtual String toString (const String & separator = String(1, ' ')) const;

	AbnfAltern & newAltern ();
	AbnfConcat & newConcat ();
	AbnfRpt &    newRpt    (int from = -1, int to = -1);
	AbnfRpt &    newRpt    (const String & ruleref, int from = -1, int to = -1);
	AbnfGroup &  newGroup  ();
	AbnfOption & newOption ();

protected:
	AbnfAbstractContainer (AbnfElementType type) : AbnfAbstractElement(type) {}
	Vector<AbnfAbstractElement *> m_elements;
};

class AbnfRule : public AbnfAbstractContainer
{
public:
	AbnfRule (const String & name) : AbnfAbstractContainer(Abnf_Rule), m_name(name) {}
	const String & name () const { return m_name; }
	virtual String toString () const { return m_name + String(" = ") + AbnfAbstractContainer::toString(); }
private:
	String m_name;
};

class AbnfAltern : public AbnfAbstractContainer
{
public:
	AbnfAltern() : AbnfAbstractContainer(Abnf_Altern) {}
	virtual String toString () const { return AbnfAbstractContainer::toString(" / "); }
};

class AbnfConcat : public AbnfAbstractContainer
{
public:
	AbnfConcat() : AbnfAbstractContainer(Abnf_Concat) {}
	virtual String toString () const { return AbnfAbstractContainer::toString(" "); }
};

class AbnfRpt : public AbnfAbstractContainer
{
public:
	AbnfRpt () : AbnfAbstractContainer(Abnf_Rpt), m_from(-1), m_to(-1) {}
	AbnfRpt (int from, int to) : AbnfAbstractContainer(Abnf_Rpt), m_from(from), m_to(to) {}
	void setBounds(int from, int to) { m_from = from; m_to = to; }

	virtual String toString () const;
private:
	int m_from, m_to;
};

class AbnfGroup : public AbnfAbstractContainer
{
public:
	AbnfGroup() : AbnfAbstractContainer(Abnf_Group) {}
	virtual String toString () const { return String("( ") + AbnfAbstractContainer::toString(" ") + String(" )"); }
};

class AbnfOption : public AbnfAbstractContainer
{
public:
	AbnfOption() : AbnfAbstractContainer(Abnf_Option) {}
	virtual String toString () const { return String("[ ") + AbnfAbstractContainer::toString(" ") + String(" ]"); }
};

// base for terminal symbols
class AbnfScalar : public AbnfAbstractElement
{
	CWT_DENY_COPY(AbnfScalar);
public:
	virtual bool isScalar() const { return true; }
	void setValue (const String & v) { m_value = v;	}

protected:
	AbnfScalar (AbnfElementType type) : AbnfAbstractElement(type), m_value() {}
	AbnfScalar (AbnfElementType type, const String & v) : AbnfAbstractElement (type), m_value(v) {}
	String m_value;
};

class AbnfRuleRef : public AbnfScalar
{
public:
	AbnfRuleRef () : AbnfScalar (Abnf_RuleRef) {}
	AbnfRuleRef (const String & v) : AbnfScalar (Abnf_RuleRef, v) {}
	virtual String toString () const { return m_value; }
};

class AbnfCharVal : public AbnfScalar
{
public:
	AbnfCharVal () : AbnfScalar (Abnf_CharVal) {}
	AbnfCharVal (const String & v) : AbnfScalar (Abnf_CharVal, v) {}
	virtual String toString () const { return String(1, '"') + m_value + String(1, '"'); }
};

class AbnfNumVal : public AbnfScalar
{
public:
	AbnfNumVal () : AbnfScalar (Abnf_NumVal), m_base(10) {}
	AbnfNumVal (ulong_t single, int base = 10)
		: AbnfScalar (Abnf_NumVal, String::number(single, base, String::NumberUppercase))
		, m_base(base)
	{
		CWT_ASSERT(base == 2 || base == 10 || base == 16);
	}

	AbnfNumVal (ulong_t from, ulong_t to, int base = 10)
		: AbnfScalar (Abnf_NumVal, String())
		, m_base(base)
	{
		CWT_ASSERT(base == 2 || base == 10 || base == 16);
		CWT_ASSERT(from <= to);
		m_value = String::number(from, base, String::NumberUppercase) + String(1, '-') + String::number(to, base, String::NumberUppercase);
	}

	AbnfNumVal & add (ulong_t single, int base = 10)
	{
		CWT_ASSERT(base == 2 || base == 10 || base == 16);
		m_value += m_value.isEmpty()
				? String::number(single, base)
				: String(1, '.') + String::number(single, base);
		return *this;
	}

	virtual String toString () const
	{
		return String(1, '%')
				+ (m_base == 10
					? String(1, 'd')
					: m_base == 16
					  	  ? String(1, 'x')
					  	  : String(1, 'b'))
		        + m_value;
	}
private:
	int m_base;
};

class AbnfProseVal : public AbnfScalar
{
public:
	AbnfProseVal () : AbnfScalar (Abnf_ProseVal) {}
	AbnfProseVal (const String & v) : AbnfScalar (Abnf_ProseVal, v) {}
	virtual String toString () const { return String(1, '<') + m_value + String(1, '>'); }
};

class AbnfComment : public AbnfScalar
{
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

class AbnfRuleSet
{
	AbnfRuleSet() : m_rules(), m_rulesIndices() {}
	~AbnfRuleSet();
	CWT_DENY_COPY(AbnfRuleSet);

public:
	AbnfRule * newRule (const String & name);
	String toString () const;
	AbnfRule * find (const String & name);
	const AbnfRule * find (const String & name) const;

	bool normalize ();
	String generateTransitions () const;

private:
	bool normalizeRule (AbnfRule & rule);
	bool normalizeElement (AbnfAbstractContainer & container, AbnfRule & rule, int & uniqn, int nesting);
	String generateTransitionTablesClass () const;
	String generateTransitionTables () const;
	String generateTransitionTable (const AbnfRule & rule) const;
	String generateTransition (const AbnfAbstractElement & element) const;

private:
	Vector<AbnfRule *> m_rules;
	Hash<String, size_t>  m_rulesIndices;

	friend class Abnf;
};

class DLL_API Abnf
{
public:
	Abnf() {}
	~Abnf() {}

	bool parse(const String & abnf);
	bool parse(const String & abnf, AbnfSimpleApi & api);

	static AbnfRuleSet * createRuleSet () { return new AbnfRuleSet; }
	static void destroyRuleSet (AbnfRuleSet * ruleset) { delete ruleset; }

	static AbnfAltern &   newAltern ()   { return * new AbnfAltern; }
	static AbnfConcat &   newConcat ()   { return * new AbnfConcat; }
	static AbnfRpt &      newRpt    (int from = -1, int to = -1) { { return * new AbnfRpt(from, to); } }
	static AbnfRpt &      newRpt    (const String & ruleref, int from = -1, int to = -1)
	{
		AbnfRpt * r = new AbnfRpt(from, to);
		r->add(Abnf::newRuleRef(ruleref));
		return *r;
	}
	static AbnfGroup &    newGroup  ()   { return * new AbnfGroup; }
	static AbnfOption &   newOption ()   { return * new AbnfOption; }
	static AbnfRuleRef &  newRuleRef  (const String & name)  { return * new AbnfRuleRef(name); }
	static AbnfCharVal &  newCharVal  (const String & v) { return * new AbnfCharVal(v); }
	static AbnfNumVal &   newNumVal   (ulong_t single, int base = 10) { return * new AbnfNumVal(single, base); }
	static AbnfNumVal &   newNumVal   (ulong_t from, ulong_t to, int base = 10) { return * new AbnfNumVal(from, to, base); }
	static AbnfProseVal & newProseVal (const String & v) { return * new AbnfProseVal(v); }
	static AbnfComment &  newComment  (const String & v = String()) { return * new AbnfComment(v); }
};

inline AbnfAbstractContainer & AbnfAbstractContainer::add (const String & ruleref)
{
	return add(Abnf::newRuleRef(ruleref));
}

inline AbnfAbstractContainer & AbnfAbstractContainer::addComment (const String & comment)
{
	return add(Abnf::newComment(comment));
}


CWT_NS_END

#endif /* __CWT_ABNF_HPP__ */
