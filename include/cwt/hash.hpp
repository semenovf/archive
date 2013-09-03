/**
 * @file   hash.hpp
 * @author wladt
 * @date   May 14, 2013 8:02:58
 *
 * @brief Hash table class.
 */

#ifndef __CWT_HASH_HPP__
#define __CWT_HASH_HPP__

#include <cwt/cwt.h>
#include <cwt/array.hpp>
#include <cwt/string.hpp>
#include <cwt/shared_ptr.hpp>
#include <cstring>

CWT_NS_BEGIN

DLL_API uint_t hash_bytes(const byte_t *p, size_t len, uint_t seed);
DLL_API uint_t hash_uchars(const UChar *p, size_t len, uint_t seed);
inline uint_t hash_func(const String &key, uint_t seed)
{
	return hash_bytes(reinterpret_cast<const byte_t*>(key.data()), key.size(), seed);
}


inline uint_t hash_func(char key, uint_t seed = 0)        { return uint_t(key) ^ seed; }
inline uint_t hash_func(byte_t key, uint_t seed = 0)      { return uint_t(key) ^ seed; }
inline uint_t hash_func(ushort_t key, uint_t seed = 0)    { return uint_t(key) ^ seed; }
inline uint_t hash_func(short_t key, uint_t seed = 0)     { return uint_t(key) ^ seed; }
inline uint_t hash_func(uint_t key, uint_t seed = 0)      { return key ^ seed; }
inline uint_t hash_func(int key, uint_t seed = 0)         { return uint_t(key) ^ seed; }
inline uint_t hash_func(ulong_t key, uint_t seed = 0)
{
    if (sizeof(ulong_t) > sizeof(uint_t)) {
        return uint_t(((key >> (8 * sizeof(uint_t) - 1)) ^ key) & (~0U)) ^ seed;
    } else {
        return uint_t(key & (~0U)) ^ seed;
    }
}
inline uint_t hash_func(long_t key, uint_t seed = 0) { return hash_func((ulong_t)key, seed); }


//#if defined(CWT_CC_MSVC)
//#pragma warning( push )
//#pragma warning( disable : 4311 ) // disable pointer truncation warning
//#endif
template <typename T> inline uint_t hash_func(const T *key, uint_t seed = 0)
{
    return hash_func(reinterpret_cast<uintptr_t>(key), seed);
}
//#if defined(CWT_CC_MSVC)
//#pragma warning( pop )
//#endif

template<typename T> inline uint_t hash_func(const T &t, uint_t seed) { return (hash_func(t) ^ seed); }

struct HashData {
	struct Node {
		Node *next;
	};
	size_t entrySize;    // size of node represents an element in a hash table.
	size_t nentries;     // actual number of elements in a hash table
	size_t primeIndex;   // current index of prime value in the table of primes for allocate nodeTable
	uint_t seed;
	Array<Node*> nodeTable;

	void   (*copy_node_helper)(Node *to, Node *from);
	uint_t (*hash_func_helper)(Node *node, uint_t seed);
	bool   (*eq_key_helper)(Node *n1, Node *n2);

	HashData(size_t entrySizeValue, uint_t seedValue
			, void (*copy_node)(Node*, Node*)
			, uint_t (*hash_func)(Node*, uint_t)
			, bool (*eq_key)(Node*, Node*))
		: entrySize(entrySizeValue)
		, nentries(0)
		, primeIndex(0)
		, seed(seedValue)
		, nodeTable()
		, copy_node_helper(copy_node)
		, hash_func_helper(hash_func)
		, eq_key_helper(eq_key)	{}

	~HashData();

	void allocNodeTable();
	Node* allocNode();
	void freeNode(Node*);
	void rehash(size_t primeIndex);
	HashData* clone();
	HashData::Node* firstNode(size_t *index);
	HashData::Node* lastNode(size_t *index);
	HashData::Node* nextNode(HashData::Node *node, size_t *index);
	HashData::Node* prevNode(HashData::Node *node, size_t *index);
	HashData::Node* lookup(HashData::Node *key, size_t *index);
	HashData::Node* insert(HashData::Node *node, size_t *index);
	HashData::Node* remove(HashData::Node *key);
};


