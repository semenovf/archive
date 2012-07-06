/*
 * ev_timr.c
 *
 *  Created on: 09.01.2012
 *      Author: wladt
 */

#include <time.h>
#include <cwt/cherry/event.h>
#include <cwt/list.h>


struct _CWT_TIMER_DATA
{
	UINT16 id;
	clock_t limit;
};
typedef struct _CWT_TIMER_DATA _CWT_TIMER_DATA;

static DList __timers = { NULL, NULL };


void _cwt_process_timer(void)
{
	DListNode* pnode = __timers.first;
	DListNode* pnode_next;

	while( pnode ) {
		_CWT_TIMER_DATA *timer_data = (_CWT_TIMER_DATA*)pnode->data;

		pnode_next = pnode->next;

		if( timer_data->limit <= clock() ) {
	    	CWT_TIMER_EVENT *evt = JQ_MALLOC(CWT_TIMER_EVENT);
	    	evt->id = timer_data->id;
	    	JQ_FREE(timer_data);
    		dlist_remove(&__timers, pnode);
			JQ_FREE(pnode);

	    	cwt_post_event(CWT_EVT_TIMER, evt, CWT_DEFAULT_FREE);
	    }

	    pnode = pnode_next;
	}
}


/**
 * Register and start timer
 *
 * @param id timer identifier
 * @param interval in milliseconds (precision is equivalent to 1000/CLC_TCK milliseconds ~ 55ms)
 */
void cwt_start_timer(UINT16 id, long interval)
{
	DListNode *pnode = JQ_MALLOC(DListNode);
	_CWT_TIMER_DATA *timer_data = JQ_MALLOC(_CWT_TIMER_DATA);
	JQ_ASSERT(pnode);

	timer_data->id = id;
	timer_data->limit = clock() + (interval*CLK_TCK/1000L);
	pnode->data = timer_data;
	dlist_insert_last(&__timers, pnode);
}
