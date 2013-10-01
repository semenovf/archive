/**
 * @file   abnf_p.hpp
 * @author wladt
 * @date   Feb 18, 2013 9:10:27 AM
 *
 * @brief
 */

#ifndef __CWT_ABNF_P_HPP__
#define __CWT_ABNF_P_HPP__

#include "../include/cwt/abnf.hpp"
#include <cwt/vector.hpp>
#include <cwt/hash.hpp>
#include <cwt/stack.hpp>

CWT_NS_BEGIN

#define __CAST_USER_CTX(userContext)   static_cast<AbnfContext *>(userContext)
#define __CAST_PARSE_CTX(parseContext) static_cast<AbnfParseContext *>(parseContext)

class AbnfNode
{
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

	AbnfNode(AbnfNodeType t) : m_type(t) {}
	virtual ~AbnfNode() {}
	void addNode(AbnfNode *node) { m_nodes.append(node); }
	AbnfNodeType ntype() const { return m_type; }

	AbnfNodeType      m_type;
	Vector<AbnfNode*> m_nodes;
};

class AbnfNodeRule : public AbnfNode
{
public:
	AbnfNodeRule(const String &s) : AbnfNode(Abnf_Rule), m_name(s) {}
	String m_name;
};

class AbnfNodeAlternation : public AbnfNode
{
public:
	AbnfNodeAlternation() : AbnfNode(Abnf_Alternation) {}
};

class AbnfNodeConcatenation : public AbnfNode
{
public:
	AbnfNodeConcatenation() : AbnfNode(Abnf_Concatenation) {}
};

class AbnfNodeRepetition : public AbnfNode
{
public:
	AbnfNodeRepetition(int f, int t) : AbnfNode(Abnf_Repetition), m_from(f), m_to(t) {}
	int m_from, m_to;
};

class AbnfNodeGroupElement : public AbnfNode
{
public:
	AbnfNodeGroupElement() : AbnfNode(Abnf_GroupElement) {}
};

class AbnfNodeOptionElement : public AbnfNode
{
public:
	AbnfNodeOptionElement() : AbnfNode(Abnf_OptionElement) {}
};

class AbnfNodeRuleRefElement : public AbnfNode
{
public:
	AbnfNodeRuleRefElement(const String &v) : AbnfNode(Abnf_RuleRefElement), m_value(v) {}
	String m_value;
};

class AbnfNodeCharValElement : public AbnfNode
{
public:
	AbnfNodeCharValElement(const String &v) : AbnfNode(Abnf_CharValElement), m_value(v) {}
	String m_value;
};

class AbnfNodeNumValElement : public AbnfNode
{
public:
	AbnfNodeNumValElement(const String &v) : AbnfNode(Abnf_NumValElement), m_value(v) {}
	String m_value;
};

class AbnfNodeProseValElement : public AbnfNode
{
public:
	AbnfNodeProseValElement(const String &v) : AbnfNode(Abnf_ProseValElement), m_value(v) {}
	String m_value;
};

typedef Hash<String, AbnfNode*> AbnfRulesHash;
typedef Hash<String, bool>      AbnfRulesDefsHash;
typedef Stack<AbnfNode*>        AbnfNodesStack;

struct AbnfContext
{
	AbnfRulesHash     rules;
	AbnfRulesDefsHash rulesdefs; // if rule is defined
	AbnfNodesStack    nodestack;
	bool isIncrementalAlternation;
};

struct AbnfParseContext
{
	void      *userContext;
	String    rulename; // rulename for the current rule
	struct    { int from, to; } rpt;
	AbnfSimpleApi & api;
};

CWT_NS_END

#endif /* __CWT_ABNF_P_HPP__ */
