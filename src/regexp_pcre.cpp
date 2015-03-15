/*
 * regexp_pcre.cpp
 *
 *  Created on: Oct 21, 2013
 *      Author: wladt
 */

#include "pfs/regexp.hpp"
#include "../pcre8/pcre.h"
#include <pfs/vector.hpp>
#include <string.h>

namespace pfs {

class regexp::impl
{
public:
	pfs::string  _pattern;
	pcre *       _re;
	pcre_extra * _extra;
	pfs::string  _errstr;
	int          _erroffset;
	int          _nsubpatterns;
	int          _lineBreak;
	bool         _isReady;

	pfs::vector<int>  _ovector;
	size_t       _capturedCount;
	int          _exec_options;
	const char * _subjectPtr;
	int          _subjectLength;

	impl()
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
		, _subjectPtr     (nullptr)
		, _subjectLength  (0)
	{}

	impl(const impl & other)
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
		, _subjectPtr     (other._subjectPtr)
		, _subjectLength  (other._subjectLength)
	{}

	~impl();

	void invalidate ();
	bool compile ();
	bool exec ();
};

//PFS_PIMPL_DEF(regexp,impl)

regexp::impl::~impl ()
{
	invalidate();
}

void regexp::impl::invalidate ()
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

bool regexp::impl::compile ()
{
	if (_isReady)
		return true;

	int compile_options = _lineBreak
			| PCRE_UTF8           // Run in UTF-8 mode
			| PCRE_NO_UTF8_CHECK  // Do not check the pattern for UTF-8 validity (only relevant if PCRE_UTF8 is set)
			| PCRE_UCP;           // Use Unicode properties for \d, \w, etc.

	const char * errstr;
	_re = pcre_compile (_pattern.c_str(), compile_options, & errstr, & _erroffset, nullptr/*tables*/);

	if (!_re) {
		_errstr = pfs::string::fromUtf8(errstr);
		return false;
	}

	_extra = pcre_study(_re, 0, & errstr);

	if (!_extra) {
		if (errstr) {
			_errstr = pfs::string::fromUtf8(errstr);
			return false;
		}
	}

	int rc;
	PFS_VERIFY((rc = pcre_fullinfo(_re, _extra, PCRE_INFO_CAPTURECOUNT, & _nsubpatterns)) == 0);
	if (rc)
		return false;

	_isReady = true;
	return true;
}

bool regexp::impl::exec ()
{
	int rc = pcre_exec(_re
		, _extra
		, _subjectPtr
		, _subjectLength
		, 0
		, _exec_options
		, _ovector.data()
		, _ovector.size());

	// Output vector must be to big enough
	PFS_ASSERT(rc != 0);

	if (rc < 0) {
		switch (rc) {
		case PCRE_ERROR_NOMATCH:
			break;
		default:
			_errstr << _Tr("pcre_error: failed with error code ") << pfs::string::toString(rc);
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

RegExpMatch::RegExpMatch() : _pimpl(new RegExpMatch::Impl)
{}

RegExpMatch::iterator RegExpMatch::begin () const
{
	RegExpMatch::iterator r;
	r.m_d->subject = _pimpl->_subjectPtr;
	r.m_d->cursor = _pimpl->_ovector;
	return r;
}

// TODO check capturedCount
RegExpMatch::iterator RegExpMatch::end () const
{
	RegExpMatch::iterator r;
	if (_pimpl->_ovector) {
		r.m_d->subject = _pimpl->_subjectPtr;
		r.m_d->cursor = _pimpl->_ovector + (_pimpl->_capturedCount * 2);
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
	return _pimpl->_capturedCount > 0;
}

String RegExpMatch::captured (size_t index) const
{
	CWT_ASSERT(_pimpl->_capturedCount >= 0);
	CWT_ASSERT(index < size_t(_pimpl->_capturedCount));
	return String(NO_UTF8_CHECK
			, _pimpl->_subjectPtr + _pimpl->_ovector[2 * index]
	        , _pimpl->_ovector[2 * index + 1] - _pimpl->_ovector[2 * index]);
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


regexp::regexp()
	: _d(new regexp::impl)
{}

regexp::regexp (const pfs::string & pattern)
	: _d(new regexp::impl)
{
	setPattern(pattern);
}

regexp::regexp (const char * latin1Pattern)
	: _d(new regexp::impl)
{
	setPattern(pfs::string::fromLatin1(latin1Pattern));
}

bool regexp::isError () const
{
	return _d.cast<impl>()->_errstr.size() > 0;
}

void regexp::setLineBreak (int lineBreak)
{
	impl & d = *_d.cast<impl>();
	d.invalidate();
	if (lineBreak & LineBreak_CR)         d._lineBreak |= PCRE_NEWLINE_CR;
	if (lineBreak & LineBreak_LF)         d._lineBreak |= PCRE_NEWLINE_LF;
	if (lineBreak & LineBreak_CRLF)       d._lineBreak |= PCRE_NEWLINE_CRLF;
	if (lineBreak & LineBreak_AnyCRLF)    d._lineBreak |= PCRE_NEWLINE_ANYCRLF;
	if (lineBreak & LineBreak_AnyUnicode) d._lineBreak |= PCRE_NEWLINE_ANY;
}

void regexp::setPattern (const pfs::string & pattern)
{
	impl & d = *_d.cast<impl>();
	d.invalidate();
	d._pattern = pattern;
}

const pfs::string & regexp::errorString () const
{
	return _d.cast<impl>()->_errstr;
}

int regexp::errorOffset () const
{
	return _d.cast<impl>()->_erroffset;
}

bool regexp::match (const pfs::string & s)
{
	return match(s.c_str());
}

bool regexp::match (const char * s)
{
	impl & d = *_d.cast<impl>();

	size_t len = strlen(s);
	PFS_ASSERT(len <= PFS_INT_MAX);

	d._errstr.clear();

	if (!d._isReady) {
		if (!d.compile())
			return false;
	}

	d._ovector.resize((d._nsubpatterns + 1) * 3);
	d._exec_options = d._lineBreak | PCRE_NO_UTF8_CHECK;
	d._subjectPtr = s;
	d._subjectLength = int(len);

	return d.exec();
}

bool regexp::matchNext ()
{
	impl & d = *_d.cast<impl>();
	d._errstr.clear();

	if (!d._isReady) {
		if (!d.compile())
			return false;
	}
	size_t offset = d._ovector[1] - d._ovector[0];
	d._subjectPtr += offset;
	d._subjectLength -= offset;
	return d.exec();
}

size_t	regexp::capturedCount() const
{
	return _d.cast<impl>()->_capturedCount;
}

pfs::string regexp::captured (size_t index) const
{
	const impl & d = *_d.cast<impl>();

	return (d._capturedCount && index < d._capturedCount)
		? pfs::string::fromUtf8(d._subjectPtr + d._ovector[2 * index]
	        , d._ovector[2 * index + 1] - d._ovector[2 * index])
		: pfs::string();
}

pfs::vector<pfs::string> regexp::groups() const
{
	const impl & d = *_d.cast<impl>();
	pfs::vector<pfs::string> r;

	if (d._capturedCount > 1) {
		for (size_t i = 1, j = 2; i < d._capturedCount; ++i, j += 2) {
			r.append(pfs::string::fromUtf8(d._subjectPtr + d._ovector[j]
			    , d._ovector[j + 1] - d._ovector[j]));
		}
	}
	return r;
}

} // pfs
