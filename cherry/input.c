/*
 * input.c
 *
 *  Created on: 13.01.2012
 *      Author: wladt
 */


#include <string.h>
#include <cwt/strutils.h>
#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>

enum _CWT_MOVE_CURSOR {
	  _CWT_MOVE_CURSOR_CUR
	, _CWT_MOVE_CURSOR_BEGIN
	, _CWT_MOVE_CURSOR_END
};
typedef enum _CWT_MOVE_CURSOR _CWT_MOVE_CURSOR;

/* Input private methods */
static void _cwt_input_insert_char(CWT_WIDGET_PTR widget, CHAR ch);
/*static void _cwt_input_insert(CWT_WIDGET_PTR widget, const CHAR *chars, size_t nchars);*/
static void _cwt_input_remove_char(CWT_WIDGET_PTR widget);
static void _cwt_input_bs_char(CWT_WIDGET_PTR widget);
static void _cwt_input_move_cursor(CWT_WIDGET_PTR widget, _CWT_MOVE_CURSOR from, int inc);


/*
static BOOL _focus_out_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	CWT_INPUT_PTR input = CWT_CAST(widget, CWT_INPUT_PTR);
	BOOL ok = TRUE;

	JQ_UNUSED(pevt);

	if( input->validate )
		ok = input->validate(strbuf_cstr(input->strbuf));

	return ok;
}
*/


BOOL _cwt_input_key_press_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	UINT16 key = CWT_KBD_KEY(pevt);
	UINT16 mods = CWT_KBD_MODS(pevt);

/*
	 if( _cwt_widget_key_press_event(widget, pevt) )
		 return TRUE;
*/
	if( CWT_KBD_TAB(pevt) || CWT_KBD_SHIFT_TAB(pevt) )
		return FALSE;

	if( CWT_KBD_NO_MODS(mods) ) {
		switch( CWT_KBD_KEY_SCAN(key) ) {
			case CWT_Key_Backspace:
				_cwt_input_bs_char(widget);
				return TRUE;

			case CWT_Key_Delete:
				_cwt_input_remove_char(widget);
				return TRUE;

			case CWT_Key_Left:
				_cwt_input_move_cursor(widget, _CWT_MOVE_CURSOR_CUR, -1);
				return TRUE;

			case CWT_Key_Right:
				_cwt_input_move_cursor(widget, _CWT_MOVE_CURSOR_CUR, 1);
				return TRUE;

			case CWT_Key_Home:
				_cwt_input_move_cursor(widget, _CWT_MOVE_CURSOR_BEGIN, 0);
				return TRUE;

			case CWT_Key_End:
				_cwt_input_move_cursor(widget, _CWT_MOVE_CURSOR_END, 0);
				return TRUE;

			default:
				break;
		}
	}


	if( (CWT_KBD_NO_MODS(mods) || CWT_KBD_SHIFT(mods)) && (CWT_KBD_KEY_ASCII(key) != 0) ) {
		_cwt_input_insert_char(widget, CWT_KBD_KEY_ASCII(key));
		return TRUE;
	}

	return FALSE;
}

void _cwt_init_input(CWT_WIDGET_PTR widget)
{
	CWT_INPUT_PTR input = CWT_CAST(widget, CWT_INPUT_PTR);
	input->strbuf    = strbuf_new(16, 0);
	input->start_pos = 0;
	input->pos       = 0;
	input->validate  = NULL;
	input->limit     = JQ_INT_MAX;
	widget->focus_policy = CWT_FP_STRONG;
	widget->key_press_event = _cwt_input_key_press_event;
	/*widget->focus_out_event = _focus_out_event;*/
}

void _cwt_destroy_input(CWT_WIDGET_PTR widget)
{
	CWT_INPUT_PTR input = (CWT_INPUT_PTR)widget;
	JQ_ASSERT(widget);
	strbuf_delete(input->strbuf);
}


CWT_WIDGET_PTR _cwt_clone_input(CWT_WIDGET_PTR orig, CWT_WIDGET_PTR clone)
{
	CWT_INPUT_PTR orig_input = CWT_CAST(orig, CWT_INPUT_PTR);
	CWT_INPUT_PTR clone_input = CWT_CAST(clone, CWT_INPUT_PTR);
	clone_input->strbuf = strbuf_clone(orig_input->strbuf);
	clone_input->start_pos = 0;
	clone_input->pos       = 0;
	clone_input->validate  = orig_input->validate;
	clone_input->limit     = orig_input->limit;
	return clone;
}


void _cwt_size_min_input(CWT_WIDGET_PTR widget, CWT_SIZE *sz)
{
	CWT_INPUT_PTR input = CWT_CAST(widget, CWT_INPUT_PTR);
	JQ_ASSERT(widget);
	_cwt_text_size(&_CWT_WIDGET_STYLE(widget)->font, strbuf_cstr(input->strbuf), sz);
}


