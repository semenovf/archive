/*
 * textblk.c
 *
 *  Created on: Mar 18, 2012
 *      Author: wladt
 */


#include <cwt/cherry/cherry.h>
#include <cwt/strutils.h>
#include <cwt/strlist.h>
#include <cwt/logger.h>

void _cwt_init_textblk(CwtTextBlkPtr tb)
{
	JQ_ASSERT(tb);
	tb->lines = cwtNewStringList();
	tb->top_line = tb->lines->strings.first;
	tb->bottom_overflow = FALSE;
	tb->style = NULL;
	tb->upwards = FALSE;
}


void _cwt_destroy_textblk(CwtTextBlkPtr tb)
{
	JQ_ASSERT(tb);
	strlist_delete(tb->lines);
	tb->lines = NULL;
	/*printf_trace("_cwt_destroy_textblock: %p", tb);*/
}

CwtTextBlkPtr _cwt_clone_textblk(CwtTextBlkPtr orig, CwtTextBlkPtr clone)
{
	JQ_ASSERT(orig);
	JQ_ASSERT(clone);
	clone->lines = strlist_clone(orig->lines);
	clone->top_line = clone->lines->strings.first;
	clone->bottom_overflow = FALSE;
	clone->style = orig->style;
	return clone;
}


void _cwt_textblk_clear(CwtTextBlkPtr tb)
{
	JQ_ASSERT(tb);
	strlist_clear(tb->lines);
	tb->top_line = tb->lines->strings.first;
}

static void _cwt_textblock_insert_helper(CwtTextBlkPtr tb,
	void (*insert_func)(StringListPtr, StringListElemPtr, const CWT_CHAR*)
	, StringListElemPtr elem, const CWT_CHAR *text)
{
	const CWT_CHAR *ptr, *ptr_begin, *ptr_end;
	int n;

	JQ_ASSERT(tb);

	n = strlen(text);

	ptr = text;
	ptr_begin = ptr;
	ptr_end = ptr + n + 1;

	while( ptr < ptr_end ) {
		if( *ptr == '\n' || *ptr == '\x0' ) {
			CWT_CHAR *tmp = jq_strndup(ptr_begin, (size_t)(ptr - ptr_begin));

			insert_func(tb->lines, elem, tmp);
			if( insert_func == cwtStringListInsertAfter ) {
				if( !elem )
					elem = tb->lines->strings.last;
				else
					elem = elem->next;
			}
			JQ_FREE(tmp);

			ptr_begin = ptr + 1;
		}
		ptr++;
	}

	/* tail */
	if( ptr_begin < ptr ) {
		CWT_CHAR *tmp = jq_strndup(ptr_begin, (size_t)(ptr - ptr_begin));
		insert_func(tb->lines, elem, tmp);
		JQ_FREE(tmp);
	}

	if( !tb->top_line )
		tb->top_line = tb->lines->strings.first;
}

void _cwt_textblk_prepend_text(CwtTextBlkPtr tb, const CWT_CHAR *text)
{
	JQ_ASSERT(tb);
	_cwt_textblock_insert_helper(tb, cwtStringListInsertBefore, tb->lines->strings.first, text);
}

void _cwt_textblk_append_text(CwtTextBlkPtr tb, const CWT_CHAR *text)
{
	JQ_ASSERT(tb);
	_cwt_textblock_insert_helper(tb, cwtStringListInsertAfter, tb->lines->strings.last, text);
}

void _cwt_textblk_set_text(CwtTextBlkPtr tb, const CWT_CHAR *text)
{
	JQ_ASSERT(tb);
	_cwt_textblk_clear(tb);
	_cwt_textblk_append_text(tb, text);
}

void _cwt_textblk_remove_first(CwtTextBlkPtr tb)
{
	JQ_ASSERT(tb);

	if( tb->lines->count > 0 ) {
		if( tb->top_line == tb->lines->strings.first )
			tb->top_line = tb->lines->strings.first->next;

		strlist_remove_first(tb->lines);
	}
}

