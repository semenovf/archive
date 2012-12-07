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

/* Hash table implementation */

#include <stdlib.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#include <string.h>
/*#include <cwt/string.h>*/
#include <cwt/str.h>
#include <cwt/hashtab.h>

/* This is a set of good hash table prime numbers, from:
 *   http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
 * Each prime is roughly double the previous value, and as far as
 * possible from the nearest powers of two. */

static const UINT hash_table_primes[] = {
	193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157u
#ifndef __MSDOS__
	, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469
	, 12582917, 25165843, 50331653, 100663319, 201326611
	, 402653189, 805306457, 1610612741
#endif
};

static const size_t hash_table_num_primes
	= sizeof(hash_table_primes) / sizeof(int);


static CwtHashTable*      __ht_create    (CwtHashTableHashFunc hash_func, CwtHashTableEqualFunc equal_func, CwtHashTableKeyFreeFunc key_free_func, CwtHashTableValueFreeFunc value_free_func);
static void               __ht_free      (CwtHashTable *hash_table);
static BOOL               __ht_insert    (CwtHashTable *hash_table, CwtHashTableKey key, CwtHashTableValue value);
static CwtHashTableValue  __ht_lookup    (CwtHashTable *hash_table, CwtHashTableKey key);
static BOOL               __ht_remove    (CwtHashTable *hash_table, CwtHashTableKey key);
static size_t             __ht_size      (CwtHashTable *hash_table);
static void               __ht_begin     (CwtHashTable *hash_table, CwtHashTableIterator *it);
static BOOL               __ht_hasMore   (CwtHashTableIterator *it);
static CwtHashTableEntry* __ht_next      (CwtHashTableIterator *it);
static ULONG              __ht_ccharHash (void *vlocation);
static ULONG              __ht_wcharHash (void *vlocation);
static ULONG              __ht_intHash   (void *vlocation);
static ULONG              __ht_uint16Hash(void *vlocation);
static ULONG              __ht_ptrHash   (void *vlocation);
static ULONG              __ht_cstrHash  (void *string);
static ULONG              __ht_cstriHash (void *string);
static ULONG              __ht_wstrHash  (void *string);
static ULONG              __ht_wstriHash (void *string);
static BOOL               __ht_cchareq   (void *plocation1, void *plocation2);
static BOOL               __ht_wchareq   (void *plocation1, void *plocation2);
static BOOL               __ht_inteq     (void *plocation1, void *plocation2);
static BOOL               __ht_cstreq    (void *string1, void *string2);
static BOOL               __ht_cstrieq   (void *string1, void *string2);
static BOOL               __ht_wstreq    (void *string1, void *string2);
static BOOL               __ht_wstrieq   (void *string1, void *string2);

/* internal functions */
static BOOL              __ht_allocate_table (CwtHashTable *hash_table);
static void              __ht_free_entry     (CwtHashTable *hash_table, CwtHashTableEntry *entry);
static BOOL              __ht_enlarge        (CwtHashTable *hash_table);
/*static int               __ht_cstrcmp   (void *string1, void *string2);*/
static int               __ht_cstricmp  (void *string1, void *string2);
/*static int               __ht_wstrcmp   (void *string1, void *string2);*/
static int               __ht_wstricmp  (void *string1, void *string2);

static CwtHashTableNS __cwtHashTableNS = {
	  __ht_create
	, __ht_free
	, __ht_insert
	, __ht_lookup
	, __ht_remove
	, __ht_size
	, __ht_begin
	, __ht_hasMore
	, __ht_next
	, __ht_ccharHash
	, __ht_wcharHash
	, __ht_intHash
	, __ht_uint16Hash
	, __ht_ptrHash
	, __ht_cstrHash
	, __ht_cstriHash
	, __ht_wstrHash
	, __ht_wstriHash
	, __ht_cchareq
	, __ht_wchareq
	, __ht_inteq
	, __ht_cstreq
	/*, __ht_cstrcmp*/
	, __ht_cstrieq
	/*, __ht_cstricmp*/
	, __ht_wstreq
	, __ht_wstrieq

#ifdef CWT_UNICODE
	, __ht_wcharHash
	, __ht_wstrHash
	, __ht_wstriHash
	, __ht_wchareq
	, __ht_wstreq
	, __ht_wstrieq
#else
	, __ht_ccharHash
	, __ht_cstrHash
	, __ht_cstriHash
	, __ht_cchareq
	, __ht_cstreq
	, __ht_cstrieq
#endif

};

