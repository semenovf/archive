/*
 * regexp_pcre.cpp
 *
 *  Created on: Oct 21, 2013
 *      Author: wladt
 */

#include "../include/cwt/regexp.hpp"
#include "../include/cwt/vector.hpp"
#include "../include/cwt/safeformat.hpp"
#include "../pcre8/pcre.h"

CWT_NS_BEGIN

class RegExp::Impl
{
public:
	String       _pattern;
	pcre *       _re;
	pcre_extra * _extra;
	String       _errstr;
	int          _erroffset;
	int          _nsubpatterns;
	int          _lineBreak;
	bool         _isReady;

	Vector<int>  _ovector;
	size_t       _capturedCount;
	int          _exec_options;
	const char * _subject;
	int          _subjectLength;

	Impl()
		: _pattern        ()
		, _re             (nullptr)
		, _extra          (nullptr)
		, _errstr         ()
		, _erroffset      (-1)
		, _nsubpatterns   (-1)
		, _lineBreak      (PCRE_NEWLINE_LF)
		, _isReady        (false)
		, _ovector        ()
		, _capturedCount  (0)
		, _exec_options   (0)
		, _subject        (nullptr)
		, _subjectLength  (0)
	{}

	Impl(const Impl & other)
		: _pattern        (other._pattern)
		, _re             (other._re)
		, _extra          (other._extra)
		, _errstr         (other._errstr)
		, _erroffset      (other._erroffset)
		, _nsubpatterns   (other._nsubpatterns)
		, _lineBreak      (other._lineBreak)
		, _isReady        (other._isReady)
		, _ovector        (other._ovector)
		, _capturedCount  (other._capturedCount)
		, _exec_options   (other._exec_options)
		, _subject        (other._subject)
		, _subjectLength  (other._subjectLength)
	{}

	~Impl();

	void invalidate ();
	bool compile ();
	bool exec ();
};

CWT_PIMPL_IMPL_COPYABLE(RegExp);

RegExp::Impl::~Impl ()
{
	invalidate();
}

void RegExp::Impl::invalidate ()
{
	_isReady = false;
	_errstr.clear();
	_erroffset = -1;

	if (_re) {
		pcre_free(_re);
		_re = nullptr;
	}

	if (_extra) {
		pcre_free_study(_extra);
		_extra = nullptr;
	}
}

bool RegExp::Impl::compile ()
{
	if (_isReady)
		return true;

	int compile_options = _lineBreak
			| PCRE_UTF8           // Run in UTF-8 mode
			| PCRE_NO_UTF8_CHECK  // Do not check the pattern for UTF-8 validity (only relevant if PCRE_UTF8 is set)
			| PCRE_UCP;           // Use Unicode properties for \d, \w, etc.

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

bool RegExp::Impl::exec ()
{
	int rc = pcre_exec(_re
		, _extra
		, _subject
		, _subjectLength
		, 0
		, _exec_options
		, _ovector.data()
		, _ovector.size());

	// Output vector must be to big enough
	CWT_ASSERT(rc != 0);

	if (rc < 0) {
		switch (rc) {
		case PCRE_ERROR_NOMATCH:
			break;
		default:
			_errstr = _Fr("pcre_error: failed with error code %d") % rc;
			break;
		}
		return false;
	}

	_capturedCount = size_t(rc);

	return true;
}

#ifdef __COMMENT__
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

#endif



RegExp::RegExp() : pimpl(new RegExp::Impl)
{}

RegExp::RegExp (const String & pattern) : pimpl(new RegExp::Impl)
{
	setPattern(pattern);
}

bool RegExp::isError () const
{
	return pimpl->_errstr.size() > 0;
}

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

bool RegExp::match (const String & s)
{
	pimpl->_errstr.clear();

	if (!pimpl->_isReady) {
		if (!pimpl->compile())
			return false;
	}

	CWT_ASSERT(s.size() <= CWT_INT_MAX);

	pimpl->_ovector.resize((pimpl->_nsubpatterns + 1) * 3);
	pimpl->_exec_options = pimpl->_lineBreak | PCRE_NO_UTF8_CHECK;
	pimpl->_subject = s.c_str();
	pimpl->_subjectLength = int(s.size());

	return pimpl->exec();
}

bool RegExp::matchNext ()
{
	pimpl->_errstr.clear();

	if (!pimpl->_isReady) {
		if (!pimpl->compile())
			return false;
	}
	size_t offset = pimpl->_ovector[1] - pimpl->_ovector[0];
	pimpl->_subject += offset;
	pimpl->_subjectLength -= offset;
	return pimpl->exec();
}

size_t	RegExp::capturedCount() const
{
	return pimpl->_capturedCount;
}

String RegExp::captured (size_t index) const
{
	return (pimpl->_capturedCount && index < pimpl->_capturedCount)
		? String(NO_UTF8_CHECK
			, pimpl->_subject + pimpl->_ovector[2 * index]
	        , pimpl->_ovector[2 * index + 1] - pimpl->_ovector[2 * index])
		: String();
}

StringList RegExp::groups() const
{
	StringList r;

	if (pimpl->_capturedCount > 1) {
		for (size_t i = 1, j = 2; i < pimpl->_capturedCount; ++i, j += 2) {
			r.append(String(NO_UTF8_CHECK
				, pimpl->_subject + pimpl->_ovector[j]
			    , pimpl->_ovector[j + 1] - pimpl->_ovector[j]));
		}
	}
	return r;
}

CWT_NS_END
