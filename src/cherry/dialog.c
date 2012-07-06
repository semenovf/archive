/*
 * dialog.c
 *
 *  Created on: 26.02.2012
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>
#include <cwt/logger.h>


static BOOL _event_handler(CWT_EVENT_PTR pevt)
{
	CWT_WIDGET_PTR dlg = cwt_active_window();

	switch( pevt->ev_type ) {
		case CWT_EVT_KEYBOARD: {
			UINT16 key = CWT_KBD_KEY(pevt);
			/*UINT16 mods = CWT_KBD_MODS(pevt);*/

			if( CWT_KBD_KEY_SCAN(key) == CWT_Key_Esc ) {
				JQ_ASSERT(dlg && dlg->wtype == CWT_WT_DIALOG );
				cwt_dialog_reject(dlg);
				return TRUE;
			}}
			break;

		case CWT_EVT_MOUSE_MOVE:
		case CWT_EVT_MOUSE_PRESS:
		case CWT_EVT_MOUSE_RELEASE:
			if( !_cwt_widget_child_at(dlg, CWT_MOUSE_POS_X(pevt), CWT_MOUSE_POS_Y(pevt)) )
				return TRUE;
			break;

		default:
			break;
	}

	return FALSE;
}

void _cwt_init_dialog(CWT_WIDGET_PTR widget)
{
	CwtDialogPtr dlg = CWT_CAST(widget, CwtDialogPtr);
	widget->event_handler = _event_handler;
	dlg->accept = FALSE;
	dlg->rc = 0;
	dlg->on_dialog_done = NULL;
	dlg->active_window = NULL;
}


void cwt_dialog_open(CWT_WIDGET_PTR widget)
{
	CwtDialogPtr dlg = CWT_CAST(widget, CwtDialogPtr);
	dlg->active_window = cwt_active_window();
	cwt_set_active_window(widget);
	cwt_widget_show(widget);
}

static void _finalize(CwtDialogPtr dlg)
{
	/* TODO костыль */
	if( dlg->active_window ) {
	    cwt_set_active_window(dlg->active_window);
	}

	cwt_widget_hide((CWT_WIDGET_PTR)dlg);
	if( dlg->on_dialog_done )
		dlg->on_dialog_done((CWT_WIDGET_PTR)dlg);

}

void cwt_dialog_done(CWT_WIDGET_PTR widget, UINT16 rc)
{
	CwtDialogPtr dlg = CWT_CAST(widget, CwtDialogPtr);
	dlg->rc = rc;
	_finalize(dlg);
}

void cwt_dialog_accept(CWT_WIDGET_PTR widget)
{
	CwtDialogPtr dlg = CWT_CAST(widget, CwtDialogPtr);
	dlg->accept = TRUE;
	_finalize(dlg);
}

void cwt_dialog_reject(CWT_WIDGET_PTR widget)
{
	CwtDialogPtr dlg = CWT_CAST(widget, CwtDialogPtr);
	dlg->accept = FALSE;
	_finalize(dlg);
}


UINT16 cwt_dialog_result(CWT_WIDGET_PTR dialog)
{
	return CWT_CAST(dialog, CwtDialogPtr)->rc;
}

BOOL cwt_dialog_accepted(CWT_WIDGET_PTR dialog)
{
	return CWT_CAST(dialog, CwtDialogPtr)->accept;
}

BOOL cwt_dialog_rejected(CWT_WIDGET_PTR dialog)
{
	return CWT_CAST(dialog, CwtDialogPtr)->accept ? FALSE : TRUE;
}

void cwt_dialog_set_hook(CWT_WIDGET_PTR widget, void (*hook)(CWT_WIDGET_PTR))
{
	JQ_ASSERT(widget);
	CWT_CAST(widget, CwtDialogPtr)->on_dialog_done = hook;
}
