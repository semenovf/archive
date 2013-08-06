/**
 * @file   vector.hpp
 * @author wladt
 * @date   Feb 1, 2013 12:21:47 PM
 *
 * @brief
 */

#ifndef __CWT_VECTOR_HPP__
#define __CWT_VECTOR_HPP__

#include <cwt/cwt.h>
#include <cwt/array.hpp>
#include <cwt/shared_ptr.hpp>
#include <cwt/utility.hpp>

CWT_NS_BEGIN

template <typename T>
class DLL_API Vector {
public:
	typedef T char_type;

public:
    class iterator {
    public:
        T *i;

        inline iterator() : i(0) {}
        inline iterator(T *n) : i(n) {}
        inline iterator(const iterator &o): i(o.i){}
        inline T               value() { return *i; }
        inline T &             operator *  () const { return *i; }
        inline T *             operator -> () const { return i; }
        inline T &             operator [] (int j) const { return *(i + j); }
        inline bool            operator == (const iterator &o) const { return i == o.i; }
        inline bool            operator != (const iterator &o) const { return i != o.i; }
        inline bool            operator <  (const iterator &o) const { return i <  o.i; }
        inline bool            operator <= (const iterator &o) const { return i <= o.i; }
        inline bool            operator >  (const iterator &o) const { return i >  o.i; }
        inline bool            operator >= (const iterator &o) const { return i >= o.i; }
        inline iterator&       operator ++ () { ++i; return *this; }
        inline iterator        operator ++ (int) { T *n = i; ++i; return n; }
        inline iterator&       operator -- () { i--; return *this; }
        inline iterator        operator -- (int) { T *n = i; i--; return n; }
        inline iterator&       operator += (size_t j) { i+=j; return *this; }
        inline iterator&       operator -= (size_t j) { i-=j; return *this; }
        inline iterator        operator +  (size_t j) const { return iterator(i+j); }
        inline iterator        operator -  (size_t j) const { return iterator(i-j); }
        inline intptr_t        operator -  (iterator j) const { return i - j.i; }
        inline                 operator T* () const { return i; }
    };
    friend class iterator;

    class const_iterator {
    public:
        const T *i;

        inline                 const_iterator() : i(0)                       {}
        inline                 const_iterator(const T *n) : i(n)             {}
        inline                 const_iterator(const const_iterator &o): i(o.i) {}
        inline explicit        const_iterator(const iterator &o): i(o.i)     {}
        inline T               value() { return *i; }
        inline const T&        operator*()                             const { return *i; }
        inline const T*        operator->()                            const { return i; }
        inline const T&        operator[](size_t j)                    const { return *(i + j); }
        inline bool            operator==(const const_iterator &o)     const { return i == o.i; }
        inline bool            operator!=(const const_iterator &o)     const { return i != o.i; }
        inline bool            operator<(const const_iterator &other)  const { return i < other.i; }
        inline bool            operator<=(const const_iterator &other) const { return i <= other.i; }
        inline bool            operator>(const const_iterator &other)  const { return i > other.i; }
        inline bool            operator>=(const const_iterator &other) const { return i >= other.i; }
        inline const_iterator& operator++()                                  { ++i; return *this; }
        inline const_iterator  operator++(int)                               { const T *n = i; ++i; return n; }
        inline const_iterator& operator--()                                  { i--; return *this; }
        inline const_iterator  operator--(int)                               { const T *n = i; i--; return n; }
        inline const_iterator& operator+=(size_t j)                          { i+=j; return *this; }
        inline const_iterator& operator-=(size_t j)                          { i-=j; return *this; }
        inline const_iterator  operator+(size_t j)                     const { return const_iterator(i+j); }
        inline const_iterator  operator-(size_t j)                     const { return const_iterator(i-j); }
        inline intptr_t        operator-(const_iterator j)             const { return i - j.i; }
        inline                 operator const T*()                     const { return i; }
    };
    friend class const_iterator;

public:
	Vector();
	Vector(size_t size);
	Vector(size_t size, const T & value);
	Vector(const T * values, size_t size) : m_d(new VectorData) { append(values, size); }
	Vector(const Vector<T> &other);

	T&             at(size_t i);
	const T&       at(size_t i) const;
	bool           endsWith (const Vector<T> & end) const;
	void           append(const T &value);
	void           append(const T * value, size_t count);
	void           append(const Vector<T> & other);
	void           clear()         { m_d.detach(); m_d->count = 0; }
	T*             data()          { m_d.detach(); return m_d->data.data(); }
	const T*       data() const    { return m_d->data.data(); }
	const T*       constData() const { return m_d->data.data(); }
	T&             first()         { return at(0); }
	const T&       first() const   { return at(0); }
	//void	       insert (size_t i, const T & value); // TODO implement this method
	bool           isEmpty() const { return m_d->count > 0 ? false : true; }
	T&             last()          { CWT_ASSERT(m_d->count > 0); return at(m_d->count-1); }
	const T&       last() const    { CWT_ASSERT(m_d->count > 0); return at(m_d->count-1); }
	void           prepend(const T & value);
	void           push_back (const T & value) { append(value); }
	void	       remove(size_t pos);
	void	       remove(size_t pos, size_t n);
	void           resize(size_t size)     { reserve(size); m_d->count = size; }
	void           reserve(size_t size);
	size_t         size() const            { return m_d->count; }
	size_t         length() const            { return size(); }
	size_t         length(const const_iterator & begin, const const_iterator & end) const { return end - begin; }
	Vector<T>	   mid(size_t pos, size_t length) const;
	Vector<T>	   left(size_t count) const   { return mid(0, count); }
	Vector<T>	   right(size_t count) const  { return mid(size() - count, count); }
	void           swap(Vector<T> & other)    { m_d.swap(other.m_d); }

