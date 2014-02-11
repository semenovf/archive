/*
 * unitype.hpp
 *
 *  Created on: Jun 3, 2013
 *      Author: wladt
 */

#ifndef __PFS_UNITYPE_HPP__
#define __PFS_UNITYPE_HPP__

#include <pfs/string.hpp>
#include <pfs/bytearray.hpp>
#include <pfs/time.hpp>
#include <pfs/date.hpp>
#include <pfs/datetime.hpp>
#include <pfs/pimpl.hpp>

// FIXME Need to apply concept of type erasure.
// See excellent article at http://www.cplusplus.com/forum/articles/18756/

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif


namespace pfs {

class unitype;

enum unitype_type {
	Null, Bool, Boolean = Bool, Integer, Float, String, Blob
};

struct DLL_API unidata
{
	virtual ~unidata () {}
	virtual unitype_type  type () const { return Null; }
	virtual bool      toBool   (bool & ok) const { ok = false; return false; }
	virtual long_t    toLong   (bool & ok) const { ok = false; return long_t(0); }
	virtual double    toDouble (bool & ok) const { ok = false; return double(0); }
	virtual pfs::string    toString (bool & ok) const { ok = false; return pfs::string(); }
	virtual pfs::bytearray toBlob   (bool & ok) const { ok = false; return pfs::bytearray(); }
};

typedef unidata unidata_null;

struct DLL_API unidata_boolean : public unidata
{
	bool _value;
	unidata_boolean  (bool v) : _value(v) {}
	unidata_boolean  (const unidata & d) : _value(false) { bool ok; _value = (d.toLong(ok) != long_t(0)); }
	virtual unitype_type  type () const { return Boolean; }
	bool   toBool    (bool & ok) const { ok = true; return _value; }
	long_t toLong    (bool & ok) const { ok = true; return _value ? long_t(1) : long_t(0); }
	double toDouble  (bool & ok) const { ok = true; return _value ? double(1) : double(0); }
	pfs::string toString  (bool & ok) const { ok = true; return _value ? pfs::string("true") : pfs::string("false"); }
	pfs::bytearray toBlob (bool & ok) const { ok = true; return _value ? pfs::bytearray(1, 1) : pfs::bytearray(1, 0); }
};

struct DLL_API unidata_long : public unidata
{
	long_t _value;
	unidata_long  (long_t v) : _value(v) {}
	unidata_long  (const unidata & d) : _value(false) { bool ok; _value = d.toLong(ok); }
	virtual unitype_type  type () const { return Integer; }
	bool   toBool    (bool & ok) const { ok = true; return _value != long_t(0); }
	long_t toLong    (bool & ok) const { ok = true; return _value; }
	double toDouble  (bool & ok) const { ok = true; return double(_value); }
	pfs::string toString  (bool & ok) const { ok = true; return pfs::string::number(_value); }
	pfs::bytearray toBlob (bool & ok) const { ok = true; return pfs::bytearray(reinterpret_cast<const char *>(& _value), sizeof(long_t)); }
};

struct DLL_API unidata_double : public unidata
{
	double _value;
	unidata_double  (double v) : _value(v) {}
	unidata_double  (const unidata & d) : _value(false) { bool ok; _value = d.toDouble(ok); }
	virtual unitype_type  type () const { return Float; }
	bool   toBool    (bool & ok) const { ok = true; return _value != double(0); }
	long_t toLong    (bool & ok) const
	{
		ok = false;
		if (_value >= double(PFS_LONG_MIN) && _value <= double(PFS_LONG_MAX)) {
			ok = true;
			return long_t(_value);
		}
		return long_t(0);
	}
	double toDouble  (bool & ok) const { ok = true; return _value; }
	pfs::string toString  (bool & ok) const { ok = true; return pfs::string::number(_value); }
	pfs::bytearray toBlob (bool & ok) const { ok = true; return pfs::bytearray(reinterpret_cast<const char *>(& _value), sizeof(double)); }
};

struct DLL_API unidata_string : public unidata
{
	pfs::string _value;
	unidata_string   (const pfs::string & v) : _value(v) {}
	unidata_string   (const unidata & d) : _value() { bool ok; _value = d.toString(ok); }
	virtual unitype_type  type () const { return String; }
	bool   toBool    (bool & ok) const
	{
		ok = !_value.isEmpty();
		return ok
				? _value != pfs::string("0") && _value != pfs::string("false")
				: false;
	}
	long_t toLong    (bool & ok) const { return _value.toLong(& ok, 10); }
	double toDouble  (bool & ok) const { return _value.toDouble(& ok); }
	pfs::string toString  (bool & ok) const { ok = true; return _value; }
	pfs::bytearray toBlob (bool & ok) const
	{
		ok = true;
		return pfs::bytearray(_value.data(), _value.cend().distance(_value.cbegin()));
	}
};

struct DLL_API unidata_blob : public unidata
{
	pfs::bytearray _value;

