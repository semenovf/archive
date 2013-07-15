/*
 * deque.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEQUE_HPP__
#define __CWT_DEQUE_HPP__

#include <cwt/cwt.hpp>
#include <cwt/shared_ptr.hpp>
#include <deque>

CWT_NS_BEGIN

template <class T>
class Deque
{
	typedef std::deque<T> DequeData;

public:
	typedef typename DequeData::iterator iterator;
	typedef typename DequeData::const_iterator const_iterator;
	typedef typename DequeData::reference reference;

public:
	Deque() : m_d(new DequeData) {}
	reference at(size_t index) { return m_d->at(index); }
	iterator begin() { return m_d->begin(); }
	iterator end()   { return m_d->end(); }
	const_iterator cbegin() const { return m_d->begin(); }
	const_iterator cend()   const { return m_d->end(); }

	void clear() { m_d.detach(); m_d->clear(); }

	void append(const T &value) { m_d.detach(); m_d->push_back(value); }
	void append(const T * value, size_t count);
	void append(const Deque &other);

	bool isEmpty() const { return m_d->size() > 0 ? false : true; }

	void prepend(const T &value) { m_d.detach(); m_d->push_front(value); }
	void prepend(const Deque &other);

	void remove(size_t i) { m_d.detach(); m_d->erase(m_d->begin() + i); }
	void remove(size_t i, size_t count) { m_d.detach(); m_d->erase(m_d->begin() + i, m_d->begin() + i + count); }

	Deque left  (size_t count) const { return mid(0, count); }
	Deque right (size_t count) const { return mid(m_d->size() - count, count); }
	Deque mid   (size_t pos, size_t count) const ;

	size_t size() const { return m_d->size(); }

private:
	shareable<DequeData> m_d;
};


template <typename T>
void Deque<T>::append(const T * value, size_t count)
{
	m_d.detach();

	for (size_t i = 0; i < count; ++i) {
		m_d->push_back(value[i]);
	}
}

template < class T>
void Deque<T>::append(const Deque & other)
{
	m_d.detach();
	Deque::const_iterator it = other.cbegin();
	Deque::const_iterator itEnd = other.cend();

	while (it != itEnd) {
		m_d->push_back(*it);
		++it;
	}
}

template < class T>
void Deque<T>::prepend(const Deque &other)
{
	m_d.detach();

	Deque::const_iterator it = other.cbegin();
	Deque::const_iterator itEnd = other.cend();

	while (it != itEnd) {
		m_d->push_front(*it);
		++it;
	}
}

template < class T>
Deque<T> Deque<T>::mid(size_t pos, size_t count) const
{
	Deque<T> r;
	Deque::const_iterator it = cbegin() + pos;
	Deque::const_iterator itEnd = cbegin() + pos + count;

	while (it != itEnd) {
		r.m_d->push_front(*it);
		++it;
	}
	return r;
}


CWT_NS_END

#endif /* __CWT_DEQUE_HPP__ */
