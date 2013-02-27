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
		, Abnf_GroupElement
		, Abnf_OptionElement
		, Abnf_RuleRefElement
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
	AbnfNodeRule(const String &s) : AbnfNode(Abnf_Rule), name(s) {}
	String name;
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
class AbnfNodeGroupElement : public AbnfNode {
public:
	AbnfNodeGroupElement() : AbnfNode(Abnf_GroupElement) {}
};
class AbnfNodeOptionElement : public AbnfNode {
public:
	AbnfNodeOptionElement() : AbnfNode(Abnf_OptionElement) {}
};
class AbnfNodeRuleRefElement : public AbnfNode {
public:
	AbnfNodeRuleRefElement(const String &v) : AbnfNode(Abnf_RuleRefElement), value(v) {}
	String value;
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

typedef Hash<String, AbnfNode*> AbnfRulesHash;
typedef Hash<String, bool>      AbnfRulesDefsHash;
typedef Stack<AbnfNode*>        AbnfNodesStack;

struct AbnfContext {
	AbnfRulesHash     rules;
	AbnfRulesDefsHash rulesdefs; // if rule is defined
	AbnfNodesStack    nodestack;
	bool isIncrementalAlternation;
};

struct AbnfParseContext {
	void      *userContext;
	String    rulename; // rulename for the current rule
	struct    { int from, to; } rpt;
	AbnfSimpleApi &api;
};

CWT_NS_END

#endif /* __CWT_ABNF_P_HPP__ */
