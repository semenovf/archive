/*
 * abnf.cpp
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */


#include "../include/cwt/abnf.hpp"
#include <cwt/fsm.hpp>
#include <cwt/hash.hpp>
#include "abnf-rfc5234.hpp"

CWT_NS_BEGIN

#define _CAST_USER_CTX(userContext) reinterpret_cast<AbnfContext*>(userContext)

static void __to_string_helper(const AbnfNode *parentNode)
{
	Vector<AbnfNode*>::const_iterator node = parentNode->nodes.begin();
	Vector<AbnfNode*>::const_iterator invalidNode = parentNode->nodes.end();

	String delim(_U(""));

	for(; node != invalidNode; node++) {
		AbnfNode *n = *node;
		CWT_ASSERT(n->type != AbnfNode::Abnf_Rule);

		printf("%s", delim.toUtf8().data());

		switch(n->type) {
		case AbnfNode::Abnf_Alternation:
			if(n->nodes.size() > 0) {
				//printf(" _ALTER_{ ");
				__to_string_helper(*node);
				//printf(" } ");
			}
			break;

		case AbnfNode::Abnf_Concatenation:
			if (n->nodes.size() > 0) {
				//printf(" _CONCAT_{ ");
				__to_string_helper(*node);
				//printf(" } ");
			}
			break;

		case AbnfNode::Abnf_Repetition:
			if (n->nodes.size() > 0) {
				int from = dynamic_cast<AbnfNodeRepetition*>(*node)->from;
				int to = dynamic_cast<AbnfNodeRepetition*>(*node)->to;

//				printf("%d*%d", from, to);
				if (from == 1 && to == 1) {
					;
				} else if (from > 1 && to > 1 && from == to) {
					printf("%d", from);
				} else {
					if (from > 0)
						printf("%d", from);
					printf("*");
					if (to > 0)
						printf("%d", to);
				}
			}
			__to_string_helper(*node);
			break;

		case AbnfNode::Abnf_GroupElement:
			if (n->nodes.size() > 0) {
				printf("(");
				__to_string_helper(*node);
				printf(")");
			}
			break;

		case AbnfNode::Abnf_OptionElement:
			if(n->nodes.size() > 0) {
				printf("[");
				__to_string_helper(*node);
				printf("]");
			}
			break;

		case AbnfNode::Abnf_RuleRefElement:
			printf("%s", dynamic_cast<AbnfNodeRuleRefElement*>(*node)->value.toUtf8().data());
			break;
		case AbnfNode::Abnf_CharValElement:
			printf("\"%s\"", dynamic_cast<AbnfNodeCharValElement*>(*node)->value.toUtf8().data());
			break;
		case AbnfNode::Abnf_NumValElement:
			printf("%s", dynamic_cast<AbnfNodeNumValElement*>(*node)->value.toUtf8().data());
			break;
		case AbnfNode::Abnf_ProseValElement:
			printf("%s", dynamic_cast<AbnfNodeProseValElement*>(*node)->value.toUtf8().data());
			break;
		default:
			break;
		}

		if(parentNode->type == AbnfNode::Abnf_Alternation)
			delim = _U(" / ");
		else if(parentNode->type == AbnfNode::Abnf_Concatenation)
			delim = _U(" ");
	}
}

static void __to_string(AbnfContext *ctx)
{
	AbnfRulesHash::const_iterator it = ctx->rules.begin();
	AbnfRulesHash::const_iterator itEnd = ctx->rules.end();
	for(; it != itEnd; ++it) {
		printf("%s = ", it.key().toUtf8().data());

		Vector<AbnfNode*>::const_iterator node = it.value()->nodes.begin();
		Vector<AbnfNode*>::const_iterator invalidNode = it.value()->nodes.end();

		for(; node != invalidNode; ++node) {
			__to_string_helper((*node));
		}

		printf("\n");
	}
}


static bool begin_document(void *userContext)
{
	CWT_UNUSED(userContext);
	return true;
}

static bool end_document(bool status, void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);

	CWT_ASSERT(ctx->nodestack.size() == 0);

	if (status) {
		AbnfRulesDefsHash::const_iterator it = ctx->rulesdefs.begin();
		AbnfRulesDefsHash::const_iterator itEnd = ctx->rulesdefs.end();
		AbnfRulesHash::const_iterator invalidRule = ctx->rules.end();

		for(; it != itEnd; ++it) {
			if (!it.value()) {
				if (ctx->rules.find(it.key()) == invalidRule) {
					Logger::warn("undefined rule: %ls", it.key().data());
				} else {
					Logger::trace("root rule: %ls", it.key().data());
				}
			}
		}
	}

	__to_string(ctx);

	// delete all rules
	AbnfRulesHash::const_iterator it = ctx->rules.begin();
	AbnfRulesHash::const_iterator itEnd = ctx->rules.end();
	for(; it != itEnd; ++it) {
		delete it.value();
	}

	return status;
}