static CwtStrNS *__cwtStrNS = NULL;

DLL_API_EXPORT CwtHashTableNS* cwt_hashtable_ns(void)
{

	if( !__cwtStrNS ) {
		__cwtStrNS = cwt_str_ns();
	}
	return &__cwtHashTableNS;
}

/* Internal function used to allocate the table on hash table creation
 * and when enlarging the table */
static BOOL __ht_allocate_table(CwtHashTable *hash_table)
{
	size_t new_table_size;

	/* Determine the table size based on the current prime index.  
	 * An attempt is made here to ensure sensible behavior if the
	 * maximum prime is exceeded, but in practice other things are
	 * likely to break long before that happens. */

	if (hash_table->prime_index < hash_table_num_primes) {
		new_table_size = hash_table_primes[hash_table->prime_index];
	} else {
		new_table_size = hash_table->entries * 10;
	}

	hash_table->table_size = new_table_size;

	/* Allocate the table and initialize to NULL for all entries */
	hash_table->table = CWT_MALLOCA(CwtHashTableEntry*, hash_table->table_size);
	cwt_bzero(hash_table->table, hash_table->table_size * sizeof(CwtHashTableEntry*));

	return hash_table->table != NULL ? TRUE : FALSE;
}

/* Free an entry, calling the free functions if there are any registered */

static void __ht_free_entry(CwtHashTable *hash_table, CwtHashTableEntry *entry)
{
	/* If there is a function registered for freeing keys, use it to free
	 * the key */
	if (hash_table->key_free_func != NULL) {
		hash_table->key_free_func(entry->key);
	}

	/* Likewise with the value */
	if (hash_table->value_free_func != NULL) {
		hash_table->value_free_func(entry->value);
	}

	/* Free the data structure */
	CWT_FREE(entry);
}


/**
 * @fn CwtHashTableNS::create(CwtHashTableHashFunc hash_func, CwtHashTableEqualFunc equal_func, CwtHashTableKeyFreeFunc key_free_func, CwtHashTableValueFreeFunc value_free_func)
 *
 * @brief Create a new hash table and register functions used to free the key and value when an entry is
 * removed from a hash table.
 *
 * @param hash_func            Function used to generate hash keys for the
 *                             keys used in the table.
 * @param equal_func           Function used to test keys used in the table
 *                             for equality.
 * @param key_free_func        Function used to free keys.
 * @param value_free_func      Function used to free values.
 *
 * @return                     A new hash table structure, or NULL if it
 *                             was not possible to allocate the new hash
 *                             table.
 */
static CwtHashTable* __ht_create(CwtHashTableHashFunc hash_func
	, CwtHashTableEqualFunc equal_func
	, CwtHashTableKeyFreeFunc key_free_func
    , CwtHashTableValueFreeFunc value_free_func)
{
	CwtHashTable *hash_table;

	/* Allocate a new hash table structure */
	hash_table = CWT_MALLOC(CwtHashTable);

	hash_table->hash_func = hash_func;
	hash_table->equal_func = equal_func;
	hash_table->key_free_func = NULL;
	hash_table->value_free_func = NULL;
	hash_table->entries = 0;
	hash_table->prime_index = 0;

	/* Allocate the table */
	if( !__ht_allocate_table(hash_table) ) {
		CWT_FREE(hash_table);
		hash_table = NULL;
	} else {
		hash_table->key_free_func = key_free_func;
		hash_table->value_free_func = value_free_func;
	}

	return hash_table;
}


