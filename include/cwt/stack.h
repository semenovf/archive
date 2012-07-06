/*
 * list.h
 *
 *  Created on: 09.12.2011
 *      Author: wladt
 */

#ifndef __CWT_STACK_H__
#define __CWT_STACK_H__

#include <cwt/list.h>

typedef struct List Stack;
typedef struct List CwtStack;
typedef struct ListNode StackNode;
typedef struct ListNode CwtStackNode;

struct StackIterator
{
	Stack *stack;
	StackNode *node; /* current node */
};
typedef struct StackIterator StackIterator;
typedef struct StackIterator CwtStackIterator;

EXTERN_C_BEGIN

DLL_API_EXPORT void  stack_init(Stack *stack);
DLL_API_EXPORT void  stack_push(Stack *stack, void *data);
DLL_API_EXPORT void* stack_pop(Stack *stack);
DLL_API_EXPORT void* stack_peek(Stack *stack);
DLL_API_EXPORT BOOL  stack_empty(Stack *stack);
#define stack_clear list_clear

#define cwtStackInit  stack_init
#define cwtStackPush  stack_push
#define cwtStackPop   stack_pop
#define cwtStackPeek  stack_peek
#define cwtStackEmpty stack_empty
#define cwtStackClear stack_clear

DLL_API_EXPORT void stack_begin(Stack *stack, StackIterator *si);
DLL_API_EXPORT BOOL stack_has_more(StackIterator *si);
DLL_API_EXPORT void *stack_next(StackIterator *si);

#define cwtStackBegin   stack_begin
#define cwtStackHasMore stack_has_more
#define cwtStackNext    stack_next

EXTERN_C_END

#endif /* __CWT_STACK_H__ */