static bool begin_rule(const String &rulename, bool incremental, void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);

	if (incremental) {
		AbnfRulesHash::const_iterator it = ctx->rules.find(rulename);
		if (it == ctx->rules.end()) { // rule for incremental alternative not initialized before
			Logger::error("%ls: rule for incremental alternative not initialized before", rulename.data());
			return false;
		}
		ctx->nodestack.push(it.value());
		AbnfNode *alt = it.value()->nodes.first();
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

	printf("RN{%s}%s", rulename.toUtf8().data(), incremental ? "=/" : "=");
	return true;
}

static bool end_rule(void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	CWT_ASSERT(ctx->nodestack.size() > 0);

	if (ctx->isIncrementalAlternation ) {
		while (ctx->nodestack.top()->type != AbnfNode::Abnf_Alternation) {
			ctx->nodestack.pop();
		}
	}

	ctx->isIncrementalAlternation = false;

	while (ctx->nodestack.top()->type != AbnfNode::Abnf_Rule) {
		ctx->nodestack.pop();
	}

	CWT_ASSERT(ctx->nodestack.size() > 0);
	CWT_ASSERT(ctx->nodestack.top()->type == AbnfNode::Abnf_Rule);
	AbnfNodeRule *rule = dynamic_cast<AbnfNodeRule*>(ctx->nodestack.pop());
	CWT_UNUSED(rule);
	printf("\n");
	return true;
}


static bool begin_alternation(void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeAlternation);
	printf("{/");
	return true;
}

static bool __end_block(AbnfNode::AbnfNodeType type, AbnfContext *ctx)
{
	while (ctx->nodestack.top()->type != type) {
		ctx->nodestack.pop();
	}

	CWT_ASSERT(ctx->nodestack.size() > 0);
	CWT_ASSERT(ctx->nodestack.top()->type == type);
	AbnfNode *node = ctx->nodestack.pop();
	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(node);
	return true;
}


static bool end_alternation(void *userContext)
{
	printf("/}");
	return __end_block(AbnfNode::Abnf_Alternation, _CAST_USER_CTX(userContext));
}

static bool begin_concatenation(void *userContext)
{
	printf("{+");
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeConcatenation);
	return true;
}

static bool end_concatenation(void *userContext)
{
	printf("+}");
	return __end_block(AbnfNode::Abnf_Concatenation, _CAST_USER_CTX(userContext));
}

static bool begin_repetition(int from, int to, void *userContext)
{
	printf("{%d*%d", from, to);
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeRepetition(from, to));
	return true;
}

static bool end_repetition(void *userContext)
{
	printf("*}");
	return __end_block(AbnfNode::Abnf_Repetition, _CAST_USER_CTX(userContext));
}

static bool begin_option(void *userContext)
{
	printf("[");
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeOptionElement);
	return true;
}

static bool end_option(void *userContext)
{
	printf("]");
	return __end_block(AbnfNode::Abnf_OptionElement, _CAST_USER_CTX(userContext));
}

static bool begin_group(void *userContext)
{
	printf("(");
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	ctx->nodestack.push(new AbnfNodeGroupElement);
	return true;
}

static bool end_group(void *userContext)
{
	printf(")");
	return __end_block(AbnfNode::Abnf_GroupElement, _CAST_USER_CTX(userContext));
}

static bool rule_ref(const String &rulename, void *userContext)
{
	printf("RF{%s}", rulename.toUtf8().data());

	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	AbnfRulesDefsHash::iterator it = ctx->rulesdefs.find(rulename);

	if (it == ctx->rulesdefs.end()) {
		ctx->rulesdefs.insert(rulename, false);
	}

	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(new AbnfNodeRuleRefElement(rulename));

	return true;
}

static bool char_val(const String &s, void *userContext)
{
	printf("CV{%s}", s.toUtf8().data());
	AbnfContext *ctx = _CAST_USER_CTX(userContext);

	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(new AbnfNodeCharValElement(s));
	return true;
}


static bool num_val(const String &num, void *userContext)
{
	printf("NV{%s}", num.toUtf8().data());
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(new AbnfNodeNumValElement(num));
	return true;
}

static bool prose_val(const String &prose, void *userContext)
{
	printf("PV{%s}", prose.toUtf8().data());
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	CWT_ASSERT(ctx->nodestack.size() > 0);
	ctx->nodestack.top()->addNode(new AbnfNodeProseValElement(prose));
	return true;
}

static bool comment(const String &comment, void *userContext)
{
	CWT_UNUSED2(comment, userContext);
	// FIXME comment parsing is incorrect
	//printf("\nC{%s}\n", comment.toUtf8().data());
	return true;
}

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
	, comment
};

bool Abnf::parse(const String &abnf)
{
	return parse(abnf, __default_api);
}


bool Abnf::parse(const String &abnf, AbnfSimpleApi &api)
{
	CWT_UNUSED(abnf);
	AbnfContext abnfContext;

	abnfContext.isIncrementalAlternation = false;

	AbnfParseContext parseContext = {
		  &abnfContext
		, String()
		, {CWT_INT_MIN, CWT_INT_MIN}
		, api
	};

	Fsm<Char> fsm(rulelist_fsm, &parseContext);

	if (fsm.exec(0, abnf.data(), abnf.length()) == abnf.length()) {
		return true;
	}

	return false;
}

CWT_NS_END
