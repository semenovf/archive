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
	r << "FSM_MATCH_FSM(Transitions::" << TRANSITION_TABLE_NAME(__c_name(name())) << ")";
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


static String generateHeader(const String & sourceDataType)
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

	r << String::EndOfLine
	  << "#include <cwt/fsm.hpp>" << String::EndOfLine
	  << String::EndOfLine
	  << "using namespace cwt;" << String::EndOfLine
	  << String::EndOfLine;

	r << "#ifdef FSM_MATCH_RPT" << String::EndOfLine
	  << "#" << INDENT << "undef FSM_MATCH_RPT" << String::EndOfLine
	  << "#endif" << String::EndOfLine
	  << "#define FSM_MATCH_RPT(fsm,f,t) FsmMatch<" << sourceDataType << ">(new FsmMatchRpt<" << sourceDataType << ">(fsm,f,t))" << String::EndOfLine
	  << String::EndOfLine;

	r << "#ifdef FSM_MATCH_NOTHING" << String::EndOfLine
	  << "#" << INDENT << "undef FSM_MATCH_NOTHING" << String::EndOfLine
	  << "#endif" << String::EndOfLine
	  << "#define FSM_MATCH_NOTHING FsmMatch<" << sourceDataType << ">(new FsmMatchNothing<" << sourceDataType << ">)" << String::EndOfLine
	  << String::EndOfLine;

	r << "#ifdef FSM_MATCH_FSM" << String::EndOfLine
	  << "#" << INDENT << "undef FSM_MATCH_FSM" << String::EndOfLine
	  << "#endif" << String::EndOfLine
	  << "#define FSM_MATCH_FSM(tr) FsmMatch<" << sourceDataType << ">(new FsmMatchFsm<" << sourceDataType << ">(tr))" << String::EndOfLine
	  << String::EndOfLine;

	r << "#ifdef FSM_MATCH_CHAR" << String::EndOfLine
	  << "#" << INDENT << "undef FSM_MATCH_CHAR" << String::EndOfLine
	  << "#endif" << String::EndOfLine
	  << "#define FSM_MATCH_CHAR(s) FsmMatch<" << sourceDataType << ">(new FsmMatchChar<" << sourceDataType << ">(s))" << String::EndOfLine
	  << String::EndOfLine;

	r << "#ifdef FSM_MATCH_STR" << String::EndOfLine
	  << "#" << INDENT << "undef FSM_MATCH_STR" << String::EndOfLine
	  << "#endif" << String::EndOfLine
	  << "#define FSM_MATCH_STR(s) FsmMatch<" << sourceDataType << ">(new FsmMatchStr<" << sourceDataType << ">(s))" << String::EndOfLine;

	return r;
}

