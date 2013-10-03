/*
 * abnf.cpp
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */


#include <cwt/fsm.hpp>
#include <cwt/hash.hpp>
#include <cwt/safeformat.hpp>
#include <iostream>
#include "abnf-rfc5234.hpp"

CWT_NS_BEGIN

static void __to_string_helper(std::ostream & os, const AbnfNode *parentNode)
{
	Vector<AbnfNode*>::const_iterator node = parentNode->m_nodes.begin();
	Vector<AbnfNode*>::const_iterator invalidNode = parentNode->m_nodes.end();

	String delim("");

	for(; node != invalidNode; node++) {
		AbnfNode *n = *node;
		CWT_ASSERT(n->m_type != Abnf_Rule);

		os << delim;

		switch(n->m_type) {
		case Abnf_Altern:
			if(n->m_nodes.size() > 0) {
				//printf(" _ALTER_{ ");
				__to_string_helper(os, *node);
				//printf(" } ");
			}
			break;

		case Abnf_Concat:
			if (n->m_nodes.size() > 0) {
				//printf(" _CONCAT_{ ");
				__to_string_helper(os, *node);
				//printf(" } ");
			}
			break;

		case Abnf_Rpt:
			if (n->m_nodes.size() > 0) {
				int from = dynamic_cast<AbnfNodeRpt*>(*node)->m_from;
				int to = dynamic_cast<AbnfNodeRpt*>(*node)->m_to;

//				printf("%d*%d", from, to);
				if (from == 1 && to == 1) {
					;
				} else if (from > 1 && to > 1 && from == to) {
					os << from;
				} else {
					if (from > 0)
						os << from;
					os << "*";
					if (to > 0)
						os << to;
				}
			}
			__to_string_helper(os, *node);
			break;

		case Abnf_Group:
			if (n->m_nodes.size() > 0) {
				os << "(";
				__to_string_helper(os, *node);
				os << ")";
			}
			break;

		case Abnf_Option:
			if(n->m_nodes.size() > 0) {
				os << "[";
				__to_string_helper(os, *node);
				os << "]";
			}
			break;

		case Abnf_RuleRef:
			os << dynamic_cast<AbnfNodeRuleRef *>(*node)->m_value;
			break;
		case Abnf_CharVal:
			os << '"' << dynamic_cast<AbnfNodeCharVal *>(*node)->m_value << '"';
			break;
		case Abnf_NumVal:
			os << dynamic_cast<AbnfNodeNumVal *>(*node)->m_value;
			break;
		case Abnf_ProseVal:
			os << dynamic_cast<AbnfNodeProseVal *>(*node)->m_value;
			break;
		default:
			break;
		}

		if (parentNode->m_type == Abnf_Altern)
			delim = String(" / ");
		else if (parentNode->m_type == Abnf_Concat)
			delim = String(" ");
	}
}

static void __to_string(std::ostream & os, AbnfContext *ctx)
{
	AbnfRulesHash::const_iterator it = ctx->rules.cbegin();
	AbnfRulesHash::const_iterator itEnd = ctx->rules.cend();
	for(; it != itEnd; ++it) {
		os << it.key() << " = ";

		Vector<AbnfNode*>::const_iterator node = it.value()->m_nodes.cbegin();
		Vector<AbnfNode*>::const_iterator invalidNode = it.value()->m_nodes.cend();

		for(; node != invalidNode; ++node) {
			__to_string_helper(os, *node);
		}

		os << std::endl;
	}
}


static bool begin_document(void * userContext)
{
	CWT_UNUSED(userContext);
	return true;
}