/**
 * @fn CwtHashTableNS::free(CwtHashTable *hash_table)
 *
 * @brief Destroy a hash table.
 *
 * @param hash_table           The hash table to destroy.
 */
static void __ht_free(CwtHashTable *hash_table)
{
	CwtHashTableEntry *rover;
	CwtHashTableEntry *next;
	size_t i;
	
	/* Free all entries in all chains */
	for( i = 0; i < hash_table->table_size; ++i ) {
		rover = hash_table->table[i];
		while (rover != NULL) {
			next = rover->next;
			__ht_free_entry(hash_table, rover);
			rover = next;
		}
	}
	
	/* Free the table */
	CWT_FREE(hash_table->table);
	hash_table->table = NULL;
	
	/* Free the hash table structure */
	CWT_FREE(hash_table);
}


static BOOL __ht_enlarge(CwtHashTable *hash_table)
{
	CwtHashTableEntry **old_table;
	size_t old_table_size;
	size_t old_prime_index;
	CwtHashTableEntry *rover;
	CwtHashTableEntry *next;
	size_t index;
	size_t i;

	/* Store a copy of the old table */
	old_table       = hash_table->table;
	old_table_size  = hash_table->table_size;
	old_prime_index = hash_table->prime_index;

	/* Allocate a new, larger table */

	++hash_table->prime_index;

	if (!__ht_allocate_table(hash_table)) {

		/* Failed to allocate the new table */

		hash_table->table = old_table;
		hash_table->table_size = old_table_size;
		hash_table->prime_index = old_prime_index;

		return FALSE;
	}

	/* Link all entries from all chains into the new table */

	for( i = 0; i < old_table_size; ++i ) {
		rover = old_table[i];

		while (rover != NULL) {
			next = rover->next;

			/* Find the index into the new table */
			index = (size_t)(hash_table->hash_func(rover->key) % hash_table->table_size);
			
			/* Link this entry into the chain */

			rover->next = hash_table->table[index];
			hash_table->table[index] = rover;
			
			/* Advance to next in the chain */
			rover = next;
		}
	}

	/* Free the old table */
	CWT_FREE(old_table);
       
	return TRUE;
}


/**
 * @fn CwtHashTableNS::insert(CwtHashTable *hash_table, CwtHashTableKey key, CwtHashTableValue value)
 *
 * @brief Insert a value into a hash table, overwriting any existing entry using the same key.
 *
 * @param hash_table           The hash table.
 * @param key                  The key for the new value.
 * @param value                The value to insert.
 * @return                     Non-zero if the value was added successfully,
 *                             or zero if it was not possible to allocate
 *                             memory for the new entry.
 */
static BOOL __ht_insert(CwtHashTable *hash_table, CwtHashTableKey key, CwtHashTableValue value)
{
	CwtHashTableEntry *rover;
	CwtHashTableEntry *newentry;
	UINT               index;

	/* If there are too many items in the table with respect to the table
	 * size, the number of hash collisions increases and performance
	 * decreases. Enlarge the table size to prevent this happening */

	if ((hash_table->entries * 3) / hash_table->table_size > 0) {

		/* Table is more than 1/3 full */

		if( !__ht_enlarge(hash_table) ) {

			/* Failed to enlarge the table */
			return FALSE;
		}
	}

	/* Generate the hash of the key and hence the index into the table */
	index = (UINT)(hash_table->hash_func(key) % hash_table->table_size);

	/* Traverse the chain at this location and look for an existing
	 * entry with the same key */
	rover = hash_table->table[index];

	while (rover != NULL) {
		if (hash_table->equal_func(rover->key, key) != 0) {

			/* Same key: overwrite this entry with new data */

			/* If there is a value free function, free the old data
			 * before adding in the new data */

			if (hash_table->value_free_func != NULL) {
				hash_table->value_free_func(rover->value);
			}

			/* Same with the key: use the new key value and free 
			 * the old one */

			if (hash_table->key_free_func != NULL) {
				hash_table->key_free_func(rover->key);
			}

			rover->key = key;
			rover->value = value;

			/* Finished */
			
			return TRUE;
		}
		rover = rover->next;
	}
	
	/* Not in the hash table yet.  Create a new entry */
	newentry = CWT_MALLOC(CwtHashTableEntry);

	if( newentry == NULL ) {
		return FALSE;
	}

	newentry->key = key;
	newentry->value = value;

	/* Link into the list */
	newentry->next = hash_table->table[index];
	hash_table->table[index] = newentry;

	/* Maintain the count of the number of entries */
	++hash_table->entries;

	/* Added successfully */
	return TRUE;
}



