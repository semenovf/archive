/*
 * init.c
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 */

#include <stdlib.h>
#include <stdio.h>
#include <cwt/strutils.h>
#include <cwt/logger.h>
#include <cwt/cherry/cherry.h>



/*
2. Differences between BGI and BCC2GRX
-----------------------------------------------------------------------------

  BCC2GRX is based on LIBGRX instead of using an  .bgi driver. This
  introduces some differences compared with the Borland GI. The (known)
  differences are listed below.

     - Most LIBGRX platforms are 32 bit. An int will take 4 bytes instead of
       2 with Turbo-C and BCC for DOS. If you need a 16 bit integer, change
       the definition from int to short which is 16 bit on all systems.
     - WHITE is a function not constant with BCC2GRX. This may cause
       problems in switch () statements.
        (You may use
         #define WHITE 15
         #include <libbcc.h>
        to improve compatibility.)
     - BCC2GRX can not use .bgi drivers. Installing an user driver and the
       register(far)bgidriver will always cause an error.
     - registerfarbgifont() and registerbgifont() are the same. Both take a
       void* to the character font (whole file with header !)
     - initgraph()/detectgraph() work slightly different. See below for
       details.
     - getmodenames() and other functions may be called before initgraph()
     - character files won't be flushed at closegraph()
     - NOT_PUT isn't supported.
     - some constants may differ in value
     - BCC2GRX's outtext() and outtextxy() do correct clipping
     - some graphics primitives slightly differ in behaviour between BGI and
       LIBGRX. Eg. the "saucer" in bccbgi.c putimage()/getimage() demo
       looks a little different.
     - the BCC2GRX header file is <libbcc.h>. You must change the #include
       statements since BCC2GRX is incompatible with <graphics.h>. For
       programs compatible with both Borland GI and BCC2GRX, conditional
       compilation may be used:
         #if defined(__MSDOS__) && defined(__TURBOC__)
         #  include <graphics.h>
         #else
         #  include <libbcc.h>
         #endif
       and such programs should be linked with the respective library,
       either Borland or GRX (but not both).
     - the color constants like REG, GREEN, etc. won't work in graphics
       modes with more than 256 colors. Use _ega_color(RED),
       _ega_color(GREEN), etc to get the right colors
*/



static CWT_FONT __cwt_default_font = {SMALL_FONT, 4};
struct palettetype __palette;             /* Used to read ette info    */
int __max_colors = 0;
double __aspect_ratio = 0.0f;

int VP_LEFT = 0;
int VP_TOP = 0;
int VP_RIGHT = 0;
int VP_BOTTOM = 0;

BOOL cwt_init(void)
{
   /* request auto detection */
   int gdriver = DETECT, gmode, errorcode = 0;
   int xasp, yasp;

#if defined(__GRAPHICS_H) && defined(JQ_CC_BORLAND)
   /* Program MUST be linked with EGAVGAF.OBJ generated by command:
    *
    * 	BGIOBG.EXE /F EGAVGA.BGI
    *
	* 	with output: public name = '_EGAVGA_driver_far', segment name = 'EGAVGA_TEXT'
	*/
   errorcode = registerfarbgidriver(EGAVGA_driver_far);
   if( errorcode < 0 ) {
	   errorcode = graphresult();
	   if( errorcode != grOk )  {/*  an error occurred*/
		  printf_error("Register BGI driver: %s\n", grapherrormsg(errorcode));
		  return FALSE;
	   }
   }
#endif


   /* initialize graphics mode */
   initgraph(&gdriver, &gmode, "");

   /* read result of initialization */
   errorcode = graphresult();

   if( errorcode != grOk )  { /* an error occurred */
	  printf_error("Borland's graphics library error: %s\n", grapherrormsg(errorcode));
	  return FALSE;
   }

#if defined(__GRAPHICS_H) && defined(JQ_CC_BORLAND)
   /* Program MUST be linked with LITTF.OBJ generated by command:
    *
    * BGIOBG.EXE /F LITT
    *
    * with output:  public name = '_small_font'
    */
   errorcode = registerfarbgifont(small_font_far/*__cwt_default_bgi_font*/);
   if( errorcode < 0 ) {
	   errorcode = graphresult();
	   if( errorcode != grOk )  {/*  an error occurred*/
		  printf_error("Register BGI font: %s\n", grapherrormsg(errorcode));
		  return FALSE;
	   }
   }
#endif


   getpalette( &__palette );               /* Read the palette from board  */

   __max_colors = getmaxcolor() + 1;        /* Read maximum number of colors*/

#if !(defined(__GRAPHICS_H) && defined(JQ_CC_BORLAND))
   if( __max_colors == 256 )
	   setrgbdefaults();
#endif

   getaspectratio( &xasp, &yasp );       /* read the hardware aspect     */
   __aspect_ratio = (double)xasp / (double)yasp; /* Get correction factor   */


   setwritemode(COPY_PUT);

   settextstyle(__cwt_default_font.font, HORIZ_DIR, __cwt_default_font.factor);
   settextjustify(LEFT_TEXT, TOP_TEXT);

   cleardevice();
   return TRUE;
}


