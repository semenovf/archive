/*
 * abnf-gen.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: wladt
 */

#include "abnf_p.hpp"
#include <cwt/logger.hpp>
#include <cwt/fsm.hpp>
#include <cwt/date.hpp>
#include <cwt/time.hpp>
#include <iostream>

CWT_NS_BEGIN

static const String INDENT("    ");
static const char * FSM_STATUS_STR[] = { "FSM_NORMAL", "FSM_REJECT", "FSM_ACCEPT" };

inline String __c_name (const String & name)
{
	return String(name).replace(String(1, '-'), String(1, '_'));
}

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

String AbnfRuleRef::toFsmMatchString() const
{
	String r;
	r << "FSM_MATCH_FSM(" << __c_name(name()) << ")";
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

// TODO need to implement
String AbnfProseVal::toFsmMatchString() const
{
	String r;
	r << "FSM_MATCH_FUNC("
	  << ")";
	return r;
}


String AbnfGenContext::generateHeader() const
{
	String r;
	String lines[3];

	lines[0] = String(" THIS CODE WAS GENERATED AUTOMATICALLY");
	lines[1] = String(" Date: ") + Date::currentDate().toString();
	lines[2] = String(" Time: ") + Time::currentTime().toString();

	size_t maxlen = 80 - 4;
	for (int i = 0; i < 3; ++i)
		maxlen = CWT_MAX(maxlen, lines[i+1].length());

	maxlen += 10;

	r << "/*" << String(maxlen, '*') << "*/" << String::EndOfLine;

	for (int i = 0; i < 3; ++i) {
		r << "/*" << lines[i] << String(maxlen - lines[i].length(), ' ') << "*/" << String::EndOfLine;
	}

	r << "/*" << String(maxlen, '*') << "*/" << String::EndOfLine;

	return r;
}

String AbnfGenContext::generateEnum () const
{
	String r;
	Vector<AbnfRule *>::const_iterator it = m_rulelist.rules().cbegin();
	Vector<AbnfRule *>::const_iterator itEnd = m_rulelist.rules().cend();

	String sep("  ");
	r << "enum TokenEnum" << String::EndOfLine
	  << '{' << String::EndOfLine;

	for (; it != itEnd; ++it) {
		r << INDENT << sep << "Token_" << __c_name((*it)->name()) << String::EndOfLine;
		sep = String(", ");
	}

	r << "};" << String::EndOfLine
	  << String::EndOfLine;

	return r;
}

String AbnfGenContext::generateTransitionTablesClass () const
{
	Vector<AbnfRule *>::const_iterator it = m_rulelist.rules().cbegin();
	Vector<AbnfRule *>::const_iterator itEnd = m_rulelist.rules().cend();

	String r;

	r << "class Transitions" << String::EndOfLine
	  << "{" << String::EndOfLine
	  << "public:" << String::EndOfLine
	  << INDENT << "typedef " << transitionType() << " Type;" << String::EndOfLine
	  << String::EndOfLine;

	r << INDENT << "struct Node" << String::EndOfLine
	  << INDENT << "{" << String::EndOfLine
	  << INDENT << INDENT << "NodeType type;" << String::EndOfLine
	  << INDENT << INDENT << sourceDataType() << "::const_iterator itBegin;" << String::EndOfLine
	  << INDENT << INDENT << sourceDataType() << "::const_iterator itEnd;" << String::EndOfLine
	  << INDENT << INDENT << "Vector<Node *> siblings;" << String::EndOfLine
	  << INDENT << INDENT << "Node * parent;" << String::EndOfLine
	  << INDENT << "};" << String::EndOfLine
	  << String::EndOfLine;

	for (; it != itEnd; ++it) {
		r << INDENT << "static Type " << TRANSITION_TABLE_NAME(__c_name((*it)->name())) << "[];" << String::EndOfLine;
	}

	r << "};" << String::EndOfLine;

	return r;
}

String AbnfGenContext::generateTransition (int state_next, int state_fail, const String & match, int status) const
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

static String __compact_char_values (Vector<AbnfElement *>::const_iterator & it, const Vector<AbnfElement *>::const_iterator & itEnd)
{
	String r;
	Vector<AbnfElement *>::const_iterator it1(it);

	while ( it1 != itEnd && (*it1)->type() == Abnf_CharVal) {
		const AbnfCharVal * v = dynamic_cast<const AbnfCharVal *>(*it1);
		if (v->value().length() != 1)
			break;
		r += v->value();
		++it1;
	}

	it = it1;
	return r;
}

String AbnfGenContext::generateTransitionTable (const AbnfRule & rule) const
{
	String r;

	CWT_ASSERT(rule.elements().size() == 1);
	CWT_ASSERT(! (rule.elements())[0]->isScalar()); // one of containers

	const AbnfElement * inner = (rule.elements())[0];
	const Vector<AbnfElement * > & elements = dynamic_cast<const AbnfContainer *>(inner)->elements();
	AbnfElementType innerType = inner->type();

	int state_next = -1;
	int state_fail = -1;
	int status = FSM_NORMAL;

//	std::cout << rule.toString() << std::endl;

	CWT_ASSERT_X(innerType == Abnf_Altern
			|| innerType == Abnf_Concat
			|| innerType == Abnf_Rpt
		, String(_Fr("ABNF rule list is corrupted or elements may be added through the bypass: %d") % innerType).c_str() );

	if (innerType == Abnf_Altern) {
		state_fail = 0;
		state_next = -1;
		status = FSM_ACCEPT;
	} else { // Abnf_Concat || Abnf_Rpt
		state_next = 0;
		state_fail = -1;
		status = FSM_NORMAL;
	}

	r << "/*" << String::EndOfLine;
	r << INDENT << rule.toString() << String::EndOfLine;
	r << "*/" << String::EndOfLine;
	r << "static Transitions::Type " << TRANSITION_TABLE_NAME(__c_name(rule.name())) << "[] = {" << String::EndOfLine;
	String sep("  ");

	Vector<AbnfElement *>::const_iterator it = elements.begin();
	Vector<AbnfElement *>::const_iterator itEnd = elements.end();

	while (it != itEnd) {
		const AbnfElement * element = *it;
		AbnfElementType t = element->type();

		if (t == Abnf_Comment) {
			++it;
			continue;
		}

		if (innerType == Abnf_Altern) {
			++state_fail;
		} else { // Abnf_Concat || Abnf_Rpt
			++state_next;
		}

		if (t == Abnf_CharVal && m_compactCharValues) {
			// Compact char values
			String s = __compact_char_values(it, itEnd);
			if (s.isEmpty()) {
				s = dynamic_cast<const AbnfScalar *>(element)->value();
				r << INDENT << sep
					<< generateTransition(state_next, state_fail
							, String("FSM_MATCH_STR(_u8(\"") + s + "\"))"
							, status)
					<< String::EndOfLine;
				++it;
			} else {
				r << INDENT << sep
					<< generateTransition(state_next, state_fail
							, (innerType == Abnf_Altern
									? String("FSM_MATCH_CHAR(_u8(\"") + s + "\"))"
									: String("FSM_MATCH_STR(_u8(\"") + s + "\"))")
							, status)
					<< String::EndOfLine;
			}
		} else {
			r << INDENT << sep << generateTransition(state_next, state_fail, element->toFsmMatchString(), status) << String::EndOfLine;
			++it;
		}
		sep = String(", ");
	}

	if (innerType == Abnf_Altern) {
		r << INDENT << sep
		  << generateTransition(-1, -1, "FSM_MATCH_NOTHING", FSM_REJECT)
		  << String::EndOfLine;
	} else { // Abnf_Concat || Abnf_Rpt
		r << INDENT << sep
		  << generateTransition(-1, -1, "FSM_MATCH_NOTHING", FSM_ACCEPT)
		  << String::EndOfLine;
	}

	r << "};" << String::EndOfLine;

	return r;
}

String AbnfGenContext::generateTransitionTables () const
{
	Vector<AbnfRule *>::const_iterator it = m_rulelist.rules().cbegin();
	Vector<AbnfRule *>::const_iterator itEnd = m_rulelist.rules().cend();

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

String AbnfGenContext::generate () const
{
	String r;

	// Do it before normalization
	String enumText(generateEnum());

	//String generateTransitionActions();

	if (! m_rulelist.normalize()) {
		Logger::error(_Tr("Failed to normalize ABNF rulelist"));
		return String();
	}

	String ttcText(generateTransitionTablesClass());
	String ttText(generateTransitionTables());

	if (ttcText.isEmpty() || ttText.isEmpty())
		return String();

	r << generateHeader()
	  << String::EndOfLine
	  << enumText
	  << String::EndOfLine
	  << ttcText
	  << String::EndOfLine
	  << ttText;

	return r;
}

CWT_NS_END