static bool end_document(bool status, void * userContext)
{
	AbnfContext *ctx = __CAST_USER_CTX(userContext);

	CWT_ASSERT(ctx->nodestack.size() == 0);

	if (status) {
		AbnfRulesDefsHash::const_iterator it = ctx->rulesdefs.cbegin();
		AbnfRulesDefsHash::const_iterator itEnd = ctx->rulesdefs.cend();
		AbnfRulesHash::const_iterator invalidRule = ctx->rules.cend();

		for(; it != itEnd; ++it) {
			if (!it.value()) {
				if (invalidRule == ctx->rules.find(it.key())) {
					Logger::warn(_Fr("Undefined rule: %s") % it.key());
				} else {
					Logger::trace(_Fr("Root rule: %s") % it.key());
				}
			}
		}
	}

	__to_string(std::cout, ctx);

	// delete all rules
	AbnfRulesHash::const_iterator it = ctx->rules.cbegin();
	AbnfRulesHash::const_iterator itEnd = ctx->rules.cend();
	for(; it != itEnd; ++it) {
		delete it.value();
	}

	return status;
}

static bool begin_rule(const String & rulename, bool incremental, void *userContext)
{
	AbnfContext *ctx = __CAST_USER_CTX(userContext);

	if (incremental) {
		AbnfRulesHash::iterator it = ctx->rules.find(rulename);
		if (it == ctx->rules.end()) { // rule for incremental alternative not initialized before
			Logger::error(_Fr("%s: rule for incremental alternative not initialized before") % rulename);
			return false;
		}
		ctx->nodestack.push(it.value());
		AbnfNode *alt = it.value()->m_nodes.first();
		CWT_ASSERT(alt);
		ctx->nodestack.push(alt);
		ctx->isIncrementalAlternation = true;
	} else {
		AbnfNodeRule *rule = new AbnfNodeRule(rulename);
		ctx->rules.insert(rulename, rule);
		ctx->nodestack.push(rule);

		AbnfRulesDefsHash::iterator it = ctx->rulesdefs.find(rulename);

		if (it != ctx->rulesdefs.end()) {
			it.value() = true;
		} else {
			ctx->rulesdefs.insert(rulename, false);
		}
	}

	std::cout << String(_F("RN{%s}%s") % rulename % (incremental ? "=/" : "="));
	return true;
}

static bool end_rule(void * userContext)
{
	AbnfContext *ctx = __CAST_USER_CTX(userContext);
	CWT_ASSERT(ctx->nodestack.size() > 0);

	if (ctx->isIncrementalAlternation ) {
		while (ctx->nodestack.top()->m_type != Abnf_Altern) {
			ctx->nodestack.pop();
		}
	}

	ctx->isIncrementalAlternation = false;

	while (ctx->nodestack.top()->m_type != Abnf_Rule) {
		ctx->nodestack.pop();
	}

	CWT_ASSERT(ctx->nodestack.size() > 0);
	CWT_ASSERT(ctx->nodestack.top()->m_type == Abnf_Rule);
	AbnfNodeRule *rule = dynamic_cast<AbnfNodeRule*>(ctx->nodestack.pop());
	CWT_UNUSED(rule);
	std::cout << std::endl;
	return true;
}


static bool begin_alternation(void * userContext)
{
	AbnfContext *ctx = __CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeAltern);
	std::cout << "{/";
	return true;
}

static bool __end_block(AbnfElementType type, AbnfContext * ctx)
{
	while (ctx->nodestack.top()->m_type != type) {
		ctx->nodestack.pop();
	}

	CWT_ASSERT(ctx->nodestack.size() > 0);
	CWT_ASSERT(ctx->nodestack.top()->m_type == type);
	AbnfNode *node = ctx->nodestack.pop();
	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(node);
	return true;
}


static bool end_alternation(void * userContext)
{
	std::cout << "/}";
	return __end_block(Abnf_Altern, __CAST_USER_CTX(userContext));
}

static bool begin_concatenation(void * userContext)
{
	std::cout << "{+";
	AbnfContext *ctx = __CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeConcat);
	return true;
}

static bool end_concatenation(void * userContext)
{
	std::cout << "+}";
	return __end_block(Abnf_Concat, __CAST_USER_CTX(userContext));
}

