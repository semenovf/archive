/*
 * mouse.c
 *
 *  Created on: 16.01.2012
 *      Author: wladt
 */

#include <cwt/logger.h>
#include <cwt/cherry/mouse.h>

static BOOL __mouse_ready = FALSE;

BOOL cwt_mouse_init(void)
{
	JQ_REGS_TYPE regs;

	regs.x.ax = 0;
	JQ_INT86(0x33, &regs);

    if( regs.x.ax == 0 ) {
    	print_error("mouse not found");
    	__mouse_ready = FALSE;
    	return FALSE;
    }

    __mouse_ready = TRUE;
	cwt_mouse_show();
    return TRUE;
}

BOOL cwt_mouse_ready(void)
{
	return __mouse_ready;
}

void cwt_mouse_show(void)
{
	JQ_REGS_TYPE regs;

	regs.x.ax = 1;
	JQ_INT86(0x33, &regs);
}

void cwt_mouse_hide(void)
{
	JQ_REGS_TYPE regs;
	regs.x.ax = 2;
	JQ_INT86(0x33, &regs);
}


/**
 * Reads status of mouse: state of buttons and pointer position
 *
 * @param buttons stores state of buttons:
 * 	0x01 - left button down
 * 	0x02 - right button down
 * 	0x04 - middle button down
 * @param x stores x-position of mouse pointer
 * @param y stores y-position of mouse pointer
 */
void cwt_mouse_stat(int *buttons, int *x, int *y)
{
	JQ_REGS_TYPE regs;

	regs.x.ax = 3;
	JQ_INT86(0x33, &regs);

	if( buttons )
		*buttons = regs.x.bx;

	if( x )
		*x = regs.x.cx;

	if( y )
		*y = regs.x.dx;
}

void cwt_mouse_setxy(int x, int y)
{
	JQ_REGS_TYPE regs;

	regs.x.ax = 4;
	regs.x.cx = x;
	regs.x.dx = y;
	JQ_INT86(0x33, &regs);
}

void cwt_mouse_region(int x1, int y1, int x2, int y2 )
{
	JQ_REGS_TYPE regs;

	regs.x.ax = 7;
	regs.x.cx = x1;
	regs.x.dx = x2;
	JQ_INT86( 0x33, &regs);

	regs.x.ax = 8;
	regs.x.cx = y1;
	regs.x.dx = y2;
	JQ_INT86( 0x33, &regs );
}
