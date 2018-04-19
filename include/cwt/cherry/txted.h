/*
 * txted.h
 *
 *  Created on: 12.05.2012
 *      Author: user
 */

#ifndef _CWT_TXTED_H_
#define _CWT_TXTED_H_

#include <cwt/dlist.h>

enum CwtCursorMoveMode
{
	  CwtCursor_MoveAnchor	/* Moves the anchor to the same position as the cursor itself. */
	, CwtCursor_KeepAnchor	/* Keeps the anchor where it is. */
};
typedef enum CwtCursorMoveMode CwtCursorMoveMode;

enum CwtCursorMoveOp {
	  CwtCursor_NoMove              /* Keep the cursor where it is */
	, CwtCursor_Start	            /* Move to the start of the document. */
	, CwtCursor_StartOfLine	        /* Move to the start of the current line. */
	, CwtCursor_StartOfBlock	    /* Move to the start of the current block. */
	, CwtCursor_StartOfWord	        /* Move to the start of the current word. */
	, CwtCursor_PreviousBlock       /* Move to the start of the previous block. */
	, CwtCursor_PreviousCharacter	/* Move to the previous character. */
	, CwtCursor_PreviousWord	    /* Move to the beginning of the previous word. */
	, CwtCursor_Up	                /* Move up one line. */
	, CwtCursor_Left	            /* Move left one character. */
	, CwtCursor_WordLeft	        /* Move left one word. */
	, CwtCursor_End	                /* Move to the end of the document. */
	, CwtCursor_EndOfLine	        /* Move to the end of the current line. */
	, CwtCursor_EndOfWord	        /* Move to the end of the current word. */
	, CwtCursor_EndOfBlock	        /* Move to the end of the current block. */
	, CwtCursor_NextBlock	        /* Move to the beginning of the next block. */
	, CwtCursor_NextCharacter	    /* Move to the next character. */
	, CwtCursor_NextWord	        /* Move to the next word. */
	, CwtCursor_Down	            /* Move down one line. */
	, CwtCursor_Right	            /* Move right one character. */
	, CwtCursor_WordRight	        /* Move right one word. */
	, CwtCursor_NextCell	        /* Move to the beginning of the next table cell inside the current table. If the current cell is the last cell in the row, the cursor will move to the first cell in the next row. */
	, CwtCursor_PreviousCell	    /* Move to the beginning of the previous table cell inside the current table. If the current cell is the first cell in the row, the cursor will move to the last cell in the previous row. */
	, CwtCursor_NextRow	            /* Move to the first new cell of the next row in the current table. */
	, CwtCursor_PreviousRow         /* Move to the last cell of the previous row in the current table. */
};
typedef enum CwtCursorMoveOp CwtCursorMoveOp;

struct CwtTextBlockX
{
	DListNode   m_node;
	size_t      m_count;
	CwtStylePtr m_style;
};
typedef struct CwtTextBlockX  CwtTextBlockX;
typedef struct CwtTextBlockX* CwtTextBlockXPtr;

struct CwtTextDocument
{
	StringBufferPtr m_text;
	DList           m_blocks;
	size_t          m_nblocks;
};
typedef struct CwtTextDocument  CwtTextDocument;
typedef struct CwtTextDocument* CwtTextDocumentPtr;

struct CwtTextEdit
{
	CwtWidget          m_base;
	CwtTextDocumentPtr m_doc;
	size_t             m_cursor;
};
typedef struct CwtTextEdit  CwtTextEdit;
typedef struct CwtTextEdit* CwtTextEditPtr;


EXTERN_C_BEGIN

void cwt_textedit_move_cursor(CwtWidgetPtr widget, CwtCursorMoveOp op, CwtCursorMoveMode mode);
void cwt_textedit_insert_text(CwtWidgetPtr widget, const CWT_CHAR *text);
void cwt_textedit_set_text(CwtWidgetPtr widget, const CWT_CHAR *text);
void cwt_textedit_set_plaintext(CwtWidgetPtr widget, const CWT_CHAR *text);
CwtTextDocumentPtr cwt_textedit_textdoc(CwtWidgetPtr widget);

EXTERN_C_END

#include <jq/cherry/txted_p.h>

#endif /* _CWT_TXTED_H_ */
