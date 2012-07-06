/*
 * screen.c
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 */
#include <cwt/cherry/cherry.h>

static CWT_WIDGET_PTR __cwt_screen = NULL;

CWT_WIDGET_PTR cwt_screen(void)
{
	if( !__cwt_screen ) {
		__cwt_screen = cwt_new_widget(0, CWT_WT_SCREEN, NULL);
		_cwt_widget_moveto(__cwt_screen, 0, 0);
		_cwt_widget_resize(__cwt_screen, _cwt_getmaxx()+1, _cwt_getmaxy()+1);
	}
	return __cwt_screen;
}


void _cwt_destroy_screen(CWT_WIDGET_PTR widget)
{
	JQ_UNUSED(widget);
	__cwt_screen = NULL;
}
