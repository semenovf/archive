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
#include <cstring>

CWT_NS_BEGIN

DLL_API const uint_t* primes_for_hash(/*out*/ size_t *count);

inline uint_t hash_func(char key, uint_t seed = 0)        { return uint_t(key) ^ seed; }
inline uint_t hash_func(uchar key, uint_t seed = 0)       { return uint_t(key) ^ seed; }
inline uint_t hash_func(signed char key, uint_t seed = 0) { return uint_t(key) ^ seed; }
inline uint_t hash_func(ushort key, uint_t seed = 0)      { return uint_t(key) ^ seed; }
inline uint_t hash_func(short key, uint_t seed = 0)       { return uint_t(key) ^ seed; }
inline uint_t hash_func(uint_t key, uint_t seed = 0)      { return key ^ seed; }
inline uint_t hash_func(int key, uint_t seed = 0)         { return uint_t(key) ^ seed; }
inline uint_t hash_func(unsigned long key, uint_t seed = 0)
{
    if (sizeof(unsigned long) > sizeof(uint_t)) {
        return uint_t(((key >> (8 * sizeof(uint_t) - 1)) ^ key) & (~0U)) ^ seed;
    } else {
        return uint_t(key & (~0U)) ^ seed;
    }
}
inline uint_t hash_func(long key, uint_t seed = 0) { return hash_func((unsigned long)key, seed); }


#ifdef CWT_HAS_INT64
inline uint_t hash_func(uint64_t key, uint_t seed = 0)
{
    if (sizeof(uint64_t) > sizeof(uint_t)) {
        return uint_t(((key >> (8 * sizeof(uint_t) - 1)) ^ key) & (~0U)) ^ seed;
    } else {
        return uint_t(key & (~0U)) ^ seed;
    }
}
inline uint_t hash_func(int64_t key, uint_t seed = 0) { return hash_func(uint64_t(key), seed); }
#endif


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


template<typename T> inline uint_t hash_func(const T &t, uint seed) { return (hash_func(t) ^ seed); }


#ifdef __COMMENT__
inline uint_t hash_func(QChar key, uint seed = 0) { return hash_func(key.unicode(), seed); }
DLL_API uint_t hash_func(const QByteArray &key, uint seed = 0);
DLL_API uint_t hash_func(const QString &key, uint seed = 0);
DLL_API uint_t hash_func(const QStringRef &key, uint seed = 0);
DLL_API uint_t hash_func(const QBitArray &key, uint seed = 0);
DLL_API uint_t hash_func(QLatin1String key, uint seed = 0);
DLL_API uint_t qt_hash(const QString &key);
#endif

template <typename Key, typename T>
class Hash {
protected:
	struct Entry {
		Entry *next;
		Key key;
		T value;
	};

	struct Node {
		Node *next;
	};

public:

	class iterator {
		Node *rover;
		size_t next_chain;
	public:
        iterator() : rover(NULL), next_chain(0) { }
        explicit iterator(Entry *e, size_t nc) : rover(e), next_chain(nc) { }

        inline const Key &key() const { return reinterpret_cast<Entry*>(rover)->key; }
        inline T&   value() const { return reinterpret_cast<Entry*>(rover)->value; }
        inline T&   operator*() const { return reinterpret_cast<Entry*>(rover)->value; }
        inline T*   operator->() const { return &reinterpret_cast<Entry*>(rover)->value; }
        inline bool operator == (const iterator &o) const { return rover == o.rover; }
        inline bool operator != (const iterator &o) const { return rover != o.rover; }

        inline iterator &operator++() {
            rover = QHashData::nextNode(i);
            return *this;
        }
        inline iterator operator++(int) {
            iterator r = *this;
            i = QHashData::nextNode(i);
            return r;
        }
        inline iterator &operator--() {
            i = QHashData::previousNode(i);
            return *this;
        }
        inline iterator operator--(int) {
            iterator r = *this;
            i = QHashData::previousNode(i);
            return r;
        }
        inline iterator operator+(int j) const
        { iterator r = *this; if (j > 0) while (j--) ++r; else while (j++) --r; return r; }
        inline iterator operator-(int j) const { return operator+(-j); }
        inline iterator &operator+=(int j) { return *this = *this + j; }
        inline iterator &operator-=(int j) { return *this = *this - j; }
	};

	class const_iterator {
		Entry *next_entry;
		size_t next_chain;
	};


public:
	Hash();
	Hash(const Hash<Key, T> &other);
	Hash<Key, T>&  operator=(const Hash<Key, T> & other);
	~Hash();

	T&	           at(const Key & key) { return operator [](key); }
	const T        at(const Key & key) const { return operator [](key); }


