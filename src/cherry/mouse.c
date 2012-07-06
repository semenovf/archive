/*
 * mouse.c
 *
 *  Created on: 16.01.2012
 *      Author: wladt
 */

#include <dos.h>
#include <cwt/logger.h>
#include <cwt/cherry/mouse.h>

static union REGS __regs_in, __regs_out;
static BOOL __mouse_ready = FALSE;

BOOL cwt_mouse_init(void)
{
	__regs_in.x.ax = 0x00;
    int86( 0x33, &__regs_in, &__regs_out );
    if( __regs_out.x.ax == 0x00 ) {
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
	__regs_in.x.ax = 1;
	int86(0x33, &__regs_in, &__regs_out);
}

void cwt_mouse_hide(void)
{
	__regs_in.x.ax = 2;
	int86(0x33, &__regs_in, &__regs_out);
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
	__regs_in.x.ax = 3;
	int86(0x33, &__regs_in, &__regs_out);

	if( buttons )
		*buttons = __regs_out.x.bx;

	if( x )
		*x = __regs_out.x.cx;

	if( y )
		*y = __regs_out.x.dx;
}

void cwt_mouse_setxy(int x, int y)
{
	__regs_in.x.ax = 4;
	__regs_in.x.cx = x;
	__regs_in.x.dx = y;
	int86( 0x33, &__regs_in, &__regs_out);
}

void cwt_mouse_region(int x1, int y1, int x2, int y2 )
{
	__regs_in.x.ax = 7;
	__regs_in.x.cx = x1;
	__regs_in.x.dx = x2;
	int86( 0x33, &__regs_in, &__regs_out);

	__regs_in.x.ax = 8;
	__regs_in.x.cx = y1;
	__regs_in.x.dx = y2;
	int86( 0x33, &__regs_in, &__regs_out );
}
