/*
 * ev_io.c
 *
 *  Created on: 08.01.2012
 *      Author: wladt
 */
#include <stdlib.h>
#include <cwt/list.h>
#include <cwt/cherry/event.h>

static DList __channel_list = {NULL, NULL};

void cwt_add_channel_listener( CHANNEL_PTR chan )
{
	DListNode *pNode = JQ_MALLOC(DListNode);
	JQ_ASSERT(pNode);

	pNode->data = chan;
	dlist_insert_last(&__channel_list, pNode);
}

void cwt_remove_channel_listener( CHANNEL_PTR chan )
{
	DListNode* pNode = dlist_find(&__channel_list, chan);

	if( pNode ) {
		dlist_remove(&__channel_list, pNode);
		JQ_FREE(pNode);
	}
}


void _cwt_process_channel(void)
{
	DListNode* pnode = __channel_list.first;

	while( pnode ) {
		CHANNEL_PTR chan = (CHANNEL_PTR)pnode->data;

	    if( channel_bytes_available(chan) > 0 ) {
	    	CWT_CHANNEL_EVENT *evt = JQ_MALLOC(CWT_CHANNEL_EVENT);
	    	evt->channel = chan;

	    	cwt_post_event(CWT_EVT_CHANNEL, evt, CWT_DEFAULT_FREE);
	    }

	    pnode = pnode->next;
	}
}


