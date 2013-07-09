/**
 * @file   bytearray_qt.cpp
 * @author wladt
 * @date   Jan 24, 2013 4:15:33 PM
 *
 * @brief
 */

#include "../include/cwt/bytearray.hpp"
#include <QByteArray>

CWT_NS_BEGIN

/*
class ByteArray::Impl : public QByteArray {
public:
	Impl() : QByteArray() {}
	Impl(const char *data, int size)
	#if QT_VERSION < 0x050000
		: QByteArray(data, size < 0 ? strlen(data) : size) {}
	#else
		: QByteArray(data, size) {}
	#endif
	Impl(int size, char ch) : QByteArray(size, ch) {}
	Impl(const Impl &other) : QByteArray(other) {}

	Impl&	operator=(const Impl& other) { QByteArray::operator  = (other); return *this; }
	Impl&	operator=(const char * str) { QByteArray::operator  = (str); return *this; }
};
*/


class ByteArray::Impl : public QByteArray
{
public:
	Impl() : QByteArray("") { } // must be not isNull()
	Impl(const Impl &other) : QByteArray(other) {}
	Impl(const QByteArray &other) : QByteArray(other) {}
	Impl(const char *data) : QByteArray(data) {}
	Impl(const char *data, size_t size) : QByteArray(data, int(size)) { CWT_ASSERT(size <= CWT_INT_MAX); }
	Impl(size_t size, char ch) : QByteArray(int(size), ch) { CWT_ASSERT(size <= CWT_INT_MAX); }
};

CWT_PIMPL_IMPL_COPYABLE(ByteArray)

ByteArray::ByteArray()                              : pimpl(new Impl()) {}
ByteArray::ByteArray(const char *data)              : pimpl(new Impl(data)) {}
ByteArray::ByteArray(const char *data, size_t size) : pimpl(new Impl(data, size)) {}
ByteArray::ByteArray(size_t size, char ch)          : pimpl(new Impl(size, ch)) {}

ByteArray& ByteArray::append(const ByteArray &bytes)     { detach(); pimpl->append(*bytes.pimpl); return *this; }
ByteArray& ByteArray::append(const char *data)           { detach(); pimpl->append(data); return *this; }
ByteArray& ByteArray::append(const char *data, size_t size) { CWT_ASSERT(size <= CWT_INT_MAX); detach(); pimpl->append(data, size); return *this; }
ByteArray& ByteArray::append(char ch)                    { detach(); pimpl->append(ch); return *this; }
char ByteArray::at(size_t i) const                       { CWT_ASSERT(i <= CWT_INT_MAX); return pimpl->at(int(i)); }
char ByteArray::at(ssize_t i) const						 { CWT_ASSERT(i <= CWT_INT_MAX); return pimpl->at(int(i)); }
bool ByteArray::isEmpty() const                          { return pimpl->isEmpty(); }
void ByteArray::clear()                                  { detach(); pimpl->clear(); }
char* ByteArray::data()                                  { detach(); return pimpl->data(); }
const char* ByteArray::data() const                      { return pimpl->data(); }
const char* ByteArray::constData() const                 { return pimpl->data(); }
size_t ByteArray::length() const                         { return size(); }
ByteArray &ByteArray::prepend(const ByteArray &bytes)    { detach(); pimpl->prepend(*bytes.pimpl); return *this; }
ByteArray &ByteArray::prepend(const char *str)           { detach(); pimpl->prepend(str); return *this; }
ByteArray &ByteArray::prepend(const char *str, size_t len)  { CWT_ASSERT(len <= CWT_INT_MAX); detach(); pimpl->prepend(str, int(len)); return *this; }
ByteArray &ByteArray::prepend(char ch)                   { detach(); pimpl->prepend(ch); return *this; }
ByteArray& ByteArray::remove(size_t pos, size_t len)     { detach(); CWT_ASSERT(pos <= CWT_INT_MAX || len > CWT_INT_MAX); pimpl->remove((int)pos, (int)len); return *this; }
void ByteArray::reserve(size_t size)                     { detach(); CWT_ASSERT(size <= CWT_INT_MAX); pimpl->reserve((int)size); }
void ByteArray::resize(size_t size)                      { detach(); CWT_ASSERT(size <= CWT_INT_MAX); pimpl->resize((int)size); }