template <typename Key, typename T>
class DLL_API Hash {
public: // TODO Make private!
	struct Entry {
		Entry *next;
		Key key;
		T value;
	};

    static inline Entry* cast_entry(HashData::Node *node) {
        return reinterpret_cast<Entry *>(node);
    }
public:

    class const_iterator;

	class iterator {
		friend class const_iterator;
		HashData *hd;
		HashData::Node *rover;
		size_t index;
	public:
        iterator() : hd(NULL), rover(NULL), index(0) { }
        explicit iterator(HashData *d, HashData::Node *node, size_t i) : hd(d), rover(node), index(i) { }
        explicit iterator(HashData *d) : hd(d) { rover = hd->firstNode(&index); }

        const Key &key() const { return cast_entry(rover)->key; }
        T&   value() const { return cast_entry(rover)->value; }
        T&   operator *  () const { return cast_entry(rover)->value; }
        T*   operator -> () const { return &cast_entry(rover)->value; }
        bool operator == (const iterator &o) const { return rover == o.rover; }
        bool operator != (const iterator &o) const { return rover != o.rover; }

        iterator& operator++() {
            rover = hd->nextNode(rover, &index);
            return *this;
        }

        iterator operator++(int) {
            iterator r = *this;
            rover = hd->nextNode(rover, &index);
            return r;
        }

        iterator &operator--() {
            rover = hd->prevNode(rover, &index);++m_d->nentries;
            return *this;
        }
        iterator operator--(int) {
            iterator r = *this;
            rover = HashData::prevNode(rover, &index);
            return r;
        }
        iterator operator+(int j) const {
        	iterator r = *this;
        	if (j > 0)
        		while (j--) ++r;
        	else
        		while (j++) --r;
        	return r;
        }

        inline iterator operator-(int j) const { return operator+(-j); }
        inline iterator &operator+=(int j) { return *this = *this + j; }
        inline iterator &operator-=(int j) { return *this = *this - j; }
	};

    class const_iterator
    {
        friend class iterator;
		HashData *hd;
		HashData::Node *rover;
		size_t index;

    public:
        const_iterator() : hd(NULL), rover(NULL), index(0) { }
        explicit const_iterator (HashData * d, HashData::Node * node, size_t i) : hd(d), rover(node), index(i) { }
        explicit const_iterator (HashData * d) : hd(d) { rover = hd->firstNode(& index); }
        explicit const_iterator (const iterator & o) : hd(o.hd), rover(o.rover), index(o.index) { }

        inline const Key & key() const { return cast_entry(rover)->key; }
        inline const T & value() const { return cast_entry(rover)->value; }
        inline const T & operator  * () const { return cast_entry(rover)->value; }
        inline const T * operator -> () const { return & cast_entry(rover)->value; }
        inline bool operator == (const const_iterator & o) const { return rover == o.rover; }
        inline bool operator == (const iterator & o) const { return rover == o.rover; }
        inline bool operator != (const const_iterator & o) const { return rover != o.rover; }

        const_iterator & operator++() {
            rover = hd->nextNode(rover, &index);
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator r = *this;
            rover = hd->nextNode(rover, &index);
            return r;
        }

        const_iterator &operator--() {
            rover = hd->prevNode(rover, &index);
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator r = *this;
            rover = HashData::prevNode(rover, &index);
            return r;
        }
        const_iterator operator+(int j) const {
        	const_iterator r = *this;
        	if (j > 0)
        		while (j--) ++r;
        	else
        		while (j++) --r;
        	return r;
        }

        inline const_iterator operator-(int j) const { return operator+(-j); }
        inline const_iterator &operator+=(int j) { return *this = *this + j; }
        inline const_iterator &operator-=(int j) { return *this = *this - j; }
    };

public:
	Hash(uint_t seed = 0);
	Hash(const Hash<Key, T> &other);
	Hash<Key, T>&  operator=(const Hash<Key, T> & other);

