/*
 * safeformat.cpp
 *
 *  Created on: Jul 30, 2013
 *      Author: wladt
 */

#include "safeformat_p.hpp"
#include <cstring>

CWT_NS_BEGIN

static bool begin_spec  (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool plain_chars (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool set_flags   (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool set_width   (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool set_prec    (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool end_spec    (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool set_percent_char (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);
static bool bad_spec    (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *action_args);

static String _SF_DIGIT("0123456789");
static String _SF_FLAG_CHAR("0- +");
static String _SF_SPEC_CHAR("diouxXeEfFgGcsp");

/* exclude '%' (0x25) */
static FsmTransition<String> plain_char_fsm[] = {
      {-1, 1, FSM_MATCH_RANGE(0x20u, 0x24u)     , FSM_ACCEPT, nullptr, nullptr }
    , {-1, 2, FSM_MATCH_RANGE(0x26u, 0x10FFFFu) , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RANGE(0x01u, 0x19u)     , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition<String> plain_chars_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FSM(plain_char_fsm, 1,-1) , FSM_ACCEPT, plain_chars, nullptr }
};

static FsmTransition<String> percent_char_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR("%") , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR("%") , FSM_ACCEPT, set_percent_char, nullptr }
};

static FsmTransition<String> prec_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(".")                 , FSM_NORMAL, nullptr, nullptr }
    , { 2,-1, FSM_MATCH_OPT_CHAR("-")             , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_SF_DIGIT,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static FsmTransition<String> width_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR("123456789")         , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RPT_CHAR(_SF_DIGIT,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};


static FsmTransition<String> spec_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("%")                     , FSM_NORMAL, begin_spec, nullptr }
	, { 2, 5, FSM_MATCH_RPT_CHAR(_SF_FLAG_CHAR,-1,-1) , FSM_NORMAL, set_flags,  nullptr }
	, { 3, 5, FSM_MATCH_OPT_FSM(width_fsm)            , FSM_NORMAL, set_width,  nullptr }
	, { 4, 5, FSM_MATCH_OPT_FSM(prec_fsm)             , FSM_NORMAL, set_prec,  nullptr }
	, {-1, 5, FSM_MATCH_CHAR(_SF_SPEC_CHAR)           , FSM_ACCEPT, end_spec,  nullptr }
	, {-1,-1, FSM_MATCH_NOTHING                       , FSM_ACCEPT, bad_spec,  nullptr }
};

static FsmTransition<String> format_fsm[] = {
	  { 0, 1, FSM_MATCH_FSM(percent_char_fsm) , FSM_ACCEPT, nullptr, nullptr }
	, { 0, 2, FSM_MATCH_FSM(spec_fsm)         , FSM_ACCEPT, nullptr, nullptr }
	, { 0,-1, FSM_MATCH_FSM(plain_chars_fsm)  , FSM_ACCEPT, nullptr, nullptr }
};


bool begin_spec (const String::const_iterator & , const String::const_iterator & , void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	__clear_spec(ctx->spec);
	return true;
}

bool plain_chars (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	if (begin < end) {
		ctx->result.append(String(begin, end));
	}
	return true;
}

bool set_flags (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);

	String::const_iterator it(begin);
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
		++it;
	}
	return true;
}

bool set_width (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	if (begin < end) {
		bool ok = false;
		String width(begin, end);
		ctx->spec.width = width.toInt(&ok);
		CWT_ASSERT(ok);
	}
	return true;
}

// prec := '.' [ '-' ] *DIGIT
bool set_prec (const String::const_iterator & begin, const String::const_iterator & end, void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);

	String::const_iterator it(begin);
	++it; // skip period

	// If the precision is given as just '.', or the precision is negative,
	// the  precision is taken to be zero.
	if (it >= end || (it < end && *it == UChar('-'))) {
		ctx->spec.prec = -1; // implicit precision 0 as opposite to explicit 0
	} else {
		bool ok = true;
		String prec(it, end);
		ctx->spec.prec = prec.toInt(&ok);
		CWT_ASSERT(ok);
	}
	return true;
}

