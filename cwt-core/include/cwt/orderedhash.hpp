/*
 * orderedhash.hpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

#ifndef __CWT_ORDEREDHASH_HPP__
#define __CWT_ORDEREDHASH_HPP__

#include <cwt/hash.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

template <typename Key, typename T>
class DLL_API OrderedHash : public Hash<Key, T>
{
	typedef Hash<Key, T> base_class;
	typedef Vector<Key>  ordered_class;

public:
	typedef typename base_class::iterator iterator;
	typedef typename base_class::const_iterator const_iterator;

	class ordered_iterator : public ordered_class::iterator
	{
		base_class * owner;
		typedef typename ordered_class::iterator base_iterator;
	public:
        ordered_iterator (base_class * o) : base_iterator(), owner(o) {}
        ordered_iterator (base_class * o, Key * n) : base_iterator(n), owner(o) {}
        ordered_iterator (const ordered_iterator & other) : base_iterator(other), owner(other.owner) {}

        const Key & key() const { return base_iterator::value(); }
        T &  value() const { CWT_ASSERT(owner); return owner->value(base_iterator::value()); }
        T &  operator *  () const { CWT_ASSERT(owner); return owner->value(base_iterator::value()); }
        T *  operator -> () const { CWT_ASSERT(owner); return & owner->value(base_iterator::value()); }
        bool operator == (const ordered_iterator & other) const { return base_iterator::operator == (other) && owner == other.owner; }
        bool operator != (const ordered_iterator & other) const { return !(base_iterator::operator == (other) && owner == other.owner); }

        ordered_iterator & operator ++ () { base_iterator::operator ++ (); return *this; }
        ordered_iterator operator ++ (int) {
        	ordered_iterator r = *this;
            ++*this;
            return r;
        }

        ordered_iterator & operator -- () { base_iterator::operator -- (); return *this; }
        ordered_iterator operator -- (int) {
        	ordered_iterator r = *this;
            --*this;
            return r;
        }

        ordered_iterator operator    + (int j) const { base_iterator::operator + (j); return *this; }
        ordered_iterator operator    - (int j) const { base_iterator::operator - (j); return *this; }
        ordered_iterator & operator += (int j)       { base_iterator::operator += (j); return *this; }
        ordered_iterator & operator -= (int j)       { base_iterator::operator -= (j); return *this; }
	};

	class const_ordered_iterator : public ordered_class::const_iterator
	{
		base_class * owner;
		typedef typename ordered_class::const_iterator base_iterator;
	public:
		const_ordered_iterator (base_class * o) : base_iterator(), owner(o) {}
		const_ordered_iterator (base_class * o, Key * n) : base_iterator(n), owner(o) {}
		const_ordered_iterator (const const_ordered_iterator & other) : base_iterator(other), owner(other.owner) {}
		const_ordered_iterator (const ordered_iterator & other) : base_iterator(other), owner(other.owner) {}

        const Key & key() const { return base_iterator::value(); }
        T &  value() const { CWT_ASSERT(owner); return owner->value(base_iterator::value()); }
        T &  operator *  () const { CWT_ASSERT(owner); return owner->value(base_iterator::value()); }
        T *  operator -> () const { CWT_ASSERT(owner); return & owner->value(base_iterator::value()); }
        bool operator == (const const_ordered_iterator & other) const { return base_iterator::operator == (other) && owner == other.owner; }
        bool operator != (const const_ordered_iterator & other) const { return !(base_iterator::operator == (other) && owner == other.owner); }

        const_ordered_iterator & operator ++ () { base_iterator::operator ++ (); return *this; }
        const_ordered_iterator operator ++ (int) {
        	const_ordered_iterator r = *this;
            ++*this;
            return r;
        }

        const_ordered_iterator & operator -- () { base_iterator::operator -- (); return *this; }
        const_ordered_iterator operator -- (int) {
        	const_ordered_iterator r = *this;
            --*this;
            return r;
        }

        const_ordered_iterator operator    + (int j) const { base_iterator::operator + (j); return *this; }
        const_ordered_iterator operator    - (int j) const { base_iterator::operator - (j); return *this; }
        const_ordered_iterator & operator += (int j)       { base_iterator::operator += (j); return *this; }
        const_ordered_iterator & operator -= (int j)       { base_iterator::operator -= (j); return *this; }
	};

public:
	OrderedHash(uint_t seed = 0) : base_class(seed), m_d(new SharedData()) {}
	OrderedHash(const OrderedHash<Key, T> & other) : base_class(other), m_d(other.m_d) {}
	OrderedHash<Key, T> & operator = (const OrderedHash<Key, T> & other) { base_class::operator = (other); m_d = other.m_d; return *this; }

    ordered_iterator beginOrdered() { m_d.detach(); return ordered_iterator(this, m_d->order.begin()); }
    ordered_iterator endOrdered()   { m_d.detach(); return ordered_iterator(this, m_d->order.end()); }
    const_ordered_iterator beginOrdered() const  { return const_ordered_iterator(this, m_d->order.begin()); }
    const_ordered_iterator cbeginOrdered() const { return const_ordered_iterator(this, m_d->order.cbegin()); }
    const_ordered_iterator endOrdered() const    { return const_ordered_iterator(this, m_d->order.end()); }
    const_ordered_iterator cendOrdered() const   { return const_ordered_iterator(this, m_d->order.cend()); }

	void	       clear() { m_d.detach(); m_d->order.clear(); base_class::clear(); }
	iterator       insert(const Key & key, const T & value) { m_d.detach(); m_d->order.append(key); return base_class::insert(key, value); }
	int	           remove(const Key & key);

private:
	struct SharedData
	{
		Vector<Key> order;

		SharedData  () : order() {}
		SharedData  (const SharedData & other) : order(other.order) {}
	};

	shareable<SharedData> m_d;
};

template <typename Key, typename T>
int	OrderedHash<Key, T>::remove(const Key & key)
{
	m_d.detach();
	size_t count = m_d->order.size();

	for (size_t i = 0; i < count; ++i) {
		if (m_d->order[i] == key) {
			m_d->order.remove(key);
			--count;
			--i;
		}
	}

	return base_class::remove(key);
}

CWT_NS_END

#endif /* __CWT_ORDEREDHASH_HPP__ */
