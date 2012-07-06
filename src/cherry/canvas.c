/*
 * canvas.c
 *
 *  Created on: 03.02.2012
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>


void _cwt_init_canvas(CWT_WIDGET_PTR w)
{
	CWT_CANVAS_PTR cs = CWT_CAST(w, CWT_CANVAS_PTR);
	cs->painter = NULL;
}

void _cwt_destroy_canvas(CWT_WIDGET_PTR w)
{
	JQ_UNUSED(w);
}

CWT_WIDGET_PTR _cwt_clone_canvas(CWT_WIDGET_PTR w, CWT_WIDGET_PTR clone)
{
	CWT_CAST(clone, CWT_CANVAS_PTR)->painter = CWT_CAST(w, CWT_CANVAS_PTR)->painter;
	return clone;
}

void _cwt_render_canvas(CWT_WIDGET_PTR w)
{
	CWT_CANVAS_PTR cs = (CWT_CANVAS_PTR)w;

	JQ_ASSERT(w);

	if( cs->painter )
		cs->painter(w);
}

void cwt_canvas_set_painter(CWT_WIDGET_PTR w, void (*painter)(CWT_WIDGET_PTR))
{
	JQ_ASSERT(w);
	if( CWT_CAST(w, CWT_CANVAS_PTR)->painter != painter ) {
		CWT_CAST(w, CWT_CANVAS_PTR)->painter = painter;
	}
}


void* cwt_canvas_painter(CWT_WIDGET_PTR w)
{
	JQ_ASSERT(w);
	return CWT_CAST(w, CWT_CANVAS_PTR)->painter;
}