#ifdef CWT_HAS_INT64
ByteArray& ByteArray::setNumber(long_t n, int base)      { detach(); pimpl->setNum((qlonglong)n, base); return *this; }
ByteArray& ByteArray::setNumber(ulong_t n, int base)     { detach(); pimpl->setNum((qulonglong)n, base); return *this; }
#else
ByteArray& ByteArray::setNumber(long_t n, int base)      { detach(); pimpl->setNum((long)n, base); return *this; }
ByteArray& ByteArray::setNumber(ulong_t n, int base)     { detach(); pimpl->setNum((unsigned long)n, base); return *this; }
#endif
ByteArray& ByteArray::setNumber(int_t n, int base)       { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(uint_t n, int base)      { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(short_t n, int base)     { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(ushort_t n, int base)    { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(double n, char f, int prec) { detach(); pimpl->setNum(n, f, prec); return *this; }
ByteArray& ByteArray::setNumber(float n, char f, int prec)  { detach(); pimpl->setNum(n, f, prec); return *this; }

double	 ByteArray::toDouble(bool *ok) const             { return pimpl->toDouble(ok); }
float	 ByteArray::toFloat(bool *ok) const              { return pimpl->toFloat(ok); }
int_t	 ByteArray::toInt(bool *ok, int base) const      { return pimpl->toInt(ok, base); }
long_t   ByteArray::toLong(bool *ok, int base) const     { return pimpl->toLongLong(ok, base); }
short_t	 ByteArray::toShort(bool *ok, int base) const    { return pimpl->toShort(ok, base); }
uint_t	 ByteArray::toUInt(bool *ok, int base) const     { return pimpl->toUInt(ok, base); }
ulong_t	 ByteArray::toULong(bool *ok, int base) const    { return pimpl->toULongLong(ok, base); }
ushort_t ByteArray::toUShort(bool *ok, int base) const   { return pimpl->toUShort(ok, base); }
ByteArray ByteArray::toBase64 () const
{
	ByteArray ba;
	*ba.pimpl = pimpl->toBase64();
	return ba;

}

ByteArray& ByteArray::setRawData(const char * data, size_t size) { CWT_ASSERT(size <= CWT_INT_MAX); detach(); pimpl->setRawData(data, (uint)size); return *this; }
size_t ByteArray::size() const { return size_t(pimpl->size()); }

/**
 * @fn ssize_t	ByteArray::indexOf (const ByteArray & ba, size_t from) const
 *
 * @brief Finds the index position of the first occurrence of the byte array @c ba in this byte array,
 *        searching forward from index position @c from. Returns -1 if ba could not be found.
 *
 * @param ba byte array searching for.
 * @param from Index position searching forward from.
 * @return Returns the index position of the first occurrence of the byte array @c ba, -1 if @c ba could not be found.
 */
ssize_t	ByteArray::indexOf (const ByteArray & ba, size_t from) const
{
	CWT_ASSERT(from <= CWT_INT_MAX);
	return ssize_t(pimpl->indexOf(ba.pimpl, int(from)));
}

ssize_t	ByteArray::indexOf ( const char * str, size_t from) const
{
	CWT_ASSERT(from <= CWT_INT_MAX);
	return ssize_t(pimpl->indexOf(str, int(from)));
}

ssize_t	ByteArray::indexOf ( char ch, size_t from) const
{
	CWT_ASSERT(from <= CWT_INT_MAX);
	return ssize_t(pimpl->indexOf(ch, int(from)));
}

bool ByteArray::endsWith    (const ByteArray & ba) const { return pimpl->endsWith(*ba.pimpl); }
bool ByteArray::endsWith    (const char * s) const { return pimpl->endsWith(s);}
bool ByteArray::endsWith    (char c) const { return pimpl->endsWith(c); }
bool ByteArray::startsWith(const ByteArray &ba) const { return pimpl->startsWith(*ba.pimpl); }
bool ByteArray::startsWith(const char *s) const { return pimpl->startsWith(s); }
bool ByteArray::startsWith(char c) const { return pimpl->startsWith(c); }

ByteArray ByteArray::substr(size_t pos, size_t n) const
{
	CWT_ASSERT(pos <= CWT_INT_MAX);
	CWT_ASSERT(n <= CWT_INT_MAX);
	ByteArray ba;
	*ba.pimpl = pimpl->mid(int(pos), int(n));
	return ba;
}

ByteArray ByteArray::substr(size_t pos) const
{
	CWT_ASSERT(pos <= CWT_INT_MAX);
	ByteArray ba;
	*ba.pimpl = pimpl->mid(int(pos));
	return ba;
}

//ByteArray&	ByteArray::operator=(const ByteArray &other) { pimpl->operator  = (*other.pimpl); return *this;}
ByteArray&	ByteArray::operator = (const char *str) { detach(); pimpl->operator  = (str); return *this;}

char ByteArray::operator[] (ssize_t i) const { CWT_ASSERT(i <= CWT_INT_MAX); return pimpl->operator [] ((int)i); }
char ByteArray::operator[] (size_t i) const { CWT_ASSERT(i <= CWT_INT_MAX); return pimpl->operator [] ((uint)i); }

// TODO return must be replaced by ByteRef instance
char & ByteArray::operator[] (ssize_t i) { CWT_ASSERT(i <= CWT_INT_MAX); return pimpl->data()[(int)i]; }
char & ByteArray::operator[] (size_t i) { CWT_ASSERT(i <= CWT_INT_MAX); return pimpl->data()[(int)i]; }

bool operator == (const ByteArray &s1, const ByteArray &s2) { return *s1.pimpl == *s2.pimpl; }


ByteArray ByteArray::number(double n, char format, int precision)
{
	ByteArray ba;
	*ba.pimpl = ByteArray::Impl(QByteArray::number(n, format, precision));
	return ba;
}

ByteArray ByteArray::number(float n, char format, int precision)
{
	ByteArray ba;
	*ba.pimpl = ByteArray::Impl(QByteArray::number(n, format, precision));
	return ba;
}

ByteArray ByteArray::number(int_t n, int base)
{
	ByteArray ba;
	*ba.pimpl = ByteArray::Impl(QByteArray::number(n, base));
	return ba;
}

ByteArray ByteArray::number(uint_t n, int base)
{
	ByteArray ba;
	*ba.pimpl = ByteArray::Impl(QByteArray::number(n, base));
	return ba;
}

ByteArray ByteArray::number(long_t n, int base)
{
	ByteArray ba;
	*ba.pimpl =
#ifdef CWT_HAS_INT64
		ByteArray::Impl(QByteArray::number((qlonglong)n, base));
#else
		ByteArray::Impl(QByteArray::number((long)n, base));
#endif
	return ba;
}

ByteArray ByteArray::number(ulong_t n, int base)
{
	ByteArray ba;
	*ba.pimpl =
#ifdef CWT_HAS_INT64
	ByteArray::Impl(QByteArray::number((qulonglong)n, base));
#else
	ByteArray::Impl(QByteArray::number((unsigned long)n, base));
#endif
	return ba;
}

ByteArray ByteArray::fromBase64 (const ByteArray & base64)
{
	ByteArray ba;
	*ba.pimpl = ByteArray::Impl(QByteArray::fromBase64(*base64.pimpl));
	return ba;
}

CWT_NS_END




