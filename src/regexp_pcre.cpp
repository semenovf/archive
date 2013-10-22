/*
 * regexp_pcre.cpp
 *
 *  Created on: Oct 21, 2013
 *      Author: wladt
 */

#include "../include/cwt/regexp.hpp"
#include "../include/cwt/logger.hpp"
#include "../pcre8/pcre.h"

// We need PCRE version at least 5.0 (with support for Unicode and Unicode general category properties (UCP)
#if PCRE_MAJOR < 5
#	error "RegExp implementation based on PCRE library with version 5.0 at least"
#else

CWT_NS_BEGIN

class RegExp::Impl
{
public:
	Impl() : _pattern()
		, _re(nullptr)
		, _extra(nullptr)
		, _errstr()
		, _erroffset(-1)
		, _nsubpatterns(-1)
		, _lineBreak(PCRE_NEWLINE_LF)
		, _isReady(false)
	{}

	Impl(const Impl & other): _pattern()
		, _re(other._re)
		, _extra(other._extra)
		, _errstr(other._errstr)
		, _erroffset(other._erroffset)
		, _nsubpatterns(other._nsubpatterns)
		, _lineBreak(PCRE_NEWLINE_LF)
		, _isReady(other._isReady)
	{}

	Impl(const String pattern): _pattern(pattern)
		, _re(nullptr)
		, _extra(nullptr)
		, _errstr()
		, _erroffset(-1)
		, _nsubpatterns(-1)
		, _lineBreak(PCRE_NEWLINE_LF)
		, _isReady(false)
	{}

	~Impl() { clear(); }

	void invalidate() { _isReady = false; }
	void clear ();
	bool compile ();

	String       _pattern;
	pcre *       _re;
	pcre_extra * _extra;
	String       _errstr;
	int          _erroffset;
	int          _nsubpatterns;
	int          _lineBreak;
	bool         _isReady;
};

class RegExpMatch::Impl
{
public:
	Impl ()
		: _ovector(nullptr)
		, _ovsize(-1)
		, _capturedCount(0)
		, _subject(nullptr)
	{}

	Impl (const Impl & other)
		: _ovector(other._ovector)
		, _ovsize(other._ovsize)
		, _capturedCount(other._capturedCount)
		, _subject(other._subject)
	{}

	~Impl () {
		if (_ovector) {
			delete [] _ovector;
			_ovector = nullptr;
		}
		_subject = nullptr;
	}

	int * _ovector;
	int   _ovsize;
	int   _capturedCount;
	const char * _subject;
};

CWT_PIMPL_IMPL_COPYABLE(RegExp);
CWT_PIMPL_IMPL_COPYABLE(RegExpMatch);

class RegExpMatch::iterator::iterator_data
{
public:
	iterator_data() : subject(nullptr), cursor(0) {}
	const char * subject;
	int * cursor;
};

RegExpMatch::iterator::iterator () : m_d(new RegExpMatch::iterator::iterator_data)
{}

RegExpMatch::iterator::iterator (const iterator & other) : m_d(other.m_d)
{}


String RegExpMatch::iterator::value () const
{
	iterator::iterator_data & d = *m_d.get();
	return String(NO_UTF8_CHECK, d.subject + d.cursor[0], d.cursor[1] - d.cursor[0]);
}

bool RegExpMatch::iterator::operator  == (const iterator & o) const
{
	return m_d->subject == o.m_d->subject && m_d->cursor == o.m_d->cursor;
}

RegExpMatch::RegExpMatch() : pimpl(new RegExpMatch::Impl)
{}

RegExpMatch::iterator RegExpMatch::begin () const
{
	RegExpMatch::iterator r;
	r.m_d->subject = pimpl->_subject;
	r.m_d->cursor = pimpl->_ovector;
	return r;
}

// TODO check capturedCount
RegExpMatch::iterator RegExpMatch::end () const
{
	RegExpMatch::iterator r;
	if (pimpl->_ovector) {
		r.m_d->subject = pimpl->_subject;
		r.m_d->cursor = pimpl->_ovector + (pimpl->_capturedCount * 2);
	}
	return r;
}

RegExpMatch::iterator & RegExpMatch::iterator::operator ++ ()
{
	m_d->cursor += 2;
	return *this;
}

bool RegExpMatch::hasMatch () const
{
	return pimpl->_capturedCount > 0;
}

