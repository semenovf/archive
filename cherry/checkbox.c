/*
 * button.c
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>
#include <cwt/logger.h>
#include <cwt/utils.h>

BOOL _cwt_checkbox_key_press_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	UINT16 key = CWT_KBD_KEY(pevt);
	UINT16 mods = CWT_KBD_MODS(pevt);

	if( CWT_KBD_NO_MODS(mods) ) {
		switch( CWT_KBD_KEY_SCAN(key) ) {
			case CWT_Key_Insert:
			case CWT_Key_Space:
				if( CWT_CAST(widget, CWT_BUTTON_PTR)->clicked )
					CWT_CAST(widget, CWT_BUTTON_PTR)->clicked(widget);
				return TRUE;
			default:
				break;
		}
	}
	return FALSE;/*_cwt_button_key_press_event(widget, pevt);*/
}


void _cwt_init_checkbox(CWT_WIDGET_PTR widget)
{
	CWT_CHECKBOX_PTR cb = CWT_CAST(widget, CWT_CHECKBOX_PTR);

	_cwt_init_button(widget);
	widget->key_press_event = _cwt_checkbox_key_press_event;
	CWT_CAST(widget, CWT_BUTTON_PTR)->clicked = cwt_checkbox_toggle;
	cb->checked_style = NULL;
	cb->saved_style = NULL;
	cb->checked = FALSE;
	cb->on_changed = NULL;
}


void _cwt_destroy_checkbox(CWT_WIDGET_PTR widget)
{
	CWT_CHECKBOX_PTR cb = CWT_CAST(widget, CWT_CHECKBOX_PTR);

	if( cb->checked ) {
		cwtSwapPtr((void**)&cb->saved_style, (void**)&widget->styles[CWT_STATE_FOCUS]);
	}

	if( cb->checked_style ) {
		cwt_delete_style(cb->checked_style);
	}
	_cwt_destroy_button(widget);
}


CWT_WIDGET_PTR _cwt_clone_checkbox(CWT_WIDGET_PTR orig, CWT_WIDGET_PTR clone)
{
	_cwt_clone_button(orig, clone);
	CWT_CAST(clone, CWT_CHECKBOX_PTR)->checked = CWT_CAST(orig, CWT_CHECKBOX_PTR)->checked;
	CWT_CAST(clone, CWT_CHECKBOX_PTR)->on_changed = CWT_CAST(orig, CWT_CHECKBOX_PTR)->on_changed;
	return clone;
}

void _cwt_size_min_checkbox(CWT_WIDGET_PTR widget, CWT_SIZE *sz)
{
	_cwt_size_min_button(widget, sz);
}


void _cwt_render_checkbox(CWT_WIDGET_PTR widget)
{
/*	CWT_CHECKBOX_PTR cb = CWT_CAST(widget, CWT_CHECKBOX_PTR);*/
	JQ_ASSERT(widget);
	_cwt_render_button(widget);
}


void cwt_checkbox_set_checked_style(CWT_WIDGET_PTR widget, CWT_STYLE_PTR style)
{
	CWT_CHECKBOX_PTR cb = (CWT_CHECKBOX_PTR)widget;

	if( cb->checked_style ) {
		cwt_delete_style(cb->checked_style);
	}

	cwt_assign_style(&cb->checked_style, style);
	cb->saved_style = cb->checked_style;
}


void cwt_checkbox_set_checked(CWT_WIDGET_PTR widget, BOOL checked)
{
	BOOL changed = FALSE;
	CWT_CHECKBOX_PTR cb = (CWT_CHECKBOX_PTR)widget;

	if( cb->checked != checked ) {
		cwtSwapPtr((void**)&cb->checked_style, (void**)&widget->styles[CWT_STATE_NORMAL]);
		cwtSwapPtr((void**)&cb->saved_style, (void**)&widget->styles[CWT_STATE_FOCUS]);
		changed = TRUE;
	}

	cb->checked = checked;

	if( changed ) {
		if( cb->on_changed )
			cb->on_changed(widget);

		cwt_widget_update(widget);
	}
}

void cwt_checkbox_toggle(CWT_WIDGET_PTR widget)
{
	CWT_CHECKBOX_PTR cb = (CWT_CHECKBOX_PTR)widget;
	if( cb->checked )
		cwt_checkbox_set_checked(widget, FALSE);
	else
		cwt_checkbox_set_checked(widget, TRUE);
}

BOOL cwt_checkbox_checked(CWT_WIDGET_PTR widget)
{
	return CWT_CAST(widget, CWT_CHECKBOX_PTR)->checked;
}

void cwt_checkbox_set_text(CWT_WIDGET_PTR widget, const CHAR *text)
{
	cwt_button_set_text(widget, text);
}

CHAR* cwt_checkbox_text(CWT_WIDGET_PTR widget)
{
	return cwt_button_text(widget);
}