void cwt_finish(void)
{
	cleardevice();
	closegraph();
}


void _cwt_set_viewport_wh(int left, int top, int width, int height, BOOL clip)
{
	VP_LEFT = left;
	VP_TOP = top;
	VP_RIGHT = left+width-1;
	VP_BOTTOM = top+height-1;

	graphresult(); /* clear error */
	setviewport(VP_LEFT, VP_TOP, VP_RIGHT, VP_BOTTOM, clip);
	JQ_ASSERT_TRACE(graphresult() == grOk
		, printf("assertion on: setviewport(left=%d, top=%d, right=%d, bottom=%d)"
			, VP_LEFT
			, VP_TOP
			, VP_RIGHT
			, VP_BOTTOM));
}


void _cwt_set_viewport(CWT_VIEWPORT *vp)
{
	JQ_ASSERT(vp);
	_cwt_set_viewport_wh(vp->left, vp->top, vp->width, vp->height, vp->clip);
}

void _cwt_get_viewport(CWT_VIEWPORT *vp)
{
	if( vp ) {
		struct viewporttype vpt;
		getviewsettings(&vpt);
		vp->left = vpt.left;
		vp->top = vpt.top;
		vp->width = vpt.right - vpt.left + 1;
		vp->height = vpt.bottom - vpt.top + 1;
		vp->clip = vpt.clip ? TRUE : FALSE;
	}
}

CWT_FONT* cwt_default_font(void)
{
	return &__cwt_default_font;
}

void _cwt_set_default_font(const CWT_FONT *font)
{
	__cwt_default_font.font = font->font;
	__cwt_default_font.factor = font->factor;
}


void _cwt_set_fill_style(int pattern, CWT_COLOR color)
{
	setfillstyle(pattern, color);
}

void _cwt_get_fill_style(int *pattern, CWT_COLOR *color)
{
	struct fillsettingstype fst;

	getfillsettings(&fst);

	if( pattern )
		*pattern = fst.pattern;
	if( color )
		*color = fst.color;
}

void _cwt_set_font(const CWT_FONT *font)
{
	settextstyle(font->font, HORIZ_DIR, font->factor);
}

void _cwt_get_font(CWT_FONT *font)
{
	if( font ) {
		struct textsettingstype ts;
		gettextsettings(&ts);
		font->font = ts.font;
		font->factor = ts.charsize;
	}
}

void _cwt_bar(int left, int top, int width, int height)
{
	bar(left, top, left + width - 1, top + height - 1);
}

void _cwt_rectangle(int left, int top, int width, int height)
{
	rectangle(left, top, left + width - 1, top + height - 1);
}

void _cwt_fill_polygon(CWT_POINT points[], int npoints)
{
	int i, j;
	int* polypoints;

	polypoints = JQ_MALLOCA(int, npoints * 2);

	for( i = 0, j = 0 ; i < npoints; i++, j+=2 ) {
		int x = points[i].x;
		int y = points[i].y;
		polypoints[j] = x;
		polypoints[j+1] = y;
	}

	fillpoly(npoints, polypoints);
	JQ_FREE(polypoints);
}

void _cwt_set_line_style(int style, unsigned int pattern, int thickness)
{
	setlinestyle(style, pattern, thickness);
}


void _cwt_get_line_style(int *style, unsigned int *pattern, int *thickness)
{
	struct linesettingstype lst;
	getlinesettings(&lst);

	if( style )
		*style = lst.linestyle;
	if( pattern )
		*pattern = lst.upattern;
	if( thickness )
		*thickness = lst.thickness;
}

void _cwt_outtext_xy(int left, int top, const CWT_CHAR* text)
{
	/* top - <correction> for valid positioning of text */
#if defined(TEXT_CORRECTION_DIR) && TEXT_CORRECTION_DIR
	CWT_FONT font;
	_cwt_get_font(&font);
	outtextxy(left, top + TEXT_CORRECTION_DIR*(font.factor-1), text);
#else
	outtextxy(left, top, text);
#endif
}

#if 0
void _cwt_outtext_xy_n(int left, int top, const CWT_CHAR* text, int n)
{
	if( text && n > 0 ) {
		CWT_FONT font;
		CWT_CHAR *text_pie = jq_strndup(text, (size_t)n);

		/* top - <correction> for valid positioning of text */
#if USE_TEXT_CORRECTION
		_cwt_get_font(&font);
		outtextxy(left, top-(font.factor-1), text_pie);
#else
		outtextxy(left, top, text_pie);
#endif

		JQ_FREE(text_pie);
	}
}
#endif