/**
 * @fn CwtHashTableNS::lookup(CwtHashTable *hash_table, CwtHashTableKey key)
 *
 * @brief Look up a value in a hash table by key.
 *
 * @param hash_table          The hash table.
 * @param key                 The key of the value to look up.
 * @return                    The value, or @ref HASH_TABLE_NULL if there
 *                            is no value with that key in the hash table.
 */
static CwtHashTableValue __ht_lookup(CwtHashTable *hash_table, CwtHashTableKey key)
{
	CwtHashTableEntry *rover;
	size_t index;

	/* Generate the hash of the key and hence the index into the table */
	index = (size_t)(hash_table->hash_func(key) % hash_table->table_size);

	/* Walk the chain at this index until the corresponding entry is
	 * found */
	rover = hash_table->table[index];

	while (rover != NULL) {
		if (hash_table->equal_func(key, rover->key) != 0) {

			/* Found the entry.  Return the data. */
			return rover->value;
		}
		rover = rover->next;
	}

	/* Not found */
	return HASH_TABLE_NULL;
}


/**
 * @fn CwtHashTableNS::remove(CwtHashTable *hash_table, CwtHashTableKey key)
 *
 * @brief Remove a value from a hash table.
 *
 * @param hash_table          The hash table.
 * @param key                 The key of the value to remove.
 * @return                    Non-zero if a key was removed, or zero if the
 *                            specified key was not found in the hash table.
 */
static BOOL __ht_remove(CwtHashTable *hash_table, CwtHashTableKey key)
{
	CwtHashTableEntry **rover;
	CwtHashTableEntry *entry;
	size_t index;
	BOOL ok;

	/* Generate the hash of the key and hence the index into the table */

	index = (size_t)(hash_table->hash_func(key) % hash_table->table_size);

	/* Rover points at the pointer which points at the current entry
	 * in the chain being inspected.  ie. the entry in the table, or
	 * the "next" pointer of the previous entry in the chain.  This
	 * allows us to unlink the entry when we find it. */

	ok = 0;
	rover = &hash_table->table[index];

	while (*rover != NULL) {

		if (hash_table->equal_func(key, (*rover)->key) != 0) {

			/* This is the entry to remove */
			entry = *rover;

			/* Unlink from the list */
			*rover = entry->next;

			/* Destroy the entry structure */
			__ht_free_entry(hash_table, entry);

			/* Track count of entries */
			--hash_table->entries;

			ok = TRUE;

			break;
		}
		
		/* Advance to the next entry */

		rover = &((*rover)->next);
	}

	return ok;
}


/**
 * @fn CwtHashTableNS::size(CwtHashTable *hash_table)
 *
 * @brief Retrieve the number of entries in a hash table.
 *
 * @param hash_table          The hash table.
 * @return                    The number of entries in the hash table.
 */
static size_t __ht_size(CwtHashTable *hash_table)
{
	return hash_table->entries;
}


/**
 * @fn CwtHashTableNS::begin(CwtHashTable *hash_table, CwtHashTableIterator *it)
 *
 * @brief Initialise a @ref CwtHashTableIterator to iterate over a hash table.
 *
 * @param hash_table          The hash table.
 * @param iter                Pointer to an iterator structure to
 *                            initialise.
 */