String RegExpMatch::captured (size_t index) const
{
	CWT_ASSERT(pimpl->_capturedCount >= 0);
	CWT_ASSERT(index < size_t(pimpl->_capturedCount));
	return String(NO_UTF8_CHECK
			, pimpl->_subject + pimpl->_ovector[2 * index]
	        , pimpl->_ovector[2 * index + 1] - pimpl->_ovector[2 * index]);
}

StringList RegExpMatch::captured() const
{
	StringList r;

	if (hasMatch()) {
		iterator it = begin();
		iterator itEnd = end();

		while (it != itEnd) {
			r.append(*it++);
		}
	}
	return r;
}


void RegExp::Impl::clear ()
{
	if (_re) {
		pcre_free(_re);
		_re = nullptr;
	}

	// FIXME does i need to free extra?
	if (_extra) {
		pcre_free_study(_extra);
		_extra = nullptr;
	}

	_errstr.clear();
	_erroffset = -1;
	_isReady = false;
}


bool RegExp::Impl::compile ()
{
	invalidate();

	int compile_options = _lineBreak
			| PCRE_UTF8  // Run in UTF-8 mode
			| PCRE_NO_UTF8_CHECK      // Do not check the pattern for UTF-8 validity (only relevant if PCRE_UTF8 is set)
			| PCRE_UCP;               // Use Unicode properties for \d, \w, etc.

	const char * errsstr;
	_re = pcre_compile (_pattern.c_str(), compile_options, & errsstr, & _erroffset, nullptr/*tables*/);

	if (!_re) {
		_errstr = String::fromUtf8(errsstr);
		return false;
	}

	_extra = pcre_study(_re, 0, & errsstr);

	if (!_extra) {
		if (errsstr) {
			_errstr = String::fromUtf8(errsstr);
			return false;
		}
	}

	int rc;
	CWT_VERIFY((rc = pcre_fullinfo(_re, _extra, PCRE_INFO_CAPTURECOUNT, & _nsubpatterns)) == 0);
	if (rc)
		return false;

	_isReady = true;
	return true;
}

RegExp::RegExp() : pimpl(new RegExp::Impl)
{}

RegExp::RegExp (const String & pattern) : pimpl(new RegExp::Impl(pattern))
{}

void RegExp::setLineBreak (int lineBreak)
{
	pimpl->invalidate();
	if (lineBreak & LineBreak_CR)         pimpl->_lineBreak |= PCRE_NEWLINE_CR;
	if (lineBreak & LineBreak_LF)         pimpl->_lineBreak |= PCRE_NEWLINE_LF;
	if (lineBreak & LineBreak_CRLF)       pimpl->_lineBreak |= PCRE_NEWLINE_CRLF;
	if (lineBreak & LineBreak_AnyCRLF)    pimpl->_lineBreak |= PCRE_NEWLINE_ANYCRLF;
	if (lineBreak & LineBreak_AnyUnicode) pimpl->_lineBreak |= PCRE_NEWLINE_ANY;
}

void RegExp::setPattern (const String & pattern)
{
	pimpl->invalidate();
	pimpl->_pattern = pattern;
}

const String & RegExp::errorString () const
{
	return pimpl->_errstr;
}

int RegExp::errorOffset () const
{
	return pimpl->_erroffset;
}

RegExpMatch RegExp::match (const String & s)
{
	if (!pimpl->_isReady) {
		if (!pimpl->compile())
			return RegExpMatch();
	}

	CWT_ASSERT(pimpl->_isReady);

	int   ovsize = (pimpl->_nsubpatterns + 1) * 3;
	int * ovector = new int[ovsize];

	int exec_options = pimpl->_lineBreak
			| PCRE_NO_UTF8_CHECK;

	int rc = pcre_exec(pimpl->_re, pimpl->_extra, s.c_str(), s.size(), 0
			, exec_options, ovector, ovsize);

	// Output vector must be to big enough
	CWT_ASSERT(rc != 0);

	if (rc < 0) {
		switch (rc) {
		case PCRE_ERROR_NOMATCH:
			break;
		default:
			CWT_VERIFY_X(rc > 0
				, String(_Fr("pcre_error: failed with error code %d") % rc).c_str());
			break;
		}
	}

	RegExpMatch r;

	if (rc > 0) {
		r.pimpl->_ovector = ovector;
		r.pimpl->_ovsize = ovsize;
		r.pimpl->_capturedCount = rc;
		r.pimpl->_subject = s.c_str();
	}

	return r;
}

#endif
CWT_NS_END