static bool begin_repetition(int from, int to, void * userContext)
{
	std::cout << String(_F("{%d*%d") % from % to);
	AbnfContext *ctx = __CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeRpt(from, to));
	return true;
}

static bool end_repetition(void * userContext)
{
	std::cout << "*}";
	return __end_block(Abnf_Rpt, __CAST_USER_CTX(userContext));
}

static bool begin_option(void * userContext)
{
	std::cout << "[";
	AbnfContext *ctx = __CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeOption);
	return true;
}

static bool end_option(void * userContext)
{
	std::cout << "]";
	return __end_block(Abnf_Option, __CAST_USER_CTX(userContext));
}

static bool begin_group(void * userContext)
{
	std::cout << "(";
	AbnfContext *ctx = __CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeGroup);
	return true;
}

static bool end_group(void * userContext)
{
	std::cout << ")";
	return __end_block(Abnf_Group, __CAST_USER_CTX(userContext));
}

static bool rule_ref(const String & rulename, void * userContext)
{
	std::cout << String(_F("RF{%s}") % rulename);

	AbnfContext *ctx = __CAST_USER_CTX(userContext);
	AbnfRulesDefsHash::iterator it = ctx->rulesdefs.find(rulename);

	if (it == ctx->rulesdefs.end()) {
		ctx->rulesdefs.insert(rulename, false);
	}

	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(new AbnfNodeRuleRef(rulename));

	return true;
}

static bool char_val(const String & s, void * userContext)
{
	std::cout << String(_F("CV{%s}") % s);
	AbnfContext *ctx = __CAST_USER_CTX(userContext);

	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(new AbnfNodeCharVal(s));
	return true;
}


static bool num_val(const String & num, void * userContext)
{
	std::cout << String(_F("NV{%s}") % num);
	AbnfContext *ctx = __CAST_USER_CTX(userContext);
	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(new AbnfNodeNumVal(num));
	return true;
}

static bool prose_val(const String & prose, void * userContext)
{
	std::cout << String(_F("PV{%s}") % prose);
	AbnfContext *ctx = __CAST_USER_CTX(userContext);
	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(new AbnfNodeProseVal(prose));
	return true;
}

/*
static bool comment(const String & comment, void * userContext)
{
	CWT_UNUSED2(comment, userContext);
	// FIXME comment parsing is incorrect
	std::cout << std::endl << String(_F("C{%s}") % comment) << std::endl;
	return true;
}
*/

static AbnfSimpleApi __default_api = {
	  begin_document
	, end_document
	, begin_rule
	, end_rule
	, begin_alternation
	, end_alternation
	, begin_concatenation
	, end_concatenation
	, begin_repetition
	, end_repetition
	, begin_option
	, end_option
	, begin_group
	, end_group
	, rule_ref
	, char_val
	, num_val
	, prose_val
};

bool Abnf::parse(const String &abnf)
{
	return parse(abnf, __default_api);
}


bool Abnf::parse(const String & abnf, AbnfSimpleApi & api)
{
	CWT_UNUSED(abnf);
	AbnfContext abnfContext;

	abnfContext.isIncrementalAlternation = false;

	AbnfParseContext ctx = {
		  & abnfContext
		, String()
		, {CWT_INT_MIN, CWT_INT_MIN}
		, api
	};

	Fsm<String> fsm(rulelist_fsm, & ctx);

	if (fsm.exec(0, abnf.begin(), abnf.end()) >= 0) {
		return true;
	}

	return false;
}

AbnfRuleSet::~AbnfRuleSet()
{
	Vector<AbnfRule *>::iterator it = m_rules.begin();
	Vector<AbnfRule *>::iterator itEnd = m_rules.end();

	for (; it != itEnd; ++it)
		delete *it;
}

AbnfRule * AbnfRuleSet::newRule (const String & name)
{
	AbnfRule * rule = new AbnfRule(name);
	size_t index = m_rules.size();
	m_rules.append(rule);
	m_rulesIndices.insert(name, index);
	return rule;
}