	unidata_blob (const pfs::bytearray & v) : _value(v) {}
	unidata_blob (const unidata & d) : _value() { bool ok; _value = d.toBlob(ok); }
	virtual unitype_type  type () const { return Blob; }
	bool toBool  (bool & ok) const
	{
		ok = !_value.isEmpty();
		return ok
				? (_value.size() == 1 && _value[0] == 0 ? false : true)
				: false;
	}
	long_t toLong     (bool & ok) const;
	double toDouble   (bool & ok) const;
	pfs::string toString   (bool & ok) const;
	pfs::bytearray toBlob  (bool & ok) const { ok = true; return _value; }
};


class DLL_API unitype
{
	PFS_PIMPL_INLINE(unitype, unidata);

public:
	unitype ()                    : _pimpl (new unidata_null) {}
	unitype (bool v)              : _pimpl (new unidata_boolean(v)) { }
	unitype (char v)              : _pimpl (new unidata_long(long_t(v))) { }
	unitype (byte_t v)            : _pimpl (new unidata_long(long_t(v))) { }
	unitype (short_t v)           : _pimpl (new unidata_long(long_t(v))) { }
	unitype (ushort_t v)          : _pimpl (new unidata_long(long_t(v))) { }
	unitype (int_t v)             : _pimpl (new unidata_long(long_t(v))) { }
	unitype (uint_t v)            : _pimpl (new unidata_long(long_t(v))) { }
#if PFS_OS_BITS == 32 // 'long_t' is 'long long'
//#ifdef HAVE_INT64
	unitype (long v)              : _pimpl (new unidata_long(long_t(v))) { }
	unitype (unsigned long v)     : _pimpl (new unidata_long(long_t(v))) { }
#endif
	unitype (long_t v)            : _pimpl (new unidata_long(long_t(v))) { }
	unitype (ulong_t v)           : _pimpl (new unidata_long(long_t(v))) { }
	unitype (float v)             : _pimpl (new unidata_double(double(v))) { }
	unitype (double v)            : _pimpl (new unidata_double(v)) { }
	unitype (const pfs::string & v)    : _pimpl (new unidata_string(v)) { }
	unitype (const char * v)           : _pimpl (new unidata_blob(pfs::bytearray(v))) { }
	unitype (const pfs::bytearray & v) : _pimpl (new unidata_blob(v)) { }
	unitype (const pfs::time & v)      : _pimpl (new unidata_long(v.millis())) { }
	unitype (const pfs::date & v)      : _pimpl (new unidata_long(v.julianDay())) { }
	unitype (const pfs::datetime & v)  : _pimpl (new unidata_long(v.millisSinceEpoch())) { }

/*
	template <typename T>
	static unitype make_object ();
*/
	unitype_type  type () const { return _pimpl->type(); }
	bool isNull () const { return _pimpl.get() == nullptr || toBlob().isEmpty(); }