static void __ht_begin(CwtHashTable *hash_table, CwtHashTableIterator *it)
{
	size_t chain;
	
	it->hash_table = hash_table;

	/* Default value of next if no entries are found. */
	it->next_entry = NULL;
	
	/* Find the first entry */
	for( chain = 0; chain < hash_table->table_size; ++chain ) {
		if(hash_table->table[chain] != NULL) {
			it->next_entry = hash_table->table[chain];
			it->next_chain = chain;
			break;
		}
	}
}

/**
 * @fn CwtHashTableNS::hasMore(CwtHashTableIterator *it)
 *
 * @brief Determine if there are more keys in the hash table to iterate over.
 *
 * @param iterator            The hash table iterator.
 * @return                    Zero if there are no more values to iterate
 *                            over, non-zero if there are more values to
 *                            iterate over.
 */
static BOOL __ht_hasMore(CwtHashTableIterator *it)
{
	return it->next_entry != NULL ? TRUE : FALSE;
}


static ULONG __ht_ccharHash(void *vlocation)
{
	char *location;
	location = (char*)vlocation;
	return (ULONG)*location;
}

static ULONG __ht_wcharHash(void *vlocation)
{
	wchar_t *location;
	location = (wchar_t*)vlocation;
	return (ULONG)*location;
}



/* Hash function for a pointer to an integer */
/**
 * Generate a hash key for a pointer to an integer. The value pointed
 * at is used to generate the key.
 *
 * @param location        The pointer.
 * @return                A hash key for the value at the location.
 */
static ULONG __ht_intHash(void *vlocation)
{
	int *location;

	location = (int*)vlocation;

	return (ULONG)*location;
}


static ULONG __ht_uint16Hash(void *vlocation)
{
	UINT16 *location;

	location = (UINT16*) vlocation;

	return (ULONG) *location;
}


/**
 * Generate a hash key for a pointer.  The value pointed at by the pointer
 * is not used, only the pointer itself.
 *
 * @param location        The pointer
 * @return                A hash key for the pointer.
 */
static ULONG __ht_ptrHash(void *plocation)
{
	return (ULONG)plocation;
}

/**
 * @fn CwtHashTableNS::next(CwtHashTableIterator *it)
 *
 * @brief Using a hash table iterator, retrieve the next entry.
 *
 * @param iterator            The hash table iterator.
 * @return                    The next entry from the hash table, or
 *                            @ref HASH_TABLE_NULL if there are no more
 *                            keys to iterate over.
 */
static CwtHashTableEntry* __ht_next(CwtHashTableIterator *it)
{
	CwtHashTableEntry *current_entry;
	CwtHashTableEntry *result;
	CwtHashTable *hash_table;
	/*CwtHashTableValue result;*/
	size_t chain;

	hash_table = it->hash_table;

	/* No more entries? */
	if (it->next_entry == NULL) {
		return HASH_TABLE_NULL;
	}
	
	/* Result is immediately available */
	current_entry = it->next_entry;
	result = current_entry;

	/* Find the next entry */
	if (current_entry->next != NULL) {
		
		/* Next entry in current chain */
		it->next_entry = current_entry->next;
	} else {
	
		/* None left in this chain, so advance to the next chain */
		chain = it->next_chain + 1;

		/* Default value if no next chain found */
		it->next_entry = NULL;

		while (chain < hash_table->table_size) {
			/* Is there anything in this chain? */
			if (hash_table->table[chain] != NULL) {
				it->next_entry = hash_table->table[chain];
				break;
			}

			/* Try the next chain */
			++chain;
		}

		it->next_chain = chain;
	}

	return result;
}


/**
 * Generate a hash key from a string.
 *
 * @param string           The string.
 * @return                 A hash key for the string.
 */
