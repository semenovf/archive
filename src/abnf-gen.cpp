/*
 * abnf-gen.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: wladt
 */

#include "abnf_p.hpp"
#include <cwt/logger.hpp>
#include <cwt/fsm.hpp>

CWT_NS_BEGIN

static const String INDENT("    ");
static const String TRANSITION_TYPE("FsmTransition<String>");
static const char * FSM_STATUS_STR[] = { "FSM_NORMAL", "FSM_REJECT", "FSM_ACCEPT" };

String AbnfRpt::toFsmMatchString() const
{
	String r;

	// must be normalized before
	CWT_ASSERT(m_elements.size() == 1 && m_elements[0]->isScalar());

	r << "FSM_MATCH_RPT("
	  << m_elements[0]->toFsmMatchString()
	  << ", " << String::number(m_from)
	  << ", " << String::number(m_to)
	  << ")";

	return r;
}

String AbnfOption::toFsmMatchString() const
{
	String r;

	// must be normalized before
	CWT_ASSERT(m_elements.size() == 1 && m_elements[0]->isScalar());

	r << "FSM_MATCH_RANGE("
	  << m_elements[0]->toFsmMatchString()
	  << ", 0, 1)";

	return r;
}

String AbnfRuleRef::toFsmMatchString() const
{
	String r;
	r << "FSM_MATCH_FSM(" << name() << ")";
	return r;
}

String AbnfCharVal::toFsmMatchString() const
{
	String r;
	r << "FSM_MATCH_STR(_u8(\"" << value() << "\"))";
	return r;
}

String AbnfNumVal::toFsmMatchString() const
{
	String r;

	int base = m_base == 16 || m_base == 8 ? m_base : 10;
	String pfx(base == 16 ? "0x" : base == 8 ? "0" : "");

	r << "FSM_MATCH_RANGE("
	  << pfx << String::number(m_min, m_base)
	  << ", " << pfx << String::number(m_max, m_base)
	  << ")";
	return r;
}


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

String AbnfRuleSet::generateTransition (int state_next, int state_fail, const String & match, int status) const
{
	String r;

	r << "{";
	r <<           String::number(state_next)
		<< ", " << String::number(state_fail)
		<< ", " << match
		<< ", " << FSM_STATUS_STR[status]
		<< ", " << "nullptr"
		<< ", " << "nullptr";

	r << " }";
	return r;
}

/*

static String __compact_char_values (Vector<AbnfAbstractElement *>::const_iterator & it, const Vector<AbnfAbstractElement *>::const_iterator & itEnd)
{
	String r;
	Vector<AbnfAbstractElement *>::const_iterator it1(it);

	while ( it1 != itEnd && (*it1)->type() == Abnf_CharVal) {
		const AbnfCharVal * v = dynamic_cast<const AbnfCharVal *>(*it1);
		if (v->value().length() == 1)
			r += v->value();

		++it1;
	}

	it = it1;
	return r;
}
*/

static String __comment_for_transition_table(AbnfElementType type)
{
	String r;
	r << "// "
	  << (type == Abnf_Rpt
		  ? "Repeat"
		  : type == Abnf_Altern
				? "Alternation"
				: type == Abnf_Concat
					  ? "Concatenation" : "<not_supported_yet>");
	return r;
}

String AbnfRuleSet::generateTransitionTable (const AbnfRule & rule) const
{
	String r;

	CWT_ASSERT(rule.elements().size() == 1);
	CWT_ASSERT(! (rule.elements())[0]->isScalar()); // one of containers

	const AbnfAbstractElement * inner = (rule.elements())[0];
	const Vector<AbnfAbstractElement * > & elements = dynamic_cast<const AbnfAbstractContainer *>(inner)->elements();
	AbnfElementType innerType = inner->type();

	int state_next = -1;
	int state_fail = -1;
	//int state_last = elements.size();
	int status = FSM_NORMAL;

	CWT_ASSERT_X(innerType == Abnf_Altern
			|| innerType == Abnf_Concat
		, _Tr("Elements may be added to rule through the bypass"));

	if (innerType == Abnf_Altern) {
		state_fail = 0;
		state_next = -1;
		status = FSM_ACCEPT;
	} else { // Abnf_Concat
		state_next = 0;
		state_fail = -1;
		status = FSM_NORMAL;
	}

	r << __comment_for_transition_table(innerType) << String::EndOfLine;
	r << "static Transitions::Type " << TRANSITION_TABLE_NAME(rule.name()) << "[] = {" << String::EndOfLine;
	String sep("  ");

	Vector<AbnfAbstractElement *>::const_iterator it = elements.begin();
	Vector<AbnfAbstractElement *>::const_iterator itEnd = elements.end();

	for (; it != itEnd; ++it) {
		const AbnfAbstractElement * element = *it;
		AbnfElementType t = element->type();

		if (t == Abnf_Comment)
			continue;

		if (innerType == Abnf_Altern) {
			++state_fail;
		} else { // Abnf_Concat:
			++state_next;
		}

		r << INDENT << sep << generateTransition(state_next, state_fail, element->toFsmMatchString(), status) << String::EndOfLine;
		sep = String(", ");
	}

	if (innerType == Abnf_Altern) {
		r << INDENT << sep
		  << generateTransition(-1, -1, "FSM_MATCH_NOTHING", FSM_REJECT)
		  << String::EndOfLine;
	} else { // Abnf_Concat
		r << INDENT << sep
		  << generateTransition(-1, -1, "FSM_MATCH_NOTHING", FSM_ACCEPT)
		  << String::EndOfLine;
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
		String tt(generateTransitionTable (**it));

		if (tt.isEmpty()) {
			return String(); // error
		}

		r << tt;
		r << String::EndOfLine;
	}

	return r;
}

String AbnfRuleSet::generateTransitions () const
{
	String r;
	String ttc(generateTransitionTablesClass());
	String tt(generateTransitionTables());

	if (ttc.isEmpty() || tt.isEmpty())
		return String();

	r << ttc;
	r << String::EndOfLine;
	r << tt;

	return r;
}

CWT_NS_END
