/*
 * style.c
 *
 *  Created on: Mar 14, 2012
 *      Author: wladt
 */


#include <cwt/cherry/cherry.h>
#include <cwt/logger.h>


static CWT_STYLE_PTR __default_style = NULL;

const CWT_STYLE_PTR _cwt_style_default(void)
{
	if( !__default_style ) {
		__default_style = cwt_new_style();
	}
	return __default_style;
}

CWT_STYLE_PTR cwt_new_style(void)
{
	CWT_STYLE_PTR style = __default_style;
	CWT_FONT *pfont;

	pfont = cwt_default_font();

	style = JQ_MALLOC(CWT_STYLE);

	printf_trace("STYLE_NEW: %p", style);

	style->ref = 1;
	style->magic = CWT_MAGIC;
	style->color = CWT_White;
	style->bgcolor = CWT_Transparent;
	style->font.font = pfont->font;
	style->font.factor = pfont->factor;

	style->border.left
		= style->border.top
		= style->border.right
		= style->border.bottom
		= 0;
	style->border.color = CWT_White;

	style->padding.left
		= style->padding.top
		= style->padding.right
		= style->padding.bottom
		= 0;

	style->align = CWT_ALIGN_LEFT | CWT_VALIGN_CENTER;
	style->line_height = 100;

	return style;
}

void cwt_delete_style(CWT_STYLE_PTR style)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);

	if( style == __default_style )
		return;

	JQ_ASSERT(style->ref > 0);
	style->ref--;

	if( !style->ref ) {
		printf_trace("STYLE_DELETE: %p", style);
		style->magic = 0;
		JQ_FREE(style);
	}
}

CWT_STYLE_PTR cwt_assign_style(CWT_STYLE_PTR* pstyle, CWT_STYLE_PTR style)
{
	JQ_ASSERT(pstyle);
	JQ_ASSERT(style);

	if( *pstyle && *pstyle != style ) {
		cwt_delete_style(*pstyle);
	}

	*pstyle = style;
	style->ref++;

	return style;
}

CWT_STYLE_PTR cwt_clone_style(const CWT_STYLE_PTR pstyle)
{
	CWT_STYLE_PTR clone;

	clone = JQ_MALLOC(CWT_STYLE);
	memcpy(clone, pstyle, sizeof(CWT_STYLE));
	clone->ref = 0;

	return clone;
}

const CWT_BORDER* cwt_style_border(const CWT_STYLE_PTR style)
{
	return &style->border;
}

BOOL cwt_style_eq(const CWT_STYLE_PTR style0, const CWT_STYLE_PTR style1)
{
	return	style0->color             == style1->color
			&& style0->bgcolor        == style1->bgcolor
			&& style0->font.factor    == style1->font.factor
			&& style0->font.font      == style1->font.font
			&& style0->border.color   == style1->border.color
			&& style0->border.left    == style1->border.left
			&& style0->border.top     == style1->border.top
			&& style0->border.right   == style1->border.right
			&& style0->border.bottom  == style1->border.bottom
			&& style0->padding.left   == style1->padding.left
			&& style0->padding.top    == style1->padding.top
			&& style0->padding.right  == style1->padding.right
			&& style0->padding.bottom == style1->padding.bottom
			&& style0->align          == style1->align
			&& style0->line_height    == style1->line_height
			? TRUE : FALSE;
}

BOOL _cwt_style_eq_default(const CWT_STYLE_PTR style)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);
	return style == __default_style ? TRUE : FALSE;
}

void cwt_style_set_border(CWT_STYLE_PTR style, CWT_COLOR color, int left, int top, int right, int bottom)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);
/*	JQ_ASSERT(style->ref == 0);*/
	style->border.color  = color;
	style->border.left   = left;
	style->border.top    = top;
	style->border.right  = right;
	style->border.bottom = bottom;
}

void cwt_style_setx_border(CWT_STYLE_PTR style, CWT_BORDER *border)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);
	JQ_ASSERT(border);
	/*JQ_ASSERT(style->ref == 0);*/
	cwt_style_set_border(style
		, border->color
		, border->left
		, border->top
		, border->right
		, border->bottom);
}


void cwt_style_set_padding(CWT_STYLE_PTR style, int left, int top, int right, int bottom)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);
  /*	JQ_ASSERT(style->ref == 0);*/
	style->padding.left   = left;
	style->padding.top    = top;
	style->padding.right  = right;
	style->padding.bottom = bottom;
}


void cwt_style_setx_padding(CWT_STYLE_PTR style, CWT_PADDING *padding)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);
	JQ_ASSERT(padding);
/*	JQ_ASSERT(style->ref == 0);*/
	cwt_style_set_padding(style
		, padding->left
		, padding->top
		, padding->right
		, padding->bottom);
}

void cwt_style_set_color(CWT_STYLE_PTR style, CWT_COLOR color, CWT_COLOR bgcolor)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);
/*	JQ_ASSERT(style->ref == 0);*/
	style->color = color;
	style->bgcolor = bgcolor;
}

void cwt_style_set_font(CWT_STYLE_PTR style, CWT_FONT *font)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);
	JQ_ASSERT(font);
/*	JQ_ASSERT(style->ref == 0);*/
	style->font.factor = font->factor;
	style->font.font = font->font;
}

void cwt_style_set_align(CWT_STYLE_PTR style, UINT16 align)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);
/*	JQ_ASSERT(style->ref == 0);*/
	style->align = align;
}

void cwt_style_set_line_height(CWT_STYLE_PTR style, UINT16 line_height)
{
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);
/*	JQ_ASSERT(style->ref == 0);*/
	style->line_height = line_height;
}

