/*
 * mouse.h
 *
 *  Created on: 16.01.2012
 *      Author: wladt
 */

#ifndef _JQ_MOUSE_H_
#define _JQ_MOUSE_H_

#include <cwt/global.h>
#include <cwt/types.h>

BOOL cwt_mouse_init(void);
BOOL cwt_mouse_ready(void);
void cwt_mouse_show(void);
void cwt_mouse_hide(void);
void cwt_mouse_stat(int *buttons, int *x, int *y);
void cwt_mouse_setxy(int x, int y);
void cwt_mouse_region(int x1, int y1, int x2, int y2 );

#endif /* _JQ_MOUSE_H_ */

