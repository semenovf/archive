/*
 * unitype.hpp
 *
 *  Created on: Jun 3, 2013
 *      Author: wladt
 */

#ifndef __CWT_UNITYPE_HPP__
#define __CWT_UNITYPE_HPP__

#include <cwt/string.hpp>
#include <cwt/bytearray.hpp>
#include <cwt/time.hpp>
#include <cwt/date.hpp>
#include <cwt/datetime.hpp>
#include <cwt/shared_ptr.hpp>

CWT_NS_BEGIN

typedef long_t integer_type;
typedef double number_type;

class UniType;
typedef UniType unitype;

struct AbstractUniObject {
	virtual ~AbstractUniObject() { ; }
};

template <typename T>
struct UniObject : public AbstractUniObject
{
	UniObject (const T & v) : value(v) { ; }
	UniObject & operator = (const T & v) { value = v; return *this; }
	~UniObject () { ; }
	T value;

/*
	T   getValue() const { return value; }
	T & getValue() { return value; }
*/

/*
	T &   getRef() const { return value; }
	const T & getRef()   { return value; }
*/

};


class DLL_API UniType
{
public:
	enum Type {
		  NullValue
		, BoolValue
		, LongValue
		, IntegerValue = LongValue
		, FloatValue
		, DoubleValue
		, StringValue
		, BlobValue
		, BinaryValue = BlobValue
		, TimeValue
		, DateValue
		, DateTimeValue
		, ObjectValue
	};

public:
	UniType ()           : m_d(new SharedData)          { m_d->type = NullValue; }
	UniType (bool v)     : m_d(new SharedData)          { m_d->type = BoolValue; m_d->d.long_val = long_t(v ? 1L : 0L); }
	UniType (char v)     : m_d(new SharedData)          { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType (byte_t v)   : m_d(new SharedData)          { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType (short_t v)  : m_d(new SharedData)          { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType (ushort_t v) : m_d(new SharedData)          { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType (int_t v)    : m_d(new SharedData)          { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType (uint_t v)   : m_d(new SharedData)          { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
//#if CWT_OS_BITS == 32 // 'long_t' is 'long long'
#ifdef CWT_HAVE_INT64
	UniType (long v)          : m_d(new SharedData)     { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType (unsigned long v) : m_d(new SharedData)     { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
#endif
	UniType (long_t v)   : m_d(new SharedData)          { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType (ulong_t v)  : m_d(new SharedData)          { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType (float v)    : m_d(new SharedData)          { m_d->type = FloatValue; m_d->d.double_val = v; }
	UniType (double v)   : m_d(new SharedData)          { m_d->type = DoubleValue; m_d->d.double_val = v; }
	UniType (const String & v) : m_d(new SharedData)    { m_d->type = StringValue; m_d->d.string_val = new String(v); }
	UniType (const char * v) : m_d(new SharedData)      { m_d->type = StringValue; m_d->d.string_val = new String(v); }
	UniType (const ByteArray & v) : m_d(new SharedData) { m_d->type = BlobValue; m_d->d.blob_val = new ByteArray(v); }
	UniType (const Time & v) : m_d(new SharedData)      { m_d->type = TimeValue; m_d->d.time_val = new Time(v); }
	UniType (const Date & v) : m_d(new SharedData)      { m_d->type = DateValue; m_d->d.date_val = new Date(v); }
	UniType (const DateTime & v) : m_d(new SharedData)  { m_d->type = DateTimeValue; m_d->d.datetime_val = new DateTime(v); }

	UniType(const UniType & other) { m_d = other.m_d; }
	UniType & operator = (const UniType & other)
	{
		m_d = other.m_d;
		return *this;
	}

	template <typename T>
	static UniType make_object ();

	bool     isNull () const { return m_d->type == NullValue; }
	Type type   () const { return m_d->type; }

	void setFromString (const String &s);
	void setNull       ();
	void setBool       (bool b);
	void setChar       (char c)     { setLong(long_t(c)); }
	void setByte       (byte_t n)   { setLong(long_t(n)); }
	void setShort      (short_t n)  { setLong(long_t(n)); }
	void setUShort     (ushort_t n) { setLong(long_t(n)); }
	void setInt        (int_t n)    { setLong(long_t(n)); }
	void setUInt       (uint_t n)   { setLong(long_t(n)); }
	void setLong       (long_t n);
	void setULong      (ulong_t n)  { setLong(long_t(n)); }
	void setFloat      (float n);
	void setDouble     (double n);
	void setUChar      (UChar ch)   { setLong(long_t(ch)); }
	void setString     (const String & s);
	void setBlob       (const char * blob, size_t sz);
	void setBlob       (const ByteArray & blob);
	void setTime       (const Time & time);
	void setDate       (const Date & date);
	void setDateTime   (const DateTime & dt);

	UniType & operator = (bool b)                 { setBool(b);      return *this; }
	UniType & operator = (char c)                 { setChar(c);      return *this; }
	UniType & operator = (byte_t n)               { setByte(n);      return *this; }
	UniType & operator = (short_t n)              { setShort(n);     return *this; }
	UniType & operator = (ushort_t n)             { setUShort(n);    return *this; }
	UniType & operator = (int_t n)                { setInt(n);       return *this; }
	UniType & operator = (uint_t n)               { setUInt(n);      return *this; }
	UniType & operator = (long_t n)               { setLong(n);      return *this; }
	UniType & operator = (ulong_t n)              { setULong(n);     return *this; }
	UniType & operator = (float n)                { setFloat(n);     return *this; }
	UniType & operator = (double n)               { setDouble(n);    return *this; }
	UniType & operator = (UChar ch)               { setUChar(ch);    return *this; }
	UniType & operator = (const String & s)       { setString(s);    return *this; }
	UniType & operator = (const ByteArray & blob) { setBlob(blob);   return *this; }
	UniType & operator = (const Time & time)      { setTime(time);   return *this; }
	UniType & operator = (const Date & date)      { setDate(date);   return *this; }
	UniType & operator = (const DateTime & dt)    { setDateTime(dt); return *this; }

	template <typename T>
	void setObject (const T & o);

	void swap (UniType & other) { m_d.swap(other.m_d); }

	bool      boolean () const  { return toBool(); }
	long_t    integer () const  { return toLong(); }
	double    number  () const  { return toDouble(); }
	String    string  () const  { return toString(); }

	bool      toBool   (bool * ok = nullptr) const;
	byte_t    toByte   (bool * ok = nullptr) const;
	sbyte_t   toSByte  (bool * ok = nullptr) const;
	short_t   toShort  (bool * ok = nullptr) const;
	ushort_t  toUShort (bool * ok = nullptr) const;
	int_t     toInt    (bool * ok = nullptr) const;
	uint_t    toUInt   (bool * ok = nullptr) const;
	long_t    toLong   (bool * ok = nullptr) const;
	ulong_t   toULong  (bool * ok = nullptr) const;
	float     toFloat  (bool * ok = nullptr) const;
	double    toDouble (bool * ok = nullptr) const;
	UChar     toUChar  (bool * ok = nullptr) const;
	String    toString (bool * ok = nullptr) const;
	ByteArray toBlob   (bool * ok = nullptr) const;

	template <typename T>
	T         toObject (bool * ok = nullptr) const;

	template <typename T>
	T &       objectRef()       { return dynamic_cast<UniObject<T>* >(m_d->d.object_val)->value; }

	template <typename T>
	const T & objectRef() const { return dynamic_cast<UniObject<T>* >(m_d->d.object_val)->value; }

	static String typeToString(UniType::Type t);
	static UniType::Type typeFromString(const String & s);

private:
	struct SharedData {
		Type type;
		union {
			long_t              long_val;
			double              double_val;
			String *            string_val;
			ByteArray *         blob_val;
			Time *              time_val;
			Date *              date_val;
			DateTime *          datetime_val;
			AbstractUniObject * object_val;
		} d;
		~SharedData();
	};

	SharedData* clone();
	void detach();
	void setUniTypeData(SharedData *d);

private:
	shared_ptr<SharedData> m_d;
};

template <typename T>
inline UniType UniType::make_object ()
{
	UniType ut;
	ut.m_d->type = ObjectValue;
	ut.m_d->d.object_val = new UniObject<T>(T());
	return ut;
}

template <typename T>
T UniType::toObject(bool * ok) const
{
	if (ok) *ok = false;

	if (m_d->type == ObjectValue) {
		if (ok)
			*ok = true;
		return (dynamic_cast<UniObject<T>* >(m_d->d.object_val))->value;
	}
	return T();
}

inline void UniType::detach()
{
	if (!m_d.unique()) {
		shared_ptr<SharedData> d(clone());
		m_d.swap(d);
	}
}

inline void UniType::setUniTypeData(SharedData * data)
{
	shared_ptr<SharedData> d(data);
	m_d.swap(d);
}


template <typename T>
void UniType::setObject(const T & o)
{
	SharedData *d = new SharedData;
	d->type = UniType::ObjectValue;
	d->d.object_val = new UniObject<T>(o);
	setUniTypeData(d);
}

CWT_NS_END

#endif /* __CWT_UNITYPE_HPP__ */
