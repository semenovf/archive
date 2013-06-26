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
#include <cwt/shared_ptr.hpp>

CWT_NS_BEGIN

typedef long_t integer_type;
typedef double number_type;

struct AbstractUniObject {
	virtual ~AbstractUniObject() { ; }
};

template <typename T>
struct UniObject : public AbstractUniObject
{
	UniObject(const T &v) : value(v) { ; }
	UniObject& operator = (const T &v) { value = v; return *this; }
	~UniObject() { ; }
	T value;

	T getValue() const { return value; }
	T& getValue() { return value; }
};


class UniType {
public:
	enum TypeEnum {
		  NullValue
		, BoolValue
		, LongValue
		, FloatValue
		, DoubleValue
		, StringValue
		, BlobValue
		, ObjectValue
	};

public:
	UniType() : m_d(new Data)                   { m_d->type = NullValue; }
	UniType(bool v) : m_d(new Data)             { m_d->type = BoolValue; m_d->d.long_val = long_t(v ? 1L : 0L); }
	UniType(int_t v) : m_d(new Data)            { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType(uint_t v) : m_d(new Data)           { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType(long_t v) : m_d(new Data)           { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType(ulong_t v) :m_d(new Data)           { m_d->type = LongValue; m_d->d.long_val = long_t(v); }
	UniType(float v) : m_d(new Data)            { m_d->type = FloatValue; m_d->d.float_val = v; }
	UniType(double v) : m_d(new Data)           { m_d->type = DoubleValue; m_d->d.double_val = v; }
	UniType(Char v) : m_d(new Data)             { m_d->type = LongValue; m_d->d.long_val = long_t(v.unicode()); }
	UniType(const String &v) : m_d(new Data)    { m_d->type = StringValue; m_d->d.string_val = new String(v); }
	UniType(const ByteArray &v) : m_d(new Data) { m_d->type = BlobValue; m_d->d.blob_val = new ByteArray(v); }

	template <typename T>
	static UniType make_object(const T &v);

	UniType(const UniType &other) { m_d = other.m_d; }
	UniType& operator = (const UniType &other)
	{
		m_d = other.m_d;
		return *this;
	}

	bool isNull() const { return m_d->type == NullValue; }
	TypeEnum type() const { return m_d->type; }

	void setFromString (const String &s);
	void setNull       ();
	void setBool       (bool b);
	void setInt        (int_t n)   { setLong(long_t(n)); }
	void setUInt       (uint_t n)  { setLong(long_t(n)); }
	void setLong       (long_t n);
	void setULong      (ulong_t n) { setLong(long_t(n)); }
	void setFloat      (float n);
	void setDouble     (double n);
	void setChar       (Char ch)   { setLong(long_t(ch.unicode())); }
	void setString     (const String &s);
	void setBlob       (const char *blob, size_t sz);
	void setBlob       (const ByteArray &blob);

	template <typename T>
	void setObject(const T & o);

	void swap(UniType &other) { m_d.swap(other.m_d); }

	bool      boolean() const  { return toBool(); }
	long_t    integer() const  { return toLong(); }
	double    number() const   { return toDouble(); }
	String    string() const   { return toString(); }

	bool      toBool(bool *ok = NULL) const;
	byte_t    toByte(bool *ok = NULL) const;
	sbyte_t   toSByte(bool *ok = NULL) const;
	short_t   toShort(bool *ok = NULL) const;
	ushort_t  toUShort(bool *ok = NULL) const;
	int_t     toInt(bool *ok = NULL) const;
	uint_t    toUInt(bool *ok = NULL) const;
	long_t    toLong(bool *ok = NULL) const;
	ulong_t   toULong(bool *ok = NULL) const;
	float     toFloat(bool *ok = NULL) const;
	double    toDouble(bool *ok = NULL) const;
	Char      toChar(bool *ok = NULL) const;
	String    toString(bool *ok = NULL) const;
	ByteArray toBlob(bool *ok = NULL) const;

	template <typename T>
	T         toObject(bool *ok = NULL) const;


/* Do not use this methods!!! For internal use only !!! */
	long_t& longRef()    { return m_d->d.long_val; }
	const long_t& longRef() const    { return m_d->d.long_val; }
	float& floatRef()    { return m_d->d.float_val; }
	const float& floatRef() const { return m_d->d.float_val; }
	double& doubleRef()  { return m_d->d.double_val; }
	const double& doubleRef() const { return m_d->d.double_val; }
	String& stringRef()  { return *m_d->d.string_val; }
	const String& stringRef() const { return *m_d->d.string_val; }
	ByteArray& blobRef() { return *m_d->d.blob_val; }
	const ByteArray& blobRef() const { return *m_d->d.blob_val; }

	template <typename T>
	T& objectRef() { return dynamic_cast<UniObject<T>* >(m_d->d.object_val)->getValue(); }
	template <typename T>
	const T& objectRef() const { return dynamic_cast<UniObject<T>* >(m_d->d.object_val)->getValue(); }

private:
	struct Data {
		TypeEnum type;
		union {
			long_t     long_val;
			float      float_val;
			double     double_val;
			String    *string_val;
			ByteArray *blob_val;
			AbstractUniObject *object_val;
		} d;
		~Data();
	};

	Data* clone();
	void detach();
	void setUniTypeData(Data *d);

private:
	shared_ptr<Data> m_d;
};

template <typename T>
inline UniType UniType::make_object(const T &v)
{
	UniType ut;
	ut.m_d->type = ObjectValue;
	ut.m_d->d.object_val = new UniObject<T>(v);
	return ut;
}


template <typename T>
T UniType::toObject(bool *ok) const
{
	if (ok) *ok = false;

	if (m_d->type == ObjectValue) {
		if (ok)
			*ok = true;
		return (dynamic_cast<UniObject<T>* >(m_d->d.object_val))->getValue();
	}
	return T();
}

inline void UniType::detach()
{
	if (!m_d.unique()) {
		shared_ptr<Data> d(clone());
		m_d.swap(d);
	}
}

inline void UniType::setUniTypeData(Data *data)
{
	shared_ptr<Data> d(data);
	m_d.swap(d);
}

inline void UniType::setNull()
{
	Data *d = new Data;
	d->type = UniType::NullValue;
	setUniTypeData(d);
}

inline void UniType::setLong (long_t n)
{
	Data *d = new Data;
	d->type = UniType::LongValue;
	d->d.long_val = long_t(n);
	setUniTypeData(d);
}

inline void UniType::setBool (bool b)
{
	setLong(b ? 1L : 0L);
	m_d->type = UniType::BoolValue;
}

inline void UniType::setFloat (float n)
{
	Data *d = new Data;
	d->type = UniType::FloatValue;
	d->d.float_val = n;
	setUniTypeData(d);
}

inline void UniType::setDouble (double n)
{
	Data *d = new Data;
	d->type = UniType::DoubleValue;
	d->d.double_val = n;
	setUniTypeData(d);
}

inline void UniType::setString (const String &s)
{
	Data *d = new Data;
	d->type = UniType::StringValue;
	d->d.string_val = new String(s);
	setUniTypeData(d);
}

inline void UniType::setBlob (const char *blob, size_t sz)
{
	ByteArray ba(blob, sz);
	setBlob(ba);
}

inline void UniType::setBlob (const ByteArray &blob)
{
	Data *d = new Data;
	d->type = UniType::BlobValue;
	d->d.blob_val = new ByteArray(blob);
	setUniTypeData(d);
}

template <typename T>
void UniType::setObject(const T & o)
{
	Data *d = new Data;
	d->type = UniType::ObjectValue;
	d->d.object_val = new UniObject<T>(o);
	setUniTypeData(d);
}

CWT_NS_END

#endif /* __CWT_UNITYPE_HPP__ */
