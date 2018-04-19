/*
 * stack.c
 *
 *  Created on: 31.07.2012
 *      Author: wladt
 */



#include <cwt/stack.h>
#include <cwt/str.h>

#ifndef NULL
#	define NULL ((void*)0)
#endif

static CwtStack*     __stack_create  (size_t sizeofData, void (*data_cleanup)(void*));
static CwtStack*     __stack_createPtr(void (*data_cleanup)(void*));
static void          __stack_free    (CwtStack*);
static void          __stack_clear   (CwtStack*);
static BOOL          __stack_isEmpty (CwtStack*);
static void          __stack_push    (CwtStack*, void *data);
static BOOL          __stack_pop     (CwtStack*, void *data);
static BOOL          __stack_peek    (CwtStack*, void *data);

static CwtStackNS __cwtStackNS = {
	  __stack_create
	, __stack_createPtr
	, __stack_free
	, __stack_clear
	, __stack_isEmpty
	, __stack_push
	, __stack_pop
	, __stack_peek
};

static CwtStrNS  *__strNS = NULL;
static CwtListNS *__listNS = NULL;


/**
 *
 */
DLL_API_EXPORT CwtStackNS* cwt_stack_ns(void)
{
	if( !__strNS ) {
		__strNS = cwt_str_ns();
		__listNS = cwt_list_ns();
	}
	return &__cwtStackNS;
}



static CwtStack* __stack_create(size_t sizeofData, void (*data_cleanup)(void*))
{
	return (CwtStack*)__listNS->create(sizeofData, data_cleanup);
}

static CwtStack* __stack_createPtr(void (*data_cleanup)(void*))
{
	return (CwtStack*)__listNS->createPtr(data_cleanup);
}

static void __stack_free(CwtStack *stack)
{
	__listNS->free(stack);
}

static void __stack_clear(CwtStack *stack)
{
	__listNS->clear(stack);
}

static inline BOOL  __stack_isEmpty(CwtStack *stack)
{
	return __listNS->isEmpty(stack);
}

static void  __stack_push(CwtStack *stack, void *data)
{
	__listNS->prepend(stack, data);
}

static BOOL __stack_pop(CwtStack *stack, void *data)
{
	if( __stack_isEmpty(stack) )
		return FALSE;

	if( data ) {
		__strNS->memcpy(data, stack->f->d, stack->datasz);
	}

	__listNS->removeFirst(stack);

	return TRUE;
}

static BOOL __stack_peek(CwtStack *stack, void *data)
{
	if( __stack_isEmpty(stack) )
		return FALSE;

	if( data ) {
		__strNS->memcpy(data, stack->f->d, stack->datasz);
	}
	return TRUE;
}
