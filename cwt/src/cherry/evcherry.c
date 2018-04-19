/*
 * evcherry.c
 *
 *  Created on: 21.01.2012
 *      Author: wladt
 */
#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>
#include <cwt/logger.h>


BOOL _cwt_handle_events(CWT_EVENT_PTR pevt)
{
	CWT_WIDGET_PTR widget = NULL;
	CWT_WIDGET_PTR active_window = NULL;

/*
	if( pevt->ev_type ==  CWT_EVT_WINDOW_ACTIVATE ) {
		widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
		JQ_ASSERT(widget);
		JQ_ASSERT(widget->magic == CWT_MAGIC);

		if( !_cwt_widget_deleting(widget) )
			_cwt_set_active_window(widget);

		return TRUE;
	}
*/

	active_window = cwt_active_window();

	if( active_window ) {
		JQ_ASSERT(active_window->magic == CWT_MAGIC);
		if( active_window->event_handler ) {
			if( active_window->event_handler(pevt) )
				return TRUE;
		}
	}

	switch( pevt->ev_type ) {

		case CWT_EVT_KEYBOARD:
			if( active_window ) {
				CWT_WIDGET_PTR focused_widget = active_window->focused;

				if( focused_widget ) {
					JQ_ASSERT(focused_widget->magic == CWT_MAGIC);
					JQ_ASSERT(!_cwt_widget_deleting(focused_widget));

					if( focused_widget->key_press_event ) {
						if( focused_widget->key_press_event(focused_widget, pevt) )
							return TRUE;
					}
				}

				if( CWT_KBD_TAB(pevt) || CWT_KBD_SHIFT_TAB(pevt) ) {
					BOOL forward_focus = TRUE;

					if( CWT_KBD_SHIFT_TAB(pevt) )
						forward_focus = FALSE;

					focused_widget = _cwt_widget_next_focused(active_window, forward_focus);
					if( focused_widget ) {
						JQ_ASSERT(focused_widget->magic == CWT_MAGIC);
						_cwt_widget_set_focused(focused_widget, CWT_FOCUS_REASON_TAB);
					}

					return TRUE;
				}

				if( active_window->key_press_event && active_window->key_press_event(active_window, pevt) )
					return TRUE;
			}
			break;

		case CWT_EVT_MOUSE_MOVE:
			if( active_window )  {
				widget = _cwt_widget_child_at(cwt_screen(), CWT_MOUSE_POS_X(pevt), CWT_MOUSE_POS_Y(pevt));

				JQ_ASSERT(widget);
				JQ_ASSERT(widget->magic == CWT_MAGIC);

				/* TODO Remove this */
				if( widget )
					JQ_ASSERT(!_cwt_widget_deleting(widget));

				_cwt_widget_set_hover(widget);
			}
			break;

		case CWT_EVT_MOUSE_PRESS:
			if( active_window )  {
				widget = _cwt_widget_child_at(cwt_screen(), CWT_MOUSE_POS_X(pevt), CWT_MOUSE_POS_Y(pevt));

				JQ_ASSERT(widget);
				JQ_ASSERT(widget->magic == CWT_MAGIC);

				/* TODO Remove this */
				if( widget )
					JQ_ASSERT(!_cwt_widget_deleting(widget));

				if( widget && widget->mouse_down_event ) {
					widget->mouse_down_event(widget, pevt);
				}
			}
			break;

		case CWT_EVT_MOUSE_RELEASE:
			if( active_window )  {
				widget = _cwt_widget_child_at(cwt_screen(), CWT_MOUSE_POS_X(pevt), CWT_MOUSE_POS_Y(pevt));

				JQ_ASSERT(widget);
				JQ_ASSERT(widget->magic == CWT_MAGIC);

				/* TODO Remove this */
				JQ_ASSERT(!_cwt_widget_deleting(widget));

				if( widget->nchilds ) {
					_cwt_widget_set_focused(widget, CWT_FOCUS_REASON_MOUSE);
					cwt_set_active_window(widget);
				} else {
					_cwt_widget_set_focused(widget, CWT_FOCUS_REASON_MOUSE);
				}

				if( widget->mouse_up_event ) {
					widget->mouse_up_event(widget, pevt);
				}
			}
			break;

		case CWT_EVT_WIDGET_PAINT:
			widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
			JQ_ASSERT(widget);
			JQ_ASSERT(widget->magic == CWT_MAGIC);

			/*JQ_ASSERT(!_cwt_widget_deleting(widget));*/
			if( !_cwt_widget_deleting(widget) )
				cwt_widget_repaint(widget);
			return TRUE;

		case CWT_EVT_WIDGET_DELETE: {
			CWT_WIDGET_PTR parent;
			widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
			JQ_ASSERT(widget);
			JQ_ASSERT(widget->magic == CWT_MAGIC);

			parent = widget->parent;

			JQ_ASSERT(_cwt_widget_deleting(widget));

			cwt_delete_widget_now(widget);
			if( parent )
				cwt_widget_repaint(parent);
			}
			return TRUE;

		case CWT_EVT_WIDGET_ENTER:
			widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
			JQ_ASSERT(widget);
			JQ_ASSERT(widget->magic == CWT_MAGIC);

			JQ_ASSERT(!_cwt_widget_deleting(widget));

			_cwt_widget_change_state(widget, CWT_STATE_HOVER);
			if( widget->mouse_over_event ) {
				widget->mouse_over_event(widget, pevt);
			}
			break;

		case CWT_EVT_WIDGET_LEAVE:
			widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
			JQ_ASSERT(widget);
			JQ_ASSERT(widget->magic == CWT_MAGIC);

			if( !_cwt_widget_deleting(widget) ) {
				if( cwt_widget_is_focused(widget) ) {
					_cwt_widget_change_state(widget, CWT_STATE_FOCUS);
				} else {
					_cwt_widget_change_state(widget, CWT_STATE_NORMAL);
				}

				if( widget->mouse_out_event ) {
					widget->mouse_out_event(widget, pevt);
				}
			}
			break;

		case CWT_EVT_INFOCUS:
			widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
			JQ_ASSERT(widget);
			JQ_ASSERT(widget->magic == CWT_MAGIC);

			if( !_cwt_widget_deleting(widget) && widget->focus_in_event )
				widget->focus_in_event(widget, pevt);

			/*printf_debug("CWT_EVT_INFOCUS: wtype=%d, id=%u", widget->wtype, widget->id);*/
			break;

		case CWT_EVT_OUTFOCUS:
			widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
			JQ_ASSERT(widget);
			JQ_ASSERT(widget->magic == CWT_MAGIC);

			if( widget->focus_out_event )
				widget->focus_out_event(widget, pevt);

			/*printf_debug("CWT_EVT_OUTFOCUS: wtype=%d, id=%u", widget->wtype, widget->id);*/
			break;

		case CWT_EVT_CHILD:
			widget = CWT_CAST(CWT_CAST(pevt->ev_data, CWT_CHILD_EVENT*)->child, CWT_WIDGET_PTR);

			JQ_ASSERT(widget);
			JQ_ASSERT(widget->magic == CWT_MAGIC);
			JQ_ASSERT(widget->parent);

			if( widget->parent->child_event )
				widget->parent->child_event(widget, pevt);
			break;


		case CWT_EVT_WINDOW_ACTIVATE:
			widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
			JQ_ASSERT(widget);
			JQ_ASSERT(widget->magic == CWT_MAGIC);

			if( !_cwt_widget_deleting(widget) )
				_cwt_set_active_window(widget);

			break;

		default:
			break;
	}

	return TRUE;
}

