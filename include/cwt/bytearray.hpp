/**
 * @file   bytearray.hpp
 * @author wladt
 * @date   Jan 24, 2013 4:13:15 PM
 *
 * @brief
 */

#ifndef __CWT_BYTEARRAY_HPP__
#define __CWT_BYTEARRAY_HPP__

#include <string>
#include <cwt/string.h>
#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

class DLL_API ByteArray
{
	CWT_PIMPL_DECL_COPYABLE(ByteArray);
public:
	typedef char char_type;

    class const_iterator;

	class iterator {
		friend class const_iterator;
		friend class ByteArray;
		const char * m_cursor;

	public:
        iterator() : m_cursor(nullptr) {}
    	iterator(const iterator & o) : m_cursor(o.m_cursor) {}
    	explicit iterator(const char * cursor) : m_cursor(cursor) {}

    	char value() const                           { return *m_cursor; }
        char operator  * () const                    { return *m_cursor; }
        size_t distance (const iterator & o) const   { ssize_t d = o.m_cursor - m_cursor; return d > 0 ? d : d * -1; }
        bool operator  == (const iterator & o) const { return m_cursor == o.m_cursor; }
        bool operator  != (const iterator & o) const { return !(m_cursor == o.m_cursor); }
        iterator & operator ++ ()                    { ++m_cursor; return *this; }
        iterator   operator ++ (int) {
            iterator r(*this);
            this->operator ++();
            return r;
        }

        iterator & operator -- ()  { --m_cursor; return *this; }
        iterator   operator -- (int) {
            iterator r(*this);
            this->operator --();
            return r;
        }
        iterator   operator  + (int n) const {
        	iterator it(*this);
        	it += n;
        	return it;
        }
        iterator   operator  - (int n ) const {
        	iterator it(*this);
        	it -= n;
        	return it;
        }

        iterator & operator += (int n) {
         	m_cursor += n;
          	return *this;
        }

        iterator & operator -= (int n) {
          	m_cursor -= n;
          	return *this;
        }

		bool operator  > (const iterator & o) const { return m_cursor > o.m_cursor; }
		bool operator >= (const iterator & o) const { return m_cursor >= o.m_cursor; }
		bool operator  < (const iterator & o) const { return m_cursor < o.m_cursor; }
		bool operator <= (const iterator & o) const { return m_cursor <= o.m_cursor; }
	};

	class const_iterator {
		friend class iterator;
		friend class ByteArray;
		const char * m_cursor;

	public:
        const_iterator() : m_cursor(nullptr) {}
    	const_iterator(const iterator & o) : m_cursor(o.m_cursor) {}
    	const_iterator(const const_iterator & o) : m_cursor(o.m_cursor) {}
    	explicit const_iterator(const char * cursor)  : m_cursor(cursor) {}

    	char value() const                                { return *m_cursor; }
    	char operator  * () const                         { return *m_cursor; }
        size_t distance (const const_iterator & o) const   { ssize_t d = o.m_cursor - m_cursor; return d > 0 ? d : d * -1; }
        bool operator  == (const iterator & o) const       { return m_cursor == o.m_cursor; }
        bool operator  == (const const_iterator & o) const { return m_cursor == o.m_cursor; }
        bool operator  != (const iterator & o) const       { return !(m_cursor == o.m_cursor); }
        bool operator  != (const const_iterator & o) const { return !(m_cursor == o.m_cursor); }
        const_iterator & operator ++ ()                    { ++m_cursor; return *this; }
        const_iterator   operator ++ (int) {
            const_iterator r(*this);
            this->operator ++();
            return r;
        }

        const_iterator & operator -- () { --m_cursor; return *this; }

        const_iterator   operator -- (int) {
            const_iterator r(*this);
            this->operator --();
            return r;
        }

        const_iterator   operator  + (int n) const {
        	const_iterator it(*this);
        	it += n;
        	return it;
        }

        const_iterator   operator  - (int n) const {
        	const_iterator it(*this);
        	it -= n;
        	return it;
        }

        const_iterator & operator += (int n) {
        	m_cursor += n;
        	return *this;
        }

