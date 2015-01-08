/**
 * @file safeformat.cpp
 *
 * Created on: Jul 30, 2013
 * Modified on: Nov 01, 2014
 * @author wladt
 */

#include "safeformat_p.hpp"
#include <cstring>

/*
 * conversion_specification := '%' *flag [ field_width ] [ prec ] conversion_specifier
 * flag := '0' ; the value should be zero padded (ignored when 'prec' is specified)
 * 		 / '-' ; the value is left-justified
 * 		 / ' ' ; the value should be padded whith spaces (ignored when 'prec' is specified)
 * 		 / '+'
 *
 * field_width := 1*DIGIT
 * prec := '.' [ '-' ] *DIGIT
 * conversion_specifier := 'd' / 'i' / 'o' / 'u' / 'x' / 'X'
 * 		 / 'e' / 'E' / 'f' / 'F' / 'g' / 'G'
 * 		 / 'c' / 's'
 * 		 / 'p'
 * */
namespace pfs {

static bool begin_spec  (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void *context, void *action_args);
static bool plain_chars (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void *context, void *action_args);
static bool set_flags   (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void *context, void *action_args);
static bool set_width   (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void *context, void *action_args);
static bool set_prec    (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void *context, void *action_args);
static bool end_spec    (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void *context, void *action_args);
static bool set_percent_char (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void *context, void *action_args);
static bool bad_spec    (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void *context, void *action_args);

static pfs::string SF_DIGIT("0123456789");
static pfs::string SF_FLAG_CHAR("0- +");
static pfs::string SF_SPEC_CHAR("diouxXeEfFgGcsp");

/* exclude '%' (0x25) */
static pfs::fsm::transition<pfs::string> plain_char_fsm[] = {
      {-1, 1, FSM_MATCH_RANGE(pfs::ucchar(0x20u), pfs::ucchar(0x24u))     , FSM_ACCEPT, nullptr, nullptr }
    , {-1, 2, FSM_MATCH_RANGE(pfs::ucchar(0x26u), pfs::ucchar(0x10FFFFu)) , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RANGE(pfs::ucchar(0x01u), pfs::ucchar(0x19u))     , FSM_ACCEPT, nullptr, nullptr }
};

static pfs::fsm::transition<pfs::string> plain_chars_fsm[] = {
      {-1,-1, FSM_MATCH_RPT_FSM(plain_char_fsm, 1,-1) , FSM_ACCEPT, plain_chars, nullptr }
};

static pfs::fsm::transition<pfs::string> percent_char_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_l1("%")) , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_l1("%")) , FSM_ACCEPT, set_percent_char, nullptr }
};

static pfs::fsm::transition<pfs::string> prec_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_l1("."))           , FSM_NORMAL, nullptr, nullptr }
    , { 2,-1, FSM_MATCH_OPT_CHAR(_l1("-"))       , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RPT_CHAR(SF_DIGIT,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};

static pfs::fsm::transition<pfs::string> width_fsm[] = {
      { 1,-1, FSM_MATCH_CHAR(_l1("123456789"))   , FSM_NORMAL, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_RPT_CHAR(SF_DIGIT,-1,-1) , FSM_ACCEPT, nullptr, nullptr }
};


static pfs::fsm::transition<pfs::string> spec_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_l1("%"))               , FSM_NORMAL, begin_spec, nullptr }
	, { 2, 5, FSM_MATCH_RPT_CHAR(SF_FLAG_CHAR,-1,-1) , FSM_NORMAL, set_flags,  nullptr }
	, { 3, 5, FSM_MATCH_OPT_FSM(width_fsm)           , FSM_NORMAL, set_width,  nullptr }
	, { 4, 5, FSM_MATCH_OPT_FSM(prec_fsm)            , FSM_NORMAL, set_prec,  nullptr }
	, {-1, 5, FSM_MATCH_CHAR(SF_SPEC_CHAR)           , FSM_ACCEPT, end_spec,  nullptr }
	, {-1,-1, FSM_MATCH_NOTHING                      , FSM_ACCEPT, bad_spec,  nullptr }
};

static pfs::fsm::transition<pfs::string> format_fsm[] = {
	  { 0, 1, FSM_MATCH_FSM(percent_char_fsm) , FSM_ACCEPT, nullptr, nullptr }
	, { 0, 2, FSM_MATCH_FSM(spec_fsm)         , FSM_ACCEPT, nullptr, nullptr }
	, { 0,-1, FSM_MATCH_FSM(plain_chars_fsm)  , FSM_ACCEPT, nullptr, nullptr }
};


bool begin_spec (const pfs::string::const_iterator & , const pfs::string::const_iterator & , void * context, void *)
{
	safeformat_context *ctx = reinterpret_cast<safeformat_context *>(context);
	clear_conversion_spec(ctx->spec);
	return true;
}

bool plain_chars (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void * context, void *)
{
	safeformat_context *ctx = reinterpret_cast<safeformat_context *>(context);
	if (begin < end) {
		ctx->result.append(pfs::string(begin, end));
	}
	return true;
}

