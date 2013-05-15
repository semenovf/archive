/*

Copyright (c) 2005-2008, Simon Howard

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/**
 * @file   hash.cpp
 * @author wladt
 * @date   May 14, 2013 9:51:17 AM
 *
 * @brief
 */


#include "../include/cwt/hash.hpp"


CWT_NS_BEGIN

/* This is a set of good hash table prime numbers, from:
* http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
* Each prime is roughly double the previous value, and as far as
* possible from the nearest powers of two. */

static const uint_t __primes[] = {
	  53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157u
#ifndef CWT_OS_MSDOS
	, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469
	, 12582917, 25165843, 50331653, 100663319, 201326611
	, 402653189, 805306457, 1610612741
#endif
};

static const size_t __primesCount = sizeof(__primes) / sizeof(uint_t);

DLL_API uint_t hash_bytes(const byte_t *p, size_t len, uint_t seed)
{
    uint_t h = seed;

    for (size_t i = 0; i < len; ++i)
        h = 31 * h + p[i];

    return h;
}

DLL_API uint_t hash_chars(const Char *p, size_t len, uint seed)
{
    uint_t h = seed;

    for (size_t i = 0; i < len; ++i)
        h = 31 * h + p[i].unicode();

    return h;
}

HashData::Node* HashData::allocNode()
{
	return reinterpret_cast<HashData::Node*>(malloc(entrySize));
}

void HashData::freeNode(HashData::Node *node)
{
	free(node);
}

HashData::~HashData()
{
	HashData::Node *rover;
	HashData::Node *next;
	size_t i, size;

	/* Free all entries in all chains */
	size = nodeTable.size();
	for (i = 0; i < size; ++i) {
		rover = nodeTable[i];
		while (rover != NULL) {
			next = rover->next;
			delete rover;
			rover = next;
		}
	}
}


void HashData::allocNodeTable()
{
	size_t size;
	uint_t *primes;
	size_t nprimes;

	/* Determine the table size based on the current prime index.
	* An attempt is made here to ensure sensible behavior if the
	* maximum prime is exceeded, but in practice other things are
	* likely to break long before that happens. */

	if (primeIndex < nprimes) {
		size = primes[primeIndex];
	} else {
		size = nentries * 10;
	}

	/* Allocate the table and initialize to NULL for all entries */
	nodeTable.alloc(size);
}

void HashData::rehash(size_t pindex)
{
	size_t dest_size, orig_size;
	size_t index;
	HashData::Node *rover;
	HashData::Node *next;

	if (pindex >= __primesCount )
		pindex = __primesCount - 1;

	if (primeIndex == pindex)
		return;

	primeIndex = pindex;
	dest_size = __primes[primeIndex];
	orig_size = nodeTable.size();

	Array<HashData::Node*> table(dest_size);

	/* Link all entries from all chains into the new table */
	for (size_t i = 0; i < orig_size; ++i) {
		rover = nodeTable[i];

		while (rover != NULL) {
			next = rover->next;

			/* Find the index into the new table */
			index = (size_t)(hash_func_helper(rover, seed) % dest_size);

			/* Link this entry into the chain */
			rover->next = table[index];
			table[index] = rover;

			/* Advance to next in the chain */
			rover = next;
		}
	}
	nodeTable.swap(table);
}

HashData* HashData::clone()
{
	HashData *hd = new HashData(
			  entrySize
			, seed
			, copy_node_helper
			, hash_func_helper
			, eq_key_helper);

	if (nentries > 0) {
		size_t orig_size = nodeTable.size();
		HashData::Node *rover;

		hd->nentries   = nentries;
		hd->primeIndex = primeIndex;
		hd->nodeTable.alloc(orig_size);

		for (size_t i = 0; i < orig_size; ++i) {
			rover = nodeTable[i];

			while (rover != NULL) {
				HashData::Node *e = allocNode();

				copy_node_helper(e, rover);

				/* Link this entry into the chain */
				e->next = hd->nodeTable[i];
				hd->nodeTable[i] = e;

				/* Advance to next in the chain */
				rover = rover->next;
			}
		}
	}

	return hd;
}


HashData::Node* HashData::firstNode(size_t *index)
{
	HashData::Node *rover = NULL;
	if (nentries > 0) {
		size_t size = nodeTable.size();
		for (size_t i = 0; i < size; ++i) {
			if (nodeTable[i] != NULL) {
				*index = i;
				rover = nodeTable[i];
				break;
			}
		}
	}
	return rover;
}

HashData::Node* HashData::lastNode(size_t *index)
{
	HashData::Node *rover = NULL;

	if (nentries > 0) { // nodeTable has at leat one element
		size_t i = nodeTable.size() - 1;
		do {
			if (nodeTable[i] != NULL) {
				rover = nodeTable[i];
				*index = i;
				while (rover->next != NULL)
					rover = rover->next;
				break;
			}
		} while(i--); // disable gcc warning: comparison of unsigned expression >= 0 is always true
	}
	return rover;
}

HashData::Node* HashData::nextNode(HashData::Node *rover, size_t *index)
{
	if (rover->next != NULL)
		return rover->next;

	++*index;
	size_t size = nodeTable.size();
	for (size_t i = *index; i < size; ++i ) {
		rover = nodeTable[i];
		if (rover != NULL) {
			*index = i;
			return rover;
		}
	}
	return NULL;
}

HashData::Node* HashData::prevNode(HashData::Node *rover, size_t *index)
{
	HashData::Node *prev = nodeTable[*index];

	/* rover is a start node.
	 * Look up last node in a previous chunk of nodes (with the same hash value) */
	if (prev == rover) {

		if(*index == 0)
			return NULL;

		--*index;

		for (size_t i = *index; i >= 0; --i) {
			if (nodeTable[i] != NULL) {
				prev = nodeTable[i];
				*index = i;
				while (prev->next != NULL)
					prev = prev->next;
				return prev;
			}
		}
	}
	/* Look up node previous to rover in the same chunk */
	else {
		while (prev->next != rover)
			prev = prev->next;
		return prev;
	}

	return NULL;
}

HashData::Node* HashData::lookup(HashData::Node *node, size_t *index)
{
	HashData::Node *rover = NULL;
	if (nentries > 0) {
		*index = hash_func_helper(node, seed) % nodeTable.size();
		rover = nodeTable[*index];

		while (rover != NULL) {
			if (eq_key_helper(node, rover)) {
				break;
			}
			rover = rover->next;
		}
	}
	return rover;
}


HashData::Node* HashData::insert(HashData::Node *node, size_t *index)
{
	/* Generate the hash of the key and hence the index into the table */
	*index = hash_func_helper(node, seed) % nodeTable.size();
	node->next = nodeTable[*index];
	nodeTable[*index] = node;
	++nentries;
	return node;
}

HashData::Node* HashData::remove(HashData::Node *key)
{
	if (nentries > 0) {
		size_t i = hash_func_helper(key, seed) % nodeTable.size();
		HashData::Node *rover = nodeTable[i];

		if (eq_key_helper(key, rover)) {
			nodeTable[i] = rover->next;
			return rover;
		} else {
			while (rover->next != NULL) {
				if (eq_key_helper(key, rover->next)) {
					HashData::Node *r = rover->next;
					rover->next = rover->next->next;
					--nentries;
					return r;
				}
				rover = rover->next;
			}
		}
	}
	return NULL;
}

CWT_NS_END