	iterator       begin() { return __impl.begin(); }
	const_iterator begin() const { return __impl.begin(); }
	iterator       end() { return __impl.end(); }
	const_iterator end() const { return __impl.end(); }
	void	       clear() { __impl.clear(); }
	iterator	   find(const Key & key) { return __impl.find(key); }
	const_iterator find(const Key & key) const { return __impl.find(key); }
	bool	       isEmpty() const { return __impl.isEmpty(); }
	iterator       insert(const Key & key, const T & value) { return __impl.insert(key, value); }
	const Key	   key ( const T & value ) const { return __impl->key(value); }
	const Key	   key ( const T & value, const Key & defaultKey ) const { return __impl.key(value, defaultKey); }
	int	           remove(const Key & key) { return __impl.remove(key); }
	int  	       size() const { return __impl.size(); }
	const T	       value(const Key & key) const;
	const T	       value(const Key & key, const T &defaultValue) const;

	bool	       operator!=(const QHash<Key, T> & other) const { return __impl.operator !=(other.__impl); }

	bool	       operator==(const QHash<Key, T> &other) const { return __impl.operator ==(other.__impl); }
	T&	           operator[](const Key & key) { return __impl.operator [](key); }
	const T        operator[](const Key & key) const { return __impl.operator [](key); }

protected:
	void           detach();
	void           detachAndRealloc();

private:

	struct HashData {
		Array<Entry*> table;
		size_t nentries;
		size_t prime_index;

		HashData() : table(), nentries(0), prime_index(0) {}
		~HashData();
		void alloc();
		void enlarge();
		struct HashData* clone();
		struct HashData* cloneAndEnlarge();
		static void move_helper(Array<Entry*> &dest, Array<Entry*> &orig);
		static void copy_helper(Array<Entry*> &dest, Array<Entry*> &orig);
	};

	shared_ptr<HashData> m_d;
};


template <typename Key, typename T>
void Hash<Key,T>::HashData::alloc()
{
	size_t size;
	uint_t *primes;
	size_t nprimes;

	primes = primes_for_hash(&nprimes);

	/* Determine the table size based on the current prime index.
	* An attempt is made here to ensure sensible behavior if the
	* maximum prime is exceeded, but in practice other things are
	* likely to break long before that happens. */

	if (prime_index < nprimes) {
		size = primes[prime_index];
	} else {
		size = nentries * 10;
	}

	/* Allocate the table and initialize to NULL for all entries */
	table.alloc(size);
}

template <typename Key, typename T>
Hash<Key,T>::HashData::~HashData()
{
	Hash::Entry *rover;
	Hash::Entry *next;
	size_t i, size;

	/* Free all entries in all chains */
	size = table.size();
	for (i = 0; i < size; ++i) {
		rover = table[i];
		while (rover != NULL) {
			next = rover->next;
			delete rover;
			rover = next;
		}
	}
}

/*
 * Moves entries from array 'orig_table' to array 'dest_table'.
 * After this operation array 'orig_table' become not valid.
 */
template <typename Key, typename T>
void Hash<Key,T>::HashData::move_helper(Array<Entry*> &dest_table, Array<Entry*> &orig_table)
{
	size_t dest_size, orig_size;
	size_t index;
	Hash::Entry *rover;
	Hash::Entry *next;

	dest_size = dest_table.size();
	orig_size = orig_table.size();

	/* Link all entries from all chains into the new table */
	for (size_t i = 0; i < orig_size; ++i) {
		rover = orig_table[i];

		while (rover != NULL) {
			next = rover->next;

			/* Find the index into the new table */
			index = (size_t)(hash_func(rover->key) % dest_size);

			/* Link this entry into the chain */
			rover->next = dest_table[index];
			dest_table[index] = rover;

			/* Advance to next in the chain */
			rover = next;
		}
	}
}

/*
 * Deep copy entries from array 'orig_table' to array 'dest_table'.
 */
template <typename Key, typename T>
void Hash<Key,T>::HashData::copy_helper(Array<Entry*> &dest_table, Array<Entry*> &orig_table)
{
	size_t dest_size, orig_size;
	Hash::Entry *rover;
	Hash::Entry *next;

	dest_size = dest_table.size();
	orig_size = orig_table.size();

	for (size_t i = 0; i < orig_size; ++i) {
		rover = orig_table[i];

		if (rover != NULL) {
			Hash::Entry *e = new Hash::Entry;

			e->key = rover->key;
			e->value = rover->value;
			e->next = NULL;

			/* Find the index into the new table */
			uint_t index = hash_func(rover->key) % dest_size;

			dest_table[index] = e;
			rover = rover->next;

			while (rover != NULL) {
				Hash::Entry *next = new Hash::Entry;

				next->key = rover->key;
				next->value = rover->value;
				next->next = NULL;

				e->next = next;
				e = next;
				rover = rover->next;
			}
		}
	}
}