bool set_flags (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void * context, void *)
{
	safeformat_context *ctx = reinterpret_cast<safeformat_context *>(context);

	pfs::string::const_iterator it(begin);
	while (it < end) {
		switch (char(it->value())) {
		case '0':
			ctx->spec.flags |= safeformat::ZeroPadding;
			break;
		case '-':
			ctx->spec.flags |= safeformat::LeftJustify;
			break;
		case ' ':
			ctx->spec.flags |= safeformat::SpaceBeforePositive;
			break;
		case '+':
			ctx->spec.flags |= safeformat::NeedSign;
			break;
		default:
			break;
		}
		++it;
	}
	return true;
}

bool set_width (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void * context, void *)
{
	safeformat_context *ctx = reinterpret_cast<safeformat_context *>(context);
	if (begin < end) {
		bool ok = false;
		pfs::string width(begin, end);
		ctx->spec.width = width.toInt(&ok);
		PFS_ASSERT(ok);
	}
	return true;
}

// prec := '.' [ '-' ] *DIGIT
bool set_prec (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void * context, void *)
{
	safeformat_context *ctx = reinterpret_cast<safeformat_context *>(context);

	pfs::string::const_iterator it(begin);
	++it; // skip period

	// If the precision is given as just '.', or the precision is negative,
	// the  precision is taken to be zero.
	if (it >= end || (it < end && *it == pfs::ucchar('-'))) {
		ctx->spec.prec = -1; // implicit precision 0 as opposite to explicit 0
	} else {
		bool ok = true;
		pfs::string prec(it, end);
		ctx->spec.prec = prec.toInt(&ok);
		PFS_ASSERT(ok);
	}
	return true;
}

static const int __EXPECT_UNKNOWN  = 0;
static const int __EXPECT_DECIMAL  = 1;
static const int __EXPECT_FLOAT    = 2;
static const int __EXPECT_CHAR     = 3;
static const int __EXPECT_STRING   = 4;