static ULONG __ht_cstrHash(void *string)
{
	/* This is the djb2 string hash function */
	ULONG hash = 5381;
	BYTE *p;

	p = (BYTE*) string;

	while (*p != '\0') {
		hash = ((hash << 5) ^ hash ) ^ (*p);
		++p;
	}

	return hash;

}


/* The same function, with a tolower on every character so that
 * case is ignored.  This code is duplicated for performance. */

/**
 * Generate a hash key from a string, ignoring the case of letters.
 *
 * @param string           The string.
 * @return                 A hash key for the string.
 */
static ULONG __ht_cstriHash(void *string)
{
	ULONG hash = 5381;
	BYTE *p;

	p = (BYTE*) string;

	while (*p != '\0') {
		hash = ((hash << 5) ^ hash ) ^ tolower(*p);
		++p;
	}

	return hash;
}



/**
 * Generate a hash key from a string.
 *
 * @param string           The string.
 * @return                 A hash key for the string.
 */
static ULONG __ht_wstrHash(void *string)
{
	/* This is the djb2 string hash function */
	ULONG result = 5381;
	wchar_t *p;

	p = (wchar_t*) string;

	while (*p != L'\0') {
		result = ((result << 5) ^ result ) ^ (*p);
		++p;
	}

	return result;
}

/**
 * Generate a hash key from a string, ignoring the case of letters.
 *
 * @param string           The string.
 * @return                 A hash key for the string.
 */
static ULONG __ht_wstriHash(void *string)
{
	unsigned long result = 5381;
	wchar_t *p;

	p = (wchar_t *) string;

	while (*p != L'\0') {
		result = ((result << 5) ^ result ) ^ towlower(*p);
		++p;
	}

	return result;
}



static BOOL __ht_cchareq (void *plocation1, void *plocation2)
{
	return (*(char*)plocation1) == (*(char*)plocation2) ?  TRUE : FALSE;
}

static BOOL __ht_wchareq (void *plocation1, void *plocation2)
{
	return (*(wchar_t*)plocation1) == (*(wchar_t*)plocation2) ?  TRUE : FALSE;
}

/**
 * Compare the integer values pointed at by two pointers to determine
 * if they are equal.
 *
 * @param location1       Pointer to the first value to compare.
 * @param location2       Pointer to the second value to compare.
 * @return                @c TRUE if the two values are equal, @c FALSE if the
 *                        two values are not equal.
 */

static BOOL __ht_inteq(void *plocation1, void *plocation2)
{
	return (*(int*)plocation1) == (*(int*)plocation2) ?  TRUE : FALSE;
}

/*
 * Compare the integer values pointed at by two pointers.
 *
 * @param location1        Pointer to the first value to compare.
 * @param location2        Pointer to the second value to compare.
 * @return                 A negative value if the first value is less than
 *                         the second value, a positive value if the first
 *                         value is greater than the second value, zero if
 *                         they are equal.
 */

/*
int int_compare(void *plocation1, void *plocation2)
{
	int *location1;
	int *location2;

	location1 = (int*)plocation1;
	location2 = (int*)plocation2;

	if (*location1 < *location2) {
		return -1;
	} else if (*location1 > *location2) {
		return 1;
	}

	return 0;
}
*/


/* Comparison functions for strings */

/**
 * Compare two strings to determine if they are equal.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                @c TRUE if the strings are equal, @c FALSE if they are
 *                        not equal.
 */
static BOOL __ht_cstreq(void *string1, void *string2)
{
	return strcmp((char*)string1, (char*)string2) == 0 ? TRUE : FALSE;
}


/*
 * Compare two strings.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                A negative value if the first string should be
 *                        sorted before the second, a positive value if the
 *                        first string should be sorted after the second,
 *                        zero if the two strings are equal.
 */
/*
static int __ht_cstrcmp(void *string1, void *string2)
{
	int result;

	result = strcmp((char *) string1, (char *) string2);

	if (result < 0) {
		return -1;
	} else if (result > 0) {
		return 1;
	} else {
		return 0;
	}
}
*/

