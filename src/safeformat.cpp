/*
 * safeformat.cpp
 *
 *  Created on: Jul 30, 2013
 *      Author: wladt
 */

#include "safeformat_p.hpp"
#include <cstring>

CWT_NS_BEGIN

static bool begin_spec (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool set_flags (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool set_width (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool set_prec (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool set_spec_char (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool bad_spec (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);

static String _SF_DIGIT("0123456789");
static String _SF_FLAG_CHAR("0- +");
static String _SF_SPEC_CHAR("diouxXeEfFgGcsp");

/* exclude '%' (0x25) */
static UChar plain_char[] = {
	  0x20, 0x24
	, 0x26, 0x10FFFF
};

static FsmTransition<String> plain_char_fsm[] = {
      {-1, 1, FSM_MATCH_RANGE(plain_char[0], plain_char[1]) , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RANGE(plain_char[2], plain_char[3]) , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition<String> plain_str_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FSM(plain_char_fsm, 1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition<String> percent_char_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR("%") , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR("%") , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition<String> prec_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(".")                 , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_SF_DIGIT, 1,-1) , FSM_ACCEPT, set_prec, nullptr }
};

static FsmTransition<String> spec_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("%")                     , FSM_NORMAL, begin_spec, nullptr }
	, { 2, 5, FSM_MATCH_RPT_CHAR(_SF_FLAG_CHAR,-1,-1) , FSM_NORMAL, set_flags,  nullptr }
	, { 3, 5, FSM_MATCH_RPT_CHAR(_SF_DIGIT,-1,-1)     , FSM_NORMAL, set_width,  nullptr }
	, { 4, 5, FSM_MATCH_OPT_FSM(prec_fsm)             , FSM_NORMAL, set_width,  nullptr }
	, {-1, 5, FSM_MATCH_CHAR(_SF_SPEC_CHAR)           , FSM_ACCEPT, set_spec_char,  nullptr }
	, {-1,-1, FSM_MATCH_NOTHING                       , FSM_ACCEPT, bad_spec,  nullptr }
};

bool begin_spec (const String::const_iterator & , const String::const_iterator & , void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	__clear_spec(ctx->spec);
	return true;
}

bool set_flags (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);

	const String::const_iterator it(begin);
	while (it < end) {
		switch (char(*it)) {
		case '0':
			ctx->spec.flags |= SafeFormat::ZeroPadding;
			break;
		case '-':
			ctx->spec.flags |= SafeFormat::LeftJustify;
			break;
		case ' ':
			ctx->spec.flags |= SafeFormat::SpaceBeforePositive;
			break;
		case '+':
			ctx->spec.flags |= SafeFormat::NeedSign;
			break;
		default:
			break;
		}
	}
	return true;
}

bool set_width (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	if (begin < end) {
		bool ok = false;
		String width(begin, end);
		ctx->spec.field_width = width.toInt(&ok);
		CWT_ASSERT(ok);
	}
	return true;
}

bool set_prec (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	bool ok = false;
	String prec(begin, end);
	ctx->spec.prec = prec.toInt(&ok);
	CWT_ASSERT(ok);
	return true;
}

bool set_spec_char (const String::const_iterator & begin, const String::const_iterator & , void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	ctx->spec.spec_char = *begin;
	return true;
}

bool bad_spec (const String::const_iterator & , const String::const_iterator & , void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	CWT_ERROR(_Tr("Bad conversion specification in format string: >>"));
	CWT_ERROR(ctx->format.c_str());
	return false;
}

SafeFormat::SafeFormat(const String & format)
	: m_context(new SafeFormatContext)
{
	__clear_context(*m_context);
	m_context->format = format;
	m_context->cursor = m_context->format.begin();
	m_context->end = m_context->format.end();
	m_context->fsm.setUserContext(m_context.get());
}

SafeFormat::operator String & ()
{
	if (m_context->cursor < m_context->end)
		m_context->result.append(String(m_context->cursor, m_context->end));
	return m_context->result;
}

bool SafeFormat::nextArg()
{
	if (m_context->cursor < m_context->end) {
		m_context->fsm.setTransitionTable(plain_str_fsm);

		ssize_t r = m_context->fsm.exec(0, m_context->cursor, m_context->end);
		if (r > 0) {
			m_context->result.append(String(m_context->cursor, m_context->cursor + size_t(r)));
			m_context->cursor += size_t(r);
		}

		m_context->fsm.setTransitionTable(percent_char_fsm);
		r = m_context->fsm.exec(0, m_context->cursor, m_context->end);

		if ( r > 0 ) {
			m_context->cursor += size_t(r);
			m_context->result.append(String(1, '%'));
		} else {
			m_context->fsm.setTransitionTable(spec_fsm);
			r = m_context->fsm.exec(0, m_context->cursor, m_context->end);

			if (r > 0) {
				m_context->cursor += size_t(r);
				return true;
			}
		}
	}
	return false;
}

SafeFormat & SafeFormat::operator () (const UniType & ut)
{
	if (nextArg()) {
		bool ok = true;
		String r;

		switch (char(m_context->spec.spec_char)) {
		case 'd':
		case 'i':
			r.setNumber(ut.toLong(&ok), 10);
			break;
		case 'o':
			r.setNumber(ut.toULong(&ok), 8);
			break;
		case 'u':
			r.setNumber(ut.toULong(&ok), 10);
			break;
		case 'x':
		case 'p':
			r.setNumber(ut.toULong(&ok), 16, false);
			break;
		case 'X':
			r.setNumber(ut.toULong(&ok), 16, true); // uppercase
			break;
		case 'e':
		case 'E':
		case 'f':
		case 'F':
		case 'g':
		case 'G':
			r.setNumber(ut.toDouble(&ok), char(m_context->spec.spec_char), m_context->spec.prec);
			break;
		case 'c':
			r = String(1, char(ut.toSByte(&ok)));
			break;
		case 's':
			r = ut.toString(&ok);
			break;
		}

		if (!ok) {
			CWT_WARN(_Tr("Incompatable value for conversion specification in format string: >>"));
			CWT_WARN(m_context->format.c_str());
		}
		m_context->result.append(r);
	}
	return *this;
}

CWT_NS_END