template <typename Key, typename T>
Hash<Key,T>::HashData* Hash<Key,T>::HashData::clone()
{
	HashData *hd = new HashData;

	hd->prime_index = prime_index;
	hd->nentries = nentries;
	hd->alloc();
	HashData::copy_helper(hd->table, table);

	return hd;
}

template <typename Key, typename T>
Hash<Key,T>::HashData* Hash<Key,T>::HashData::cloneAndEnlarge()
{
	HashData *hd = new HashData;

	hd->prime_index = prime_index + 1;
	hd->nentries = nentries;
	hd->alloc();
	HashData::copy_helper(hd->table, table);

	return hd;
}

template <typename Key, typename T>
void Hash<Key,T>::HashData::enlarge()
{
	Array<Hash::Entry*> old_table(table);

	++prime_index;
	alloc();
	HashData::move_helper(table, old_table);
}


template <typename Key, typename T>
inline Hash<Key,T>::Hash() : m_d(new HashData)
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
Hash<Key,T>::~Hash()
{
}

template <typename Key, typename T>
inline void Hash<Key,T>::detach()
{
	if (m_d.use_count() > 1) {
		shared_ptr<HashData> d(m_d->clone());
		m_d.swap(d);
	}
}

template <typename Key, typename T>
inline void Hash<Key,T>::detachAndRealloc()
{
	if (m_d.use_count() > 1) {
		shared_ptr<HashData> d(m_d->cloneAndEnlarge());
		m_d.swap(d);
	} else {
		m_d->enlarge();
	}
}

/**
* @brief Look up a value in a hash table by key.
*
* @param key The key of the value to look up.
* @return The value, or @ref HASH_TABLE_NULL if there
* is no value with that key in the hash table.
*/
template <typename Key, typename T>
Hash<Key,T>::iterator Hash<Key,T>::find(const Key &key)
{
	Hash::Entry *rover;
	uint_t index;

	/* Generate the hash of the key and hence the index into the table */
	index = hash_func(key) % m_d->size;

	/* Walk the chain at this index until the corresponding entry is
	* found */
	rover = m_d->table[index];

	while (rover != NULL) {
		if (key == rover->key) {

			/* Found the entry. Return the data. */
			return rover->value;
		}
		rover = rover->next;
	}

	/* Not found */
	return HASH_TABLE_NULL;
}



/**
* @fn CwtHashTableNS::insert(CwtHashTable *hash_table, CwtHashTableKey key, CwtHashTableValue value)
*
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
Hash<Key,T>::iterator Hash<Key,T>::insert(Key key, T value)
{
	Hash::Entry *rover;
	Hash::Entry *newentry;
	uint_t index;

	/*
	 * If there are too many items in the table with respect to the table
	 * size, the number of hash collisions increases and performance
	 * decreases. Enlarge the table size to prevent this happening
	 */
	if ((m_d->nentries * 3) / m_d->size > 0) {
		/* Table is more than 1/3 full */
		detachAndRealloc();
	} else {
		detach();
	}

	/* Generate the hash of the key and hence the index into the table */
	index = hash_func(key) % m_d->size;

	/* Traverse the chain at this location and look for an existing
	* entry with the same key */
	rover = m_d->table[index];

	while (rover != NULL) {
		if (rover->key == key) {
			rover->key = key;
			rover->value = value;

			/* Finished */
			return TRUE;
		}

		rover = rover->next;
	}

	/* Not in the hash table yet. Create a new entry */
	newentry = new Hash::Entry;
	newentry->key = key;
	newentry->value = value;

	/* Link into the list */
	newentry->next = m_d->table[index];
	m_d->table[index] = newentry;

	/* Maintain the count of the number of entries */
	++m_d->nentries;

	/* Added successfully */
	return TRUE;
}


template <typename Key, typename T>
inline const T Hash<Key, T>::value(const Key &key) const
{
    if (m_d->nentries == 0)
        return T();

    const_iterator it = find(key);
    if (it == cend())
    	return T();
    return it->value;
}

template <typename Key, typename T>
inline const T Hash<Key, T>::value(const Key &akey, const T &defaultValue) const
{
    if (m_d->nentries == 0)
        return defaultValue;

    const_iterator it = find(key);
    if (it == cend())
    	return defaultValue;
    return it->value;
}



CWT_NS_END

#endif /* __CWT_HASH_HPP__ */
