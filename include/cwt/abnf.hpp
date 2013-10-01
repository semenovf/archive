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

CWT_NS_BEGIN

class Abnf;

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
	  Abnf_Rule  // TODO Delete this element
	, Abnf_Altern
	, Abnf_Concat
	, Abnf_Rpt
	, Abnf_Group
	, Abnf_Option
	, Abnf_RuleRef
	, Abnf_CharVal
	, Abnf_NumVal
	, Abnf_ProseVal
};


class AbnfAbstractElement
{
	CWT_DENY_COPY(AbnfAbstractElement);
protected:
	AbnfAbstractElement (AbnfElementType type) : m_type(type) {}
	AbnfElementType m_type;
};


class AbnfAltern : public AbnfAbstractElement
{
public:
};

class AbnfRule
{
	CWT_DENY_COPY(AbnfRule);

public:
	AbnfRule (const String & name) : m_name(name) {}
	const String & name () const { return m_name; }

private:
	String m_name;
	Vector<AbnfAbstractElement *> m_elements;
};

class AbnfRuleSet
{
	AbnfRuleSet() : m_rules(), m_rulesIndices() {}
	~AbnfRuleSet() { /* TODO implement cleanup of vector m_rules */ }
	CWT_DENY_COPY(AbnfRuleSet);

public:
	AbnfRule & addRule(const String & name)
	{
		AbnfRule * rule = new AbnfRule(name);
		size_t index = m_rules.size();
		m_rules.append(rule);
		m_rulesIndices.insert(name, index);
		return * rule;
	}

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

	static AbnfRuleSet & createRuleSet () { return * new AbnfRuleSet; }
	static void destroyRuleSet (AbnfRuleSet & ruleset) { delete & ruleset; }
};

CWT_NS_END

#endif /* __CWT_ABNF_HPP__ */
