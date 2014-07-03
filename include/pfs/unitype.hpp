/**
 * @file unitype.hpp
 * @author wladt
 * @date Apr 10, 2014
 */

#ifndef __PFS_UNITYPE_HPP__
#define __PFS_UNITYPE_HPP__

#include <pfs/string.hpp>
#include <pfs/bytearray.hpp>

namespace pfs {

static const int Null      = 0;
static const int Boolean   = 1;
static const int Integer   = 2;
static const int Float     = 3;
static const int String    = 4;
static const int ByteArray = 5;
static const int Bool = Boolean;
static const int Double = Float;
static const int Blob = ByteArray;

// Type Name
template <typename T> pfs::string unitype_type_name ();

template <> inline pfs::string unitype_type_name<bool> () { return _l1("bool"); }
template <> inline pfs::string unitype_type_name<char> () { return _l1("char"); }
template <> inline pfs::string unitype_type_name<unsigned char> () { return _l1("byte"); }
template <> inline pfs::string unitype_type_name<short> () { return _l1("short int"); }
template <> inline pfs::string unitype_type_name<unsigned short> () { return _l1("unsigned short int"); }
template <> inline pfs::string unitype_type_name<int> () { return _l1("int"); }
template <> inline pfs::string unitype_type_name<unsigned int> () { return _l1("unsigned int"); }
template <> inline pfs::string unitype_type_name<long> () { return _l1("long int"); }
template <> inline pfs::string unitype_type_name<unsigned long> () { return _l1("unsigned long int"); }
template <> inline pfs::string unitype_type_name<float> () { return _l1("float"); }
template <> inline pfs::string unitype_type_name<double> () { return _l1("double"); }
#ifdef HAVE_LONGLONG
template <> inline pfs::string unitype_type_name<long long> () { return _l1("long long int"); }
template <> inline pfs::string unitype_type_name<unsigned long long> () { return _l1("unsigned long long int"); }
#endif
template <> inline pfs::string unitype_type_name<pfs::string> () { return _l1("pfs::string"); }
template <> inline pfs::string unitype_type_name<pfs::bytearray> () { return _l1("pfs::bytearray"); }

// Type Id
template <typename T> int unitype_type_id ();

template <> inline int unitype_type_id<bool> () { return Boolean; }
template <> inline int unitype_type_id<char> () { return Integer; }
template <> inline int unitype_type_id<unsigned char> () { return Integer; }
template <> inline int unitype_type_id<short> () { return Integer; }
template <> inline int unitype_type_id<unsigned short> () { return Integer; }
template <> inline int unitype_type_id<int> () { return Integer; }
template <> inline int unitype_type_id<unsigned int> () { return Integer; }
template <> inline int unitype_type_id<long> () { return Integer; }
template <> inline int unitype_type_id<unsigned long> () { return Integer; }
template <> inline int unitype_type_id<float> () { return Float; }
template <> inline int unitype_type_id<double> () { return Float; }
#ifdef HAVE_LONGLONG
template <> inline int unitype_type_id<long long> () { return Integer; }
template <> inline int unitype_type_id<unsigned long long> () { return Integer; }
#endif
template <> inline int unitype_type_id<pfs::string> () { return String; }
template <> inline int unitype_type_id<pfs::bytearray> () { return ByteArray; }


class unitype_base
{
public:
	virtual ~unitype_base () {}
	virtual pfs::string type_name () const = 0;
	virtual int type_id () const { return -1; }

	virtual bool isNull () const = 0;
	virtual bool toBoolean () const = 0;
	virtual long_t toInteger () const = 0;
	virtual ulong_t toUInteger () const = 0;
	virtual double toFloat () const = 0;
	virtual pfs::string toString () const = 0;
	virtual pfs::bytearray toByteArray () const = 0;
	pfs::bytearray toBlob () const { return toByteArray(); }
};


class unitype_null : public unitype_base
{
public:
	virtual ~unitype_null () {}
	virtual pfs::string type_name () const override { return pfs::string(); }
	virtual int type_id () const { return Null; }

	virtual bool isNull () const { return true; }

