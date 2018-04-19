/*
 * textview.c
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>
#include <cwt/strutils.h>

/*static BOOL _textview_is_bottom_overflow(CWT_PTEXTVIEW tv);*/


BOOL _cwt_textview_key_press_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	CWT_TEXTVIEW_PTR tv = CWT_CAST(widget, CWT_TEXTVIEW_PTR);
	UINT16 key = CWT_KBD_KEY(pevt);
	UINT16 mods = CWT_KBD_MODS(pevt);

	if( CWT_KBD_NO_MODS(mods) ) {
		switch( CWT_KBD_KEY_SCAN(key) ) {
			case CWT_Key_Down:
				if( tv->textblock->top_line->next && tv->textblock->bottom_overflow ) {
					tv->textblock->top_line = tv->textblock->top_line->next;
					cwt_widget_update(widget);
				}
				return TRUE;

			case CWT_Key_Up:
				if( tv->textblock->top_line->prev ) {
					tv->textblock->top_line = tv->textblock->top_line->prev;
					cwt_widget_update(widget);
				}
				return TRUE;

			case CWT_Key_Home:
				if( tv->textblock->top_line != tv->textblock->lines->strings.first )
					cwt_textview_scroll_top(widget);
				return TRUE;

			case CWT_Key_End:
				if( tv->textblock->bottom_overflow )
					cwt_textview_scroll_bottom(widget);
				return TRUE;

				/*
			case CWT_Key_PgUp:
				_cwt_input_move_cursor(w, _CWT_MOVE_CURSOR_BEGIN, 0);
				cwt_wupdate(w);cwt_wrepaint(w);
				return TRUE;

			case CWT_Key_PgDn:
				_cwt_input_move_cursor(w, _CWT_MOVE_CURSOR_END, 0);
				cwt_wupdate(w);cwt_wrepaint(w);
				return TRUE;
				*/

			default:
				break;
		}
	}

	return FALSE;
}



void _cwt_init_textview(CWT_WIDGET_PTR widget)
{
	CWT_TEXTVIEW_PTR tv = CWT_CAST(widget, CWT_TEXTVIEW_PTR);

	tv->textblock = JQ_MALLOC(CWT_TEXTBLK);
	_cwt_init_textblk(tv->textblock);
	tv->textblock->style = _CWT_WIDGET_STYLE(widget);
	widget->focus_policy = CWT_FP_STRONG;
	widget->key_press_event = _cwt_textview_key_press_event;
}


void _cwt_destroy_textview(CWT_WIDGET_PTR widget)
{
	CWT_TEXTVIEW_PTR tv = CWT_CAST(widget, CWT_TEXTVIEW_PTR);

	JQ_ASSERT(widget);
	JQ_ASSERT(tv->textblock);

	_cwt_destroy_textblk(tv->textblock);
	JQ_FREE(tv->textblock);

}

CWT_WIDGET_PTR _cwt_clone_textview(CWT_WIDGET_PTR orig, CWT_WIDGET_PTR clone)
{
	CWT_TEXTVIEW_PTR orig_tv = CWT_CAST(orig, CWT_TEXTVIEW_PTR);
	CWT_TEXTVIEW_PTR clone_tv = CWT_CAST(clone, CWT_TEXTVIEW_PTR);
	JQ_ASSERT(orig_tv);
	JQ_ASSERT(clone_tv);
	_cwt_clone_textblk(orig_tv->textblock, clone_tv->textblock);
	return clone;
}


void _cwt_render_textview(CWT_WIDGET_PTR widget)
{
	CWT_TEXTVIEW_PTR tv = CWT_CAST(widget, CWT_TEXTVIEW_PTR);
	CWT_VIEWPORT vp;
	CWT_SIZE sz;

	JQ_ASSERT(widget);

	_cwt_get_viewport(&vp);
	tv->textblock->style = _CWT_WIDGET_STYLE(widget);
	sz.width = vp.width;
	sz.height = vp.height;
	_cwt_render_textblk(tv->textblock, &sz);
}

void _cwt_size_min_textview(CWT_WIDGET_PTR widget, CWT_SIZE *sz)
{
	CWT_TEXTVIEW_PTR tv = CWT_CAST(widget, CWT_TEXTVIEW_PTR);
	JQ_ASSERT(widget);
	tv->textblock->style = _CWT_WIDGET_STYLE(widget);
	_cwt_size_min_textblk(tv->textblock, sz);
}


void cwt_textview_clear(CWT_WIDGET_PTR widget)
{
	CWT_TEXTVIEW_PTR tv = CWT_CAST(widget, CWT_TEXTVIEW_PTR);
	JQ_ASSERT(widget);
	_cwt_textblk_clear(tv->textblock);
	cwt_widget_update(widget);
}


void cwt_textview_prepend_text(CWT_WIDGET_PTR widget, const CWT_CHAR *text)
{
	JQ_ASSERT(widget);
	_cwt_textblk_prepend_text(CWT_CAST(widget, CWT_TEXTVIEW_PTR)->textblock, text);
	cwt_widget_update(widget);
}

void cwt_textview_append_text(CWT_WIDGET_PTR widget, const CWT_CHAR *text)
{
	JQ_ASSERT(widget);
	_cwt_textblk_append_text(CWT_CAST(widget, CWT_TEXTVIEW_PTR)->textblock, text);
	cwt_widget_update(widget);
}


void cwt_textview_set_text(CWT_WIDGET_PTR widget, const CWT_CHAR *text)
{
	JQ_ASSERT(widget);
	_cwt_textblk_set_text(CWT_CAST(widget, CWT_TEXTVIEW_PTR)->textblock, text);
	cwt_widget_update(widget);
}


void cwt_textview_remove_first(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	_cwt_textblk_remove_first(CWT_CAST(widget, CWT_TEXTVIEW_PTR)->textblock);
	cwt_widget_update(widget);
}



void cwt_textview_remove_last(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	_cwt_textblk_remove_last(CWT_CAST(widget, CWT_TEXTVIEW_PTR)->textblock);
	cwt_widget_update(widget);
}


void cwt_textview_scroll_top(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	_cwt_textblk_scroll_top(CWT_CAST(widget, CWT_TEXTVIEW_PTR)->textblock);
	cwt_widget_update(widget);
}

void cwt_textview_scroll_bottom(CWT_WIDGET_PTR widget)
{
	CWT_SIZE sz;
	CWT_TEXTVIEW_PTR tv = CWT_CAST(widget, CWT_TEXTVIEW_PTR);

	JQ_ASSERT(widget);

	sz.width = widget->geometry.width;
	sz.height = widget->geometry.height;
	tv->textblock->style = _CWT_WIDGET_STYLE(widget);
	_cwt_textblk_scroll_bottom(tv->textblock, &sz);
	cwt_widget_update(widget);
}

int cwt_textview_nlines(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	return _cwt_textblk_nlines(CWT_CAST(widget, CWT_TEXTVIEW_PTR)->textblock);
}
