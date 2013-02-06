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
#include <QtCore/QVector>

CWT_NS_BEGIN

template <typename T>
class Vector {
public:
	typedef typename QVector<T>::iterator iterator;
	typedef typename QVector<T>::const_iterator const_iterator;

public:
	Vector();
	explicit Vector(int size);
	Vector(int size, const T &value);
	Vector(const Vector<T> &other);

	void append(const T &value);
	iterator       begin() { return __impl.begin(); }
	const_iterator begin() const { return __impl.begin(); }
	void clear();
	T* data() { return __impl.data(); }
	const T* data() const { return __impl.data(); }
	iterator       end() { return __impl.end(); }
	const_iterator end() const { return __impl.end(); }
	T& first();
	const T& first() const;
	bool isEmpty() const { return __impl.isEmpty(); }
	T& last();
	const T& last() const;
	void prepend(const T &value);
	void resize(int size) { __impl.resize(size); }
	int size() const;

	T& operator[](int i);
	const T& operator[](int i) const;

public:
	QVector<T> __impl;
};

template <typename T>
inline Vector<T>::Vector() : __impl() {}

template <typename T>
inline Vector<T>::Vector(int size) : __impl(size) {}

template <typename T>
inline Vector<T>::Vector(int size, const T &value) : __impl(size, value) {}

template <typename T>
inline Vector<T>::Vector(const Vector<T> &other) : __impl(other.__impl) {}

template <typename T>
inline void Vector<T>::append(const T &value) { __impl.append(value); }

template <typename T>
inline void Vector<T>::clear() { __impl.clear(); }

template <typename T>
inline T& Vector<T>::first() { return __impl.first(); }

template <typename T>
inline const T& Vector<T>::first() const { return __impl.first(); }

template <typename T>
inline T& Vector<T>::last() { return __impl.first(); }

template <typename T>
inline const T& Vector<T>::last() const { return __impl.first(); }

template <typename T>
inline void Vector<T>::prepend(const T &value) { __impl.prepend(value); }

template <typename T>
inline int Vector<T>::size() const { return __impl.size(); }

template <typename T>
inline T& Vector<T>::operator [] (int i) { return __impl.operator [](i); }

template <typename T>
inline const T& Vector<T>::operator [] (int i) const { return __impl.operator [](i); }


CWT_NS_END

#endif /* __CWT_VECTOR_HPP__ */