bool end_spec (const pfs::string::const_iterator & begin, const pfs::string::const_iterator & end, void * context, void *)
{
	int expect = __EXPECT_UNKNOWN;
	safeformat_context *ctx = reinterpret_cast<safeformat_context *>(context);
	ctx->spec.spec_char = *begin;

	if (ctx->spec.spec_char == pfs::ucchar('p'))
		ctx->spec.spec_char = pfs::ucchar('x');

	bool ok = PFS_VERIFY_X(
			PFS_VERIFY_X(ctx->argi < ctx->bind_args.size()
				, _Tr("More arguments expected according to format string:"))
			, ctx->format.c_str());

	if (! ok) {
		ctx->result.append(pfs::string(begin, end));
	} else {
		pfs::string r;

		pfs::unitype ut = ctx->bind_args[ctx->argi++];

		switch (char(ctx->spec.spec_char)) {
		case 'd':
		case 'i':
			r.setNumber(ut.toInteger(), 10);
			expect = __EXPECT_DECIMAL;
			break;
		case 'o':
			r.setNumber(ulong_t(ut.toInteger()), 8);
			expect = __EXPECT_DECIMAL;
			break;
		case 'u':
			r.setNumber(ulong_t(ut.toInteger()), 10);
			expect = __EXPECT_DECIMAL;
			break;
		case 'x':
		//case 'p': - already replaced
			r.setNumber(ulong_t(ut.toInteger()), 16, false);
			expect = __EXPECT_DECIMAL;
			break;
		case 'X':
			r.setNumber(ulong_t(ut.toInteger()), 16, true); // uppercase
			expect = __EXPECT_DECIMAL;
			break;
		case 'e':
		case 'f':
		case 'g':
		case 'E':
		case 'F':
		case 'G':
			r.setNumber(ut.toFloat(), char(ctx->spec.spec_char), ctx->spec.prec > 0 ? ctx->spec.prec : 0);
			expect = __EXPECT_FLOAT;
			break;
		case 'c':
			r = pfs::string(1, char(ut.toInteger()));
			expect = __EXPECT_CHAR;

			ok = PFS_VERIFY_X(
				PFS_VERIFY_X(!(ctx->spec.flags & safeformat::ZeroPadding)
					, _Tr("'0' flag used with ‘%c’ specifier in format string:"))
				, ctx->format.c_str());

			if (!ok)
				ctx->spec.flags &= ~safeformat::ZeroPadding;

			ok = PFS_VERIFY_X(
				PFS_VERIFY_X(!(ctx->spec.flags & safeformat::NeedSign)
					, _Tr("'+' flag used with ‘%c’ specifier in format string:"))
				, ctx->format.c_str());

			if (!ok)
				ctx->spec.flags &= ~safeformat::NeedSign;
			break;

		case 's':
			r = ut.toString();
			expect = __EXPECT_STRING;

			ok = PFS_VERIFY_X(
				PFS_VERIFY_X(!(ctx->spec.flags & safeformat::ZeroPadding)
					, _Tr("'0' flag used with ‘%s’ specifier in format string"))
				, ctx->format.c_str());

			if (!ok)
				ctx->spec.flags &= ~safeformat::ZeroPadding;

			ok = PFS_VERIFY_X(
				PFS_VERIFY_X(!(ctx->spec.flags & safeformat::NeedSign)
					, _Tr("'0' flag used with ‘%s’ specifier in format string"))
				, ctx->format.c_str());

			if (!ok)
				ctx->spec.flags &= ~safeformat::NeedSign;
			break;
		default:
			break;
		}

		PFS_ASSERT(expect != __EXPECT_UNKNOWN);

//		if (false) {
//			PFS_WARN(_Tr("Incompatible value for conversion specification in format string:"));
//			PFS_WARN(ctx->format.c_str());
//		} else {

			if (ctx->spec.flags & safeformat::ZeroPadding) {

				// If the 0 and - flags both appear, the 0 flag is ignored.
				if (ctx->spec.flags & safeformat::LeftJustify)
					ctx->spec.flags &= ~safeformat::ZeroPadding;

				// If a precision is given with a numeric conversion (d, i, o, u, x, and X), the 0 flag is ignored.
				if (ctx->spec.prec > -1
						&& (  ctx->spec.spec_char == pfs::ucchar('d')
							|| ctx->spec.spec_char == pfs::ucchar('i')
							|| ctx->spec.spec_char == pfs::ucchar('o')
							|| ctx->spec.spec_char == pfs::ucchar('u')
							|| ctx->spec.spec_char == pfs::ucchar('x')
							|| ctx->spec.spec_char == pfs::ucchar('X')
							))
					ctx->spec.flags &= ~safeformat::ZeroPadding;
			}

			// A + overrides a space if both are used
			if ((ctx->spec.flags & safeformat::NeedSign)
					&& (ctx->spec.flags & safeformat::SpaceBeforePositive))
				ctx->spec.flags &= ~safeformat::SpaceBeforePositive;

			if (expect == __EXPECT_DECIMAL || expect == __EXPECT_FLOAT) {
				bool isNegative = r.startsWith("-");

				// When 0 is printed with an explicit precision 0, the output is empty.
				if (ctx->spec.prec == 0 && r == pfs::string("0"))
					r.clear();

				// The precision, if any, gives the minimum number of digits that must appear;
				// if the converted value requires fewer digits, it is padded on the left with zeros.
				if (ctx->spec.prec > 0 && r.length() < size_t(ctx->spec.prec))
					r.prepend(pfs::string(ctx->spec.prec - r.length(), '0'));

				if (! isNegative) {
					// A sign (+ or -) should always be placed before a number produced by a signed conversion
					if (ctx->spec.flags & safeformat::NeedSign) {
						r.prepend(pfs::string("+"));
					}
					// A blank should be left before a positive number
					else if (ctx->spec.flags & safeformat::SpaceBeforePositive) {
						r.prepend(pfs::string(" "));
					}
				}
			}

			// If the converted value has fewer characters than the field width, it will be padded with spaces
			// on the left (or right, if the left-adjustment flag has been given).
			PFS_ASSERT(ctx->spec.width >= 0);
			if (r.length() < size_t(ctx->spec.width)) {
				size_t count = size_t(ctx->spec.width) - r.length();
				char paddingChar = (ctx->spec.flags & safeformat::ZeroPadding) ? '0' : ' ';

				if (ctx->spec.flags & safeformat::LeftJustify)
					r.append(pfs::string(count, paddingChar));
				else
					r.prepend(pfs::string(count, paddingChar));
			}
//		}
		ctx->result.append(r);
	}

	return true;
}

bool set_percent_char (const pfs::string::const_iterator & , const pfs::string::const_iterator & , void * context, void *)
{
	safeformat_context *ctx = reinterpret_cast<safeformat_context *>(context);
	ctx->result.append(pfs::string(1, '%'));
	return true;
}

bool bad_spec (const pfs::string::const_iterator & , const pfs::string::const_iterator & , void * context, void *)
{
	safeformat_context * ctx = reinterpret_cast<safeformat_context *>(context);
	PFS_VERIFY_X(
		PFS_VERIFY_X(false, _Tr("Bad conversion specification in format string:"))
		, ctx->format.c_str());
	return false;
}

safeformat::safeformat(const pfs::string & format)
	: _context(new safeformat_context)
{
	clear_safeformat_context(*_context);
	_context->format = format;
}

safeformat::safeformat(const char * latin1Format)
	: _context(new safeformat_context)
{
	clear_safeformat_context(*_context);
	_context->format = pfs::string(latin1Format);
}

safeformat::operator pfs::string & ()
{
	pfs::fsm::fsm<pfs::string> fsm(format_fsm, _context.get());
	ssize_t n = fsm.exec(0, _context->format.begin(), _context->format.end());
	if (n < 0) {
		;
	} else {
		// add tail
		_context->result.append(pfs::string(_context->format.begin() + size_t(n), _context->format.end()));
	}
	return _context->result;
}

const pfs::string & safeformat::operator () ()
{
	return _context->result;
}

safeformat & safeformat::operator () (const pfs::unitype & ut)
{
	_context->bind_args.append(ut);
	return *this;
}

} // pfs