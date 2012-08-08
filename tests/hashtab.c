/*
 * Copyright (c) 2012, Fedor Semenov
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * hashtab.c
 *
 *  Created on: 07.08.2012
 *      Author: wladt
 */

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


#include <cwt/test.h>
#include <cwt/hashtab.h>
/*
#include <stdio.h>
#include <cwt/types.h>

#include <cwt/utils.h>
#include <cwt/string.h>
*/

#define NUM_TEST_VALUES_0 200

#define NUM_TEST_VALUES 10000

int value1 = 1, value2 = 2, value3 = 3, value4 = 4;
int allocated_keys = 0;
int allocated_values = 0;

static CwtHashTableNS *__htNS = NULL;

void test_pointer_hash(void)
{
	int array[NUM_TEST_VALUES_0];
	int i, j;

	/* Initialize the array to all zeros */
	for( i = 0; i < NUM_TEST_VALUES_0; ++i ) {
		array[i] = 0;
	}

	/* Check hashes are never the same */
	for( i = 0; i < NUM_TEST_VALUES_0; ++i ) {
		for( j = i + 1; j < NUM_TEST_VALUES_0; ++j ) {
			CWT_TEST_FAIL(__htNS->ptrHash(&array[i]) != __htNS->ptrHash(&array[j]));
		}
	}
}

void test_int_hash(void)
{
	int array[NUM_TEST_VALUES_0];
	int i, j;

	/* Initialise all entries in the array */
	for( i = 0; i < NUM_TEST_VALUES_0; ++i ) {
		array[i] = i;
	}

	/* Check hashes are never the same */
	for( i = 0; i < NUM_TEST_VALUES_0; ++i ) {
		for( j =  i + 1; j < NUM_TEST_VALUES_0; ++j ) {
			CWT_TEST_FAIL( __htNS->intHash(&array[i]) != __htNS->intHash(&array[j]) );
		}
	}

	/* Hashes of two variables containing the same value are the same */
	i = 5000;
	j = 5000;

	CWT_TEST_OK(__htNS->intHash(&i) == __htNS->intHash(&j));
}

void test_string_hash(void)
{
	char test1[] = "this is a test";
	char test2[] = "this is a testu";
	char test3[] = "this is a test ";
	char test4[] = "this is a test";
	char test5[] = "This is a test";

	/* Contents affect the hash */
	CWT_TEST_OK(__htNS->cstrHash(test1) != __htNS->cstrHash(test2));

	/* Length affects the hash */
	CWT_TEST_OK(__htNS->cstrHash(test1) != __htNS->cstrHash(test3));

	/* Case sensitive */
	CWT_TEST_OK(__htNS->cstrHash(test1) != __htNS->cstrHash(test5));

	/* The same strings give the same hash */
	CWT_TEST_OK(__htNS->cstrHash(test1) == __htNS->cstrHash(test4));
}

void test_string_nocase_hash(void)
{
	char test1[] = "this is a test";
	char test2[] = "this is a tesu";
	char test3[] = "this is a test ";
	char test4[] = "this is a test";
	char test5[] = "This is a test";

	/* Contents affect the hash */
	CWT_TEST_OK(__htNS->cstriHash(test1) != __htNS->cstriHash(test2));

	/* Length affects the hash */
	CWT_TEST_OK(__htNS->cstriHash(test1) != __htNS->cstriHash(test3));

	/* Case insensitive */
	CWT_TEST_OK(__htNS->cstriHash(test1) == __htNS->cstriHash(test5));

	/* The same strings give the same hash */
	CWT_TEST_OK(__htNS->cstriHash(test1) == __htNS->cstriHash(test4));
}


/* Generates a hash table for use in tests containing 10,000 entries */

