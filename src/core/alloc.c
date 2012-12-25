/*
 * alloc.c
 *
 *  Created on: 14.03.2012
 *      Author: user
 */

#include <stdlib.h>
#include <cwt/global.h>
/*#include <cwt/logger.h>*/

DLL_API_EXPORT void* cwt_malloc(size_t size)
{
	void *block;
/*
	size_t sz = 64;

	while( (block = malloc(sz))!= NULL ) {
		free(block);
		if( JQ_UINT_MAX - sz < 64 )
			break;
		sz += 64;
	}
	printf_trace("Available RAM at least: %u", sz);
*/

	block = malloc(size);
	CWT_ASSERT(block);
/*	printf_trace("ALLOC: %p (size=%d)", block, size);*/
	return block;
}

DLL_API_EXPORT void cwt_free(void *block)
{
	if( block )
		free(block);
}
