/*
 * ev_kb.c
 *
 *  Created on: 08.01.2012
 *      Author: wladt
 */

#include <bios.h> /* bioskey */
#include <cwt/cherry/mouse.h>
#include <cwt/cherry/event.h>

#define _LEFT_BUTTON_MASK   0x01
#define _RIGHT_BUTTON_MASK  0x02
#define _MIDDLE_BUTTON_MASK 0x04
#define _LEFT_BUTTON_PRESSED(buttons)   ((buttons) & _LEFT_BUTTON_MASK)
#define _RIGHT_BUTTON_PRESSED(buttons)  ((buttons) & _RIGHT_BUTTON_MASK)
#define _MIDDLE_BUTTON_PRESSED(buttons) ((buttons) & _MIDDLE_BUTTON_MASK)

static unsigned int __buttons = 0;
static int __x = 0;
static int __y = 0;

void _cwt_process_mouse(void)
{
	if( cwt_mouse_ready() ) {
		int buttons;
		int x;
		int y;

		cwt_mouse_stat(&buttons, &x, &y);

		/* mouse status changed */
		if( buttons != __buttons || x != __x || y != __y ) {
			UINT16 mods;

			CWT_MOUSE_EVENT *evt = JQ_MALLOC(CWT_MOUSE_EVENT);

			mods = bioskey(_KEYBRD_SHIFTSTATUS);

			evt->status = CWT_KBD_NO_MODIFIER;
			evt->x = x;
			evt->y = y;

			if( CWT_KBD_SHIFT(mods) ) {
				evt->status |= CWT_KBD_SHIFT_MODIFIER;
			}

			if( CWT_KBD_CTRL(mods) ) {
				evt->status |= CWT_KBD_CTRL_MODIFIER;
			}

			if( CWT_KBD_ALT(mods) ) {
				evt->status |= CWT_KBD_ALT_MODIFIER;
			}


			/* Buttons event */
			if( buttons != __buttons ) {
				int i;
				int changes[3];
				CWT_MOUSE_BUTTON changed[3] = {
					  CWT_MOUSE_LEFT_BUTTON
					, CWT_MOUSE_RIGHT_BUTTON
					, CWT_MOUSE_MIDDLE_BUTTON
				};

				changes[0] = _LEFT_BUTTON_PRESSED(buttons) - _LEFT_BUTTON_PRESSED(__buttons);
				changes[1] = _RIGHT_BUTTON_PRESSED(buttons) - _RIGHT_BUTTON_PRESSED(__buttons);
				changes[2] = _MIDDLE_BUTTON_PRESSED(buttons) - _MIDDLE_BUTTON_PRESSED(__buttons);

				for( i = 0; i < 3; i++ ) {
					if( changes[i] ) {
						evt->status |= changed[i];
						if( changes[i] > 0 )
							cwt_post_event(CWT_EVT_MOUSE_PRESS, evt, CWT_DEFAULT_FREE);
						else
							cwt_post_event(CWT_EVT_MOUSE_RELEASE, evt, CWT_DEFAULT_FREE);
					}
				}
			} else { /* Move event */
				cwt_post_event(CWT_EVT_MOUSE_MOVE, evt, CWT_DEFAULT_FREE);
			}


			__buttons = buttons;
			__x = x;
			__y = y;
		}
	}
}


void cwt_post_mouse_move_event(UINT16 status, int x, int y)
{
	CWT_MOUSE_EVENT *evt = JQ_MALLOC(CWT_MOUSE_EVENT);

	evt->status = status;
	evt->x = x;
	evt->y = y;
	cwt_post_event(CWT_EVT_MOUSE_MOVE, evt, CWT_DEFAULT_FREE);
}

void cwt_post_mouse_press_event(UINT16 status, int x, int y)
{
	CWT_MOUSE_EVENT *evt = JQ_MALLOC(CWT_MOUSE_EVENT);

	evt->status = status;
	evt->x = x;
	evt->y = y;
	cwt_post_event(CWT_EVT_MOUSE_PRESS, evt, CWT_DEFAULT_FREE);
}

void cwt_post_mouse_release_event(UINT16 status, int x, int y)
{
	CWT_MOUSE_EVENT *evt = JQ_MALLOC(CWT_MOUSE_EVENT);

	evt->status = status;
	evt->x = x;
	evt->y = y;
	cwt_post_event(CWT_EVT_MOUSE_RELEASE, evt, CWT_DEFAULT_FREE);
}

