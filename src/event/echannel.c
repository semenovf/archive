/*
 * event/channel.c
 *
 *  Created on: 25.06.2012
 *      Author: wladt
 */
#include <cwt/event/channel.h>
#include <cwt/event/kbd.h>
#include <cwt/event/event.h>
#include <cwt/list.h>

typedef struct _CwtEventChannel {
	CwtEvent __base;
	CwtChannel *channel;
} CwtEventChannel;


typedef struct _CwtChannelListener {
	CwtChannel  *channel;
	CwtList     *readers;
} CwtChannelListener;

static void __pollChannel(void);
static void __finish(CwtEventSource*);
static void __cleanupListener(void*);
static BOOL __findListener(CwtChannel *pchan, CwtListIterator *it);

static CwtList        *__listeners = NULL;
static CwtEventSource  __channel_source = { __pollChannel, __finish };

static CwtEventSource* __source        (void);
static void            __addListener   (CwtChannel *pchan, BOOL (*reader)(CwtEventPtr));
static void 		   __removeListener(CwtChannel *pchan);
static void            __peekChannel   (CwtEvent *pevt, CwtChannel **pchan);

static CwtEventChannelNS __cwtEventChannelNS = {
	  __source
	, __addListener
	, __removeListener
	, __peekChannel
};

static CwtListNS    *__listNS = NULL;
static CwtChannelNS *__channelNS = NULL;
static CwtEventNS   *__eventNS = NULL;

DLL_API_EXPORT CwtEventChannelNS* cwtEventChannelNS(void)
{
	if( !__listNS ) {
		__listNS = cwtListNS();
		__channelNS = cwtChannelNS();
		__eventNS = cwtEventNS();
	}
	if( !__listeners ) {
		__listeners = __listNS->create(sizeof(CwtChannelListener), __cleanupListener);
	}
	return &__cwtEventChannelNS;
}


static void __cleanupListener(void *pl)
{
	CwtChannelListener *plistener = (CwtChannelListener*)pl;
	__listNS->free(plistener->readers);
}

static BOOL __findListener(CwtChannel *pchan, CwtListIterator *it)
{
	__listNS->begin(__listeners, it);

	while( __listNS->hasMore(it) ) {
		CwtChannelListener *l = (CwtChannelListener*)__listNS->data(it);

		if( l->channel == pchan )
			return TRUE;

		(void*)__listNS->next(it);
	}

	return FALSE;
}

static CwtEventSource* __source(void)
{
	return &__channel_source;
}


static void __addListener(CwtChannel *pchan, BOOL (*reader)(CwtEventPtr))
{
	CwtListIterator it;

	CWT_ASSERT(pchan);
	CWT_ASSERT(reader);

	if( __findListener(pchan, &it) ) {
		CwtChannelListener *plistener = (CwtChannelListener*)__listNS->data(&it);
		__listNS->append(plistener->readers, reader);
	} else {
		CwtChannelListener listener;
		listener.channel = pchan;
		listener.readers = __listNS->createPtr(NULL);
		__listNS->append(listener.readers, reader);
		__listNS->append(__listeners, &listener);
	}
}


static void __removeListener(CwtChannel *pchan)
{
	CwtListIterator it;

	if( __findListener(pchan, &it) ) {
		__listNS->remove(&it);
	}
}

static void __peekChannel(CwtEvent *pevt, CwtChannel **pchan)
{
	CwtEventChannel *pevt_chan = (CwtEventChannel*)pevt;

	CWT_ASSERT(pevt);

	if( pchan )
		*pchan = pevt_chan->channel;
}


static void __pollChannel(void)
{
	CwtListIterator it;

	if( !__listeners )
		return;

	__listNS->begin(__listeners, &it);

	while( __listNS->hasMore(&it) ) {
		CwtChannelListener *plistener = (CwtChannelListener*)__listNS->next(&it);

	    if( __channelNS->bytesAvailable(plistener->channel) > 0 ) {
	    	CwtEventChannel *pevt = CWT_MALLOC(CwtEventChannel);
	    	pevt->channel = plistener->channel;

	    	__eventNS->initEvent((CwtEvent*)pevt, plistener->readers, __eventNS->defaultDestructor);
	    	__eventNS->post((CwtEvent*)pevt);
	    }
	}
}


static void __finish(CwtEventSource *source)
{
	/* TODO need to cleanup channel event resources */
	CWT_UNUSED(source);
}



