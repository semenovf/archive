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

static const size_t __primes_count = sizeof(__primes) / sizeof(uint_t);

const uint_t* primes_for_hash(/*out*/ size_t *count)
{
	*count = __primes_count;
	return __primes;
}
