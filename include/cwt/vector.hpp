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
class Vector {
public:
    class iterator {
    public:
        T *i;

        inline iterator() : i(0) {}
        inline iterator(T *n) : i(n) {}
        inline iterator(const iterator &o): i(o.i){}
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
        inline iterator&       operator += (int j) { i+=j; return *this; }
        inline iterator&       operator -= (int j) { i-=j; return *this; }
        inline iterator        operator +  (int j) const { return iterator(i+j); }
        inline iterator        operator -  (int j) const { return iterator(i-j); }
        inline int             operator -  (iterator j) const { return i - j.i; }
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
        inline const T&        operator*()                             const { return *i; }
        inline const T*        operator->()                            const { return i; }
        inline const T&        operator[](int j)                       const { return *(i + j); }
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
        inline const_iterator& operator+=(int j)                             { i+=j; return *this; }
        inline const_iterator& operator-=(int j)                             { i-=j; return *this; }
        inline const_iterator  operator+(int j)                        const { return const_iterator(i+j); }
        inline const_iterator  operator-(int j)                        const { return const_iterator(i-j); }
        inline int             operator-(const_iterator j)             const { return i - j.i; }
        inline                 operator const T*()                     const { return i; }
    };
    friend class const_iterator;

public:
	Vector();
	Vector(int size);
	Vector(int size, const T &value);
	Vector(const Vector<T> &other);

	T&             at(size_t i);
	const T&       at(size_t i) const;
	void           append(const T &value);
	void           clear()         { detach(); m_d->count = 0; }
	T*             data()          { detach(); return m_d->data.data(); }
	const T*       data() const    { return m_d->data.data(); }
	T&             first()         { return at(0); }
	const T&       first() const   { return at(0); }
	bool           isEmpty() const { return m_d.use_count() > 0 && m_d->count > 0 ? false : true; }
	T&             last()          { CWT_ASSERT(m_d->count > 0); return at(m_d->count-1); }
	const T&       last() const    { CWT_ASSERT(m_d->count > 0); return at(m_d->count-1); }
	void           prepend(const T &value);
	void	       remove(size_t i);
	void	       remove(size_t i, size_t count);
	void           resize(size_t size)     { reserve(size); m_d->count = size; }
	void           reserve(size_t size)    { detachAndRealloc(CWT_MAX(size, m_d->data.size())); }
	size_t         size() const            { return m_d->count; }
	void           swap(Vector<T> & other) { m_d.swap(other.m_d); }

    iterator       begin()        { detach(); return iterator(m_d->data.data()); }
    iterator       end()          { detach(); return iterator(m_d->data.data()) + m_d->count; }
    const_iterator begin() const  { return const_iterator(m_d->data.data()); }
    const_iterator end() const    { return const_iterator(m_d->data.data()) + m_d->count; }
    const_iterator cbegin() const { return const_iterator(m_d->data.data()); }
    const_iterator cend() const   { return const_iterator(m_d->data.data()) + m_d->count; }

	T&             operator[](size_t i)       { return at(i); }
	const T&       operator[](size_t i) const { return at(i); }
	Vector<T>&	   operator = (const Vector<T> & other);

protected:
	void           detach() { detachAndRealloc(m_d->count); }
	void           detachAndRealloc(size_t newsize);

protected:
	typedef struct {
		Array<T> data;
		size_t   count;
	} VectorData;

	shared_ptr<VectorData> m_d;
};


template <typename T>
void Vector<T>::detachAndRealloc(size_t newsize)
{
	if (m_d.use_count() > 1) {
		shared_ptr<VectorData> d(new VectorData);
		m_d.swap(d);
		m_d->data.alloc(d->data.capacity());
		Array<T>::deep_copy(m_d->data, d->data, 0, 0, CWT_MIN(newsize, d->count));
	} else {
		if (newsize > m_d->data.capacity())
			m_d->data.realloc(newsize);
	}
	m_d->count = CWT_MIN(newsize, m_d->count);
}


template <typename T>
inline Vector<T>::Vector() : m_d(new VectorData)
{
	m_d->count = 0;
//	m_d->data.alloc(size);
}

template <typename T>
inline Vector<T>::Vector(int size) : m_d(new VectorData)
{
	m_d->count = 0;
	m_d->data.alloc(size);
}

template <typename T>
Vector<T>::Vector(int size, const T &value)	: m_d(new VectorData)
{
	m_d->count = size;
	m_d->data.alloc(size);
	T *d = m_d->data.data();
	for(int i = 0; i< size; ++i) {
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
	detach();
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
void Vector<T>::append(const T &value)
{
	detach();
	if (m_d->count == m_d->data.size())
		reserve(m_d->count + 32);
	T *d = m_d->data.data();
	d[m_d->count++] = value;
}

template <typename T>
void Vector<T>::prepend(const T &value)
{
	detach();
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
		n = CWT_MIN(m_d->data.size() - i, n);
		detach();
		if (i + n < m_d->count) { // removing not last elements
			m_d->data.move(i, i + n, m_d->data.size() - i - n);
		}
		m_d->count -= n;
	}
}

CWT_NS_END

#endif /* __CWT_VECTOR_HPP__ */