static String generateEnum (const AbnfRuleList & rulelist, size_t origRulesCount)
{
	String r;
	Vector<AbnfRule *>::const_iterator it = rulelist.rules().cbegin();
	Vector<AbnfRule *>::const_iterator itEnd = it + origRulesCount;

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

static String generateActionArgs (const AbnfRuleList & rulelist, size_t origRulesCount)
{
	String r;
	Vector<AbnfRule *>::const_iterator it = rulelist.rules().cbegin();
	Vector<AbnfRule *>::const_iterator itEnd = it + origRulesCount;

	String sep("  ");

	r << "static int ACTION_ARGS[] = {" << String::EndOfLine;

	for (; it != itEnd; ++it) {
		r << INDENT << sep << "Token_" << __c_name((*it)->name()) << String::EndOfLine;
		sep = String(", ");
	}

	r << "};" << String::EndOfLine;

	return r;
}

static String generateActions (const AbnfRuleList & /*rulelist*/, size_t /*origRulesCount*/, const String & sourceDataType)
{
	String r;
	//Vector<AbnfRule *>::const_iterator it = rulelist.rules().cbegin();
	//Vector<AbnfRule *>::const_iterator itEnd = it + origRulesCount;

	// on_begin
	r << "bool Transitions::on_begin(const "
	  << sourceDataType << "::const_iterator & begin, const "
	  << sourceDataType << "::const_iterator & end, void * context, void * action_args)" << String::EndOfLine
	  << "{" << String::EndOfLine;
	r << INDENT << "TokenEnum tokenType = *static_cast<TokenEnum *>(action_args);" << String::EndOfLine;

    r << INDENT << "Transitions::Node * node = new Transitions::Node(tokenType, nullptr);" << String::EndOfLine
      << INDENT << "node->itBegin = begin;" << String::EndOfLine
      << INDENT << "node->itEnd = end;" << String::EndOfLine
      << INDENT << "CWT_UNUSED(context);" << String::EndOfLine
      << String::EndOfLine;

	r << INDENT << "return true;" << String::EndOfLine;
	r << "}" << String::EndOfLine;

	r << String::EndOfLine;

	// on_end_accepted
	r << "bool Transitions::on_end_accepted(const "
	  << sourceDataType << "::const_iterator & begin, const "
	  << sourceDataType << "::const_iterator & end, void * context, void * action_args)" << String::EndOfLine
	  << "{" << String::EndOfLine;
	r << INDENT << "TokenEnum tokenType = *static_cast<TokenEnum *>(action_args);" << String::EndOfLine;

	r << INDENT << "Transitions::Node * node = new Transitions::Node(tokenType, nullptr);" << String::EndOfLine
      << INDENT << "node->itBegin = begin;" << String::EndOfLine
      << INDENT << "node->itEnd = end;" << String::EndOfLine
      << INDENT << "CWT_UNUSED(context);" << String::EndOfLine
      << String::EndOfLine;

	r << INDENT << "return true;" << String::EndOfLine;
	r << "}" << String::EndOfLine;

	r << String::EndOfLine;

	// on_end_rejected
	r << "bool Transitions::on_end_rejected(const "
	  << sourceDataType << "::const_iterator & begin, const "
	  << sourceDataType << "::const_iterator & end, void * context, void * action_args)" << String::EndOfLine
	  << "{" << String::EndOfLine;
	r << INDENT << "TokenEnum tokenType = *static_cast<TokenEnum *>(action_args);" << String::EndOfLine;

	r << INDENT << "Transitions::Node * node = new Transitions::Node(tokenType, nullptr);" << String::EndOfLine
      << INDENT << "node->itBegin = begin;" << String::EndOfLine
      << INDENT << "node->itEnd = end;" << String::EndOfLine
      << INDENT << "CWT_UNUSED(context);" << String::EndOfLine
      << String::EndOfLine;

    r << INDENT << "return true;" << String::EndOfLine;
	r << "}" << String::EndOfLine;

	return r;
}


static String generateTransitionTablesClass (const AbnfRuleList & rulelist
		, const String & transitionType
		, const String & sourceDataType)
{
	Vector<AbnfRule *>::const_iterator it = rulelist.rules().cbegin();
	Vector<AbnfRule *>::const_iterator itEnd = rulelist.rules().cend();

	String r;

	r << "class Transitions" << String::EndOfLine
	  << "{" << String::EndOfLine
	  << "public:" << String::EndOfLine
	  << INDENT << "typedef " << transitionType << " Type;" << String::EndOfLine
	  << String::EndOfLine;

	r << INDENT << "struct Node" << String::EndOfLine
	  << INDENT << "{" << String::EndOfLine
  	  << INDENT << INDENT << "Node(TokenEnum type, Node * parent)" << String::EndOfLine
  	  << INDENT << INDENT << INDENT << ": tokenType(type)" << String::EndOfLine
  	  << INDENT << INDENT << INDENT << ", parent(parent)" << String::EndOfLine
  	  << INDENT << INDENT << "{}" << String::EndOfLine
  	  << String::EndOfLine;

	r << INDENT << INDENT << "TokenEnum tokenType;" << String::EndOfLine
	  << INDENT << INDENT << sourceDataType << "::const_iterator itBegin;" << String::EndOfLine
	  << INDENT << INDENT << sourceDataType << "::const_iterator itEnd;" << String::EndOfLine
	  << INDENT << INDENT << "Vector<Node *> siblings;" << String::EndOfLine
	  << INDENT << INDENT << "Node * parent;" << String::EndOfLine
	  << INDENT << "};" << String::EndOfLine
	  << String::EndOfLine;

	for (; it != itEnd; ++it) {
		r << INDENT << "static Type " << TRANSITION_TABLE_NAME(__c_name((*it)->name())) << "[];" << String::EndOfLine;
	}

	r << String::EndOfLine;

	r << INDENT << "static bool on_begin(const "
	  << sourceDataType << "::const_iterator & begin, const "
	  << sourceDataType << "::const_iterator & end, void * context, void * action_args);" << String::EndOfLine;

	r << INDENT << "static bool on_end_accepted(const "
	  << sourceDataType << "::const_iterator & begin, const "
	  << sourceDataType << "::const_iterator & end, void * context, void * action_args);" << String::EndOfLine;

	r << INDENT << "static bool on_end_rejected(const "
	  << sourceDataType << "::const_iterator & begin, const "
	  << sourceDataType << "::const_iterator & end, void * context, void * action_args);" << String::EndOfLine;

	r << "};" << String::EndOfLine;

	return r;
}

static String generateTransition (int state_next, int state_fail, const String & match, int status
		, const String & action = String(), const String & action_args = String())
{
	String r;

	r << "{";
	r <<           String::number(state_next)
		<< ", " << String::number(state_fail)
		<< ", " << match
		<< ", " << FSM_STATUS_STR[status]
		<< ", " << (action.isEmpty() ? "nullptr" : action)
		<< ", " << (action_args.isEmpty() ? "nullptr" : action_args);

	r << " }";
	return r;
}

static String compactCharValues (Vector<AbnfElement *>::const_iterator & it, const Vector<AbnfElement *>::const_iterator & itEnd)
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


static size_t calculateNonComments (const Vector<AbnfElement *> & elements)
{
	size_t r = 0;
	Vector<AbnfElement *>::const_iterator it = elements.begin();
	Vector<AbnfElement *>::const_iterator itEnd = elements.end();

	while (it != itEnd) {
		if ((*it)->type() != Abnf_Comment)
			++r;
		++it;
	}
	return r;
}

static String generateTransitionTable (const AbnfRule & rule, bool isOrigRule, const AbnfGenContext::Options & options)
{
	String r;

	CWT_ASSERT(rule.elements().size() == 1);
	CWT_ASSERT(! (rule.elements())[0]->isScalar()); // one of containers

	const AbnfElement * inner = (rule.elements())[0];
	const Vector<AbnfElement * > & elements = dynamic_cast<const AbnfContainer *>(inner)->elements();
	AbnfElementType innerType = inner->type();

	String c_rulename(__c_name(rule.name()));
	String actionArgsText(String("& ACTION_ARGS[Token_") + c_rulename + "]");
	int state_next = -1;
	int state_fail = -1;
	int status = FSM_NORMAL;

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
	r << "Transitions::Type Transitions::" << TRANSITION_TABLE_NAME(c_rulename) << "[] = {" << String::EndOfLine;
	String sep("  ");

	size_t nelements = calculateNonComments(elements);

	if (isOrigRule) {
		r << INDENT << sep
		  << generateTransition(1, -1, "FSM_MATCH_NOTHING", FSM_NORMAL, "Transitions::on_begin", actionArgsText)
		  << String::EndOfLine;

		// begin action
		if (innerType == Abnf_Altern) {
			++state_fail;
			state_next = nelements + 2;
		} else { // Abnf_Concat || Abnf_Rpt
			++state_next;
			state_fail = nelements + 2;
		}

		sep = String(", ");
	}

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

		if (t == Abnf_CharVal && options.compactCharValues) {
			// Compact char values
			String s = compactCharValues(it, itEnd);
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
		  << generateTransition(-1, -1, "FSM_MATCH_NOTHING", FSM_REJECT
				  , (isOrigRule ? "Transitions::on_end_rejected" : "")
				  , (isOrigRule ? actionArgsText : ""))
		  << String::EndOfLine;

		if (isOrigRule) {
			r << INDENT << sep
			  << generateTransition(-1, -1, "FSM_MATCH_NOTHING", FSM_ACCEPT, "Transitions::on_end_accepted", actionArgsText)
			  << String::EndOfLine;
		}
	} else { // Abnf_Concat || Abnf_Rpt
		r << INDENT << sep
		  << generateTransition(-1, -1, "FSM_MATCH_NOTHING", FSM_ACCEPT
				  , (isOrigRule ? "Transitions::on_end_accepted" : "")
				  , (isOrigRule ? actionArgsText : ""))
		  << String::EndOfLine;

		if (isOrigRule) {
			r << INDENT << sep
			  << generateTransition(-1, -1, "FSM_MATCH_NOTHING", FSM_REJECT , "Transitions::on_end_rejected", actionArgsText)
			  << String::EndOfLine;
		}
	}


	r << "};" << String::EndOfLine;

	return r;
}

String generateTransitionTables (const AbnfRuleList & rulelist, const AbnfGenContext::Options & options, size_t origRulesCount)
{
	Vector<AbnfRule *>::const_iterator it = rulelist.rules().cbegin();
	Vector<AbnfRule *>::const_iterator itEnd = rulelist.rules().cend();

	String r;
	size_t i = 0;

	for (; it != itEnd; ++it, ++i) {
		String tt(generateTransitionTable (**it, i < origRulesCount, options));

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

	// Before normalization
	size_t origRulesCount = m_rulelist.elements().size();

	if (! m_rulelist.normalize()) {
		Logger::error(_Tr("Failed to normalize ABNF rulelist"));
		return String();
	}

	String ttcText(generateTransitionTablesClass(m_rulelist, transitionType(), sourceDataType()));
	String ttText(generateTransitionTables(m_rulelist, m_options, origRulesCount));

	if (ttcText.isEmpty() || ttText.isEmpty())
		return String();

	String headerText(generateHeader(sourceDataType()));
	String enumText(generateEnum(m_rulelist, origRulesCount));
	String actionFlags(generateActionArgs(m_rulelist, origRulesCount));
	String actionsText(generateActions(m_rulelist, origRulesCount, sourceDataType()));

	r << headerText   << String::EndOfLine
	  << enumText     << String::EndOfLine
	  << ttcText      << String::EndOfLine
	  << actionFlags  << String::EndOfLine
	  << ttText       << String::EndOfLine
	  << actionsText;

	return r;
}

CWT_NS_END