String AbnfRuleSet::toString () const
{
	Vector<AbnfRule *>::const_iterator it = m_rules.begin();
	Vector<AbnfRule *>::const_iterator itEnd = m_rules.end();

	String r;

	for (; it != itEnd; ++it) {
		r.append((*it)->toString());
		if (! r.endsWith(String::EndOfLine))
			r.append(String::EndOfLine);
		r.append(String::EndOfLine);
	}

	return r;
}

AbnfRule * AbnfRuleSet::find (const String & name)
{
	AbnfRule * r = nullptr;
	Hash<String, size_t>::iterator it = m_rulesIndices.find(name);
	if (it != m_rulesIndices.end()) {
		size_t i = it.value();
		CWT_ASSERT(i < m_rules.size());
		r = m_rules[i];
	}
	return r;
}

const AbnfRule * AbnfRuleSet::find (const String & name) const
{
	AbnfRule * r = nullptr;
	Hash<String, size_t>::const_iterator it = m_rulesIndices.find(name);
	if (it != m_rulesIndices.cend()) {
		size_t i = it.value();
		CWT_ASSERT(i < m_rules.size());
		r = m_rules[i];
	}
	return r;
}

AbnfAbstractContainer::~AbnfAbstractContainer()
{
	Vector<AbnfAbstractElement *>::iterator it = m_elements.begin();
	Vector<AbnfAbstractElement *>::iterator itEnd = m_elements.end();

	for (; it != itEnd; ++it) {
		delete *it;
	}
}

String AbnfAbstractContainer::toString (const String & separator) const
{
	Vector<AbnfAbstractElement *>::const_iterator it = m_elements.begin();
	Vector<AbnfAbstractElement *>::const_iterator itEnd = m_elements.end();

	String r;
	String sep;

	for (; it != itEnd; ++it) {
		if ((*it)->type() != Abnf_Comment)
			r.append(sep);
		r.append((*it)->toString());
		sep = separator;
	}

	return r;
}

String AbnfRpt::toString () const
{
	String r;

	if (0 == m_elements.size())
		return String();

	if (m_from < 0 && m_to < 0) {
		r.append(1, '*');
	} else {
		if (m_from > 0)
			r.append(String::number(m_from));
		if (m_from != m_to) {
			r.append(1, '*');
			if (m_to > 0) {
				r.append(String::number(m_to));
			}
		}
	}

	if (m_elements.size() == 1
			&& (m_elements[0]->isScalar()
					|| Abnf_Group == m_elements[0]->type()
					|| Abnf_Option == m_elements[0]->type())) {
		r.append(AbnfAbstractContainer::toString(" "));
	} else {
		r.append("( ");
		r.append(AbnfAbstractContainer::toString(" "));
		r.append(" )");
	}
	return r;
}

AbnfAltern & AbnfAbstractContainer::newAltern()
{
	AbnfAltern & altern = Abnf::newAltern();
	m_elements.append(& altern);
	return altern;
}

AbnfConcat & AbnfAbstractContainer::newConcat()
{
	AbnfConcat & concat = Abnf::newConcat();
	m_elements.append(& concat);
	return concat;
}

AbnfRpt & AbnfAbstractContainer::newRpt(int from, int to)
{
	AbnfRpt & rpt = Abnf::newRpt(from, to);
	m_elements.append(& rpt);
	return rpt;
}

AbnfRpt & AbnfAbstractContainer::newRpt (const String & ruleref, int from, int to)
{
	AbnfRpt & rpt = Abnf::newRpt(ruleref, from, to);
	m_elements.append(& rpt);
	return rpt;
}

AbnfGroup & AbnfAbstractContainer::newGroup ()
{
	CWT_TRACE_METHOD();
	AbnfGroup & group = Abnf::newGroup();
	m_elements.append(& group);
	return group;
}

AbnfOption & AbnfAbstractContainer::newOption ()
{
	AbnfOption & option = Abnf::newOption();
	m_elements.append(& option);
	return option;
}

CWT_NS_END