    iterator       begin()        { m_d.detach(); return iterator(m_d->data.data()); }
    iterator       end()          { m_d.detach(); return iterator(m_d->data.data()) + m_d->count; }
    const_iterator begin() const  { return const_iterator(m_d->data.data()); }
    const_iterator end() const    { return const_iterator(m_d->data.data()) + m_d->count; }
    const_iterator cbegin() const { return const_iterator(m_d->data.data()); }
    const_iterator cend() const   { return const_iterator(m_d->data.data()) + m_d->count; }

	T&             operator[](size_t i)       { return at(i); }
	const T&       operator[](size_t i) const { return at(i); }
	Vector<T>&	   operator = (const Vector<T> & other);

protected:
	struct VectorData {
		VectorData() : count(0) {}
		VectorData(const VectorData & other) : count(other.count)
		{
			data.alloc(count);
			Array<T>::deep_copy(data, other.data, 0, 0, count);
		}
		Array<T> data;
		size_t   count;
	};

	shareable<VectorData> m_d;
};

template <typename T>
inline Vector<T>::Vector() : m_d(new VectorData)
{
	m_d->count = 0;
//	m_d->data.alloc(size);
}

template <typename T>
inline Vector<T>::Vector(size_t size) : m_d(new VectorData)
{
	m_d->count = 0;
	m_d->data.alloc(size);
}

template <typename T>
Vector<T>::Vector(size_t size, const T &value)	: m_d(new VectorData)
{
	m_d->count = size;
	m_d->data.alloc(size);
	T *d = m_d->data.data();
	for(size_t i = 0; i < size; ++i) {
		d[i] = value;
	}
}

template <typename T>
inline Vector<T>::Vector(const Vector<T> &other) : m_d(other.m_d)
{
}

template <typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T> & other)
{
	m_d = other.m_d;
	return *this;
}

template <typename T>
inline T& Vector<T>::at(size_t i)
{
	CWT_ASSERT(!isEmpty());
	CWT_ASSERT(i < m_d->count);
	m_d.detach();
	return m_d->data.at(i);
}

template <typename T>
inline const T& Vector<T>::at(size_t i) const
{
	CWT_ASSERT(!isEmpty());
	CWT_ASSERT(i < m_d->count);
	return m_d->data.at(i);
}

template <typename T>
bool Vector<T>::endsWith (const Vector<T> & end) const
{
	size_t sz1 = this->size();
	size_t sz2 = end.size();

	if (sz1 == 0 || sz2 == 0)
		return false;

	if (sz1 < sz2)
		return false;

	bool r = true;

	for (size_t i = sz1 - 1, j = sz2 - 1; r && sz2 > 0; --i, --j, --sz2) {
		if (this->at(i) != end[j]) {
			r = false;
		}
	}

	return r;
}

template <typename T>
void Vector<T>::append(const T &value)
{
	m_d.detach();
	if (m_d->count == m_d->data.size())
		reserve(m_d->count + 32);
	T *d = m_d->data.data();
	d[m_d->count++] = value;
}

template <typename T>
void Vector<T>::append(const T * value, size_t count)
{
	m_d.detach();

	if (m_d->count + count > m_d->data.size())
		reserve(m_d->count + CWT_MAX(32, count));

	T *d = m_d->data.data();

	for (size_t i = 0; i < count; ++i) {
		d[m_d->count++] = value[i];
	}
}

template <typename T>
void Vector<T>::append(const Vector<T> & other)
{
	append(other.constData(), other.size());
}


template <typename T>
void Vector<T>::prepend(const T &value)
{
	m_d.detach();
	if (m_d->count == m_d->data.size())
		reserve(m_d->count + 32);
	m_d->data.move(1, 0, m_d->count);

	T *d = m_d->data.data();
	d[0] = value;
	m_d->count++;
}

template <typename T>
inline void Vector<T>::remove(size_t i)
{
	remove(i, 1);
}

template <typename T>
void Vector<T>::remove(size_t i, size_t n)
{
	if (m_d->count > 0) {
		CWT_ASSERT(i < m_d->count);
		n = CWT_MIN(m_d->count - i, n);
		m_d.detach();
		if (i + n < m_d->count) { // removing not last elements
			m_d->data.move(i, i + n, m_d->count - i - n);
		}
		m_d->count -= n;
	}
}

template <typename T>
inline void Vector<T>::reserve(size_t size)
{
	m_d.detach();
	if (size > m_d->data.size())
		m_d->data.realloc(size);
}

template <typename T>
Vector<T> Vector<T>::mid(size_t pos, size_t length) const
{
	Vector<T> r;

	if (pos < this->size()) {
		length = CWT_MIN(length, this->size() - pos);
		r.resize(length);
		for (size_t i = 0, j = pos; i < length; ++i, ++j)
			r[i] = this->at(j);
	}

	return r;
}

CWT_NS_END

#endif /* __CWT_VECTOR_HPP__ */
