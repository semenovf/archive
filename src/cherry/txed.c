/*
 * rview.c
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>
#include <cwt/strutils.h>

BOOL _cwt_textedit_key_press_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt)
{
	CwtTextEditPtr tedit = CWT_CAST(widget, CwtTextEditPtr);
	UINT16 key = CWT_KBD_KEY(pevt);
	UINT16 mods = CWT_KBD_MODS(pevt);

    JQ_UNUSED(tedit);

	if( CWT_KBD_NO_MODS(mods) ) {
		switch( CWT_KBD_KEY_SCAN(key) ) {
			case CWT_Key_Down:
/*
				if( tv->textblock->top_line->next && tv->textblock->bottom_overflow ) {
					tv->textblock->top_line = tv->textblock->top_line->next;
					cwt_widget_update(widget);
				}
*/
				return TRUE;

			case CWT_Key_Up:
/*
				if( tv->textblock->top_line->prev ) {
					tv->textblock->top_line = tv->textblock->top_line->prev;
					cwt_widget_update(widget);
				}
*/
				return TRUE;

			case CWT_Key_Home:
/*
				if( tv->textblock->top_line != tv->textblock->lines->strings.first )
					cwt_textview_scroll_top(widget);
*/
				return TRUE;

			case CWT_Key_End:
/*
				if( tv->textblock->bottom_overflow )
					cwt_textview_scroll_bottom(widget);
*/
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



void _cwt_init_textedit(CwtWidgetPtr widget)
{
	CwtTextEditPtr tedit = CWT_CAST(widget, CwtTextEditPtr);

	widget->focus_policy = CWT_FP_STRONG;
	widget->key_press_event = _cwt_textedit_key_press_event;
	tedit->m_doc = NULL;
	tedit->m_cursor = 0;
}


void _cwt_destroy_textedit(CwtWidgetPtr widget)
{
	CwtTextEditPtr tedit = CWT_CAST(widget, CwtTextEditPtr);
	_cwt_delete_textdoc(tedit->m_doc);
}



void _cwt_render_textedit(CwtWidgetPtr widget)
{
	CwtTextEditPtr tedit = CWT_CAST(widget, CwtTextEditPtr);
	CwtTextDocumentPtr doc;
	DListIterator it;
/*	size_t off = 0;*/

	JQ_ASSERT(widget);
	JQ_ASSERT(tedit->m_doc);

	doc = tedit->m_doc;

	dlist_begin(&doc->m_blocks, &it);
	while( dlist_has_more(&it) ) {
		CwtTextBlockXPtr block = dlist_next(&it);
		CwtStylePtr style = block->m_style;

		if( !style )
			style = _CWT_WIDGET_STYLE(widget);

/*
		off += block->m_count;
		if( off > pos )
			return block;
*/
	}

#ifdef __COMMENT__


	if( tb->lines->count == 1 ) {
		CWT_RECT bounds;
		CHAR *text;

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
			const CHAR* text;
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
#endif
}

/* TODO supported only:
 * 	CwtCursor_NoMove
 * 	CwtCursor_Start
 * 	CwtCursor_Start
 * */
void cwt_textedit_move_cursor(CwtWidgetPtr widget, CwtCursorMoveOp op, CwtCursorMoveMode mode)
{
	CwtTextEditPtr tedit = CWT_CAST(widget, CwtTextEditPtr);
	CwtTextDocumentPtr doc;

	JQ_ASSERT(widget);
	JQ_UNUSED(mode);

	doc = tedit->m_doc;

	switch( op ) {
	case CwtCursor_NoMove:
		break;
	case CwtCursor_Start:	            /* Move to the start of the document. */
		tedit->m_cursor = 0;
		break;
	case CwtCursor_StartOfLine:	        /* Move to the start of the current line. */
	case CwtCursor_StartOfBlock:	    /* Move to the start of the current block. */
	case CwtCursor_StartOfWord:	        /* Move to the start of the current word. */
	case CwtCursor_PreviousBlock:       /* Move to the start of the previous block. */
	case CwtCursor_PreviousCharacter:	/* Move to the previous character. */
	case CwtCursor_PreviousWord:	    /* Move to the beginning of the previous word. */
	case CwtCursor_Up:	                /* Move up one line. */
	case CwtCursor_Left:	            /* Move left one character. */
	case CwtCursor_WordLeft:	        /* Move left one word. */
		break;
	case CwtCursor_End:	                /* Move to the end of the document. */
		tedit->m_cursor = strbuf_size(doc->m_text);
		break;
	case CwtCursor_EndOfLine:	        /* Move to the end of the current line. */
	case CwtCursor_EndOfWord:	        /* Move to the end of the current word. */
	case CwtCursor_EndOfBlock:	        /* Move to the end of the current block. */
	case CwtCursor_NextBlock:	        /* Move to the beginning of the next block. */
	case CwtCursor_NextCharacter:	    /* Move to the next character. */
	case CwtCursor_NextWord:	        /* Move to the next word. */
	case CwtCursor_Down:	            /* Move down one line. */
	case CwtCursor_Right:	            /* Move right one character. */
	case CwtCursor_WordRight:	        /* Move right one word. */
	case CwtCursor_NextCell:	        /* Move to the beginning of the next table cell inside the current table. If the current cell is the last cell in the row, the cursor will move to the first cell in the next row. */
	case CwtCursor_PreviousCell:	    /* Move to the beginning of the previous table cell inside the current table. If the current cell is the first cell in the row, the cursor will move to the last cell in the previous row. */
	case CwtCursor_NextRow:	            /* Move to the first new cell of the next row in the current table. */
	case CwtCursor_PreviousRow:         /* Move to the last cell of the previous row in the current table. */
		break;
	}
}


/**
 * @brief Inserts text at the current cursor position
 *
 * @param widget
 * @param text
 */
void cwt_textedit_insert_text(CwtWidgetPtr widget, const CHAR *text)
{
	CwtTextEditPtr tedit = CWT_CAST(widget, CwtTextEditPtr);

	JQ_ASSERT(widget);

	if( !text )
		return;

	cwt_textedit_move_cursor(widget, CwtCursor_Start, CwtCursor_MoveAnchor);

	if( !tedit->m_doc ) {
		tedit->m_doc = _cwt_new_textdoc();
	}
	/*_cwt_textdoc_insert_plaintext(tedit->m_doc, text);*/
}


void cwt_textedit_set_text(CwtWidgetPtr widget, const CHAR *text)
{
	cwt_textedit_set_plaintext(widget, text);
}

void cwt_textedit_set_plaintext(CwtWidgetPtr widget, const CHAR *text)
{
	CwtTextEditPtr tedit = CWT_CAST(widget, CwtTextEditPtr);
    JQ_UNUSED(tedit);

	JQ_ASSERT(widget);

	if( !text )
		return;

	cwt_textedit_move_cursor(widget, CwtCursor_Start, CwtCursor_MoveAnchor);
	cwt_textedit_insert_text(widget, text);
}

CwtTextDocumentPtr cwt_textedit_textdoc(CwtWidgetPtr widget)
{
	JQ_ASSERT(widget);
	return CWT_CAST(widget, CwtTextEditPtr)->m_doc;
}
