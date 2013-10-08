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

#ifdef __COMMENT__

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

#endif // __COMMENT__

/*static AbnfSimpleApi __default_api = {
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
};*/

bool Abnf::parse(const String & abnf, AbnfRuleList & ruleset)
{
	CWT_UNUSED(abnf);
	AbnfParseContext ctx = {
		  & ruleset
		, Stack<AbnfElement *>()
		, 10  // numValBase
		, 0   // numValMin
		, 0   // numValMax
	};

/*
	AbnfContext abnfContext;
	abnfContext.isIncrementalAlternation = false;
*/
	/*
	AbnfParseContext ctx = {
		  & abnfContext
		, String()
		, {CWT_INT_MIN, CWT_INT_MIN}
		, api
	};
*/
	Fsm<String> fsm(rulelist_fsm, & ctx);

	String s(abnf);

	if (!s.endsWith(String::EndOfLine))
		s.append(String::EndOfLine);

	if (fsm.exec(0, s.begin(), s.end()) >= 0) {
		return true;
	}

	return false;
}

AbnfRule & AbnfRuleList::newRule (const String & name)
{
	String rulename(name);
	rulename.replace("-", "_");
	AbnfRule * rule = new AbnfRule(rulename);
	size_t index = m_elements.size();
	this->add(*rule);

	CWT_ASSERT_X(m_rulesIndices.cend() == m_rulesIndices.find(rulename)
		, String(_Fr("Rule name must be unique: %s") % name).c_str());
	m_rulesIndices.insert(rulename, index);
	return *rule;
}


String AbnfRuleList::toString () const
{
	Vector<AbnfRule *>::const_iterator it = rules().cbegin();
	Vector<AbnfRule *>::const_iterator itEnd = rules().cend();

	String r;

	for (; it != itEnd; ++it) {
		r << (*it)->toString();
		if (! r.endsWith(String::EndOfLine))
			r << String::EndOfLine;
		r << String::EndOfLine;
	}

	return r;
}

AbnfRule * AbnfRuleList::find (const String & name)
{
	AbnfRule * r = nullptr;
	Hash<String, size_t>::iterator it = m_rulesIndices.find(name);
	Vector<AbnfRule *> & vrules = rules();
	if (it != m_rulesIndices.end()) {
		size_t i = it.value();
		CWT_ASSERT(i < vrules.size());
		r = vrules[i];
	}
	return r;
}

const AbnfRule * AbnfRuleList::find (const String & name) const
{
	AbnfRule * r = nullptr;
	Hash<String, size_t>::const_iterator it = m_rulesIndices.find(name);
	const Vector<AbnfRule *> & vrules = rules();
	if (it != m_rulesIndices.cend()) {
		size_t i = it.value();
		CWT_ASSERT(i < vrules.size());
		r = vrules[i];
	}
	return r;
}

AbnfContainer::~AbnfContainer()
{
	Vector<AbnfElement *>::iterator it = m_elements.begin();
	Vector<AbnfElement *>::iterator itEnd = m_elements.end();

	for (; it != itEnd; ++it) {
		delete *it;
	}
}

String AbnfContainer::toString (const String & separator) const
{
	Vector<AbnfElement *>::const_iterator it = m_elements.begin();
	Vector<AbnfElement *>::const_iterator itEnd = m_elements.end();

	String r;
	String sep;

	for (; it != itEnd; ++it) {
		if ((*it)->type() != Abnf_Comment)
			r << sep;
		r << (*it)->toString();
		sep = separator;
	}

	return r;
}

String AbnfRpt::toString () const
{
	String r;
	bool isOption = false;

	if (0 == m_elements.size())
		return String();

	// Option
	if (m_from <= 0 && m_to == 1) {
		r << "[ ";
		isOption = true;
	} else {
		if (m_from < 0 && m_to < 0) {
			r << '*';
		} else {
			if (m_from > 0)
				r << String::number(m_from);
			if (m_from != m_to) {
				r << '*';
				if (m_to > 0) {
					r << String::number(m_to);
				}
			}
		}
	}

	if (m_elements.size() == 1 && (m_elements[0]->isScalar())) {
		r << AbnfContainer::toString(" ");
	} else {
		r << (isOption ? "" : "( ")
		  << AbnfContainer::toString(" ")
		  << (isOption ? "" : " )");
	}

	if (isOption) {
		r << " ]";
	}

	return r;
}

AbnfAltern & AbnfContainer::newAltern()
{
	AbnfAltern & altern = Abnf::newAltern();
	this->add(altern);
	return altern;
}

AbnfConcat & AbnfContainer::newConcat()
{
	AbnfConcat & concat = Abnf::newConcat();
	this->add(concat);
	return concat;
}

AbnfRpt & AbnfContainer::newRpt(int from, int to)
{
	AbnfRpt & rpt = Abnf::newRpt(from, to);
	this->add(rpt);
	return rpt;
}

AbnfRpt & AbnfContainer::newRpt (const String & ruleref, int from, int to)
{
	AbnfRpt & rpt = Abnf::newRpt(ruleref, from, to);
	this->add(rpt);
	return rpt;
}

CWT_NS_END
