/*
 * menu.c
 *
 *  Created on: 18.01.2012
 *      Author: wladt
 *
 *  Menu
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>
#include <cwt/cherry/mouse.h>
#include <cwt/logger.h>


static void _finalize_menu(CWT_WIDGET_PTR menu_widget)
{
	JQ_ASSERT(menu_widget);
	JQ_ASSERT(menu_widget->magic == CWT_MAGIC);
	JQ_ASSERT(menu_widget->wtype == CWT_WT_MENU || menu_widget->wtype == CWT_WT_MENUBAR);

	if( menu_widget->wtype == CWT_WT_MENU ) {
		CWT_MENU_PTR menu = CWT_CAST(menu_widget, CWT_MENU_PTR);
/*
		cwt_mouse_hide();
		_cwt_put_image( menu_widget->abs.x, menu_widget->abs.y, menu->saved_image);
		cwt_mouse_show();
*/

		cwt_widget_hide(menu_widget);
		cwt_set_active_window(menu->active_window);
	} else {
		cwt_set_active_window(CWT_CAST(menu_widget, CWT_MENUBAR_PTR)->active_window);
	}

	_cwt_widget_focus_out(menu_widget->focused);
}



static void _finalize_menus(CWT_WIDGET_PTR menu_widget)
{
	CWT_MENU_PTR menu = CWT_CAST(menu_widget, CWT_MENU_PTR);
	JQ_ASSERT(menu_widget);
	JQ_ASSERT(menu_widget->magic == CWT_MAGIC);

	while( menu_widget->wtype == CWT_WT_MENU  ) {
		_finalize_menu(menu_widget);
		if( menu->parent_action ) {
			menu_widget = menu->parent_action->parent;
		} else {
			break;
		}
	}
	if( menu_widget->wtype == CWT_WT_MENUBAR )
		_finalize_menu(menu_widget);
}


static BOOL _event_handler(CWT_EVENT_PTR pevt)
{
	CWT_WIDGET_PTR widget;
	CWT_WIDGET_PTR menu = cwt_active_window();

	JQ_ASSERT(menu);
	JQ_ASSERT(menu->magic == CWT_MAGIC);

	switch( pevt->ev_type ) {
		case CWT_EVT_QUIT:
			_finalize_menu(menu);
			return TRUE;

		case CWT_EVT_KEYBOARD: {
			UINT16 key = CWT_KBD_KEY(pevt);
			UINT16 mods = CWT_KBD_MODS(pevt);

			switch( CWT_KBD_KEY_SCAN(key) ) {
				case CWT_Key_Esc:
					_finalize_menu(menu);
					return TRUE;

				case CWT_Key_Left:
				case CWT_Key_Up:
				case CWT_Key_Right:
				case CWT_Key_Down:
					if( CWT_KBD_NO_MODS(mods) ) {
						CWT_WIDGET_PTR focused_widget;
						BOOL forward_focus = TRUE;

						if( CWT_KBD_KEY_SCAN(key) == CWT_Key_Left || CWT_KBD_KEY_SCAN(key) == CWT_Key_Up )
							forward_focus = FALSE;

						focused_widget = _cwt_widget_next_focused(menu, forward_focus);

						JQ_ASSERT(focused_widget);

						_cwt_widget_set_focused(focused_widget, CWT_FOCUS_REASON_TAB);
					}

					return TRUE;

				case CWT_Key_Enter:
				case CWT_Key_Space:
					if( CWT_KBD_NO_MODS(mods) ) {
						return FALSE; /* do default action for button */
					}

				default:
					break;/*return TRUE; */

			}
			}
			return TRUE; /* Do not need more process keyboard event */


		case CWT_EVT_MOUSE_RELEASE: {
				widget = _cwt_widget_child_at(menu, CWT_MOUSE_POS_X(pevt), CWT_MOUSE_POS_Y(pevt));
				if( !widget ) {
					_finalize_menu(menu);
					cwt_post_mouse_release_event(CWT_MOUSE_STATUS(pevt), CWT_MOUSE_POS_X(pevt), CWT_MOUSE_POS_Y(pevt));
					return TRUE;
				}
			}
			break;

#if 0
		case CWT_EVT_WIDGET_ENTER:
			widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
			if( widget->parent == menu && menu->wtype == CWT_WT_MENUBAR ) {
				_cwt_widget_focus_in(widget, CWT_FOCUS_REASON_MOUSE);
				_finalize_menu(menu);

				if( CWT_CAST(widget, CWT_BUTTON_PTR)->clicked )
					CWT_CAST(widget, CWT_BUTTON_PTR)->clicked(widget);
				return TRUE;
			}
			break;

		case CWT_EVT_WIDGET_LEAVE:
			widget = CWT_CAST(pevt->ev_data, CWT_WIDGET_PTR);
			if( widget->parent == menu && menu->wtype == CWT_WT_MENUBAR ) {
				_cwt_widget_focus_out(widget);
			}
			break;
#endif
		default:
			break;
	}

	return FALSE;
}