        const_iterator & operator -= (int n) {
        	m_cursor -= n;
          	return *this;
        }

        bool operator  > (const const_iterator & o) const { return m_cursor  > o.m_cursor; }
        bool operator >= (const const_iterator & o) const { return m_cursor >= o.m_cursor; }
        bool operator  < (const const_iterator & o) const { return m_cursor  < o.m_cursor; }
        bool operator <= (const const_iterator & o) const { return m_cursor <= o.m_cursor; }
	};
public:
	ByteArray();
	ByteArray(const char *data);
	ByteArray(const char *data, size_t size);
	ByteArray(size_t size, char ch);

	const char * c_str() const;
	const char * data() const;
	const char * constData() const { return data(); }
	char &       charAt(size_t pos);
	const char & charAt (size_t pos) const;
	void         clear();
	bool	     isEmpty() const;
	size_t       size() const;
	size_t       length() const { return size(); }

	ByteArray & append  (const ByteArray & s) { return insert(s, end()); }
	ByteArray & append  (const char * s) { return insert(s, end()); }
	ByteArray & append  (const char * s, size_t n) { return insert(s, n, end()); }
	ByteArray & append  (size_t size, char ch) { append(ByteArray(size, ch)); return *this; }
	ByteArray & prepend (const ByteArray & s) { return insert(s, begin()); }
	ByteArray & prepend (const char * s) { return insert(s, begin()); }
	ByteArray & prepend (const char * s, size_t n)  { return insert(s, n, begin()); }
	ByteArray & prepend (size_t size, char ch) { prepend(ByteArray(size, ch)); return *this; }
	ByteArray & insert  (const ByteArray & s, size_t pos) { return insert(s, begin() + pos); }
	ByteArray & insert  (const ByteArray & s, const const_iterator & pos) { return insert(s.constData(), s.size(), pos); }
	ByteArray & insert  (const char * s, const const_iterator & pos) { return insert(s, strlen(s), pos); }
	ByteArray & insert  (const char * s, size_t n, const const_iterator & pos);

    iterator       begin();
    iterator       end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const   { return end(); }

    int compare (const ByteArray & s) const;
    int compare (size_t pos, size_t len, const ByteArray & s) const;
    int compare (size_t pos, size_t len, const ByteArray & s, size_t subpos, size_t sublen) const;
    int compare (const char * s) const;
    int compare (size_t pos, size_t len, const char * s) const;
    int compare (size_t pos, size_t len, const char * s, size_t n) const;

	bool contains(const ByteArray & s) const { return find(s, begin()) != end(); }
	bool contains(const char * s) const { return find(s, begin()) != end(); }
	bool contains(const char * s, size_t n) const { return find(s, 0, n) != end(); }

	bool endsWith(const ByteArray & s) const { return endsWith(s.data(), s.size()); }
	bool endsWith(const char * s) const { return endsWith(s, strlen(s)); }
	bool endsWith(const char * s, size_t n) const;

	bool startsWith(const ByteArray & s) const { return startsWith(s.data(), s.size()); }
	bool startsWith(const char * s) const { return startsWith(s, strlen(s)); }
	bool startsWith(const char * s, size_t n) const;

	const_iterator find(const ByteArray & s, const_iterator pos) const;
	const_iterator find(const char * s, size_t pos, size_t n) const;
	const_iterator find(const char * s, size_t pos) const { return find(s, pos, strlen(s)); }

	void reserve (size_t n = 0);
	void resize(size_t size);

	ByteArray substr(size_t pos) const { return substr(pos, length()); }
	ByteArray substr(size_t pos, size_t n) const;
	ByteArray substr(const const_iterator & from) const;
	ByteArray substr(const const_iterator & from, size_t n) const;