/**
 * Layouts the @c rect into @c bounds according to @c align value
 *
 * @param bounds
 * @param rect
 * @param align
 */
void _cwt_layout_box(CWT_RECT *bounds, CWT_RECT *rect, UINT16 align)
{
	if( align & CWT_ALIGN_LEFT )         rect->left = bounds->left;
	else if( align & CWT_ALIGN_RIGHT )   rect->left = bounds->left + bounds->width - rect->width;
	else if( align & CWT_ALIGN_CENTER )  rect->left = bounds->left + (bounds->width - rect->width)/2;

	if( align & CWT_VALIGN_TOP )         rect->top = bounds->top;
	else if( align & CWT_VALIGN_BOTTOM ) rect->top = bounds->top + bounds->height - rect->height;
	else if( align & CWT_VALIGN_CENTER ) rect->top = bounds->top +(bounds->height - rect->height)/2;
}

void _cwt_outtext_box(const CWT_CHAR* text, CWT_RECT *bounds, CWT_FONT* font, UINT16 align, CWT_COLOR color, CWT_COLOR bgcolor)
{
	CWT_SIZE sz;
	CWT_RECT rect;

	_cwt_set_font(font);
	_cwt_text_size(NULL, text, &sz);

	rect.width = sz.width;
	rect.height = sz.height;
	_cwt_layout_box(bounds, &rect, align);

	if( !_cwt_color_is_transparent(bgcolor) ) {
		_cwt_set_fill_style(SOLID_FILL, bgcolor);
		_cwt_bar(rect.left, rect.top, rect.width, rect.height);
	}

	_cwt_set_color(color);
	_cwt_outtext_xy(rect.left, rect.top, text);
}

void _cwt_outtext(const CWT_CHAR* text)
{
	outtext(text);
}


void _cwt_set_color(CWT_COLOR color)
{
	setcolor(color);
}


CWT_COLOR _cwt_get_color(void)
{
	return (CWT_COLOR)getcolor();
}

void _cwt_set_bg_color(CWT_COLOR color)
{
	setbkcolor(color);
}

CWT_COLOR _cwt_get_bg_color(void)
{
	return (CWT_COLOR)getbkcolor();
}


int _cwt_getmaxx(void)
{
	return getmaxx();
}

int _cwt_getmaxy(void)
{
	return getmaxy();
}


void _cwt_set_textjustify(UINT16 align)
{
	int h, v;

	if( align & CWT_ALIGN_LEFT )    h = LEFT_TEXT;
	else if( align & CWT_ALIGN_RIGHT )   h = RIGHT_TEXT;
	else if( align & CWT_ALIGN_CENTER )  h = CENTER_TEXT;

	if( align & CWT_VALIGN_TOP )    v = TOP_TEXT;
	else if( align & CWT_VALIGN_BOTTOM ) v = BOTTOM_TEXT;
	else if( align & CWT_VALIGN_CENTER ) v = CENTER_TEXT;

	settextjustify(h, v);
}

/**
 * Returns size of text with specified font @c font or current font
 * if @c font is null.
 *
 * @param font
 * @param text
 * @param width
 * @param height
 */
void _cwt_text_size_wh(const CWT_FONT *font, const CWT_CHAR *text, int *width, int *height)
{
	CWT_FONT tmp_font;
	const CWT_CHAR* t = "Wj";

	if( font ) {
		_cwt_get_font(&tmp_font); /* save font */
		_cwt_set_font(font);
	}

	if( text )
		t = text;

	if( width )
		*width = textwidth(t);
	if( height )
		*height = textheight(t);

	if( font )
		_cwt_set_font(&tmp_font); /* restore font */
}

void _cwt_text_size(const CWT_FONT *font, const CWT_CHAR *text, CWT_SIZE *psize)
{
	JQ_ASSERT(psize);
	_cwt_text_size_wh(font, text, &psize->width, &psize->height);
}

int _cwt_text_width(const CWT_FONT *font, const CWT_CHAR *text)
{
	int width;
	_cwt_text_size_wh(font, text, &width, NULL);
	return width;
}

int _cwt_text_height(const CWT_FONT *font, const CWT_CHAR *text)
{
	int height;
	if( !text )
		_cwt_text_size_wh(font, "Jj", NULL, &height);
	else
		_cwt_text_size_wh(font, text, NULL, &height);
	return height;
}

void _cwt_moveto(int x, int y)
{
	moveto(x, y);
}

void _cwt_line(int x1, int y1, int x2, int y2)
{
	line(x1, y1, x2, y2);
}

