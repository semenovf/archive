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

static bool begin_rule(const String &rulename, bool incremental, void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);

	// FIXME incremental support
	if (!incremental)
		ctx->rulestack.push(new AbnfNodeRule(rulename));
	printf("RN{%s}%s", rulename.toUtf8().data(), incremental ? "=/" : "=");
	CWT_UNUSED(ctx);
	//ctx->rn.clear();
	return true;
}

static bool end_rule(void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	CWT_ASSERT(ctx->rulestack.size() > 0);
	CWT_ASSERT(ctx->rulestack.top()->type == AbnfNode::Abnf_Rule);
	AbnfNodeRule *rule = dynamic_cast<AbnfNodeRule*>(ctx->rulestack.pop());
	ctx->rules.insert(rule->name, rule);
	printf("\n");
	return true;
}


static bool begin_alternation(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("{/");
	return true;
}

static bool end_alternation(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("/}");
	return true;
}

static bool begin_concatenation(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("{+");
	return true;
}

static bool end_concatenation(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("+}");
	return true;
}

static bool begin_repetition(int from, int to, void *userContext)
{
	CWT_UNUSED(userContext);
	printf("{%d*%d", from, to);
	return true;
}

static bool end_repetition(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("*}");
	return true;
}

static bool begin_option(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("[");
	return true;
}

static bool end_option(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("]");
	return true;
}

static bool begin_group(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("(");
	return true;
}

static bool end_group(void *userContext)
{
	CWT_UNUSED(userContext);
	printf(")");
	return true;
}

static bool rule_ref(const String &rulename, void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	printf("RF{%s}", rulename.toUtf8().data());
	Hash<String, AbnfNode*>::const_iterator it = ctx->rules.find(rulename);
	if (it != ctx->rules.end()) {
		dynamic_cast<AbnfNodeRule*>(it.value())->refs++;
	} else {
		ctx->rulestack.push(new AbnfNodeRule(rulename));
	}
	return true;
}

static bool char_val(const String &ch, void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	printf("CV{%s}", ch.toUtf8().data());
	CWT_UNUSED(ctx);
	return true;
}


static bool num_val(const String &num, void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	printf("NV{%s}", num.toUtf8().data());
	CWT_UNUSED(ctx);
	return true;
}

static bool prose_val(const String &prose, void *userContext)
{
	AbnfContext *ctx = _CAST_USER_CTX(userContext);
	printf("PV{%s}", prose.toUtf8().data());
	CWT_UNUSED(ctx);
	return true;
}


static bool comment(const String &comment, void *userContext)
{
	CWT_UNUSED2(comment, userContext);
	// FIXME comment parsing is incorrect
	//printf("\nC{%s}\n", comment.toUtf8().data());
	return true;
}

bool Abnf::parse(const String &abnf)
{
	CWT_UNUSED(abnf);
	AbnfContext abnfContext;

	AbnfParseContext parseContext = {
		  &abnfContext
		, String()
		, {1, 1}
		, Callback3<const String&, bool, void*>(begin_rule)
		, Callback1<void*>               (end_rule)
		, Callback1<void*>               (begin_alternation)
		, Callback1<void*>               (end_alternation)
		, Callback1<void*>               (begin_concatenation)
		, Callback1<void*>               (end_concatenation)
		, Callback3<int, int, void*>     (begin_repetition)
		, Callback1<void*>               (end_repetition)
		, Callback1<void*>               (begin_option)
		, Callback1<void*>               (end_option)
		, Callback1<void*>               (begin_group)
		, Callback1<void*>               (end_group)
		, Callback2<const String&, void*>(rule_ref)
		, Callback2<const String&, void*>(char_val)
		, Callback2<const String&, void*>(num_val)
		, Callback2<const String&, void*>(prose_val)
		, Callback2<const String&, void*>(comment)
	};

	Fsm<Char> fsm(rulelist_fsm, &parseContext);

	if (fsm.exec(0, abnf.data(), abnf.length()) == abnf.length()) {
		Hash<String, AbnfNode*>::const_iterator it = abnfContext.rules.begin();
		Hash<String, AbnfNode*>::const_iterator itEnd = abnfContext.rules.end();

		for(; it != itEnd; ++it) {
			if (dynamic_cast<AbnfNodeRule*>(it.value())->refs == 1)
				Logger::trace("Root rule: %ls", it.key().data());
		}

		return true;
	}

	return false;
}

CWT_NS_END
