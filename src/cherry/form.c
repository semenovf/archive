/*
 * form.c
 *
 *  Created on: 22.01.2012
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>



#if 0
static BOOL _on_key_press(CWT_WIDGET_PTR widget, UINT16 key, UINT16 mods)
{
	JQ_ASSERT(widget);

	if( widget->nchilds > 0 ) {

		if( /*KBD_NO_MODS(mods) && */key == CWT_Key_Tab ) {
			CWT_WIDGET_PTR child = (CWT_WIDGET_PTR)widget->children.first;

			child = (CWT_WIDGET_PTR)child->base.next;

			while( child ) {
				if( child && cwt_wcan_focused(child) ) {
					cwt_wset_active(child);
					return TRUE;
				}
				child = (CWT_WIDGET_PTR)child->base.next;
			}
			return TRUE;
		} else if( KBD_SHIFT(mods) && key == CWT_Key_Tab ) {
			CWT_WIDGET_PTR child = (CWT_WIDGET_PTR)widget->children.last;

			while( child ) {
				if( child && cwt_wcan_focused(child) ) {
					cwt_wset_active(child);
					return TRUE;
				}
				child = (CWT_WIDGET_PTR)child->base.prev;
			}
			return TRUE;
		}
	}

	return FALSE;
}

#endif

/*
void _cwt_init_form(CWT_WIDGET_PTR widget)
{
	w->on_key_press = _on_key_press;
}
*/


/*
void _cwt_render_form(CWT_WIDGET_PTR w)
{
	JQ_UNUSED(w);
}
*/
