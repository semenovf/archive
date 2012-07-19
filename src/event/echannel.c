/*
 * event/channel.c
 *
 *  Created on: 25.06.2012
 *      Author: wladt
 */
#include <cwt/event/channel.h>
#include <cwt/event/kbd.h>
#include <cwt/event/event.h>
#include <cwt/dlist.h>

typedef struct CwtEventChannel {
	CwtEvent base;
	CwtChannelPtr channel;
} CwtEventChannel;


typedef struct CwtChannelListener {
	CwtChannelPtr channel;
	CwtStack      readers;
} CwtChannelListener;

static CwtDListNode* __cwtChannelFindListenerNode(CwtChannelPtr chan);
static void __cwtEventChannelPoll(void);

static CwtDList       __cwtChannelListeners = {NULL, NULL};
static CwtEventSource __cwtEventChannelSource = { __cwtEventChannelPoll };


static CwtDListNode* __cwtChannelFindListenerNode(CwtChannelPtr chan)
{
	CwtDListIterator it;
	cwtDListBegin(&__cwtChannelListeners, &it);

	while( cwtDListHasMore(&it) ) {
		CwtDListNode *pnode = it.node;
		CwtChannelListener *plistener = (CwtChannelListener*)cwtDListNext(&it);
		if( plistener->channel == chan )
			return pnode;
	}

	return NULL;
}


static void __cwtEventChannelPoll(void)
{
	CwtChannelNS *cns = cwtChannelNS();
	CwtDListIterator it;
	cwtDListBegin(&__cwtChannelListeners, &it);

	while( cwtDListHasMore(&it) ) {
		CwtChannelListener *plistener = (CwtChannelListener*)cwtDListNext(&it);

	    if( cns->bytesAvailable(plistener->channel) > 0 ) {
	    	CwtEventChannel *pevt = CWT_MALLOC(CwtEventChannel);
	    	pevt->channel = plistener->channel;

	    	__cwtEventInitEvent((CwtEventPtr)pevt, &plistener->readers, cwtEventDefaultDestructor);
	    	cwtEventPost((CwtEventPtr)pevt);
	    }
	}
}


CwtEventSourcePtr cwtEventChannelSource(void)
{
	return &__cwtEventChannelSource;
}


/**
 * @brief �������� "���⥫�" @c reader ��� ������ @c chan
 *
 * ��᫥���� ���������� "���⥫�" �㤥� ���� � 楯�窥 ��ࠡ�⪨ ᮮ���� �� ������
 *
 * @param chan
 * @param reader
 */
void cwtEventChannelAddListener( CwtChannelPtr chan, BOOL (*reader)(CwtEventPtr) )
{
	CwtDListNode* pnode;

	CWT_ASSERT(chan);
	CWT_ASSERT(reader);

	pnode = __cwtChannelFindListenerNode(chan);
	if( pnode ) {
		CwtChannelListener *plistener = (CwtChannelListener*)pnode->data;
		cwtStackPush(&plistener->readers, reader);
	} else {
		CwtChannelListener *plistener = CWT_MALLOC(CwtChannelListener);
		pnode = CWT_MALLOC(CwtDListNode);

		plistener->channel = chan;
		cwtStackInit(&plistener->readers);
		cwtStackPush(&plistener->readers, reader);

		pnode->data = plistener;
		cwtDListInsertLast(&__cwtChannelListeners, pnode);
	}
}

void cwtEventChannelRemoveListener( CwtChannelPtr chan )
{
	CwtDListNode* pnode = __cwtChannelFindListenerNode(chan);

	if( pnode ) {
		cwtDListRemove(&__cwtChannelListeners, pnode);
		CWT_FREE(pnode->data);
		CWT_FREE(pnode);
	}
}


void cwtEventPeekChannel(CwtEventPtr pevt, CwtChannelPtr *pchan)
{
	CwtEventChannel *pevt_chan = (CwtEventChannel*)pevt;

	CWT_ASSERT(pevt);

	if( pchan ) *pchan = pevt_chan->channel;
}