void _cwt_lineto(int x, int y)
{
	lineto(x, y);
}


#if __DEPRICATED__
BYTE* _cwt_get_image(int left, int top, int width, int height)
{
	BYTE *image;
	int right = left + width;
	int bottom = top + height;
	unsigned int img_sz;

	img_sz = imagesize(left, top, right, bottom);
	image = JQ_MALLOCA(BYTE, img_sz);
	getimage(left, top, right, bottom, image);
	return image;
}

void _cwt_put_image(int left, int top, BYTE *image)
{
	putimage(left, top, image, COPY_PUT);
}

BYTE* _cwt_save_video_memory(int left, int top, int width, int height)
{
	int x, y, right, bottom;
	BYTE *buf, *pbuf;

	y = top;
	right = left + width;
	bottom = top + height;

	buf = JQ_MALLOCA(BYTE, width * height);
	pbuf = buf;

	cwt_mouse_hide();
	while( y < bottom) {
		for( x = left; x < right; x++ )
			*buf++ = (BYTE)getpixel(x, y);
		y++;
	}
	cwt_mouse_show();

	return pbuf;
}


void _cwt_restore_video_memory(int left, int top, int width, int height, BYTE *buf)
{
	int x, y, right, bottom;

	y = top;
	right = left + width;
	bottom = top + height;

	cwt_mouse_hide();
	while( y < bottom) {
		for( x = left; x < right; x++ )
			putpixel(x, y, *buf++);
		y++;
	}
	cwt_mouse_show();
}
#endif

/**
 * @brief Save settings
 *
 * @param settings
 */
void _cwt_save_settings(CWT_SETTINGS *settings)
{
	JQ_ASSERT(settings);

	_cwt_get_viewport(&settings->vp);
	settings->color = _cwt_get_color();
	settings->bg_color = _cwt_get_bg_color();
	_cwt_get_font(&settings->font);

	_cwt_get_line_style(
		  &settings->line_style
		, &settings->line_pattern
		, &settings->line_thickness);

	_cwt_get_fill_style(&settings->fill_pattern, &settings->fill_color);
}

/**
 * @brief Restore previously saved settings
 *
 * @param settings previously saved settings
 */
void _cwt_restore_settings(CWT_SETTINGS *settings)
{
	JQ_ASSERT(settings);

	_cwt_set_viewport(&settings->vp);
	_cwt_set_color(settings->color);
	_cwt_set_bg_color(settings->bg_color);
	_cwt_set_font(&settings->font);

	_cwt_set_line_style(
		  settings->line_style
		, settings->line_pattern
		, settings->line_thickness);

	_cwt_set_fill_style(settings->fill_pattern, settings->fill_color);
}





CwtBitMapPtr cwt_getimage(int left, int top, int width, int height)
{
	CwtBitMapPtr bitmap = JQ_MALLOC(CwtBitMap);
	ListNode* last_node = NULL;
	int b
		, h /* heigth per block */
		, bottom
		, right;

	JQ_ASSERT(bitmap);

	bottom = top + height - 1;
	right  = left + width - 1;
	b = bottom;

	/* ��।����� ࠧ��� ����� ����� ��� ��࠭���� ��� ���� */
	while( imagesize(left, top, right, b) == 0xFFFF )
		b /= 2;

	h = b + 1;

	while( top <= bottom ) {
		ListNode *node = JQ_MALLOC(ListNode);
		CwtBitMapElem *block = JQ_MALLOC(CwtBitMapElem);
		node->data = block;

		block->bitmap = JQ_MALLOCA(BYTE, imagesize(left, top, right, b));
		getimage(left, top, right, b, block->bitmap);
		block->height = b - top + 1;

		if( !last_node ) {
			list_insert_first(bitmap, node);
		} else {
			list_insert_after(last_node, node);
		}
		last_node = node;

		top += h;
		b += JQ_MIN(h, bottom-top+1);
	}

	return bitmap;
}

void cwt_putimage(int left, int top, CwtBitMapPtr bitmap)
{
	ListIterator it;

	list_begin(bitmap, &it);
	while( list_has_more(&it) ) {
		CwtBitMapElem *block = (CwtBitMapElem*)list_next(&it);
		putimage( left, top, block->bitmap, COPY_PUT);
		top += block->height;
	}
}


void cwt_free_bitmap(CwtBitMapPtr bitmap)
{
	ListIterator it;
	ListNode *node;

	list_begin(bitmap, &it);
	while( list_has_more(&it) ) {
		CwtBitMapElem *block = (CwtBitMapElem*)list_next(&it);
		JQ_FREE(block->bitmap);
		JQ_FREE(block);
	}

	while( (node = list_remove_first(bitmap)) != NULL )
		JQ_FREE(node);

	JQ_FREE(bitmap);
}