void _cwt_textblk_remove_last(CwtTextBlkPtr tb)
{
	JQ_ASSERT(tb);

	if( tb->lines->count > 0 ) {
		if( tb->top_line == tb->lines->strings.last )
			tb->top_line = tb->lines->strings.last->prev;

		strlist_remove_last(tb->lines);
	}
}


/* TODO необходимо доработать алгоритм с возможностью расположения по вертикали
 * блока текста с произвольным количество строк.
 * В настоящий момент правильно располагается по вертикали только блок,
 * состящий из одной строки.
 * */
void _cwt_render_textblk(CwtTextBlkPtr tb, CWT_SIZE *sz)
{
	JQ_ASSERT(tb);

	if( tb->lines->count == 1 ) {
		CWT_RECT bounds;
		CWT_CHAR *text;

		bounds.left = 0;
		bounds.top = 0;
		bounds.width = sz->width;
		bounds.height = sz->height;

		text = strlist_at(tb->lines, 0);
		if( text ) {
			_cwt_outtext_box(text
				, &bounds
				, &tb->style->font
				, tb->style->align
				, tb->style->color
				, tb->style->bgcolor);
		}
	} else if( tb->lines->count > 0 ) {
		StringListIterator it;
		int top;

		JQ_ASSERT(tb->style);

		tb->bottom_overflow = FALSE;

		if( !tb->upwards ) {
			strlist_begin_from(tb->lines, tb->top_line, &it);
			top = 0;
		} else {
			strlist_rbegin(tb->lines, &it);
			top = sz->height;
			tb->bottom_overflow = FALSE;
		}

		while( strlist_has_more(&it) ) {
			const CWT_CHAR* text;
			StringListElemPtr node = strlist_node(&it);
			CWT_RECT line_rect;
			int text_height;
			int line_height;

			text = strlist_next(&it);

			text_height = _cwt_text_height(&tb->style->font, text);
			line_height = (tb->style->line_height * text_height)/100 ;

			if( !tb->upwards ) {
				if( top + text_height > sz->height ) {
					tb->bottom_overflow = TRUE;
					break;
				}
			} else {
				if( top - text_height < 0 ) {
					break;
				}
				top -= line_height;
				tb->top_line = node;
			}

			line_rect.top    = top;
			line_rect.left   = 0;
			line_rect.width  = sz->width;
			line_rect.height = line_height;

			_cwt_outtext_box(text
				, &line_rect
				, &tb->style->font
				, (tb->style->align & 0x0F) | CWT_VALIGN_CENTER
				, tb->style->color
				, tb->style->bgcolor);

			if( !tb->upwards )
				top += line_height;
		}
	}

	tb->upwards = FALSE;
}


void _cwt_size_min_textblk(CwtTextBlkPtr tb, CWT_SIZE *sz)
{
	JQ_ASSERT(tb);
	sz->width = sz->height = 0;

	if( tb->lines->count > 0 ) {
		StringListIterator it;

		JQ_ASSERT(tb->style);

		strlist_begin(tb->lines, &it);

		while( strlist_has_more(&it) ) {
			const CWT_CHAR* text = strlist_next(&it);
			CWT_SIZE text_sz;

			_cwt_text_size(&tb->style->font, text, &text_sz);
			sz->height += tb->style->line_height/100 * text_sz.height;
			sz->width = JQ_MAX(sz->width, text_sz.width);
		}
	}
}


void _cwt_textblk_scroll_top(CwtTextBlkPtr tb)
{
	tb->top_line = tb->lines->strings.first;
	tb->upwards = FALSE;
}

/*TODO remove sz argument */
void _cwt_textblk_scroll_bottom(CwtTextBlkPtr tb, CWT_SIZE *sz)
{
	JQ_UNUSED(sz);
	tb->upwards = TRUE;
	tb->top_line = tb->lines->strings.last;
}


int _cwt_textblk_nlines(CwtTextBlkPtr tb)
{
	return tb->lines->count;
}
