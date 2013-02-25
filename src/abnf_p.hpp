/**
 * @file   abnf_p.hpp
 * @author wladt
 * @date   Feb 18, 2013 9:10:27 AM
 *
 * @brief
 */

#ifndef __CWT_ABNF_P_HPP__
#define __CWT_ABNF_P_HPP__

#include "cwt/abnf.hpp"
#include <cwt/callback.hpp>
#include <cwt/vector.hpp>
#include <cwt/hash.hpp>
#include <cwt/stack.hpp>

CWT_NS_BEGIN

#define _CAST_PARSE_CTX(ctx) reinterpret_cast<AbnfParseContext*>(ctx)

class AbnfNode {
public:
	enum AbnfNodeType {
		  Abnf_Rule
		, Abnf_Alternation
		, Abnf_Concatenation
		, Abnf_Repetition
		, Abnf_RuleNameElement
		, Abnf_GroupElement
		, Abnf_OptionElement
		, Abnf_CharValElement
		, Abnf_NumValElement
		, Abnf_ProseValElement
	};

	AbnfNode(AbnfNodeType t) : type(t) {}
	virtual ~AbnfNode() {}
	void addNode(AbnfNode *node) { nodes.append(node); }

	AbnfNodeType      type;
	Vector<AbnfNode*> nodes;
};

class AbnfNodeRule : public AbnfNode {
public:
	AbnfNodeRule(const String &s) : AbnfNode(Abnf_Rule), name(s), refs(1) {}
	String name;
	int refs;
};
class AbnfNodeAlternation : public AbnfNode {
public:
	AbnfNodeAlternation() : AbnfNode(Abnf_Alternation) {}
};
class AbnfNodeConcatenation : public AbnfNode {
public:
	AbnfNodeConcatenation() : AbnfNode(Abnf_Concatenation) {}
};
class AbnfNodeRepetition : public AbnfNode {
public:
	AbnfNodeRepetition(int f, int t) : AbnfNode(Abnf_Repetition), from(f), to(t) {}
	int from, to;
};
class AbnfNodeRuleNameElement : public AbnfNode {
public:
	AbnfNodeRuleNameElement() : AbnfNode(Abnf_RuleNameElement) {}
};
class AbnfNodeGroupElement : public AbnfNode {
public:
	AbnfNodeGroupElement() : AbnfNode(Abnf_GroupElement) {}
};
class AbnfNodeOptionElement : public AbnfNode {
public:
	AbnfNodeOptionElement() : AbnfNode(Abnf_OptionElement) {}
};
class AbnfNodeCharValElement : public AbnfNode {
public:
	AbnfNodeCharValElement(const String &v) : AbnfNode(Abnf_CharValElement), value(v) {}
	String value;
};
class AbnfNodeNumValElement : public AbnfNode {
public:
	AbnfNodeNumValElement(const String &v) : AbnfNode(Abnf_NumValElement), value(v) {}
	String value;
};
class AbnfNodeProseValElement : public AbnfNode {
public:
	AbnfNodeProseValElement(const String &v) : AbnfNode(Abnf_ProseValElement), value(v) {}
	String value;
};

struct AbnfContext {
	Hash<String, AbnfNode*> rules;
	Stack<AbnfNode*>        rulestack;
};

struct AbnfParseContext {
	void      *userContext;
	String    rulename; // rulename for the current rule
	struct    { int from, to; } rpt;

	Callback3<const String&, bool, void*> beginRule; // callback(rulename, incremental, abnfContext)
	Callback1<void*> endRule;
	Callback1<void*> beginAlternation;
	Callback1<void*> endAlternation;
	Callback1<void*> beginConcatenation;
	Callback1<void*> endConcatenation;
	Callback3<int, int, void*> beginRepetition; // callback(from, to, abnfContext)
	Callback1<void*> endRepetition;
	Callback1<void*> beginOption;
	Callback1<void*> endOption;
	Callback1<void*> beginGroup;
	Callback1<void*> endGroup;
	Callback2<const String&, void*> ruleRef;
	Callback2<const String&, void*> charVal;
	Callback2<const String&, void*> numVal;
	Callback2<const String&, void*> proseVal;

	Callback2<const String&, void*> comment;
/*
	Callback1<void*> on_incremental_alternative;
	Callback2<const String&, void*> on_rulename;

	Callback1<void*> on_begin_option;
	Callback1<void*> on_end_option;
	Callback1<void*> on_begin_group;
	Callback1<void*> on_end_group;
	Callback2<const String&, void*> on_numeric_value;
	Callback2<const String&, void*> on_prose_value;
*/
};

CWT_NS_END

#endif /* __CWT_ABNF_P_HPP__ */
