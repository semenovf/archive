/*
 * msgbox.c
 *
 *  Created on: 26.02.2012
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>



static void _on_btn_ok(CWT_WIDGET_PTR w);
static void _on_btn_cancel(CWT_WIDGET_PTR w);
static void _on_btn_yes(CWT_WIDGET_PTR w);
static void _on_btn_no(CWT_WIDGET_PTR w);

static const CWT_CHAR* __BUTTON_CAPTIONS[] = {
	  "Ok"
	, "Cancel"
	, "Yes"
	, "No"
};

static CWT_BUTTON_CLICK_CALLBACK __BUTTON_CALLBACKS[] = {
	  _on_btn_ok
	, _on_btn_cancel
	, _on_btn_yes
	, _on_btn_no
};


void _on_btn_ok(CWT_WIDGET_PTR w)
{
	_cwt_dialog_done(w, MSGBOX_OK);
	cwt_dialog_accept(w);
	stop_event_loop();
}

void _on_btn_cancel(CWT_WIDGET_PTR w)
{
	_cwt_dialog_done(w, MSGBOX_CANCEL);
	cwt_dialog_reject(w);
	stop_event_loop();
}

void _on_btn_yes(CWT_WIDGET_PTR w)
{
	_cwt_dialog_done(w, MSGBOX_YES);
	cwt_dialog_accept(w);
	stop_event_loop();
}

void _on_btn_no(CWT_WIDGET_PTR w)
{
	_cwt_dialog_done(w, MSGBOX_NO);
	cwt_dialog_reject(w);
	stop_event_loop();
}


void _cwt_layout_msgbox(CWT_WIDGET_PTR widget)
{
	CWT_PMSGBOX mb = CWT_CAST(widget, CWT_PMSGBOX);

	if( widget->need_layout ) {
		int max_button_width = 0, max_button_height = 0;
		int button_left;
		int mb_width = 0;
		int mb_height = 0;
		int nbuttons = 0;
		CWT_WIDGET_PTR child;
		CWT_SIZE sz;

		child = (CWT_WIDGET_PTR)widget->children.first;
		while( child ) {
			if( child->wtype == CWT_WT_LABEL ) {
				cwt_wmoveto(child, 0, mb_height);
				cwt_wresize_min(child);
				cwt_wsize(child, &sz);
				cwt_wresize_insets(child, &sz);
				mb_height += cwt_wheight(child);
				mb_height += mb->vspace;
				mb_width = JQ_MAX(mb_width, cwt_wwidth(child));
			} else if( child->wtype == CWT_WT_BUTTON ) {
				cwt_wresize_min(child);
				cwt_wsize(child, &sz);
				cwt_wresize_insets(child, &sz);
				max_button_width = JQ_MAX(max_button_width, cwt_wwidth(child));
				max_button_height = JQ_MAX(max_button_height, cwt_wheight(child));
			}

			child = (CWT_WIDGET_PTR)child->base.next;
		}

		mb_height += mb->vspace;


		button_left = 0;

		child = (CWT_WIDGET_PTR)widget->children.first;

		while( child ) {
			if( child->wtype == CWT_WT_BUTTON ) {
				cwt_wresize_wh(child, max_button_width, max_button_height);
				cwt_wmoveto(child, button_left, mb_height);
				button_left += cwt_wwidth(child) + mb->hspace;
				nbuttons++;
			}

			child = (CWT_WIDGET_PTR)child->base.next;
		}

		button_left -= mb->hspace;

		mb_height += max_button_height;

		mb_width = JQ_MAX(mb_width, button_left);
		cwt_wresize_insets_wh(widget, mb_width, mb_height);

		/* Align buttons {{{ */
		cwt_wcontent_size(widget, &sz);

		switch( mb->buttons_align ) {
			case CWT_ALIGN_LEFT:
				button_left = 0;
				break;
			case CWT_ALIGN_CENTER:
				button_left = sz.width/2 - (max_button_width*nbuttons + mb->hspace *(nbuttons-1))/2;
				break;
			case CWT_ALIGN_RIGHT:
				button_left = sz.width - (max_button_width*nbuttons + mb->hspace *(nbuttons-1));
				break;
		}

		if( button_left > 0 ) {
			child = (CWT_WIDGET_PTR)widget->children.first;

			while( child ) {
				if( child->wtype == CWT_WT_BUTTON ) {
					cwt_wmoveto(child, cwt_wleft(child) + button_left, cwt_wtop(child));
				}
				child = (CWT_WIDGET_PTR)child->base.next;
			}
		}

		/* }} Align buttons */

		if( widget->parent ) {
			cwt_wcontent_size(widget->parent, &sz);
			cwt_wmoveto(widget
				, (sz.width - cwt_wwidth(widget))/2
				, (sz.height - cwt_wheight(widget))/2);
		}

		widget->need_layout = FALSE;
	} /* need_layout */
}


void _cwt_init_msgbox(CWT_WIDGET_PTR widget)
{
	CWT_PMSGBOX mb = CWT_CAST(widget, CWT_PMSGBOX);
	_cwt_init_dialog(widget);
	widget->layout = _cwt_layout_msgbox;
	mb->vspace = 3;
	mb->hspace = 5;
}

void _cwt_destroy_msgbox(CWT_WIDGET_PTR widget)
{
	JQ_UNUSED(widget);
}


void _cwt_render_msgbox(CWT_WIDGET_PTR widget)
{
	JQ_UNUSED(widget);
}


/**
 *
 * @param widget
 * @param msg
 */
void cwt_msgbox_set_msg(CWT_WIDGET_PTR widget, const CWT_CHAR* msg)
{
	if( msg ) {
		CWT_WIDGET_PTR label;
		const CWT_CHAR *ptr = msg;
		StringBufferPtr sb;

		sb = strbuf_new_defaults();

		while( *ptr ) {
			if( *ptr == '\n' ) {
				label = cwt_new_widget(0, CWT_WT_LABEL, widget);
				cwt_label_set_text(label, strbuf_cstr(sb));
				strbuf_clear(sb);
			} else {
				strbuf_append_char(sb, *ptr);
			}
			ptr++;
		}

		if( strbuf_size(sb) > 0 ) {
			label = cwt_new_widget(0, CWT_WT_LABEL, widget);
			cwt_label_set_text(label, strbuf_cstr(sb));
		}

		strbuf_delete(sb);
		widget->need_layout = TRUE;
	}
}


/**
 *
 * @param widget
 * @param buttons
 */
void cwt_msgbox_set_buttons(CWT_WIDGET_PTR widget, UINT16 buttons, CWT_ALIGN align, UINT16 default_button)
{
	int i;
	CWT_PMSGBOX mb = CWT_CAST(widget, CWT_PMSGBOX);
	CWT_WIDGET_PTR btn;

	mb->buttons_align = align;
	mb->default_button = default_button;

	for( i = 0; i < sizeof(__BUTTON_CAPTIONS)/sizeof(__BUTTON_CAPTIONS[0]); i++ ) {
		if( buttons & (1 << i) ) {
			btn = cwt_new_widget(0, CWT_WT_BUTTON, widget);
			cwt_button_set_text(btn, __BUTTON_CAPTIONS[i]);
			CWT_CAST(btn, CWT_BUTTON_PTR)->clicked = __BUTTON_CALLBACKS[i];
		}
	}

	widget->need_layout = TRUE;
}


