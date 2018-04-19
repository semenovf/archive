/*
 * spinbox.c
 *
 *  Created on: 14.02.2012
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>
#include <cwt/string.h>


static BOOL _spinbox_prev_item(CWT_WIDGET_PTR widget);
static BOOL _spinbox_next_item(CWT_WIDGET_PTR widget);


BOOL _cwt_spinbox_key_press_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt)
{
	UINT16 key = CWT_KBD_KEY(pevt);
	UINT16 mods = CWT_KBD_MODS(pevt);

	if( CWT_KBD_NO_MODS(mods) ) {
		switch( CWT_KBD_KEY_SCAN(key) ) {
			case CWT_Key_Up:
				_spinbox_prev_item(widget);
				return TRUE;
			case CWT_Key_Down:
				_spinbox_next_item(widget);
				return TRUE;
			default:
				break;
		}
	}
	return FALSE;
}

static void _spinbox_layout(CWT_WIDGET_PTR widget)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget, CWT_SPINBOX_PTR);
	CWT_SIZE inner_box;
	int border_right;
	/*int state;*/
	int btn_width, btn_height;

	JQ_ASSERT(widget);
	JQ_ASSERT(spb->label);
	JQ_ASSERT(spb->btn_prev);
	JQ_ASSERT(spb->btn_next);

	if( !(widget->flags & CWT_WF_LAYOUT) )
		return;

	border_right = cwt_style_border(cwt_widget_style(widget, widget->state))->right;
	cwt_widget_inner_size(widget, &inner_box);


	btn_height = inner_box.height/2;
	btn_width  = btn_height + btn_height * 2/3;

	_cwt_widget_resize(spb->label, inner_box.width - btn_width - border_right, inner_box.height);
	_cwt_widget_resize(spb->btn_prev, btn_width, btn_height);
	_cwt_widget_resize(spb->btn_next, btn_width, btn_height);

	_cwt_widget_moveto(spb->label, 0, 0);
	_cwt_widget_moveto(spb->btn_prev, inner_box.width - btn_width, 0);

	if( btn_height * 2 < inner_box.height )
		_cwt_widget_moveto(spb->btn_next, inner_box.width - btn_width, btn_height+1);
	else
		_cwt_widget_moveto(spb->btn_next, inner_box.width - btn_width, btn_height);

	_cwt_widget_recalc_abs(spb->label);
	_cwt_widget_recalc_abs(spb->btn_prev);
	_cwt_widget_recalc_abs(spb->btn_next);

}


static BOOL _spinbox_prev_item(CWT_WIDGET_PTR widget)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget, CWT_SPINBOX_PTR);

	if( spb->current->prev ) {
		spb->current = spb->current->prev;
		cwt_label_set_text(spb->label
			, CWT_CAST(spb->current, CWT_SPINBOX_ITEM*)->text);
		if( spb->on_item_changed )
			spb->on_item_changed(widget);
		return TRUE;
	}
	return FALSE;
}


static BOOL _spinbox_next_item(CWT_WIDGET_PTR widget)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget, CWT_SPINBOX_PTR);

	if( spb->current->next ) {
		spb->current = spb->current->next;
		cwt_label_set_text(spb->label
			, CWT_CAST(spb->current, CWT_SPINBOX_ITEM*)->text);
		if( spb->on_item_changed )
			spb->on_item_changed(widget);
		return TRUE;
	}
	return FALSE;
}


static void _spinbox_prev_clicked(CWT_WIDGET_PTR widget)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget->parent, CWT_SPINBOX_PTR);

	if( _spinbox_next_item(widget->parent) )
		cwt_widget_update(spb->label);
}


static void _spinbox_next_clicked(CWT_WIDGET_PTR widget)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget->parent, CWT_SPINBOX_PTR);

	if( _spinbox_prev_item(widget->parent) )
		cwt_widget_update(spb->label);
}



static void _button_prev_painter(CWT_WIDGET_PTR widget)
{
	CWT_SIZE sz;
	CWT_POINT points[3];

	cwt_widget_inner_size(widget, &sz);

	points[0].x = 2;
	points[0].y = sz.height-2;
	points[1].x = sz.width-2;
	points[1].y = sz.height-2;
	points[2].x = sz.width/2;
	points[2].y = 2;

	_cwt_set_color(CWT_White); /* border color */
	_cwt_set_fill_style(SOLID_FILL, CWT_White);
	_cwt_fill_polygon(points, 3);
}

static void _button_next_painter(CWT_WIDGET_PTR widget)
{
	CWT_SIZE sz;
	CWT_POINT points[3];

	cwt_widget_inner_size(widget, &sz);

	points[0].x = sz.width-2;
	points[0].y = 2;
	points[1].x = sz.width/2;
	points[1].y = sz.height-2;
	points[2].x = 2;
	points[2].y = 2;

	_cwt_set_color(CWT_White); /* border color */
	_cwt_set_fill_style(SOLID_FILL, CWT_White);
	_cwt_fill_polygon(points, 3);
}


