/**
 * @file utf8string_p.hpp
 * @author wladt
 * @date Nov 27, 2013
 */

#ifndef __PFS_UTF8STRING_P_HPP__
#define __PFS_UTF8STRING_P_HPP__

#include "../include/pfs/utf8string.hpp"
#include <string>

namespace pfs {

class utf8string::impl
{
	friend class ucchar_ref;
	friend class ucchar_ptr;

public:
	std::string _buffer;
	size_t _length; // length in unicode code points
	bool _initialized;

	typedef std::string::iterator iterator;
	typedef std::string::const_iterator const_iterator;
	typedef std::string::const_pointer const_pointer;

	static const size_t npos;// = std::string::npos;

	impl () : _buffer(), _length(0), _initialized(false) {}
	impl (const impl & other) : _buffer(other._buffer), _length(other._length), _initialized(true) { }
	impl (size_t n, char ch) : _buffer(n, ch), _length(0), _initialized(true) { }
	impl (const std::string & other) : _buffer(other), _length(0), _initialized(true) { }

	const_pointer data () const { return _buffer.data(); }
	const_pointer c_str () const { return _buffer.c_str(); }
	size_t size () const { return _buffer.size(); }

	bool isNull () const { return !_initialized; }

	impl & assign (const std::string & str)
	{
		_buffer.assign(str);
		_initialized = true;
		return *this;
	}

	void append (const char * s, size_t n)
	{
		_buffer.append(s, n);
		_initialized = true;
	}

	void append (size_t n, char c)
	{
		_buffer.append(n, c);
		_initialized = true;
	}

	int compare (size_t pos, size_t len, const std::string & str,
	             size_t subpos, size_t sublen) const
	{
		return _buffer.compare(pos, len, str, subpos, sublen);
	}

	int compare (size_t pos, size_t len, const char * s, size_t n) const
	{
		return _buffer.compare(pos, len, s, n);
	}

	void clear ()
	{
		_buffer.clear();
	}

	void erase (size_t pos = 0, size_t len = npos)
	{
		_buffer.erase(pos, len);
	}

	void insert (size_t pos, const char * s, size_t n)
	{
		_buffer.insert(pos, s, n);
		_initialized = true;
	}

	size_t find (const char * s, size_t pos, size_t n) const
	{
		return _initialized ? _buffer.find(s, pos, n) : npos;
	}

	void replace (size_t pos, size_t len, const impl & str,
	                 size_t subpos, size_t sublen)
	{
		if (_initialized)
			_buffer.replace(pos, len, str._buffer, subpos, sublen);
	}

	void replace (size_t pos, size_t len, const char * str,
	                 size_t subpos, size_t sublen)
	{
		if (_initialized)
			_buffer.replace(pos, len, str, subpos, sublen);
	}

	void reserve (size_t n = 0)
	{
		_buffer.reserve(n);
	}

	std::string substr (size_t pos = 0, size_t len = npos) const
	{
		return _buffer.substr(pos, len);
	}

	void setLength (size_t len)
	{
		_length = len;
		_initialized = true;
	}
};


inline void utf8string::detach ()
{
	if (!_pimpl.unique()) {
		shared_ptr<utf8string::impl> d(new utf8string::impl(*_pimpl));
		_pimpl.swap(d);
	}
}

} // pfs

#endif /* __PFS_UTF8STRING_P_HPP__ */