/* Comparison functions for strings, which ignore the case of letters. */

/**
 * Compare two strings to determine if they are equal, ignoring the case of letters.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                @c TRUE if the strings are equal, @c FALSE if they are
 *                        not equal.
 */
static BOOL __ht_cstrieq(void *string1, void *string2)
{
	return __ht_cstricmp((char *) string1, (char *) string2) == 0 ? TRUE : FALSE;
}

/* On many systems, strcasecmp or stricmp will give the same functionality
 * as this function.  However, it is non-standard and cannot be relied
 * on to be present. */

/*
 * Compare two strings, ignoring the case of letters.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                A negative value if the first string should be
 *                        sorted before the second, a positive value if the
 *                        first string should be sorted after the second,
 *                        zero if the two strings are equal.
 */
static int __ht_cstricmp(void *string1, void *string2)
{
	char *p1;
	char *p2;
	int c1, c2;

	/* Iterate over each character in the strings */

	p1 = (char *) string1;
	p2 = (char *) string2;

	for (;;) {

		c1 = tolower(*p1);
		c2 = tolower(*p2);

		if (c1 != c2) {

			/* Strings are different */

			if (c1 < c2) {
				return -1;
			} else {
				return 1;
			}
		}

		/* End of string */

		if (c1 == '\0')
			break;

		/* Advance to the next character */
		++p1;
		++p2;
	}

	/* Reached the end of string and no difference found */
	return 0;
}


/* Comparison functions for strings */

/**
 * Compare two wide strings to determine if they are equal.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                @c TRUE if the strings are equal, @c FALSE if they are
 *                        not equal.
 */
static BOOL __ht_wstreq(void *string1, void *string2)
{
	return wcscmp((wchar_t *) string1, (wchar_t *) string2) == 0 ? TRUE : FALSE;
}

/*
 * Compare two strings.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                A negative value if the first string should be
 *                        sorted before the second, a positive value if the
 *                        first string should be sorted after the second,
 *                        zero if the two strings are equal.
 */
/*
static int __ht_wstrcmp(void *string1, void *string2)
{
	int result;

	result = wcscmp((wchar_t *) string1, (wchar_t *) string2);

	if (result < 0) {
		return -1;
	} else if (result > 0) {
		return 1;
	} else {
		return 0;
	}
}
*/

/* Comparison functions for strings, which ignore the case of letters. */

/**
 * Compare two strings to determine if they are equal, ignoring the
 * case of letters.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                @c TRUE if the strings are equal, @c FALSE if they are
 *                        not equal.
 */
static BOOL __ht_wstrieq(void *string1, void *string2)
{
	return __ht_wstricmp((wchar_t *) string1, (wchar_t *) string2) == 0 ? TRUE : FALSE;
}

/* On many systems, strcasecmp or stricmp will give the same functionality
 * as this function.  However, it is non-standard and cannot be relied
 * on to be present. */
/*
 * Compare two strings, ignoring the case of letters.
 *
 * @param string1         The first string.
 * @param string2         The second string.
 * @return                A negative value if the first string should be
 *                        sorted before the second, a positive value if the
 *                        first string should be sorted after the second,
 *                        zero if the two strings are equal.
 */
static int __ht_wstricmp(void *string1, void *string2)
{
	wchar_t *p1;
	wchar_t *p2;
	wint_t c1, c2;

	/* Iterate over each character in the strings */

	p1 = (wchar_t *) string1;
	p2 = (wchar_t *) string2;

	for (;;) {

		c1 = towlower(*p1);
		c2 = towlower(*p2);

		if (c1 != c2) {

			/* Strings are different */

			if (c1 < c2) {
				return -1;
			} else {
				return 1;
			}
		}

		/* End of string */

		if (c1 == '\0')
			break;

		/* Advance to the next character */

		++p1;
		++p2;
	}

	/* Reached the end of string and no difference found */

	return 0;
}
