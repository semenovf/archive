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

inline String TRANSITION_TABLE_NAME(const String & rulename, int count = -1)
{
	return rulename
			+ (count >= 0 ? String(1, '_') + String::number(count) : String())
			+ "_fsm";
}

#define __CAST_USER_CTX(userContext)   static_cast<AbnfContext *>(userContext)
#define __CAST_PARSE_CTX(parseContext) static_cast<AbnfParseContext *>(parseContext)

class AbnfNode
{
public:

	AbnfNode(AbnfElementType t) : m_type(t) {}
	virtual ~AbnfNode() {}
	void addNode(AbnfNode * node) { m_nodes.append(node); }
	AbnfElementType ntype() const { return m_type; }

	AbnfElementType    m_type;
	Vector<AbnfNode*> m_nodes;
};

class AbnfNodeRule : public AbnfNode
{
public:
	AbnfNodeRule(const String & s) : AbnfNode(Abnf_Rule), m_name(s) {}
	String m_name;
};

class AbnfNodeAltern : public AbnfNode
{
public:
	AbnfNodeAltern() : AbnfNode(Abnf_Altern) {}
};

class AbnfNodeConcat : public AbnfNode
{
public:
	AbnfNodeConcat() : AbnfNode(Abnf_Concat) {}
};

class AbnfNodeRpt : public AbnfNode
{
public:
	AbnfNodeRpt(int f, int t) : AbnfNode(Abnf_Rpt), m_from(f), m_to(t) {}
	int m_from, m_to;
};

class AbnfNodeGroup : public AbnfNode
{
public:
	AbnfNodeGroup() : AbnfNode(Abnf_Group) {}
};

class AbnfNodeOption : public AbnfNode
{
public:
	AbnfNodeOption() : AbnfNode(Abnf_Option) {}
};

class AbnfNodeRuleRef : public AbnfNode
{
public:
	AbnfNodeRuleRef(const String & v) : AbnfNode(Abnf_RuleRef), m_value(v) {}
	String m_value;
};

class AbnfNodeCharVal : public AbnfNode
{
public:
	AbnfNodeCharVal(const String & v) : AbnfNode(Abnf_CharVal), m_value(v) {}
	String m_value;
};

class AbnfNodeNumVal : public AbnfNode
{
public:
	AbnfNodeNumVal(const String & v) : AbnfNode(Abnf_NumVal), m_value(v) {}
	String m_value;
};

class AbnfNodeProseVal : public AbnfNode
{
public:
	AbnfNodeProseVal(const String & v) : AbnfNode(Abnf_ProseVal), m_value(v) {}
	String m_value;
};

typedef Hash<String, AbnfNode *> AbnfRulesHash;
typedef Hash<String, bool>       AbnfRulesDefsHash;
typedef Stack<AbnfNode *>        AbnfNodesStack;

struct AbnfContext
{
	AbnfRulesHash     rules;
	AbnfRulesDefsHash rulesdefs; // if rule is defined
	AbnfNodesStack    nodestack;
	bool isIncrementalAlternation;
};

struct AbnfParseContext
{
	void *    userContext;
	String    rulename; // rulename for the current rule
	struct    { int from, to; } rpt;
	AbnfSimpleApi & api;
};

CWT_NS_END

#endif /* __CWT_ABNF_P_HPP__ */