	virtual bool toBoolean () const  { return false; }
	virtual long_t toInteger () const   { return 0; }
	virtual ulong_t toUInteger () const   { return 0; }
	virtual double toFloat () const { return double(0.0f); }
	virtual pfs::string toString () const { return pfs::string(); }
	virtual pfs::bytearray toByteArray () const { return pfs::bytearray(); }
};

template <typename T> bool to_boolean_trait (const T & v);
template <> inline bool to_boolean_trait<bool> (const bool & v) { return v; }
template <> inline bool to_boolean_trait<char> (const char & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<unsigned char> (const unsigned char & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<short> (const short & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<unsigned short> (const unsigned short & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<int> (const int & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<unsigned int> (const unsigned int & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<long> (const long & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<unsigned long> (const unsigned long & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<float> (const float & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<double> (const double & v) { return v ? true : false; }
#ifdef HAVE_LONGLONG
template <> inline bool to_boolean_trait<long long> (const long long & v) { return v ? true : false; }
template <> inline bool to_boolean_trait<unsigned long long> (const unsigned long long & v) { return v ? true : false; }
#endif
template <> inline bool to_boolean_trait<pfs::string> (const pfs::string & v)
{
	return v.isEmpty() ? false
		: (v == "false" ? false : true);
}
template <> inline bool to_boolean_trait<pfs::bytearray> (const pfs::bytearray & v)
{
	return v.isEmpty() ? false
		: (v[0] == '\x00' ? false : true);
}

template <typename T> long_t to_integer_trait (const T & v);
template <> inline long_t to_integer_trait<bool> (const bool & v) { return v ? long_t(1) : long_t(0); }
template <> inline long_t to_integer_trait<char> (const char & v) { return long_t(v); }
template <> inline long_t to_integer_trait<unsigned char> (const unsigned char & v) { return long_t(v); }
template <> inline long_t to_integer_trait<short> (const short & v) { return long_t(v); }
template <> inline long_t to_integer_trait<unsigned short> (const unsigned short & v) { return long_t(v); }
template <> inline long_t to_integer_trait<int> (const int & v) { return long_t(v); }
template <> inline long_t to_integer_trait<unsigned int> (const unsigned int & v) { return long_t(v); }
template <> inline long_t to_integer_trait<long> (const long & v) { return long_t(v); }
template <> inline long_t to_integer_trait<unsigned long> (const unsigned long & v) { return long_t(v); }
template <> inline long_t to_integer_trait<float> (const float & v) { return long_t(v); }
template <> inline long_t to_integer_trait<double> (const double & v) { return long_t(v); }
#ifdef HAVE_LONGLONG
template <> inline long_t to_integer_trait<long long> (const long long & v) { return long_t(v); }
template <> inline long_t to_integer_trait<unsigned long long> (const unsigned long long & v) { return long_t(v); }
#endif
template <> inline long_t to_integer_trait<pfs::string> (const pfs::string & v) { return v.toLong(); }
template <> inline long_t to_integer_trait<pfs::bytearray> (const pfs::bytearray & v) { return v.toLong(); }


template <typename T> ulong_t to_uinteger_trait (const T & v);
template <> inline ulong_t to_uinteger_trait<bool> (const bool & v) { return v ? ulong_t(1) : ulong_t(0); }
template <> inline ulong_t to_uinteger_trait<char> (const char & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<unsigned char> (const unsigned char & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<short> (const short & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<unsigned short> (const unsigned short & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<int> (const int & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<unsigned int> (const unsigned int & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<long> (const long & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<unsigned long> (const unsigned long & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<float> (const float & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<double> (const double & v) { return ulong_t(v); }
#ifdef HAVE_LONGLONG
template <> inline ulong_t to_uinteger_trait<long long> (const long long & v) { return ulong_t(v); }
template <> inline ulong_t to_uinteger_trait<unsigned long long> (const unsigned long long & v) { return ulong_t(v); }
#endif
template <> inline ulong_t to_uinteger_trait<pfs::string> (const pfs::string & v) { return v.toULong(); }
template <> inline ulong_t to_uinteger_trait<pfs::bytearray> (const pfs::bytearray & v) { return v.toULong(); }


template <typename T> double to_float_trait (const T & v);
template <> inline double to_float_trait<bool> (const bool & v) { return v ? double(1) : double(0); }
template <> inline double to_float_trait<char> (const char & v) { return double(v); }
template <> inline double to_float_trait<unsigned char> (const unsigned char & v) { return double(v); }
template <> inline double to_float_trait<short> (const short & v) { return double(v); }
template <> inline double to_float_trait<unsigned short> (const unsigned short & v) { return double(v); }
template <> inline double to_float_trait<int> (const int & v) { return double(v); }
template <> inline double to_float_trait<unsigned int> (const unsigned int & v) { return double(v); }
template <> inline double to_float_trait<long> (const long & v) { return double(v); }
template <> inline double to_float_trait<unsigned long> (const unsigned long & v) { return double(v); }
template <> inline double to_float_trait<float> (const float & v) { return double(v); }
template <> inline double to_float_trait<double> (const double & v) { return v; }
#ifdef HAVE_LONGLONG
template <> inline double to_float_trait<long long> (const long long & v) { return double(v); }
template <> inline double to_float_trait<unsigned long long> (const unsigned long long & v) { return double(v); }
#endif
template <> inline double to_float_trait<pfs::string> (const pfs::string & v) { return v.toDouble(); }
template <> inline double to_float_trait<pfs::bytearray> (const pfs::bytearray & v) { return v.toDouble(); }

template <typename T> pfs::string to_string_trait (const T & v);
template <> inline pfs::string to_string_trait<bool> (const bool & v) { return v ? pfs::string("true") : pfs::string("false"); }
template <> inline pfs::string to_string_trait<char> (const char & v) { return pfs::string(1, v); }
template <> inline pfs::string to_string_trait<unsigned char> (const unsigned char & v) { return pfs::string(1, (const char)v); }
template <> inline pfs::string to_string_trait<short> (const short & v) { return pfs::string::number(v); }
template <> inline pfs::string to_string_trait<unsigned short> (const unsigned short & v) { return pfs::string::number(v); }
template <> inline pfs::string to_string_trait<int> (const int & v) { return pfs::string::number(v); }
template <> inline pfs::string to_string_trait<unsigned int> (const unsigned int & v) { return pfs::string::number(v); }
template <> inline pfs::string to_string_trait<long> (const long & v) { return pfs::string::number(v); }
template <> inline pfs::string to_string_trait<unsigned long> (const unsigned long & v) { return pfs::string::number(v); }
template <> inline pfs::string to_string_trait<float> (const float & v) { return pfs::string::number(v); }
template <> inline pfs::string to_string_trait<double> (const double & v) { return pfs::string::number(v); }
#ifdef HAVE_LONGLONG
template <> inline pfs::string to_string_trait<long long> (const long long & v) { return pfs::string::number(long_t(v)); }
template <> inline pfs::string to_string_trait<unsigned long long> (const unsigned long long & v) { return pfs::string::number(ulong_t(v)); }
#endif
template <> inline pfs::string to_string_trait<pfs::string> (const pfs::string & v) { return pfs::string(v); }
template <> inline pfs::string to_string_trait<pfs::bytearray> (const pfs::bytearray & v) { return _l1(v.toBase64()); }

template <typename T> pfs::bytearray to_bytearray_trait (const T & v);
template <> inline pfs::bytearray to_bytearray_trait<bool> (const bool & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<char> (const char & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<unsigned char> (const unsigned char & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<short> (const short & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<unsigned short> (const unsigned short & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<int> (const int & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<unsigned int> (const unsigned int & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<long> (const long & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<unsigned long> (const unsigned long & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<float> (const float & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<double> (const double & v) { return pfs::bytearray::toBytes(v); }
#ifdef HAVE_LONGLONG
template <> inline pfs::bytearray to_bytearray_trait<long long> (const long long & v) { return pfs::bytearray::toBytes(v); }
template <> inline pfs::bytearray to_bytearray_trait<unsigned long long> (const unsigned long long & v) { return pfs::bytearray::toBytes(v); }
#endif
template <> inline pfs::bytearray to_bytearray_trait<pfs::string> (const pfs::string & v) { return pfs::bytearray(v.constData(), v.sizeInBytes()); }
template <> inline pfs::bytearray to_bytearray_trait<pfs::bytearray> (const pfs::bytearray & v) { return pfs::bytearray(v); }


// bool, integers, bytearray, string
template <typename T>
class unitype_impl : public unitype_base
{
	T _value;

public:
	unitype_impl () {}
	unitype_impl (const T & o) : _value(o) {}

	virtual pfs::string type_name () const override { return unitype_type_name<T>(); }
	virtual int type_id () const override { return unitype_type_id<T>(); }

	virtual bool isNull     () const { return false; }

	virtual bool           toBoolean   () const { return to_boolean_trait(_value); }
	virtual long_t         toInteger   () const { return to_integer_trait(_value); }
	virtual ulong_t        toUInteger  () const { return to_uinteger_trait(_value); }
	virtual double         toFloat     () const { return to_float_trait(_value); }
	virtual pfs::string    toString    () const { return to_string_trait(_value); }
	virtual pfs::bytearray toByteArray () const { return to_bytearray_trait(_value); }
};

class unitype
{
	pimpl _d;

public:
	unitype () : _d(new unitype_null) {}

	template <typename T>
	unitype (const T & o) : _d(new unitype_impl<T>(o)) {}

	template <typename T>
	unitype & operator = (const T & o)
	{
		_d = pimpl(new unitype_impl<T>(o));
		return *this;
	}

	template <typename T>
	static unitype make ()
	{
		return unitype(T());
	}

	pfs::string type_name () const override { return _d.cast<unitype_base>()->type_name(); }
	int type_id () const override { return _d.cast<unitype_base>()->type_id(); }

	bool           isNull      () const { return _d.cast<unitype_base>()->isNull(); }
	bool           toBoolean   () const { return _d.cast<unitype_base>()->toBoolean(); }
	long_t         toInteger   () const { return _d.cast<unitype_base>()->toInteger(); }
	long_t         toUInteger  () const { return _d.cast<unitype_base>()->toUInteger(); }
	double         toFloat     () const { return _d.cast<unitype_base>()->toFloat(); }
	pfs::string    toString    () const { return _d.cast<unitype_base>()->toString(); }
	pfs::bytearray toByteArray () const { return _d.cast<unitype_base>()->toByteArray(); }

	double         toDouble    () const { return toFloat(); }
	pfs::bytearray toBlob      () const { return toByteArray(); }
};


} // pfs

#endif /* __PFS_UNITYPE_HPP__ */
