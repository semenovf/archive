/*
 * regexp_pcre.cpp
 *
 *  Created on: Oct 21, 2013
 *      Author: wladt
 */

#include "../include/cwt/regexp.hpp"
#include "../include/cwt/logger.hpp"
#include <pcre.h>

// We need PCRE version at least 5.0 (with support for Unicode and Unicode general category properties (UCP)
#if PCRE_MAJOR < 5
#	error "RegExp implementation based on PCRE library with version 5.0 at least"
#else

CWT_NS_BEGIN

class RegExp::Impl
{
public:
	Impl() : pattern(), pcre(nullptr), extra(nullptr), errstr(), erroffset(-1), options(0), nsubpatterns(-1), isReady(false) {}
	Impl(const Impl & other);
	Impl(const String & pattern);
	~Impl() { clear(); }

	void invalidate() { isReady = false; }
	void clear ();
	bool compile ();

	String pattern;
	pcre * pcre;
	pcre_extra * extra;
	String errstr;
	int    erroffset;
	int    options;
	int    nsubpatterns;
	bool   isReady;
};

class RegExpMatch::Impl
{
public:
	Impl () : ovector(nullptr), ovsize(-1), capturedCount(0) {}
	Impl(const Impl & other);
	~Impl() {
		if (ovector) {
			delete [] ovector;
			ovector = nullptr;
		}
	}

	int * ovector;
	int   ovsize;
	int   capturedCount;
};

CWT_PIMPL_IMPL_COPYABLE(RegExp);

class RegExpMatch::iterator::iterator_data
{
public:
	iterator_data() : cursor(0) {}
	int * cursor;
};

String RegExpMatch::iterator::value () const
{
	iterator::iterator_data & d = *m_d.get();
	return String(reinterpret_cast<char *>(d.cursor), *(d.cursor + 1) - d.cursor);
}

RegExpMatch::iterator RegExpMatch::begin ()
{

}



// TODO check capturedCount
RegExpMatch::iterator RegExpMatch::end ()
{
	RegExpMatch::iterator r;
	r.m_d->cursor = pimpl->ovector + (pimpl->capturedCount * 2);
	return r;
}

RegExpMatch::iterator::iterator & RegExpMatch::iterator::operator ++ ()
{
	m_d->cursor += 2;
	return *this;
}

void RegExp::Impl::clear ()
{
	if (pcre) {
		pcre_free(pcre);
		pcre = nullptr;
	}

	// FIXME does i need to free extra?
	if (extra) {
		pcre_free(extra);
		extra = nullptr;
	}

	errstr.clear();
	erroffset = -1;
	options = 0;
	isReady = false;
}


bool RegExp::Impl::compile ()
{
	invalidate();

	options |= PCRE_UTF8         // Run in UTF-8 mode
			| PCRE_NO_UTF8_CHECK // Do not check the pattern for UTF-8 validity (only relevant if PCRE_UTF8 is set)
			| PCRE_UCP           // Use Unicode properties for \d, \w, etc.
		;
	const char * errstr;
	pimpl->pcre = pcre_compile (pattern.c_str(), options, & errstr, & erroffset, nullptr/*tables*/);

	if (!pcre) {
		errstr = String::fromUtf8(errstr);
		return false;
	}

	extra = pcre_study(pimpl->pcre, 0, & errstr);

	if (!extra) {
		if (errstr) {
			pimpl->errstr = String::fromUtf8(errstr);
			return false;
		}
	}

	int rc;
	CWT_VERIFY((rc = pcre_fullinfo(pcre, extra, PCRE_INFO_CAPTURECOUNT, & nsubpatterns)) == 0);
	if (rc)
		return false;

	isReady = true;
	return true;
}

RegExp::RegExp() : pimpl(new RegExp::Impl)
{
}

RegExp::RegExp (const String & pattern)
{
	setPattern(pattern);
}

void RegExp::setLineBreak (int lineBreak)
{
	pimpl->invalidate();
	if (lineBreak & LineBreak_CR)         pimpl->options |= PCRE_NEWLINE_CR;
	if (lineBreak & LineBreak_LF)         pimpl->options |= PCRE_NEWLINE_LF;
	if (lineBreak & LineBreak_CRLF)       pimpl->options |= PCRE_NEWLINE_CRLF;
	if (lineBreak & LineBreak_AnyCRLF)    pimpl->options |= PCRE_NEWLINE_ANYCRLF;
	if (lineBreak & LineBreak_AnyUnicode) pimpl->options |= PCRE_NEWLINE_ANY;
}

void RegExp::setPattern (const String & pattern)
{
	pimpl->invalidate();
	pimpl->pattern = pattern;
}

const String & RegExp::errorString () const
{
	return pimpl->errstr;
}

int RegExp::errorOffset () const
{
	return pimpl->erroffset;
}

RegExpMatch RegExp::match (const String & s)
{
	RegExpMatch r;

	if (!pimpl->isReady) {
		if (!pimpl->compile())
			return RegExpMatch();
	}

	CWT_ASSERT(pimpl->isReady);

	int   ovsize = (pimpl->nsubpatterns + 1) * 3;
	int * ovector = new int[ovsize];

	int result = pcre_exec(pimpl->pcre, pimpl->extra, s.c_str(), s.size(), 0
	         , pimpl->options, ovector, ovsize);

	RegExp::iterator r;

	if (result > 0) {
		r.m_d->ovector = ovector;
		r.m_d->ovsize = ovsize;
		r.m_d->capturedCount = result;
		r.m_d->cursor = 0;
	}

	return r;
}

/*
String RegExp::capturedText(size_t index)
{

}
*/

#endif
CWT_NS_END