CwtHashTable* generate_hash_table(void)
{
	CwtHashTable *hash_table;
	char buf[10];
	char *value;
	int i;

	/* Allocate a new hash table.  We use a hash table with keys that are
	 * string versions of the integer values 0..9999 to ensure that there
	 * will be collisions within the hash table (using integer values
	 * with int_hash causes no collisions) */
	hash_table = __htNS->create(__htNS->cstrHash, __htNS->cstreq, NULL, cwtFree);

	/* Insert lots of values */
	for( i = 0; i < NUM_TEST_VALUES; ++i ) {
		sprintf(buf, "%d", i);

		value = CWT_ISO_CPP_NAME(strdup)(buf);

		__htNS->insert(hash_table, value, value);
	}

	return hash_table;
}


/* Test insert and lookup functions */
void test_hash_table_insert_lookup(void)
{
	CwtHashTable *hash_table;
	char buf[10];
	char *value;
	int i;

	/* Generate a hash table */
	hash_table = generate_hash_table();

	CWT_TEST_OK(__htNS->size(hash_table) == NUM_TEST_VALUES);

	/* Check all values */
	for( i = 0; i < NUM_TEST_VALUES; ++i ) {
		sprintf(buf, "%d", i);
		value = __htNS->lookup(hash_table, buf);
		CWT_TEST_FAIL(value);
		CWT_TEST_OK(strcmp(value, buf) == 0);
	}

	/* Lookup on invalid values returns NULL */
	sprintf(buf, "%d", -1);
	CWT_TEST_OK(__htNS->lookup(hash_table, buf) == NULL);
	sprintf(buf, "%d", NUM_TEST_VALUES);
	CWT_TEST_OK(__htNS->lookup(hash_table, buf) == NULL);

	/* Insert overwrites existing entries with the same key */
	sprintf(buf, "%d", 12345);
	__htNS->insert(hash_table, buf, CWT_ISO_CPP_NAME(strdup)("hello world"));
	value = __htNS->lookup(hash_table, buf);
	CWT_TEST_FAIL(value);
	CWT_TEST_OK(strcmp(value, "hello world") == 0);

	__htNS->free(hash_table);
}


void test_hash_table_remove(void)
{
	CwtHashTable *hash_table;
	char buf[10];

	hash_table = generate_hash_table();

	CWT_TEST_OK(__htNS->size(hash_table) == NUM_TEST_VALUES);
	sprintf(buf, "%d", 5000);
	CWT_TEST_OK(__htNS->lookup(hash_table, buf) != NULL);

	/* Remove an entry */
	__htNS->remove(hash_table, buf);

	/* Check entry counter */
	CWT_TEST_OK(__htNS->size(hash_table) == 9999);

	/* Check that NULL is returned now */
	CWT_TEST_OK(__htNS->lookup(hash_table, buf) == NULL);

	/* Try removing a non-existent entry */
	sprintf(buf, "%d", -1);
	__htNS->remove(hash_table, buf);

	CWT_TEST_OK(__htNS->size(hash_table) == 9999);

	__htNS->free(hash_table);
}



void test_hash_table_iterating(void)
{
	CwtHashTable *hash_table;
	CwtHashTableIterator it;
	int count;

	hash_table = generate_hash_table();

	/* Iterate over all values in the table */
	count = 0;

	__htNS->begin(hash_table, &it);

	while (__htNS->hasMore(&it)) {
		__htNS->next(&it);

		++count;
	}

	CWT_TEST_OK(count == NUM_TEST_VALUES);

	/* Test iter_next after iteration has completed. */
	CWT_TEST_OK(__htNS->next(&it) == HASH_TABLE_NULL);

	__htNS->free(hash_table);

	/* Test iterating over an empty table */
	hash_table = __htNS->create(__htNS->intHash, __htNS->inteq, NULL, NULL);

	__htNS->begin(hash_table, &it);

	CWT_TEST_OK(__htNS->hasMore(&it) == 0);

	__htNS->free(hash_table);
}


/* Demonstrates the ability to iteratively remove objects from
 * a hash table: i.e. removing the current key being iterated over
 * does not break the iterator. */

