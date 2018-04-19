/*
 * groupbox.c
 *
 *  Created on: 16.04.2012
 *      Author: wladt
 */


#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>
#include <cwt/logger.h>


static void _finalize_groupbox(CwtWidgetPtr gb)
{
	JQ_ASSERT(gb);
	JQ_ASSERT(gb->magic == CWT_MAGIC);
	JQ_ASSERT(gb->wtype == CWT_WT_GROUPBOX);

	_cwt_widget_focus_out(gb->focused);
	_cwt_widget_focus_out(gb);

	cwt_set_active_window(CWT_CAST(gb, CWT_GROUPBOX_PTR)->active_window);
}


static BOOL _event_handler(CWT_EVENT_PTR pevt)
{
	CwtWidgetPtr widget;
	CwtWidgetPtr gb = cwt_active_window();

	JQ_ASSERT(gb);
	JQ_ASSERT(gb->magic == CWT_MAGIC);

	switch( pevt->ev_type ) {

		case CWT_EVT_KEYBOARD: {
			UINT16 key = CWT_KBD_KEY(pevt);
			UINT16 mods = CWT_KBD_MODS(pevt);

			if( (CWT_KBD_NO_MODS(mods) && CWT_KBD_KEY_SCAN(key) == CWT_Key_Esc)
					|| CWT_KBD_TAB(pevt) || CWT_KBD_SHIFT_TAB(pevt) ) {
				_finalize_groupbox(gb);
				/*cwt_post_kbd_event(key, mods);*/
				return TRUE;
			}

			switch( CWT_KBD_KEY_SCAN(key) ) {
				case CWT_Key_Left:
				case CWT_Key_Up:
				case CWT_Key_Right:
				case CWT_Key_Down:
					if( CWT_KBD_NO_MODS(mods) ) {
						CwtWidgetPtr focused_widget;
						BOOL forward_focus = TRUE;

						focused_widget = gb->focused;

						if( focused_widget ) {
							JQ_ASSERT(focused_widget->magic == CWT_MAGIC);
							JQ_ASSERT(!_cwt_widget_deleting(focused_widget));

							if( focused_widget->key_press_event ) {
								if( focused_widget->key_press_event(focused_widget, pevt) )
									return TRUE;
							}
						}

						if( CWT_KBD_KEY_SCAN(key) == CWT_Key_Left || CWT_KBD_KEY_SCAN(key) == CWT_Key_Up )
							forward_focus = FALSE;

						focused_widget = _cwt_widget_next_focused(gb, forward_focus);

						if( focused_widget)
							_cwt_widget_set_focused(focused_widget, CWT_FOCUS_REASON_TAB);
					}

					return TRUE;

				default:
					break;/*return TRUE; */

			}}

			break;

		case CWT_EVT_MOUSE_RELEASE: {
				widget = _cwt_widget_child_at(gb, CWT_MOUSE_POS_X(pevt), CWT_MOUSE_POS_Y(pevt));
				if( !widget ) {
					_finalize_groupbox(gb);
					cwt_post_mouse_release_event(CWT_MOUSE_STATUS(pevt), CWT_MOUSE_POS_X(pevt), CWT_MOUSE_POS_Y(pevt));
					return TRUE;
				}
			}
			break;

		default:
			break;
	}

	return FALSE;
}

BOOL _cwt_groupbox_focus_in_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt)
{
	JQ_ASSERT(widget);
	JQ_UNUSED(pevt);

	if( !widget->focused ) {
		CwtWidgetPtr child = _cwt_widget_first_focused(widget);
		if( child ) {
			cwt_widget_set_focused(child, CWT_FOCUS_REASON_OTHER);
		}
	}
	return TRUE;
}

BOOL _cwt_groupbox_focus_out_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt)
{
	JQ_ASSERT(widget);
	JQ_UNUSED(widget);
	JQ_UNUSED(pevt);

	return FALSE;
}


BOOL _cwt_groupbox_key_press_event(CwtWidgetPtr widget, CwtEventPtr pevt)
{
	JQ_UNUSED(widget);
	JQ_UNUSED(pevt);
	return FALSE;
}

void _cwt_init_groupbox(CwtWidgetPtr widget)
{
	CWT_MENU_PTR gb = CWT_CAST(widget, CWT_MENU_PTR);
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	widget->focus_policy = CWT_FP_STRONG;
	widget->event_handler = _event_handler;
	widget->focus_in_event = _cwt_groupbox_focus_in_event;
	widget->focus_out_event = _cwt_groupbox_focus_out_event;
	gb->active_window = NULL;
}

