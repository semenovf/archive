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
        inline bool            operator <  (const iterator& other) const { return i < other.i; }
        inline bool            operator <= (const iterator& other) const { return i <= other.i; }
        inline bool            operator >  (const iterator& other) const { return i > other.i; }
        inline bool            operator >= (const iterator& other) const { return i >= other.i; }
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
	explicit Vector(int size);
	Vector(int size, const T &value);
	Vector(const Vector<T> &other);
	~Vector() { delete m_data; }

	T&             at(size_t i);
	const T&       at(size_t i) const;
	void           append(const T &value);
	void           clear()         { detach(); (*m_data)->count = 0; }
	T*             data()          { detach(); return (*m_data)->data.data(); }
	const T*       data() const    { return (*m_data)->data.data(); }
	T&             first()         { return at(0); }
	const T&       first() const   { return at(0); }
	bool           isEmpty() const { return m_data->use_count() > 0 && (*m_data)->count > 0 ? false : true; }
	T&             last()          { CWT_ASSERT((*m_data)->count > 0); return at((*m_data)->count-1); }
	const T&       last() const    { CWT_ASSERT((*m_data)->count > 0); return at((*m_data)->count-1); }
	void           prepend(const T &value);
	void           resize(size_t size)        { reserve(size); (*m_data)->count = size; }
	void           reserve(size_t size)       { detachAndRealloc(CWT_MAX(size, (*m_data)->data.size())); }
	size_t         size() const            { return (*m_data)->count; }
	void           swap(Vector<T> & other) { cwt::swap(m_data, other.m_data); }

    iterator       begin()        { detach(); return iterator((*m_data)->data.data()); }
    iterator       end()          { detach(); return iterator((*m_data)->data.data()) + (*m_data)->count; }
    const_iterator begin() const  { return const_iterator((*m_data)->data.data()); }
    const_iterator end() const    { return const_iterator((*m_data)->data.data()) + (*m_data)->count; }
    const_iterator cbegin() const { return const_iterator((*m_data)->data.data()); }
    const_iterator cend() const   { return const_iterator((*m_data)->data.data()) + (*m_data)->count; }

	T&             operator[](size_t i)       { return at(i); }
	const T&       operator[](size_t i) const { return at(i); }

protected:
	void           detach() { detachAndRealloc((*m_data)->count); }
	void           detachAndRealloc(size_t newsize);

protected:
	typedef struct {
		Array<T> data;
		size_t   count;
	} VectorData;

	shared_ptr<VectorData> *m_data;
};


template <typename T>
void Vector<T>::detachAndRealloc(size_t newsize)
{
	CWT_ASSERT(m_data);

	shared_ptr<VectorData> *orig = m_data;

	if (m_data->use_count() > 1) {
		delete m_data;
		m_data = new shared_ptr<VectorData>(new VectorData);
		(*m_data)->data.alloc((*orig)->data.capacity());
		Array<T>::copy((*m_data)->data, (*orig)->data, 0, 0, CWT_MIN(newsize, (*orig)->count));
	} else {
		if (newsize > (*orig)->data.capacity())
			(*m_data)->data.realloc(newsize);
	}
	(*m_data)->count = CWT_MIN(newsize, (*orig)->count);
}


template <typename T>
inline Vector<T>::Vector()
	: m_data(new shared_ptr<VectorData>(new VectorData))
{
	(*m_data)->count = 0;
	(*m_data)->data.alloc(32);
}


template <typename T>
inline Vector<T>::Vector(int size)
	: m_data(new shared_ptr<VectorData>(new VectorData))
{
	(*m_data)->count = 0;
	(*m_data)->data.alloc(size);
}

template <typename T>
Vector<T>::Vector(int size, const T &value)
	: m_data(new shared_ptr<VectorData>(new VectorData))
{
	(*m_data)->count = size;
	(*m_data)->data.alloc(size);
	T *d = (*m_data)->data.data();
	for(int i = 0; i< size; ++i) {
		d[i] = value;
	}
}

template <typename T>
inline Vector<T>::Vector(const Vector<T> &other)
	: m_data(new shared_ptr<VectorData>(new VectorData))
{
	(*m_data)->data.alloc((*other.m_data)->count);
	Array<T>::copy((*m_data)->data, (*other.m_data)->data, 0, 0, (*other.m_data)->count);
	(*m_data)->count = (*other.m_data)->count;
}


template <typename T>
inline T& Vector<T>::at(size_t i)
{
	CWT_ASSERT(!isEmpty());
	CWT_ASSERT(i < (*m_data)->count);
	detach();
	return (*m_data)->data.at(i);
}

template <typename T>
inline const T& Vector<T>::at(size_t i) const
{
	CWT_ASSERT(!isEmpty());
	CWT_ASSERT(i < (*m_data)->count);
	return (*m_data)->data.at(i);
}

template <typename T>
void Vector<T>::append(const T &value)
{
	T *d = (*m_data)->data.data();
	if ((*m_data)->count == (*m_data)->data.size())
		reserve((*m_data)->count + 32);
	d[(*m_data)->count++] = value;
}


template <typename T>
void Vector<T>::prepend(const T &value)
{
	if ((*m_data)->count == (*m_data)->data.size())
		reserve((*m_data)->count + 32);
	(*m_data)->data.move(1, 0, (*m_data)->count);

	T *d = (*m_data)->data.data();
	d[0] = value;
	(*m_data)->count++;
}

CWT_NS_END

#endif /* __CWT_VECTOR_HPP__ */
