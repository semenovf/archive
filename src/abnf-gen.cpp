/*
 * abnf-gen.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: wladt
 */

#include "abnf_p.hpp"
#include <cwt/logger.hpp>
#include <cwt/hash.hpp>
#include <cwt/stack.hpp>
#include <cwt/fsm.hpp>

CWT_NS_BEGIN

static const String INDENT("    ");
static const String TRANSITION_TYPE("FsmTransition<String>");
static const char * FSM_STATUS_STR[] = { "FSM_NORMAL", "FSM_REJECT", "FSM_ACCEPT" };

String AbnfRuleSet::generateTransitionTablesClass () const
{
	Vector<AbnfRule *>::const_iterator it = m_rules.begin();
	Vector<AbnfRule *>::const_iterator itEnd = m_rules.end();

	String r;

	r << "class Transitions" << String::EndOfLine;
	r << "{" << String::EndOfLine;
	r << "public:" << String::EndOfLine;
	r << INDENT << "typedef " << TRANSITION_TYPE << " Type;" << String::EndOfLine;
	r << String::EndOfLine;

	for (; it != itEnd; ++it) {
		r << INDENT << "static Type " << TRANSITION_TABLE_NAME((*it)->name()) << "[];" << String::EndOfLine;
	}

	r << "}" << String::EndOfLine;

	return r;
}

String AbnfRuleSet::generateTransition (const AbnfAbstractElement & element) const
{
	String r;

	r << "{";

	int state_next = -1;
	int state_fail = -1;
	String match;
	int status = FSM_NORMAL;

	switch (element.type()) {
	case Abnf_Altern:

		break;
	case Abnf_Concat:
		break;
	case Abnf_Rpt:
		break;
	case Abnf_Group:
		break;
	case Abnf_Option:
		break;
	case Abnf_RuleRef:
		break;
	case Abnf_CharVal:
		break;
	case Abnf_NumVal:
		break;
	case Abnf_ProseVal:
		// TODO Unsupported yet
		break;
	default:
		break;
	}

	r <<           String::number(state_next)
		<< ", " << String::number(state_fail)
		<< ", " << match
		<< ", " << FSM_STATUS_STR[status]
		<< ", " << "nullptr"
		<< ", " << "nullptr";

	r << " }";
	return r;
}

String AbnfRuleSet::generateTransitionTable (const AbnfRule & rule) const
{
	String r;

	r << "static Transitions::Type " << TRANSITION_TABLE_NAME(rule.name()) << "[] = {" << String::EndOfLine;

	const Vector<AbnfAbstractElement *> * elements = & rule.elements();

	Vector<AbnfAbstractElement *>::const_iterator it = elements->begin();
	Vector<AbnfAbstractElement *>::const_iterator itEnd = elements->end();

	String sep("  ");

	for (; it != itEnd; ++it) {
		AbnfAbstractElement * element = (*it);
		AbnfElementType t = element->type();

		CWT_ASSERT(t != Abnf_Rule);

		if (t == Abnf_Comment)
			continue;

		r << INDENT << sep << generateTransition(*element) << String::EndOfLine;
		sep = String(", ");
	}

	r << "};" << String::EndOfLine;

	return r;
}

String AbnfRuleSet::generateTransitionTables () const
{
	Vector<AbnfRule *>::const_iterator it = m_rules.begin();
	Vector<AbnfRule *>::const_iterator itEnd = m_rules.end();

	String r;

	for (; it != itEnd; ++it) {
		r << generateTransitionTable (*(*it));
		r << String::EndOfLine;
	}

	return r;
}

String AbnfRuleSet::generateTransitions () const
{
	String r;
	r << generateTransitionTablesClass();
	r << String::EndOfLine;
	r << generateTransitionTables();
	return r;
}

CWT_NS_END
