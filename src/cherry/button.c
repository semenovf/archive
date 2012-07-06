/*
 * button.c
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>

typedef void (*_button_painter_t)(CWT_WIDGET_PTR);

BOOL _cwt_button_key_press_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	UINT16 key = CWT_KBD_KEY(pevt);
	UINT16 mods = CWT_KBD_MODS(pevt);

	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	if( CWT_KBD_NO_MODS(mods) ) {
		switch( CWT_KBD_KEY_SCAN(key) ) {
			case CWT_Key_Enter:
			case CWT_Key_Space:
				if( CWT_CAST(widget, CWT_BUTTON_PTR)->clicked )
					CWT_CAST(widget, CWT_BUTTON_PTR)->clicked(widget);
				return TRUE;
			default:
				break;
		}
	}
	return FALSE;
}


BOOL _cwt_button_mouse_up_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	if( CWT_MOUSE_LEFT_BUTTON_EVENT(pevt) ) {
		if( CWT_CAST(widget, CWT_BUTTON_PTR)->clicked ) {
			CWT_CAST(widget, CWT_BUTTON_PTR)->clicked(widget);
		}
	}
	return TRUE;
}

static void _delete_content(CWT_BUTTON_PTR button)
{
	if( button->content ) {
		if( button->type == CWT_BUTTON_TEXT ) {
			_cwt_destroy_textblk(CWT_CAST(button->content, CWT_TEXTBLK_PTR));
			JQ_FREE(button->content);
		}
	}
	button->content = NULL;
}

void _cwt_init_button(CWT_WIDGET_PTR widget)
{
	CWT_BUTTON_PTR button = CWT_CAST(widget, CWT_BUTTON_PTR);

	JQ_ASSERT(widget);

	widget->mouse_up_event = _cwt_button_mouse_up_event;
	widget->key_press_event = _cwt_button_key_press_event;
	widget->focus_policy = CWT_FP_STRONG;
	button->type = CWT_BUTTON_TEXT;
	button->content = JQ_MALLOC(CWT_TEXTBLK);
	_cwt_init_textblk(CWT_CAST(button->content, CWT_TEXTBLK_PTR));
	button->clicked = NULL;
}

void _cwt_destroy_button(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	_delete_content(CWT_CAST(widget, CWT_BUTTON_PTR));
}


CWT_WIDGET_PTR _cwt_clone_button(CWT_WIDGET_PTR orig, CWT_WIDGET_PTR clone)
{
	CWT_BUTTON_PTR orig_button  = CWT_CAST(orig, CWT_BUTTON_PTR);
	CWT_BUTTON_PTR clone_button  = CWT_CAST(clone, CWT_BUTTON_PTR);

	JQ_ASSERT(orig_button->type == clone_button->type);

	switch( orig_button->type ) {
		case CWT_BUTTON_TEXT:
			_cwt_clone_textblk((CWT_TEXTBLK_PTR)orig_button->content, (CWT_TEXTBLK_PTR)clone_button->content);
			break;
		case CWT_BUTTON_CANVAS:
			cwt_button_set_painter(clone, (_button_painter_t)orig_button->content);
			break;
	}

	return clone;
}

void _cwt_size_min_button(CWT_WIDGET_PTR widget, CWT_SIZE *sz)
{
	CWT_BUTTON_PTR button = (CWT_BUTTON_PTR)widget;

	JQ_ASSERT(widget);

	switch( button->type ) {
		case CWT_BUTTON_TEXT:
			CWT_CAST(button->content, CWT_TEXTBLK_PTR)->style = _CWT_WIDGET_STYLE(widget);
			_cwt_size_min_textblk(CWT_CAST(button->content, CWT_TEXTBLK_PTR), sz);
			break;

		case CWT_BUTTON_CANVAS:
			sz->width = 10;
			sz->height = 10;
			break;
	}
}

void _cwt_render_button(CWT_WIDGET_PTR widget)
{
	CWT_BUTTON_PTR button = (CWT_BUTTON_PTR)widget;

	JQ_ASSERT(widget);

	if( button->content ) {

		switch( button->type ) {

			case CWT_BUTTON_TEXT: {
				CWT_VIEWPORT vp;
				CWT_SIZE sz;

				_cwt_get_viewport(&vp);

				CWT_CAST(button->content, CWT_TEXTBLK_PTR)->style = _CWT_WIDGET_STYLE(widget);
				sz.width = vp.width;
				sz.height = vp.height;

				_cwt_render_textblk(CWT_CAST(button->content, CWT_TEXTBLK_PTR), &sz);
				}
				break;

			case CWT_BUTTON_CANVAS:
				((_button_painter_t)button->content)(widget);
				break;
		}
	}
}

void cwt_button_set_text(CWT_WIDGET_PTR widget, const CHAR *text)
{
	CWT_BUTTON_PTR button = (CWT_BUTTON_PTR)widget;

	if( button->type != CWT_BUTTON_TEXT ) {
		_delete_content(button);
		button->type = CWT_BUTTON_TEXT;
		button->content = JQ_MALLOC(CWT_TEXTBLK);
		_cwt_init_textblk(CWT_CAST(button->content, CWT_TEXTBLK_PTR));
	}

	_cwt_textblk_set_text(CWT_CAST(button->content, CWT_TEXTBLK_PTR), text);
	cwt_widget_update(widget);
}


void cwt_button_set_painter(CWT_WIDGET_PTR widget, void (*painter)(CWT_WIDGET_PTR))
{
	CWT_BUTTON_PTR button = (CWT_BUTTON_PTR)widget;

	_delete_content(button);

	button->type = CWT_BUTTON_CANVAS;
	button->content = painter;
}


CHAR* cwt_button_text(CWT_WIDGET_PTR widget)
{
	CWT_BUTTON_PTR button = (CWT_BUTTON_PTR)widget;

	JQ_ASSERT(widget);

	if( button->type == CWT_BUTTON_TEXT ) {
		return strlist_cat_delim(CWT_CAST(button->content, CWT_TEXTBLK_PTR)->lines, "\n");
	}

	return NULL;
}

void* cwt_button_painter(CWT_WIDGET_PTR widget)
{
	return CWT_CAST(widget, CWT_BUTTON_PTR)->content;
}