	T&	           at(const Key & key) { return operator [](key); }
	const T        at(const Key & key) const { return operator [](key); }

    iterator       begin() { detach(); return iterator(m_d.get()); }
    const_iterator begin() const { return const_iterator(m_d.get()); }
    const_iterator cbegin() const { return const_iterator(m_d.get()); }
    iterator       end() { detach(); return iterator(); }
    const_iterator end() const { return const_iterator(); }
    const_iterator cend() const { return const_iterator(); }
	void	       clear() { *this = Hash<Key,T>(); }
	bool           contains(const Key &key) const;
	iterator	   find(const Key & key);
	const_iterator find(const Key & key) const;
	bool	       isEmpty() const { return m_d->nentries == 0; }
	iterator       insert(const Key & key, const T & value);
	const Key	   key ( const T & value ) const;
	const Key	   key ( const T & value, const Key & defaultKey ) const;
	int	           remove(const Key & key);
	size_t 	       size() const { return m_d->nentries; }
	const T	       value(const Key & key) const;
	const T	       value(const Key & key, const T &defaultValue) const;

	bool	       operator != (const Hash<Key, T> & other) const { return !(*this == other); }
	bool	       operator == (const Hash<Key, T> &other) const;
	T&	           operator [] (const Key & key);
	const T        operator [] (const Key & key) const { return value(key); }

protected:
	void           detach();

private:
	static void    copy_node_helper(HashData::Node *to, HashData::Node *from) {
						cast_entry(to)->key = cast_entry(from)->key;
						cast_entry(to)->value = cast_entry(from)->value;
		           }
	static uint_t  hash_func_helper(HashData::Node *node, uint_t seed) {
						return hash_func(cast_entry(node)->key, seed);
				   }
	static bool    eq_key_helper(HashData::Node *n1, HashData::Node *n2) {
						return cast_entry(n1)->key == cast_entry(n2)->key;
				   }

private:
	shared_ptr<HashData> m_d;
};

template <typename Key, typename T>
inline Hash<Key,T>::Hash(uint_t seed)
	: m_d(new HashData(
		  sizeof(Hash::Entry)
		, seed
		, copy_node_helper
		, hash_func_helper
		, eq_key_helper))
{
}

template <typename Key, typename T>
inline Hash<Key,T>::Hash(const Hash<Key, T> &other)
	: m_d(other.m_d)
{
}

template <typename Key, typename T>
inline Hash<Key, T>&  Hash<Key,T>::operator=(const Hash<Key, T> & other)
{
	m_d = other.m_d;
	return *this;
}

template <typename Key, typename T>
inline void Hash<Key,T>::detach()
{
	if (m_d.use_count() > 1) {
		shared_ptr<HashData> d(m_d->clone());
		m_d.swap(d);
	}
}

/*
template <typename Key, typename T>
inline void Hash<Key,T>::detachAndRehash()
{
	if (m_d.use_count() > 1) {
		shared_ptr<HashData> d(m_d->cloneAndEnlarge());
		m_d.swap(d);
	} else {
		m_d->enlarge();
	}
}
*/


template <class Key, class T>
inline bool Hash<Key, T>::contains(const Key &key) const
{
	Entry e;
	size_t index;
	e.key = key;
	return m_d->lookup(reinterpret_cast<HashData::Node*>(&e), &index) != NULL;
}


/**
* @brief Look up a value in a hash table by key.
*
* @param key The key of the value to look up.
* @return The value, or @ref HASH_TABLE_NULL if there
* is no value with that key in the hash table.
*/
template <typename Key, typename T>
inline typename Hash<Key,T>::iterator Hash<Key,T>::find(const Key &key)
{
	Entry e;
	size_t index;

	detach();

	e.key = key;
	HashData::Node *node = m_d->lookup(reinterpret_cast<HashData::Node*>(&e), &index);
	return iterator(m_d.get(), node, index);
}

