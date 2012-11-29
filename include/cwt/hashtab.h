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
 * @file hash-table.h
 *
 * @brief Hash table.
 *
 * A hash table stores a set of values which can be addressed by a 
 * key.  Given the key, the corresponding value can be looked up
 * quickly.
 *
 * To create a hash table, use @ref hash_table_new.  To destroy a 
 * hash table, use @ref hash_table_free.
 *
 * To insert a value into a hash table, use @ref hash_table_insert.
 *
 * To remove a value from a hash table, use @ref hash_table_remove.
 *
 * To look up a value by its key, use @ref hash_table_lookup.
 *
 * To iterate over all values in a hash table, use 
 * @ref hash_table_iterate to initialise a @ref HashTableIterator
 * structure.  Each value can then be read in turn using 
 * @ref hash_table_iter_next and @ref hash_table_iter_has_more.
 */

/*
 * Code based on Simon Howard's sources
 * */

#ifndef __CWT_HASHTABLE_H__
#define __CWT_HASHTABLE_H__

#include <cwt/types.h>

/**
 * A null @ref HashTableValue.
 */
#define HASH_TABLE_NULL ((void *) 0)


/**
 * A key to look up a value in a @ref HashTable.
 */
typedef void *CwtHashTableKey;

/**
 * A value stored in a @ref HashTable.
 */
typedef void *CwtHashTableValue;


/**
 * Hash function used to generate hash values for keys used in a hash
 * table.
 *
 * @param value  The value to generate a hash value for.
 * @return       The hash value.
 */
typedef ULONG (*CwtHashTableHashFunc)(CwtHashTableKey value);

/**
 * Function used to compare two keys for equality.
 *
 * @return   @c TRUE if the two keys are equal, @c FALSE if the keys are
 *           not equal.
 */
typedef BOOL (*CwtHashTableEqualFunc)(CwtHashTableKey value1, CwtHashTableKey value2);

/**
 * Type of function used to free keys when entries are removed from a 
 * hash table.
 */
typedef void (*CwtHashTableKeyFreeFunc)(CwtHashTableKey value);

/**
 * Type of function used to free values when entries are removed from a 
 * hash table.
 */
typedef void (*CwtHashTableValueFreeFunc)(CwtHashTableValue value);

/**
 * A hash table structure.
 */
typedef struct _CwtHashTableEntry {
	CwtHashTableKey key;
	CwtHashTableValue value;
	struct _CwtHashTableEntry *next;
} CwtHashTableEntry;


typedef struct _CwtHashTable {
	CwtHashTableEntry **table;
	size_t table_size;
	CwtHashTableHashFunc hash_func;
	CwtHashTableEqualFunc equal_func;
	CwtHashTableKeyFreeFunc key_free_func;
	CwtHashTableValueFreeFunc value_free_func;
	size_t entries;
	size_t prime_index;
} CwtHashTable;


/**
 * Definition of a @ref HashTableIterator.
 */
typedef struct _CwtHashTableIterator {
	CwtHashTable *hash_table;
	CwtHashTableEntry *next_entry;
	size_t next_chain;
} CwtHashTableIterator;



typedef struct _CwtHashTableNS {
	CwtHashTable*      (*create) (CwtHashTableHashFunc hash_func, CwtHashTableEqualFunc equal_func, CwtHashTableKeyFreeFunc key_free_func, CwtHashTableValueFreeFunc value_free_func);
	void               (*free)   (CwtHashTable *hash_table);
	BOOL               (*insert) (CwtHashTable *hash_table, CwtHashTableKey key, CwtHashTableValue value);
	CwtHashTableValue  (*lookup) (CwtHashTable *hash_table, CwtHashTableKey key);
	BOOL               (*remove) (CwtHashTable *hash_table, CwtHashTableKey key);
	size_t             (*size)   (CwtHashTable *hash_table);
	void               (*begin)  (CwtHashTable *hash_table, CwtHashTableIterator *it);
	BOOL               (*hasMore)(CwtHashTableIterator *it);
	CwtHashTableEntry* (*next)   (CwtHashTableIterator *it);

	ULONG              (*ccharHash) (void *location);
	ULONG              (*wcharHash) (void *location);
	ULONG              (*intHash)   (void *location);
	ULONG              (*uint16Hash)(void *location);
	ULONG              (*ptrHash)   (void *location);

	ULONG              (*cstrHash) (void *string);
	ULONG              (*cstriHash)(void *string);
	ULONG              (*wstrHash) (void *string);
	ULONG              (*wstriHash)(void *string);
	BOOL               (*cchareq)   (void *plocation1, void *plocation2);
	BOOL               (*wchareq)  (void *plocation1, void *plocation2);
	BOOL               (*inteq)    (void *plocation1, void *plocation2);
	BOOL               (*cstreq)   (void *string1, void *string2);
	BOOL               (*cstrieq)  (void *string1, void *string2);
	BOOL               (*wstreq)   (void *string1, void *string2);
	BOOL               (*wstrieq)  (void *string1, void *string2);

	/* depends on CWT_UNICODE definition */
	ULONG              (*charHash) (void *location);
	ULONG              (*strHash)  (void *string);
	ULONG              (*striHash) (void *string);
	BOOL               (*chareq)   (void *plocation1, void *plocation2);
	BOOL               (*streq)    (void *string1, void *string2);
	BOOL               (*strieq)   (void *string1, void *string2);
} CwtHashTableNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtHashTableNS* cwt_hashtable_ns(void);
EXTERN_C_END




#endif /* __CWT_HASHTABLE_H__ */