BOOL _cwt_action_key_press_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	CWT_WIDGET_PTR menu_widget = widget->parent; /*cwt_widget_top_active();*/
	UINT16 key = CWT_KBD_KEY(pevt);
	UINT16 mods = CWT_KBD_MODS(pevt);

	JQ_ASSERT(menu_widget);
	JQ_ASSERT(menu_widget->magic == CWT_MAGIC);
	JQ_ASSERT(menu_widget->wtype == CWT_WT_MENU);

	if( CWT_KBD_NO_MODS(mods) ) {
		switch( CWT_KBD_KEY_SCAN(key) ) {
			case CWT_Key_Enter:
			case CWT_Key_Space:
				_finalize_menus(menu_widget);
				break;
			default:
				break;
		}
	}

	if( CWT_CAST(widget, CWT_BUTTON_PTR)->clicked )
		CWT_CAST(widget, CWT_BUTTON_PTR)->clicked(widget);
	return TRUE;
/*
	return _cwt_button_key_press_event(widget, pevt);*/
}


BOOL _cwt_action_mouse_up_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	CWT_WIDGET_PTR menu_widget;

	JQ_UNUSED(pevt);
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->parent);
	menu_widget = widget->parent;

	_finalize_menus(menu_widget);

	if( CWT_CAST(widget, CWT_BUTTON_PTR)->clicked )
		CWT_CAST(widget, CWT_BUTTON_PTR)->clicked(widget);
	return TRUE;
	/*return _cwt_button_mouse_up_event(widget, pevt);*/
}


static BOOL _cwt_menu_child_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	JQ_ASSERT(pevt);

	if( CWT_CAST(pevt, CWT_CHILD_EVENT*)->added ) {
		widget->key_press_event = _cwt_action_key_press_event;
		widget->mouse_up_event = _cwt_action_mouse_up_event;
	}
	return TRUE;
}


void _cwt_init_menu(CWT_WIDGET_PTR widget)
{
	CWT_MENU_PTR menu = CWT_CAST(widget, CWT_MENU_PTR);
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	menu->active_window = NULL;
	menu->saved_image = NULL;
	widget->layout = _cwt_layout_menu;
	widget->event_handler = _event_handler;
	widget->child_event = _cwt_menu_child_event;

	CWT_CAST(widget, CWT_MENU_PTR)->parent_action = NULL;
}

void _cwt_init_menubar(CWT_WIDGET_PTR widget)
{
	CWT_MENUBAR_PTR menubar = CWT_CAST(widget, CWT_MENUBAR_PTR);

 	JQ_ASSERT(widget);
 	JQ_ASSERT(widget->magic == CWT_MAGIC);

 	menubar->active_window = NULL;
	widget->event_handler  = _event_handler;
}


void _cwt_destroy_menu(CWT_WIDGET_PTR widget)
{
	JQ_UNUSED(widget);
/*
	CWT_MENU_PTR menu = CWT_CAST(widget, CWT_MENU_PTR);
	if( menu->saved_image ) {
		JQ_FREE(menu->saved_image);
		menu->saved_image = NULL;
	}
*/
}

void _cwt_destroy_menubar(CWT_WIDGET_PTR widget)
{
	JQ_UNUSED(widget);
}

void _cwt_layout_menu(CWT_WIDGET_PTR widget)
{
	DListIterator dt;
	int height = 0;
	CWT_SIZE inner_box;
	CWT_SIZE sz;
/*	CWT_MENU_PTR menu = CWT_CAST(widget, CWT_MENU_PTR);*/

	/* MainWin inner */
	cwt_widget_inner_size(widget, &inner_box);

	dlist_begin(&widget->children, &dt);

	while( dlist_has_more(&dt) ) {
		CWT_WIDGET_PTR action = (CWT_WIDGET_PTR)dlist_next(&dt);
		_cwt_widget_moveto(action, 0, height);
		_cwt_widget_resize_min(action);
		_cwt_widget_set_width(action, inner_box.width);
		_cwt_widget_recalc_abs(action);

		height += cwt_widget_height(action);
	}

	_cwt_widget_set_height(widget, height);

	cwt_widget_size(widget, &sz);

/*	cwt_mouse_hide();*/
	/*menu->saved_image = cwt_getimage(widget->abs.x, widget->abs.y, sz.width, sz.height);*//* _cwt_get_image(widget->abs.x, widget->abs.y, sz.width, sz.height);*/
/*	cwt_mouse_show();*/
}

void _cwt_render_menu(CWT_WIDGET_PTR widget)
{
	JQ_UNUSED(widget);
}

void _cwt_render_menubar(CWT_WIDGET_PTR widget)
{
	JQ_UNUSED(widget);
}

void cwt_menu_set_parent_action(CWT_WIDGET_PTR menu, CWT_WIDGET_PTR action)
{
	JQ_ASSERT(menu);
	JQ_ASSERT(menu->magic == CWT_MAGIC);
	JQ_ASSERT(menu->wtype == CWT_WT_MENU);

	CWT_CAST(menu, CWT_MENU_PTR)->parent_action = action;
}

void cwt_menu_show(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(cwt_active_window());
	JQ_ASSERT(cwt_active_window()->magic == CWT_MAGIC);

	cwt_widget_show(widget);
	_cwt_widget_set_focused(cwt_widget_first_child(widget), CWT_FOCUS_REASON_OTHER);
}