static const int __EXPECT_UNKNOWN  = 0;
static const int __EXPECT_DECIMAL  = 1;
static const int __EXPECT_FLOAT    = 2;
static const int __EXPECT_CHAR     = 3;
static const int __EXPECT_STRING   = 4;

bool end_spec (const String::const_iterator & begin, const String::const_iterator & end, void * context, void *)
{
	int expect = __EXPECT_UNKNOWN;
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	ctx->spec.spec_char = *begin;

	if (ctx->spec.spec_char == UChar('p'))
		ctx->spec.spec_char = UChar('x');

	if (ctx->argi >= ctx->bind_args.size()) {
		CWT_WARN(_Tr("More arguments expected according to format string:"));
		CWT_WARN(ctx->format.c_str());
		ctx->result.append(String(begin, end));
	} else {
		bool ok = false;
		String r;

		UniType ut = ctx->bind_args[ctx->argi++];

		switch (char(ctx->spec.spec_char)) {
		case 'd':
		case 'i':
			r.setNumber(ut.toLong(&ok), 10);
			expect = __EXPECT_DECIMAL;
			break;
		case 'o':
			r.setNumber(ut.toULong(&ok), 8);
			expect = __EXPECT_DECIMAL;
			break;
		case 'u':
			r.setNumber(ut.toULong(&ok), 10);
			expect = __EXPECT_DECIMAL;
			break;
		case 'x':
		//case 'p': - already replaced
			r.setNumber(ut.toULong(&ok), 16, false);
			expect = __EXPECT_DECIMAL;
			break;
		case 'X':
			r.setNumber(ut.toULong(&ok), 16, true); // uppercase
			expect = __EXPECT_DECIMAL;
			break;
		case 'e':
		case 'f':
		case 'g':
		case 'E':
		case 'F':
		case 'G':
			r.setNumber(ut.toDouble(&ok), char(ctx->spec.spec_char), ctx->spec.prec > 0 ? ctx->spec.prec : 0);
			expect = __EXPECT_FLOAT;
			break;
		case 'c':
			r = String(1, char(ut.toSByte(&ok)));
			expect = __EXPECT_CHAR;
			if (ctx->spec.flags & SafeFormat::ZeroPadding) {
				CWT_WARN(_Tr("'0' flag used with ‘%c’ specifier in format string"));
				CWT_WARN(ctx->format.c_str());
				ctx->spec.flags &= ~SafeFormat::ZeroPadding;
			}

			if (ctx->spec.flags & SafeFormat::NeedSign) {
				CWT_WARN(_Tr("'+' flag used with ‘%c’ specifier in format string"));
				CWT_WARN(ctx->format.c_str());
				ctx->spec.flags &= ~SafeFormat::NeedSign;
			}
			break;
		case 's':
			r = ut.toString(&ok);
			expect = __EXPECT_STRING;
			if (ctx->spec.flags & SafeFormat::ZeroPadding) {
				CWT_WARN(_Tr("'0' flag used with ‘%s’ specifier in format string"));
				CWT_WARN(ctx->format.c_str());
				ctx->spec.flags &= ~SafeFormat::ZeroPadding;
			}
			if (ctx->spec.flags & SafeFormat::NeedSign) {
				CWT_WARN(_Tr("'+' flag used with ‘%s’ specifier in format string"));
				CWT_WARN(ctx->format.c_str());
				ctx->spec.flags &= ~SafeFormat::NeedSign;
			}
			break;
		default:
			break;
		}

		CWT_ASSERT(expect != __EXPECT_UNKNOWN);

		if (!ok) {
			CWT_WARN(_Tr("Incompatible value for conversion specification in format string:"));
			CWT_WARN(ctx->format.c_str());
		} else {

			if (ctx->spec.flags & SafeFormat::ZeroPadding) {

				// If the 0 and - flags both appear, the 0 flag is ignored.
				if (ctx->spec.flags & SafeFormat::LeftJustify)
					ctx->spec.flags &= ~SafeFormat::ZeroPadding;

				// If a precision is given with a numeric conversion (d, i, o, u, x, and X), the 0 flag is ignored.
				if (ctx->spec.prec > -1
						&& (  ctx->spec.spec_char == UChar('d')
							|| ctx->spec.spec_char == UChar('i')
							|| ctx->spec.spec_char == UChar('o')
							|| ctx->spec.spec_char == UChar('u')
							|| ctx->spec.spec_char == UChar('x')
							|| ctx->spec.spec_char == UChar('X')
							))
					ctx->spec.flags &= ~SafeFormat::ZeroPadding;
			}

			// A + overrides a space if both are used
			if ((ctx->spec.flags & SafeFormat::NeedSign)
					&& (ctx->spec.flags & SafeFormat::SpaceBeforePositive))
				ctx->spec.flags &= ~SafeFormat::SpaceBeforePositive;

			if (expect == __EXPECT_DECIMAL || expect == __EXPECT_FLOAT) {
				bool isNegative = r.startsWith("-");

				// When 0 is printed with an explicit precision 0, the output is empty.
				if (ctx->spec.prec == 0 && r == String("0"))
					r.clear();

				// The precision, if any, gives the minimum number of digits that must appear;
				// if the converted value requires fewer digits, it is padded on the left with zeros.
				if (ctx->spec.prec > 0 && r.length() < size_t(ctx->spec.prec))
					r.prepend(String(ctx->spec.prec - r.length(), '0'));

				if (! isNegative) {
					// A sign (+ or -) should always be placed before a number produced by a signed conversion
					if (ctx->spec.flags & SafeFormat::NeedSign) {
						r.prepend(String("+"));
					}
					// A blank should be left before a positive number
					else if (ctx->spec.flags & SafeFormat::SpaceBeforePositive) {
						r.prepend(String(" "));
					}
				}

			}

			// If the converted value has fewer characters than the field width, it will be padded with spaces
			// on the left (or right, if the left-adjustment flag has been given).
			CWT_ASSERT(ctx->spec.width >= 0);
			if (r.length() < size_t(ctx->spec.width)) {
				size_t count = size_t(ctx->spec.width) - r.length();
				char paddingChar = (ctx->spec.flags & SafeFormat::ZeroPadding) ? '0' : ' ';

				if (ctx->spec.flags & SafeFormat::LeftJustify)
					r.append(String(count, paddingChar));
				else
					r.prepend(String(count, paddingChar));
			}

		}

		ctx->result.append(r);
	}

	return true;
}

bool set_percent_char (const String::const_iterator & , const String::const_iterator & , void * context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	ctx->result.append(String(1, '%'));
	return true;
}

bool bad_spec (const String::const_iterator & , const String::const_iterator & , void *context, void *)
{
	SafeFormatContext *ctx = reinterpret_cast<SafeFormatContext *>(context);
	CWT_ERROR(_Tr("Bad conversion specification in format string:"));
	CWT_ERROR(ctx->format.c_str());
	return false;
}

SafeFormat::SafeFormat(const String & format)
	: m_context(new SafeFormatContext)
{
	__clear_context(*m_context);
	m_context->format = format;
}

SafeFormat::operator String & ()
{
	Fsm<String> fsm(format_fsm, m_context.get());
	ssize_t n = fsm.exec(0, m_context->format.begin(), m_context->format.end());
	if (n < 0) {
		;
	} else {
		// add tail
		m_context->result.append(String(m_context->format.begin() + size_t(n), m_context->format.end()));
	}
	return m_context->result;
}


SafeFormat & SafeFormat::operator () (const UniType & ut)
{
	m_context->bind_args.append(ut);
	return *this;
}

CWT_NS_END