void test_hash_table_iterating_remove(void)
{
	CwtHashTable *hash_table;
	CwtHashTableIterator iterator;
	char buf[10];
	char *val;
	int count;
	int removed;
	int i;

	hash_table = generate_hash_table();

	/* Iterate over all values in the table */

	count = 0;
	removed = 0;

	__htNS->begin(hash_table, &iterator);

	while( __htNS->hasMore(&iterator) ) {

		/* Read the next value */
		val = __htNS->next(&iterator)->value;

		/* Remove every hundredth entry */
		if ((atoi(val) % 100) == 0) {
			__htNS->remove(hash_table, val);
			++removed;
		}

		++count;
	}

	/* Check counts */
	CWT_TEST_OK(removed == 100);
	CWT_TEST_OK(count == NUM_TEST_VALUES);

	CWT_TEST_OK(__htNS->size(hash_table) == NUM_TEST_VALUES - removed);

	/* Check all entries divisible by 100 were really removed */
	for( i = 0; i < NUM_TEST_VALUES; ++i ) {
		sprintf(buf, "%d", i);

		if( i % 100 == 0 ) {
			CWT_TEST_OK(__htNS->lookup(hash_table, buf) == NULL);
		} else {
			CWT_TEST_OK(__htNS->lookup(hash_table, buf) != NULL);
		}
	}

	__htNS->free(hash_table);
}


/* Create a new key */
int *new_key(int value)
{
	int *result;

	result = CWT_MALLOC(int);
	*result = value;

	++allocated_keys;

	return result;
}

/* Callback function invoked when a key is freed */
void free_key(void *key)
{
	CWT_FREE(key);
	--allocated_keys;
}

/* Create a new value */
int *new_value(int value)
{
	int *result;

	result = CWT_MALLOC(int);
	*result = value;

	++allocated_values;

	return result;
}

/* Callback function invoked when a value is freed */
void free_value(void *value)
{
	CWT_FREE(value);
	--allocated_values;
}

/* Test the use of functions to free keys / values when they are removed. */
void test_hash_table_free_functions(void)
{
	CwtHashTable *hash_table;
	int *key;
	int *value;
	int i;

	/* Create a hash table, fill it with values */
	hash_table = __htNS->create(__htNS->intHash, __htNS->inteq, free_key, free_value);

	allocated_values = 0;

	for( i = 0; i < NUM_TEST_VALUES; ++i ) {
		key = new_key(i);
		value = new_value(99);

		__htNS->insert(hash_table, key, value);
	}

	CWT_TEST_OK(allocated_keys == NUM_TEST_VALUES);
	CWT_TEST_OK(allocated_values == NUM_TEST_VALUES);

	/* Check that removing a key works */
	i = NUM_TEST_VALUES / 2;
	__htNS->remove(hash_table, &i);

	CWT_TEST_OK(allocated_keys == NUM_TEST_VALUES - 1);
	CWT_TEST_OK(allocated_values == NUM_TEST_VALUES - 1);

	/* Check that replacing an existing key works */

	key = new_key(NUM_TEST_VALUES / 3);
	value = new_value(999);

	CWT_TEST_OK(allocated_keys == NUM_TEST_VALUES);
	CWT_TEST_OK(allocated_values == NUM_TEST_VALUES);

	__htNS->insert(hash_table, key, value);

	CWT_TEST_OK(allocated_keys == NUM_TEST_VALUES - 1);
	CWT_TEST_OK(allocated_values == NUM_TEST_VALUES - 1);

	/* A free of the hash table should free all of the keys and values */
	__htNS->free(hash_table);

	CWT_TEST_OK(allocated_keys == 0);
	CWT_TEST_OK(allocated_values == 0);
}


int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	__htNS = cwtHashTableNS();

	CWT_BEGIN_TESTS(69835);

	test_pointer_hash();
	test_int_hash();
	test_string_hash();
	test_string_nocase_hash();

	test_hash_table_insert_lookup();
	test_hash_table_remove();
	test_hash_table_iterating();
	test_hash_table_iterating_remove();
	test_hash_table_free_functions();

	CWT_END_TESTS;
}


