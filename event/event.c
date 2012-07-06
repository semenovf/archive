/*
 * cwt/event/event.c
 *
 *  Created on: 22.06.2012
 *      Author: wladt
 */
#include <cwt/event/event.h>
#include <cwt/logger.h>
#include <cwt/dlist.h>
#include <cwt/stack.h>
#include <cwt/unistd.h>

typedef CwtDList CwtEventQueue;

static CwtDList       __cwt_evt_registered;
static CwtEventQueue  __cwt_evt_queue_pair[2];
static CwtEventQueue* __cwt_evt_output_queue;
static CwtEventQueue* __cwt_evt_input_queue;
static void         (*__cwtEventIdleProcess)(void) = NULL;
static CwtEvent       __cwt_evtQuit;


static void __cwtEventInit(void);
static BOOL __cwtEventProcessSources(void);
static BOOL __cwtEventDispatchEvents(void);


static void __cwtEventInit(void)
{
	dlist_init(&__cwt_evt_registered);
	dlist_init(&__cwt_evt_queue_pair[0]);
	dlist_init(&__cwt_evt_queue_pair[1]);
	__cwt_evt_output_queue = &__cwt_evt_queue_pair[0];
	__cwt_evt_input_queue  = &__cwt_evt_queue_pair[1];
}


/**
 * Выполняет опрос всех зарегистрированных источников событий
 *
 * @return
 */
static BOOL __cwtEventProcessSources(void)
{
	if( __cwt_evt_registered.first ) {
		DListIterator it;
		dlist_begin(&__cwt_evt_registered, &it);

		while( dlist_has_more(&it) ) {
			CwtEventSource *source = (CwtEventSource*)dlist_next(&it);

			CWT_ASSERT(source->poll);
			source->poll();
		}
	}

	//__cwt_evt_translate_events();
	return __cwtEventDispatchEvents();
}


static BOOL __cwtEventDispatchEvents(void)
{
	DListNode *pevt_node;
	BOOL quit = FALSE; /* TRUE if need to stop event loop */

	if( __cwt_evt_output_queue == &__cwt_evt_queue_pair[0] ) {
		__cwt_evt_input_queue  = &__cwt_evt_queue_pair[0];
		__cwt_evt_output_queue = &__cwt_evt_queue_pair[1];
	} else {
		__cwt_evt_input_queue  = &__cwt_evt_queue_pair[1];
		__cwt_evt_output_queue = &__cwt_evt_queue_pair[0];
	}
	CWT_ASSERT(__cwt_evt_output_queue->first == NULL);

	pevt_node = __cwt_evt_input_queue->first;

	if( !pevt_node ) {
		if( __cwtEventIdleProcess ) {
			__cwtEventIdleProcess();
		} else {
			cwtMsleep(50);
		}
	} else {
		while( pevt_node ) {
			CwtStackIterator hit;
			CwtEventPtr pevt = pevt_node->data;

			cwtDListRemove(__cwt_evt_input_queue, pevt_node);

			if( pevt == &__cwt_evtQuit) {
				quit = TRUE;
				break;
			}

			cwtStackBegin(pevt->handler_stack, &hit);

			while( cwtStackHasMore(&hit) ) {
				CwtEventHandler event_handler = (CwtEventHandler)cwtStackNext(&hit);
				if( event_handler(pevt) )
					break;
			}

			if( pevt->destructor ) {
				pevt->destructor(pevt);
			}

			CWT_FREE(pevt_node);
			pevt_node = __cwt_evt_input_queue->first;
		}
	}

	return quit ? FALSE : TRUE;
}


void __cwtEventInitEvent(CwtEventPtr pevt, CwtStack *handlers, void (*destructor)(struct CwtEvent*))
{
	CWT_ASSERT(pevt);
	pevt->handler_stack = handlers;
	pevt->destructor = destructor;
}

/**
 * @brief Регистрирует источник событий
 *
 * @param source источник событий
 * @return @c true, если источник событий зарегистрирован, иначе @c false
 */
BOOL cwtEventRegisterSource(CwtEventSourcePtr source)
{
	DListNode *src_node;

	if( !__cwt_evt_registered.first ) {
		__cwtEventInit();
	}

	if( cwtEventIsRegisteredSource(source) )
		return TRUE;

	src_node = CWT_MALLOC(DListNode);
	src_node->data = source;

	cwtDListInsertLast(&__cwt_evt_registered, src_node);
	return TRUE;
}

/**
 * @brief Отменить регистрацию источника событий
 *
 * @param source source источник событий
 * @return @c true, если требуемый источник найден и его регистрация отменена, илие @c false
 * 	, если источник событий ранее не был зарегистрирован
 */
BOOL cwtEventUnregisterSource(CwtEventSourcePtr source)
{
	if( __cwt_evt_registered.first ) {
		CwtDListIterator it;
		cwtDListBegin(&__cwt_evt_registered, &it);

		while( cwtDListHasMore(&it) ) {
			CwtDListNode *src_node = it.node;

			if( cwtDListNext(&it) == source ) {
				src_node  = cwtDListRemove(&__cwt_evt_registered, src_node);
				CWT_FREE(src_node);
				return TRUE;
			}
		}
	}

	return FALSE;
}

/**
 * @brief Проверить регистрацию источника событий
 *
 * @param source источник событий
 * @return @c true, если источник событий зарегисрирован, иначе @c false
 */
BOOL cwtEventIsRegisteredSource(CwtEventSourcePtr source)
{
	if( __cwt_evt_registered.first ) {
		CwtDListIterator it;

		cwtDListBegin(&__cwt_evt_registered, &it);

		while( cwtDListHasMore(&it) ) {
			if( cwtDListNext(&it) == source )
				return TRUE;
		}
	}

	return FALSE;
}

/**
 * @brief Отменить регистрацию всех зарегистрированных ранее источников событий
 *
 */
void cwtEventUnregisterAllSources(void)
{
	cwtDListClear(&__cwt_evt_registered, cwtFree, NULL);
}

void cwtEventQuit(void)
{
	__cwtEventInitEvent(&__cwt_evtQuit, NULL, NULL);
	cwtEventPost(&__cwt_evtQuit);
}



void cwtEventPost(CwtEventPtr pevt)
{
	CwtDListNode *pevt_node = CWT_MALLOC(CwtDListNode);
	pevt_node->data = pevt;
	cwtDListInsertLast(__cwt_evt_output_queue, pevt_node);
}


void cwtEventSetIdleProccess(void (*idle_process)(void))
{
	__cwtEventIdleProcess = idle_process;
}



void cwtEventPushHandler(CwtStack *handlers, BOOL (*handler)(CwtEventPtr pevt))
{
	CWT_ASSERT(handlers);
	CWT_ASSERT(handler);
	cwtStackPush(handlers, handler);
}

void cwtEventPopHandler(Stack *handlers)
{
	CWT_ASSERT(handlers);
	cwtStackPop(handlers);
}


void cwtEventDefaultDestructor(CwtEventPtr pevt)
{
	CWT_FREE(pevt);
}

/**
 * Event loop
 */
void cwtEventLoop(void)
{
//	cwt_mouse_init();
	while( __cwtEventProcessSources() ) {
		;
	}
}


/*
void cwtEventChannelStub(void)
{
;
}
*/
