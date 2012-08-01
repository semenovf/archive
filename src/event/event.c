/*
 * cwt/event/event.c
 *
 *  Created on: 22.06.2012
 *      Author: wladt
 */
#include <cwt/event/event.h>
#include <cwt/logger.h>
#include <cwt/unistd.h>

static CwtList *__registered_sources;
static CwtList *__queue_pair[2];
static CwtList *__output_queue;
static CwtList *__input_queue;

static void         (*__idle_process)(void) = NULL;
static CwtEvent       __cwt_evtQuit;


static BOOL __processSources(void);
static BOOL __dispatchEvents(void);

static void __initEvent(CwtEvent*, CwtList *handlers, void (*destructor)(CwtEvent*));
static void __registerSource(CwtEventSource*);
static void __unregisterSource(CwtEventSource*);
static void __unregisterAllSources(void);
static BOOL __isRegisteredSource(CwtEventSource*);
static void __setIdleProccess(void (*idle_process)(void));
static void __pushHandler(CwtList *handlers, BOOL (*handler)(CwtEvent *pevt));
static void __popHandler(CwtList *handlers);
static void __postEvent(CwtEvent*);
static void __quit(void);
static void __loop(void);
static void __defaultDestructor(CwtEvent*);


static CwtEventNS __cwtEventNS  = {
	  __initEvent
	, __registerSource
	, __unregisterSource
	, __unregisterAllSources
	, __isRegisteredSource
	, __setIdleProccess
	, __pushHandler
	, __popHandler
	, __postEvent
	, __quit
	, __loop
	, __defaultDestructor
};

static CwtListNS *__listNS = NULL;

DLL_API_EXPORT CwtEventNS* cwtEventNS(void)
{
	if( !__listNS ) {
		__listNS = cwtListNS();
	}

	if( !__registered_sources ) {
		__registered_sources = __listNS->create(sizeof(CwtEventSource*), NULL);
		__queue_pair[0] = __listNS->create(sizeof(CwtEventSource*), NULL);
		__queue_pair[1] = __listNS->create(sizeof(CwtEventSource*), NULL);
		__output_queue = __queue_pair[0];
		__input_queue  = __queue_pair[1];
	}


	return &__cwtEventNS;
}


static void __initEvent(CwtEvent *pevt, CwtList *handlers, void (*destructor)(CwtEvent*))
{
	CWT_ASSERT(pevt);
	pevt->handler_stack = handlers;
	pevt->destructor = destructor;
}


static inline void __registerSource(CwtEventSource *source)
{
	if( !__isRegisteredSource(source) ) {
		__listNS->append(__registered_sources, source);
	}
}

static inline void __unregisterSource(CwtEventSource *source)
{
	CwtListIterator it;
	if( __listNS->find(__registered_sources, source, &it) ) {
		CwtEventSource *source = (CwtEventSource*)__listNS->first(__registered_sources);
		source->finish(source);
		__listNS->remove(&it);
	}
}


static inline void __unregisterAllSources(void)
{
	while( __listNS->size(__registered_sources) ) {
		CwtEventSource *source = (CwtEventSource*)__listNS->first(__registered_sources);
		source->finish(source);
		__listNS->removeFirst(__registered_sources);
	}
}


static inline BOOL __isRegisteredSource(CwtEventSource *source)
{
	return __listNS->find(__registered_sources, source, NULL);
}


static inline void __setIdleProccess(void (*idle_process)(void))
{
	__idle_process = idle_process;
}

static inline void __pushHandler(CwtList *handlers, BOOL (*handler)(CwtEvent *pevt))
{
	CWT_ASSERT(handlers);
	CWT_ASSERT(handler);
	__listNS->prepend(handlers, handler);
}

static inline void __popHandler(CwtList *handlers)
{
	CWT_ASSERT(handlers);
	__listNS->removeFirst(handlers);
}

static inline void __postEvent(CwtEvent *pevt)
{
	__listNS->append(__output_queue, pevt);
}

static inline void __quit(void)
{
	__initEvent(&__cwt_evtQuit, NULL, NULL);
	__postEvent(&__cwt_evtQuit);
}

static void __loop(void)
{
//	cwt_mouse_init();
	while( __processSources() ) {
		;
	}
}

static void __defaultDestructor(CwtEvent *pevt)
{
	CWT_FREE(pevt);
}



static BOOL __processSources(void)
{
	if( __listNS->size(__registered_sources) ) {
		CwtListIterator it;
		__listNS->begin(__registered_sources, &it);

		while( __listNS->hasMore(&it) ) {
			CwtEventSource *source = (CwtEventSource*)__listNS->next(&it);

			CWT_ASSERT(source->poll);
			source->poll();
		}
	}

	//__cwt_evt_translate_events();
	return __dispatchEvents();
}


static BOOL __dispatchEvents(void)
{
	/*DListNode *pevt_node;*/
	BOOL quit = FALSE; /* TRUE if need to stop event loop */

	if( __output_queue == __queue_pair[0] ) {
		__input_queue  = __queue_pair[0];
		__output_queue = __queue_pair[1];
	} else {
		__input_queue  = __queue_pair[1];
		__output_queue = __queue_pair[0];
	}
	/*CWT_ASSERT(__output_queue->first == NULL);*/

	/*pevt_node = __input_queue->first;*/

	if( __listNS->size(__input_queue) ) {
		if( __idle_process ) {
			__idle_process();
		} else {
			cwtUnistdNS()->msleep(50);
		}
	} else {
		while( __listNS->size(__input_queue) ) {
			CwtListIterator it;
			CwtEvent *pevt;

			pevt = __listNS->first(__input_queue);
			__listNS->removeFirst(__input_queue);

			if( pevt == &__cwt_evtQuit) {
				quit = TRUE;
				break;
			}

			__listNS->begin(pevt->handler_stack, &it);

			while( __listNS->hasMore(&it) ) {
				CwtEventHandler event_handler = (CwtEventHandler)__listNS->next(&it);
				if( event_handler(pevt) )
					break;
			}

			if( pevt->destructor ) {
				pevt->destructor(pevt);
			}
		}
	}

	return quit ? FALSE : TRUE;
}