void cwt_input_set_text(CWT_WIDGET_PTR widget, const CHAR *text)
{
	CWT_INPUT_PTR input = (CWT_INPUT_PTR)widget;

	JQ_ASSERT(widget->wtype == CWT_WT_INPUT);

	strbuf_clear(input->strbuf);

	if( strlen(text) <= input->limit ) {
		strbuf_append(input->strbuf, text);
	} else {
		CHAR *tmp = jq_strndup(text, input->limit);
		strbuf_append(input->strbuf, tmp);
		JQ_FREE(tmp);
	}
	input->pos = strbuf_length(input->strbuf);
	cwt_widget_update(widget);
}

void cwt_input_set_validator(CWT_WIDGET_PTR widget, BOOL (*validator)(const CHAR*))
{
	JQ_ASSERT(widget);
	CWT_CAST(widget, CWT_INPUT_PTR)->validate = validator;
}


const CHAR* cwt_input_text(CWT_WIDGET_PTR widget)
{
	CWT_INPUT_PTR input = (CWT_INPUT_PTR)widget;
	JQ_ASSERT(widget);
	return strbuf_cstr(input->strbuf);
}

void cwt_input_set_limit(CWT_WIDGET_PTR widget, int limit)
{
	CWT_INPUT_PTR input = (CWT_INPUT_PTR)widget;
	JQ_ASSERT(widget);
	if( limit < 0 )
		limit = JQ_INT_MAX;
	input->limit = limit;
}


void _cwt_input_insert_char(CWT_WIDGET_PTR widget, CHAR ch)
{
	CWT_INPUT_PTR input = (CWT_INPUT_PTR)widget;
	if( strbuf_size(input->strbuf) < input->limit ) {
		strbuf_insert_char(input->strbuf, ch, input->pos);
		input->pos++;
		cwt_widget_update(widget);
	}
}


/**
 * Inserts characters into buffer
 *
 * @param widget
 * @param chars
 * @param nchars
 */
/*
void _cwt_input_insert(CWT_WIDGET_PTR widget, const CHAR *chars, size_t nchars)
{
	CWT_INPUT_PTR input = (CWT_INPUT_PTR)widget;
	strbuf_insert(input->strbuf, chars, nchars, input->pos);
	input->pos += nchars;
}
*/


void _cwt_input_remove_char(CWT_WIDGET_PTR widget)
{
	CWT_INPUT_PTR input = (CWT_INPUT_PTR)widget;

	if( !strbuf_length(input->strbuf) )
		return;

	strbuf_remove_char(input->strbuf, input->pos);
	cwt_widget_update(widget);
}


/**
 * Backspace (remove previous to input->pos) character
 *
 * @param widget
 */
void _cwt_input_bs_char(CWT_WIDGET_PTR widget)
{
	CWT_INPUT_PTR input = (CWT_INPUT_PTR)widget;

	if( input->pos < 1 )
		return;

	input->pos--;
	_cwt_input_remove_char(widget);
}


void _cwt_input_move_cursor(CWT_WIDGET_PTR widget, _CWT_MOVE_CURSOR from, int inc)
{
	CWT_INPUT_PTR input = (CWT_INPUT_PTR)widget;
	switch(from) {
		case _CWT_MOVE_CURSOR_CUR:
			input->pos += inc;
			break;
		case _CWT_MOVE_CURSOR_BEGIN:
			input->pos = inc;
			break;
		case _CWT_MOVE_CURSOR_END:
			input->pos = strbuf_length(input->strbuf) + inc;
			break;
	}

	if( input->pos < 0 )
		input->pos = 0;
	if( input->pos >= strbuf_length(input->strbuf) )
		input->pos = strbuf_length(input->strbuf);

	cwt_widget_update(widget);
}

static void _cwt_input_render_cursor(int left, int top, int height)
{
	int saved_color = _cwt_get_color();

	_cwt_set_color(CWT_Black);
	_cwt_set_line_style(SOLID_LINE, 1, 1);
	_cwt_line(left, top, left, height);
	_cwt_set_color(saved_color);
}


void _cwt_render_input(CWT_WIDGET_PTR widget)
{
	CWT_INPUT_PTR input = CWT_CAST(widget, CWT_INPUT_PTR);
	CWT_VIEWPORT vp;
	CWT_STYLE_PTR style;
	CWT_RECT text_rect;
	const CHAR* text = strbuf_cstr(input->strbuf);

	_cwt_get_viewport(&vp);

	JQ_ASSERT(widget);

	/*_cwt_text_rect(0, 0, vp.width, vp.height, _CWT_WIDGET_STYLE(widget), text + input->start_pos, &text_rect);*/
	text_rect.left = 0;
	text_rect.top = 0;
	text_rect.width = vp.width;
	text_rect.height = vp.height;

	style = _CWT_WIDGET_STYLE(widget);

	_cwt_outtext_box(text + input->start_pos, &text_rect, &style->font, style->align, style->color, CWT_Transparent);

	if( cwt_widget_is_focused(widget) ) {
		int cursor_left = 0;
		CHAR *left_text;

		left_text = strbuf_substr(input->strbuf, input->start_pos, input->pos);
		if( left_text ) {
			_cwt_text_size_wh(&style->font, left_text, &cursor_left, NULL);
			JQ_FREE(left_text);
		}
		_cwt_input_render_cursor(cursor_left, text_rect.top, text_rect.height);
	}
}

