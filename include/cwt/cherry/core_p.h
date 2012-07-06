/*
 * core_p.h
 *
 *  Created on: Mar 14, 2012
 *      Author: wladt
 */

#ifndef _CWT_CORE_P_H_
#define _CWT_CORE_P_H_

#include <cwt/global.h>
#include <cwt/types.h>
#include <cwt/cherry/config.h>

#if defined(JQ_CC_BORLAND)
#  include <graphics.h>
#else
#  include <libbcc.h>
#endif


struct CWT_SETTINGS
{
	/* viewport settings */
	CWT_VIEWPORT vp;
	CWT_COLOR color;
	CWT_COLOR bg_color;
	CWT_FONT  font;

	/* line settings */
	int line_style;
	unsigned int line_pattern;
	int line_thickness;

	/* fill settings */
	int fill_pattern;
	CWT_COLOR fill_color;
};
typedef struct CWT_SETTINGS CWT_SETTINGS;


#define _cwt_color_is_transparent(color) (((color) == CWT_Transparent) ? TRUE : FALSE)


struct CwtBitMapElem {
	int height;
	void *bitmap;
};
typedef struct CwtBitMapElem CwtBitMapElem;
typedef List       CwtBitMap;
typedef CwtBitMap* CwtBitMapPtr;


EXTERN_C_BEGIN

/* CWT core functions {{{ */
void _cwt_set_viewport_wh(int left, int top, int width, int height, BOOL clip);
void _cwt_set_viewport(CWT_VIEWPORT *vp);
void _cwt_set_fill_style(int pattern, CWT_COLOR color);
void _cwt_set_line_style(int style, unsigned int pattern, int thickness);
void _cwt_set_font(const CWT_FONT *font);
void _cwt_set_color(CWT_COLOR color);
void _cwt_set_bg_color(CWT_COLOR color);
void _cwt_set_textjustify(UINT16 align);

void _cwt_get_viewport(CWT_VIEWPORT *vp);
void _cwt_get_font(CWT_FONT *font);
void _cwt_get_fill_style(int *pattern, CWT_COLOR *color);
void _cwt_get_line_style(int *style, unsigned int *pattern, int *thickness);
CWT_COLOR _cwt_get_color(void);
CWT_COLOR _cwt_get_bg_color(void);
int _cwt_getmaxx(void);
int _cwt_getmaxy(void);


void _cwt_bar(int left, int top, int width, int height);
void _cwt_rectangle(int left, int top, int width, int height);
void _cwt_fill_polygon(CWT_POINT points[], int npoints);
void _cwt_moveto(int x, int y);
void _cwt_line(int x1, int y1, int x2, int y2);
void _cwt_lineto(int x, int y);
void _cwt_outtext(const CWT_CHAR* text);
void _cwt_outtext_xy(int left, int top, const CWT_CHAR* text);
/*void _cwt_outtext_xy_n(int left, int top, const CHAR* text, int n);*/
/*void _cwt_outtext_box(const CHAR* text, CWT_RECT *rect, CWT_STYLE_PTR pstyle);*/
void _cwt_outtext_box(const CWT_CHAR* text, CWT_RECT *bounds, CWT_FONT* font, UINT16 align, CWT_COLOR color, CWT_COLOR bgcolor);
void _cwt_layout_box(CWT_RECT *bounds, CWT_RECT *rect, UINT16 align);
int  _cwt_text_width(const CWT_FONT *font, const CWT_CHAR *text);
int  _cwt_text_height(const CWT_FONT *font, const CWT_CHAR *text);
void _cwt_text_size_wh(const CWT_FONT *font, const CWT_CHAR *text, int *width, int *height);
void _cwt_text_size(const CWT_FONT *font, const CWT_CHAR *text, CWT_SIZE *psize);

void _cwt_save_settings(CWT_SETTINGS *settings);
void _cwt_restore_settings(CWT_SETTINGS *settings);

#if 0
BYTE* _cwt_get_image(int left, int top, int width, int height);
void _cwt_put_image(int left, int top, BYTE *image);
BYTE* _cwt_save_video_memory(int left, int top, int width, int height);
void _cwt_restore_video_memory(int left, int top, int width, int height, BYTE *buf);
#endif

/* }}} CWT core functions */

CwtBitMapPtr cwt_getimage(int left, int top, int width, int height);
void cwt_putimage(int left, int top, CwtBitMapPtr bitmap);
void cwt_free_bitmap(CwtBitMapPtr bitmap);

EXTERN_C_END

#endif /* _CWT_CORE_P_H_ */