	//void setNull       ();
	//void set           (const unidata & d) { _pimpl = d; }
	void setFromString (const pfs::string & s);
	void setBool       (bool b)     { shared_ptr<unidata> d(new unidata_boolean(b)); _pimpl.swap(d); }
	void setChar       (char c)     { setLong(long_t(c)); }
	void setByte       (byte_t n)   { setLong(long_t(n)); }
	void setShort      (short_t n)  { setLong(long_t(n)); }
	void setUShort     (ushort_t n) { setLong(long_t(n)); }
	void setInt        (int_t n)    { setLong(long_t(n)); }
	void setUInt       (uint_t n)   { setLong(long_t(n)); }
	void setLong       (long_t n)   { shared_ptr<unidata> d(new unidata_long(n)); _pimpl.swap(d); }
	void setULong      (ulong_t n)  { setLong(long_t(n)); }
	void setFloat      (float n)    { shared_ptr<unidata> d(new unidata_double(n)); _pimpl.swap(d); }
	void setDouble     (double n)   { shared_ptr<unidata> d(new unidata_double(n)); _pimpl.swap(d); }
	void setUCChar     (pfs::ucchar ch)  { setLong(long_t(ch)); }
	void setString     (const pfs::string & s) { shared_ptr<unidata> d(new unidata_string(s)); _pimpl.swap(d); }
	void setBlob       (const char * blob, size_t sz) { setBlob(pfs::bytearray(blob, sz)); }
	void setBlob       (const pfs::bytearray & blob) { shared_ptr<unidata> d(new unidata_blob(blob)); _pimpl.swap(d); }
	void setTime       (const pfs::time & n) { shared_ptr<unidata> d(new unidata_long(n.millis())); _pimpl.swap(d); }
	void setDate       (const pfs::date & n) { shared_ptr<unidata> d(new unidata_long(n.julianDay())); _pimpl.swap(d); }
	void setDateTime   (const pfs::datetime & n) { shared_ptr<unidata> d(new unidata_long(n.millisSinceEpoch())); _pimpl.swap(d); }

	unitype & operator = (bool b)                 { setBool(b);      return *this; }
	unitype & operator = (char c)                 { setChar(c);      return *this; }
	unitype & operator = (byte_t n)               { setByte(n);      return *this; }
	unitype & operator = (short_t n)              { setShort(n);     return *this; }
	unitype & operator = (ushort_t n)             { setUShort(n);    return *this; }
	unitype & operator = (int_t n)                { setInt(n);       return *this; }
	unitype & operator = (uint_t n)               { setUInt(n);      return *this; }
	unitype & operator = (long_t n)               { setLong(n);      return *this; }
	unitype & operator = (ulong_t n)              { setULong(n);     return *this; }
	unitype & operator = (float n)                { setFloat(n);     return *this; }
	unitype & operator = (double n)               { setDouble(n);    return *this; }
	unitype & operator = (ucchar ch)              { setUCChar(ch);   return *this; }
	unitype & operator = (const pfs::string & s)       { setString(s);    return *this; }
	unitype & operator = (const pfs::bytearray & blob) { setBlob(blob);   return *this; }
	unitype & operator = (const pfs::time & time)      { setTime(time);   return *this; }
	unitype & operator = (const pfs::date & date)      { setDate(date);   return *this; }
	unitype & operator = (const pfs::datetime & dt)    { setDateTime(dt); return *this; }

	bool      toBool   (bool * pok = nullptr) const;
	byte_t    toByte   (bool * pok = nullptr) const;
	sbyte_t   toSByte  (bool * pok = nullptr) const;
	short_t   toShort  (bool * pok = nullptr) const;
	ushort_t  toUShort (bool * pok = nullptr) const;
	int_t     toInt    (bool * pok = nullptr) const;
	uint_t    toUInt   (bool * pok = nullptr) const;
	long_t    toLong   (bool * pok = nullptr) const;
	ulong_t   toULong  (bool * pok = nullptr) const;
	float     toFloat  (bool * pok = nullptr) const;
	double    toDouble (bool * pok = nullptr) const;
	pfs::ucchar    toUCChar (bool * pok = nullptr) const;
	pfs::string    toString (bool * pok = nullptr) const;
	pfs::bytearray toBlob   (bool * pok = nullptr) const;

};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_UNITYPE_HPP__ */