	double	 toDouble (bool *ok = 0) const { return cwt_str_to_double(c_str(), ok); }
	float	 toFloat  (bool *ok = 0) const { return cwt_str_to_float(c_str(), ok); }
	long_t   toLong   (bool *ok = 0, int base = 10) const { return cwt_str_to_long(c_str(), ok, base); }
	ulong_t	 toULong  (bool *ok = 0, int base = 10) const { return cwt_str_to_ulong(c_str(), ok, base); }
	int_t	 toInt    (bool *ok = 0, int base = 10) const { return cwt_str_to_int(c_str(), ok, base); }
	uint_t	 toUInt   (bool *ok = 0, int base = 10) const { return cwt_str_to_uint(c_str(), ok, base); }
	short_t	 toShort  (bool *ok = 0, int base = 10) const { return cwt_str_to_short(c_str(), ok, base); }
	ushort_t toUShort (bool *ok = 0, int base = 10) const { return cwt_str_to_ushort(c_str(), ok, base); }
	sbyte_t  toSByte  (bool *ok = 0, int base = 10) const { return cwt_str_to_sbyte(c_str(), ok, base); }
	byte_t	 toByte   (bool *ok = 0, int base = 10) const { return cwt_str_to_byte(c_str(), ok, base); }

	ByteArray & setNumber (long_t n, int base = 10);
	ByteArray & setNumber (ulong_t n, int base = 10);
	ByteArray & setNumber (int_t n, int base = 10)    { return setNumber(long_t(n), base); }
	ByteArray & setNumber (uint_t n, int base = 10)   { return setNumber(ulong_t(n), base); }
	ByteArray & setNumber (short_t n, int base = 10)  { return setNumber(long_t(n), base); }
	ByteArray & setNumber (ushort_t n, int base = 10) { return setNumber(ulong_t(n), base); }
	ByteArray & setNumber (sbyte_t n, int base = 10)  { return setNumber(long_t(n), base); }
	ByteArray & setNumber (byte_t n, int base = 10)   { return setNumber(ulong_t(n), base); }
	ByteArray & setNumber (float n, char f = 'g', int prec = 6) { return setNumber(double(n), f, prec); }
	ByteArray & setNumber (double n, char f = 'g', int prec = 6);

	ByteArray & sprintf(const char * cformat, ...);
	ByteArray & vsprintf(const char *cformat, va_list ap);

	ByteArray & operator += (const ByteArray & other) { return append(other); }
	char & operator [] (size_t pos) { return charAt(pos); }
	const char & operator [] (size_t pos) const { return charAt(pos); }

	friend ByteArray operator + (const ByteArray &s1, const ByteArray &s2);

	friend bool	operator != (const ByteArray &s1, const ByteArray &s2);
	friend bool	operator <  (const ByteArray &s1, const ByteArray &s2);
	friend bool	operator <= (const ByteArray &s1, const ByteArray &s2);
	friend bool	operator == (const ByteArray &s1, const ByteArray &s2);
	friend bool	operator >  (const ByteArray &s1, const ByteArray &s2);
	friend bool	operator >= (const ByteArray &s1, const ByteArray &s2);

	static ByteArray number (double n, char fmt = 'g', int prec = 6) { return ByteArray().setNumber(n, fmt, prec); }
	static ByteArray number (float n, char fmt = 'g', int prec = 6)  { return ByteArray().setNumber(n, fmt, prec); }
	static ByteArray number (int_t n, int base = 10)                 { return ByteArray().setNumber(n, base); }
	static ByteArray number (uint_t n, int base = 10)                { return ByteArray().setNumber(n, base); }
	static ByteArray number (long_t n, int base = 10)                { return ByteArray().setNumber(n, base); }
	static ByteArray number (ulong_t n, int base = 10)               { return ByteArray().setNumber(n, base); }

	static ByteArray fromBase64 (const ByteArray & base64);
	ByteArray toBase64 () const;

	friend void swap (ByteArray & x, ByteArray & y) { x.pimpl.swap(y.pimpl); }
};

inline ByteArray operator + (const ByteArray & s1, const ByteArray & s2)
{
	ByteArray s(s1);
	s.append(s2);
	return s;
}

DLL_API uint_t hash_func(const ByteArray & key, uint_t seed = 0);

CWT_NS_END

#endif /* __CWT_BYTEARRAY_QT_HPP__ */