void _cwt_init_spinbox(CWT_WIDGET_PTR widget)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget, CWT_SPINBOX_PTR);

	widget->key_press_event = _cwt_spinbox_key_press_event;
	widget->focus_policy = CWT_FP_STRONG;
	widget->layout = _spinbox_layout;
	spb->label = cwt_new_widget(0, CWT_WT_LABEL, widget);
	spb->btn_prev = cwt_new_widget(0, CWT_WT_BUTTON, widget);
	spb->btn_next = cwt_new_widget(0, CWT_WT_BUTTON, widget);
	spb->items.first = NULL;
	spb->items.last = NULL;
	spb->current = NULL;
	spb->on_item_changed = NULL;


	cwt_button_set_painter(spb->btn_prev, _button_prev_painter);
	cwt_button_set_painter(spb->btn_next, _button_next_painter);
	CWT_CAST(spb->btn_prev, CWT_BUTTON_PTR)->clicked = _spinbox_prev_clicked;
	CWT_CAST(spb->btn_next, CWT_BUTTON_PTR)->clicked = _spinbox_next_clicked;
}


void _cwt_destroy_spinbox(CWT_WIDGET_PTR widget)
{
	DListIterator it;

	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	dlist_begin(&widget->children, &it);
	while( dlist_has_more(&it) ) {
		CWT_SPINBOX_ITEM* item = CWT_CAST(dlist_next(&it), CWT_SPINBOX_ITEM*);
		if( item->text ) {
			JQ_FREE(item->text);
			item->text = NULL;
		}
		JQ_FREE(item);
	}
}


CWT_WIDGET_PTR _cwt_clone_spinbox(CWT_WIDGET_PTR orig, CWT_WIDGET_PTR clone)
{
	CWT_SPINBOX_PTR spb_orig = CWT_CAST(orig, CWT_SPINBOX_PTR);
	CWT_SPINBOX_PTR spb_clone = CWT_CAST(clone, CWT_SPINBOX_PTR);
	DListNode *node = spb_orig->items.first;

	JQ_ASSERT(orig);
	JQ_ASSERT(clone);

	while( node ) {
		cwt_spinbox_add_item(clone
			, CWT_CAST(node, CWT_SPINBOX_ITEM*)->id
			, CWT_CAST(node, CWT_SPINBOX_ITEM*)->text);

		if( spb_orig->current == node )
			spb_clone->current = spb_clone->items.last;

		node = node->next;
	}
	spb_clone->label = cwt_clone_widget(0, spb_orig->label);
	spb_clone->btn_prev = cwt_clone_widget(0, spb_orig->btn_prev);
	spb_clone->btn_next = cwt_clone_widget(0, spb_orig->btn_next);
	return clone;
}


void _cwt_set_style_spinbox(CWT_WIDGET_PTR widget, CWT_STATE state, CWT_STYLE_PTR style)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget, CWT_SPINBOX_PTR);
	CWT_STYLE_PTR style_label, style_btn;

	JQ_ASSERT(widget);
	JQ_ASSERT(spb->label);
	JQ_ASSERT(spb->btn_prev);
	JQ_ASSERT(spb->btn_next);

	style_label = cwt_clone_style(style);
	cwt_style_set_border(style_label, 0, 0, 0, 0, 0);
	cwt_assign_style(&spb->label->styles[state], style_label);

	cwt_style_set_padding(style, 0, 0, 0, 0);
	cwt_assign_style(&widget->styles[state], style);

	style_btn = cwt_clone_style(style);
	cwt_style_set_border(style_btn, CWT_White, 0, 0, 0, 0);
	cwt_style_set_padding(style_btn, 0, 0, 0, 0);
	cwt_assign_style(&spb->btn_prev->styles[state], style_btn);
	cwt_assign_style(&spb->btn_next->styles[state], style_btn);
}

void _cwt_render_spinbox(CWT_WIDGET_PTR widget)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget, CWT_SPINBOX_PTR);
	CWT_SIZE inner_box;
	const CWT_BORDER *border;
	int left, top, width, height;

	JQ_ASSERT(widget);

	border = cwt_style_border(cwt_widget_style(widget, widget->state));
	cwt_widget_inner_size(widget, &inner_box);

	/* перегородка между текстом и кнопками */
	_cwt_set_fill_style(SOLID_FILL, border->color);


	left = cwt_widget_width(spb->label);
	top = 0;
	width = border->right;
	height = cwt_widget_height(spb->label);
	_cwt_bar( left, top, width, height );
}


void cwt_spinbox_add_item(CWT_WIDGET_PTR widget, UINT16 id, const CWT_CHAR* text)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget, CWT_SPINBOX_PTR);
	CWT_SPINBOX_ITEM *item = JQ_MALLOC(CWT_SPINBOX_ITEM);

	JQ_ASSERT(widget);

	item->text = strdup(text);
	item->id = id;
	dlist_insert_last((DList*)&spb->items, (DListNode*)item);

	if( !spb->current ) {
		spb->current = spb->items.first;
		cwt_label_set_text(spb->label, CWT_CAST(spb->current, CWT_SPINBOX_ITEM*)->text);
	}
}


/**
 * Return current item's text
 *
 * @param widget SpinBox widget
 * @return current item's text
 */
const CWT_CHAR* cwt_spinbox_text(CWT_WIDGET_PTR widget)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget, CWT_SPINBOX_PTR);
	JQ_ASSERT(widget);
	return spb->current ? CWT_CAST(spb->current, CWT_SPINBOX_ITEM*)->text : NULL;
}


/**
 * Return current item's id
 *
 * @param widget SpinBox widget
 * @return current item's id
 */
UINT16 cwt_spinbox_id(CWT_WIDGET_PTR widget)
{
	CWT_SPINBOX_PTR spb = CWT_CAST(widget, CWT_SPINBOX_PTR);
	JQ_ASSERT(widget);
	return spb->current ? CWT_CAST(spb->current, CWT_SPINBOX_ITEM*)->id : 0;
}
