/*
 * txted_p.c
 *
 *  Created on: 12.05.2012
 *      Author: user
 */


#include <cwt/cherry/cherry.h>


/**
 * TextBlock constructor
 *
 * @return
 */
CwtTextBlockXPtr _cwt_new_textblockx(void)
{
	CwtTextBlockXPtr block;
	block = JQ_MALLOC(CwtTextBlockX);
	block->m_node.data = block;
	block->m_node.next = NULL;
	block->m_node.prev = NULL;
	block->m_count  = 0;
	block->m_style  = NULL;
	return block;
}

/* helper function for destroy text block node in dlist_clear function */
static void _free_textblockx(void *block)
{
	if( block ) {
		if( CWT_CAST(block, CwtTextBlockXPtr)->m_style )
			cwt_delete_style(CWT_CAST(block, CwtTextBlockXPtr)->m_style);
		JQ_FREE(block);
	}
}

/**
 * TextBlock destructor
 *
 * @param block
 */
void _cwt_delete_textblockx(CwtTextBlockXPtr block)
{
	_free_textblockx(block);
}


/**
 * TextDocument constructor
 *
 * @return TextDocument instance
 */
CwtTextDocumentPtr _cwt_new_textdoc(void)
{
	CwtTextDocumentPtr doc;

	doc = JQ_MALLOC(CwtTextDocument);
	doc->m_text = strbuf_new_defaults();
	doc->m_blocks.first = NULL;
	doc->m_blocks.last = NULL;
	doc->m_nblocks = 0;
	return doc;
}


/**
 * TextDocument destructor
 *
 * @param doc TextDocument instance
 */
void _cwt_delete_textdoc(CwtTextDocumentPtr doc)
{
	if( doc ) {
		dlist_clear(&doc->m_blocks, _free_textblockx, NULL);

		if( doc->m_text ) {
			strbuf_delete(doc->m_text);
		}
	}
}


/**
 * @brief Inserts plain text @c text with length @c len into text document @c doc starting from position @c pos
 *
 * @param doc text document instance
 * @param text text
 * @param len text length
 * @param pos insertion position
 */
void _cwt_textdoc_insert_plaintext(CwtTextDocumentPtr doc, const CWT_CHAR *text, size_t len, size_t pos)
{
	JQ_UNUSED(doc);
	JQ_UNUSED(text) ;
	JQ_UNUSED(len);
	JQ_UNUSED(pos);

#ifdef __COMMENT__
	size_t start_off = 0;
	size_t off = 0;
	CwtTextBlockXPtr block_begin;
	CwtTextBlockXPtr block_end;


	JQ_ASSERT(doc);
	JQ_ASSERT(text);

	strbuf_insert(doc->m_text, text, len, pos);

	/* find block_begin and set block_end */
	if( strbuf_size(doc->m_text) > 0 ) {
		if( pos >= strbuf_size(doc->m_text) ) { /* append text into end of document */
			block_begin = _cwt_textdoc_last_block(doc);
			block_end   = NULL;
			start_off = strbuf_size(doc->m_text);
		} else {
			block_begin = _cwt_textdoc_find_block(doc, pos);

			/* insert position is before block begin */
			if( pos == block_begin->m_offset ) {
				block_begin = block_begin->m_node.prev; /* may be NULL - will be first block */

				if( !block_begin ) {
					block_end = _cwt_textdoc_first_block(doc);
				} else {
					block_end = block_begin->m_node.next;
				}
				block_end = block_begin->m_node.next;
			} else { /* insertion is inside block */
				CwtTextBlockXPtr block_tmp_end   = _cwt_new_textblockx();

				block_tmp_end->m_count = ;
				block_tmp_end->m_style = block_begin->m_style
						? cwt_assign_style(&block_tmp_end->m_style, block_begin->m_style)
						: NULL;

				block_begin->m_count = pos - block_begin->m_offset;

				dlist_insert_after(&doc->m_blocks, block_begin, block_tmp_end);

				block_end   = block_tmp_end;
				doc->m_nblocks++;
			}

			start_off = pos;
		}
	}

	/* разбить на блоки, каждый из которых содержит линию текста целиком */
	do {
		CwtTextBlockXPtr block = _cwt_new_textblockx();

		block->m_offset = start_off;
		if( strbuf_find(doc->m_text, '\n', &off) )
			block->m_count = off - start_off;
		else
			block->m_count = strbuf_size(doc->m_text) - start_off;

		dlist_insert_last(&doc->m_blocks, (DListNode*)block);
		doc->m_nblocks++;
		off++;
		start_off = off;
	} while(TRUE);
#endif
}


void _cwt_textdoc_set_plaintext(CwtTextDocumentPtr doc, const CWT_CHAR *text, size_t len)
{
	JQ_ASSERT(doc);
	JQ_ASSERT(text);

	strbuf_clear(doc->m_text);
	strbuf_append(doc->m_text, text);
	_cwt_textdoc_insert_plaintext(doc, text, len, 0);
}


CwtTextBlockXPtr _cwt_textdoc_first_block(const CwtTextDocumentPtr doc)
{
	JQ_ASSERT(doc);
	return (CwtTextBlockXPtr)doc->m_blocks.first;
}

CwtTextBlockXPtr _cwt_textdoc_last_block(const CwtTextDocumentPtr doc)
{
	JQ_ASSERT(doc);
	return (CwtTextBlockXPtr)doc->m_blocks.last;
}

CwtTextBlockXPtr _cwt_textdoc_find_block(const CwtTextDocumentPtr doc, size_t pos)
{
	DListIterator it;
	size_t off = 0;

	JQ_ASSERT(doc);
	dlist_begin(&doc->m_blocks, &it);

	while( dlist_has_more(&it) ) {
		CwtTextBlockXPtr block = dlist_next(&it);
		off += block->m_count;
		if( off > pos )
			return block;
	}

	return NULL;
}

size_t _cwt_textdoc_block_count(const CwtTextDocumentPtr doc)
{
	JQ_ASSERT(doc);
	return doc->m_nblocks;
}

size_t _cwt_textdoc_line_count(const CwtTextDocumentPtr doc)
{
	size_t off = 0;
	size_t nlines = 0;

	if( strbuf_size(doc->m_text) > 0 ) {
		nlines++;

		while( strbuf_find(doc->m_text, '\n', &off) )
			nlines++;
	}

	return nlines;
}