/**
* @brief Look up a value in a hash table by key.
*
* @param key The key of the value to look up.
* @return The value, or @ref HASH_TABLE_NULL if there
* is no value with that key in the hash table.
*/
template <typename Key, typename T>
inline typename Hash<Key,T>::const_iterator Hash<Key,T>::find(const Key &key) const
{
	Entry e;
	size_t index;
	e.key = key;

	HashData::Node *node = m_d->lookup(reinterpret_cast<HashData::Node*>(&e), &index);
	return const_iterator(m_d.get(), node, index);
}


/**
* @brief Insert a value into a hash table, overwriting any existing entry using the same key.
*
* @param hash_table The hash table.
* @param key The key for the new value.
* @param value The value to insert.
* @return Non-zero if the value was added successfully,
* or zero if it was not possible to allocate
* memory for the new entry.
*/
template <typename Key, typename T>
typename Hash<Key,T>::iterator Hash<Key,T>::insert(const Key & key, const T & value)
{
	detach();

	if (!m_d->nodeTable.size()) {
		m_d->allocNodeTable();
	}

	/*
	 * If there are too many items in the table with respect to the table
	 * size, the number of hash collisions increases and performance
	 * decreases. Enlarge the table size to prevent this happening
	 */
	//if ((m_d->nentries * 3) / m_d->nodeTable.size() > 1) {
	if (double(m_d->nentries/m_d->nodeTable.size()) > double(1/3)) {
		/* Table is more than 1/3 full */
		m_d->rehash(m_d->primeIndex + 1);
	}

	Hash::Entry *e;
	size_t index;

	e = new Entry;
	e->key = key;
	e->value = value;
	HashData::Node *node = m_d->insert(reinterpret_cast<HashData::Node*>(e), &index);
	return iterator(m_d.get(), node, index);
}

template <class Key, class T>
const Key Hash<Key, T>::key(const T &value) const
{
    return key(value, Key());
}

template <class Key, class T>
const Key Hash<Key, T>::key(const T &value, const Key &defaultValue) const
{
    const_iterator i = begin();
    while (i != end()) {
        if (i.value() == value)
            return i.key();
        ++i;
    }

    return defaultValue;
}


template <typename Key, typename T>
inline const T Hash<Key, T>::value(const Key & key) const
{
	return value(key, T());
}

template <typename Key, typename T>
const T Hash<Key, T>::value(const Key & key, const T & defaultValue) const
{
    if (m_d->nentries == 0)
        return defaultValue;

    const_iterator it = find(key);
    if (it == cend())
    	return defaultValue;
    return it.value();
}


template <class Key, class T>
int Hash<Key, T>::remove(const Key & key)
{
    if (isEmpty())
        return 0;

    detach();

	Entry e;
	//size_t index;
	e.key = key;

    HashData::Node *r = m_d->remove(reinterpret_cast<HashData::Node*>(&e));
    if (r) {
    	delete cast_entry(r);

    	if (double(m_d->nentries/m_d->nodeTable.size()) < double(1/3)) {
    		if (m_d->primeIndex > 0)
    			m_d->rehash(m_d->primeIndex - 1);
    	}

    	return 1;
    }

    return 0;
}

template <class Key, class T>
bool Hash<Key, T>::operator == (const Hash<Key, T> &other) const
{
    if (size() != other.size())
        return false;
    if (m_d == other.m_d)
        return true;

    const_iterator it = begin();

    while (it != end()) {
        const Key &key = it.key();
        const_iterator it2 = other.find(key);

        if (it2 == other.end() || !(it2.key() == key))
        	return false;
        ++it;
    }
    return true;
}


template <class Key, class T>
T& Hash<Key, T>::operator[](const Key &key)
{
    detach();
    iterator it = find(key);
    if (it == end()) {
    	it = insert(key, T());
    }
    return it.value();
}

CWT_NS_END

#endif /* __CWT_HASH_HPP__ */
