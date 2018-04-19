/*
 * event.c
 *
 *  Created on: 08.01.2012
 *      Author: wladt
 */
#include <bios.h> /* bioskey */
#include <dos.h>  /* delay */
#include <cwt/logger.h>
#include <cwt/cherry/event.h>
#include <cwt/cherry/mouse.h>
#include <cwt/cherry/cherry.h>

extern void _cwt_process_timer(void);
extern void _cwt_process_clock(void);
extern void _cwt_process_keyboard(void);
extern void _cwt_process_mouse(void);
extern void _cwt_process_channel(void);
BOOL _cwt_handle_events(CWT_EVENT_PTR evt);

static Stack __event_handler_stack = { NULL };
static CWT_EVENT_QUEUE __event_queues[2] = {{ NULL, NULL }, { NULL, NULL }};
static CWT_EVENT_QUEUE* __event_output_queue = &__event_queues[0];

static void (*_cwt_idle_process)(void) = NULL;

static void cwt_process_idle(void)
{
	if( _cwt_idle_process )
		_cwt_idle_process();
	else
		delay(50);
}

static void _cwt_delete_event(CWT_EVENT_PTR pevt)
{
	if( pevt ) {
		if( pevt->ev_data ) {
			if( pevt->delete_data_func )
				pevt->delete_data_func(pevt->ev_data);
		}
		JQ_FREE(pevt);
	}
}

void cwt_set_idle_proccess(void (*idle_process)(void))
{
	_cwt_idle_process = idle_process;
}



void cwt_post_event(CWT_EVENT_TYPE type, void *data, void (*delete_data_func)(void*))
{
	DListNode *pevt_node = JQ_MALLOC(DListNode);
	CWT_EVENT_PTR pevt;

	pevt = JQ_MALLOC(CWT_EVENT);
	pevt->ev_type = type;
	pevt->ev_data = data;
	pevt->delete_data_func = delete_data_func;
	pevt_node->data = pevt;
	dlist_insert_last(__event_output_queue, pevt_node);
	/*printf_trace("POST_EVENT: pevt_node=%p, pevt=%p, type=%d, data=%p", pevt_node, pevt, pevt->ev_type, pevt->ev_data);*/
}


void cwt_post_child_event(void *child, BOOL added)
{
	CWT_CHILD_EVENT *evt = JQ_MALLOC(CWT_CHILD_EVENT);
	JQ_ASSERT(child);
	evt->child = child;
	evt->added = added;
	cwt_post_event(CWT_EVT_CHILD, evt, CWT_DEFAULT_FREE);
}


static void _cwt_translate_events(void)
{
}


static BOOL _cwt_dispatch_events(void)
{
	DListNode *pevt_node;
	CWT_EVENT_QUEUE* event_input_queue;
	BOOL quit = FALSE; /* TRUE if need to stop event loop */

	if( __event_output_queue == &__event_queues[0] ) {
		event_input_queue = &__event_queues[0];
		__event_output_queue = &__event_queues[1];
	} else {
		event_input_queue = &__event_queues[1];
		__event_output_queue = &__event_queues[0];
	}
	JQ_ASSERT(__event_output_queue->first == NULL);

	pevt_node = event_input_queue->first;

	if( !pevt_node ) {
		cwt_process_idle();
	} else {
		while( pevt_node ) {
			CWT_EVENT_PTR pevt = pevt_node->data;

			dlist_remove(event_input_queue, pevt_node);

			/*printf_trace("HANDLE_EVENT: pevt_node=%p, pevt=%p, type=%d, data=%p", pevt_node, pevt, pevt->ev_type, pevt->ev_data);*/

			if( pevt->ev_type == CWT_EVT_QUIT ) {
				quit = TRUE;
			} else {
				if( pevt->ev_type != CWT_EVT_NOEVENT ) {
					StackNode *node = __event_handler_stack.first;

					while( node ) {
						CWT_EVENT_HANDLER event_handler = (CWT_EVENT_HANDLER)node->data;
						if( event_handler(pevt) )
							break;
						node = node->next;
					}
				}
			}

			_cwt_delete_event(pevt);

			JQ_FREE(pevt_node);
			pevt_node = event_input_queue->first;
		}
	}

	return quit ? FALSE : TRUE;
}


static BOOL _cwt_process_events(void)
{
	_cwt_process_timer();
	_cwt_process_mouse();
	_cwt_process_clock();
	_cwt_process_keyboard();
	_cwt_process_channel();

	_cwt_translate_events();
	return _cwt_dispatch_events();
}


int cwt_main_loop(CWT_EVENT_HANDLER ehandler)
{
	cwt_mouse_init();

	JQ_ASSERT(ehandler);

	stack_push(&__event_handler_stack, _cwt_handle_events);
	stack_push(&__event_handler_stack, ehandler);

	while( _cwt_process_events() ) {
		;
	}

	stack_pop(&__event_handler_stack);
	stack_pop(&__event_handler_stack);

	return 0;
}

