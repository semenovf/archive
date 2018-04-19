/*
 * cherry.c
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 */


#include <stdlib.h>
#include <string.h>
#include <cwt/cherry/cherry.h>
#include <cwt/cherry/mouse.h>
#include <cwt/logger.h>


CwtStylePtr cwt_widget_style(CWT_WIDGET_PTR widget, CWT_STATE state)
{
	CwtStylePtr style;
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	style = widget->styles[state];

	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);

	while( widget ) {
		if( !_cwt_style_eq_default(style) )
			break;

		if( widget == cwt_screen() )
			break;

		if( !widget->parent )
			break;

		widget = widget->parent;
		style = widget->styles[state];
	}

	return style;
}



void cwt_widget_set_focus_policy(CWT_WIDGET_PTR widget, CWT_FOCUS_POLICY focus_policy)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	widget->focus_policy = focus_policy;
}

/**
 * Sets style for the widget
 *
 * @param widget widget
 * @param state widget's state for which set style
 * @param pstyle style
 *
 * If @c pstyle is null and @c state is not @c CWT_STATE_NORMAL,
 * then style is cleared for specified state
 */
void cwt_widget_set_style(CWT_WIDGET_PTR widget, CWT_STATE state, CwtStylePtr style)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	JQ_ASSERT(style);
	JQ_ASSERT(style->magic == CWT_MAGIC);

	_cwt_widget_set_style(widget, state, style);
	_cwt_widget_recalc_abs(widget);
	cwt_widget_update(widget);
}


void cwt_widget_set_width(CWT_WIDGET_PTR widget, int width)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	_cwt_widget_set_width(widget, width);
	cwt_widget_update(widget);
}

void cwt_widget_set_height(CWT_WIDGET_PTR widget, int height)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	_cwt_widget_set_height(widget, height);
	cwt_widget_update(widget);
}

void cwt_widget_set_layout(CWT_WIDGET_PTR widget, void (*layout)(CWT_WIDGET_PTR))
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	widget->layout = layout;
	widget->flags  |= CWT_WF_LAYOUT;
}

/**
 * Returns widget's size
 *
 * @param widget
 * @param sz
 */
void cwt_widget_size(CWT_WIDGET_PTR widget, CWT_SIZE *sz)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	JQ_ASSERT(sz);
	sz->width = widget->geometry.width;
	sz->height = widget->geometry.height;

}

int cwt_widget_left(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	return widget->geometry.left;
}

int cwt_widget_top(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	return widget->geometry.top;
}


int cwt_widget_width(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	return widget->geometry.width;
}

int cwt_widget_height(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	return widget->geometry.height;
}

int cwt_widget_abs_left(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	return widget->abs.x;
}

int cwt_widget_abs_top(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	return widget->abs.y;
}


void cwt_widget_geometry(CWT_WIDGET_PTR widget, CWT_RECT *rect)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	JQ_ASSERT(rect);

	rect->left = widget->geometry.left;
	rect->top = widget->geometry.top;
	rect->width = widget->geometry.width;
	rect->height = widget->geometry.height;
}

/**
 * @short Return widget's geometry in absolute screen coordinates
 *
 * @param widget widget
 * @param rect widget's geometry in absolute screen coordinates
 */
void cwt_widget_abs_geometry(CWT_WIDGET_PTR widget, CWT_RECT *rect)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	JQ_ASSERT(rect);

	rect->left = widget->abs.x;
	rect->top = widget->abs.y;
	rect->width = widget->geometry.width;
	rect->height = widget->geometry.height;
}

/**
 * Returns widget's content region size
 * (widget's widths and height excluding border and padding)
 *
 * @param widget
 * @param sz
 */
void cwt_widget_inner_size(CWT_WIDGET_PTR widget, CWT_SIZE *sz)
{
	CwtStylePtr style;
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	JQ_ASSERT(sz);

	style = _CWT_WIDGET_STYLE(widget);

	sz->width = widget->geometry.width
		- style->border.left - style->border.right
		- style->padding.left - style->padding.right;

	sz->height = widget->geometry.height
			- style->border.top - style->border.bottom
			- style->padding.top - style->padding.bottom;
}


void cwt_widget_repaint(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	if(!_cwt_widget_deleting(widget)) {
		if( !(widget->flags & CWT_WF_HIDDEN) ) {
			cwt_mouse_hide();
			_cwt_widget_repaint(widget);
			cwt_mouse_show();
		}
	}
}


void cwt_widget_update(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	if( cwt_widget_visible(widget) && !_cwt_widget_invalid(widget) && !_cwt_widget_deleting(widget)) {
		DListIterator it;
		CWT_WIDGET_PTR w;
		CWT_WIDGET_PTR parent = widget->parent;

		/*_cwt_widget_dump(widget, "WIDGET_UPDATE:");*/

		if( parent ) {
			/* If any children is intersect update parent */
			dlist_begin(&parent->children, &it);
			while( dlist_has_more(&it) ) {
				w = CWT_CAST(dlist_next(&it), CWT_WIDGET_PTR);
				JQ_ASSERT(w);

				if( w != widget && _cwt_intersect_widgets(widget, w) ) {
					widget = parent;
					break;
				}
			}
		}
		widget->flags |= CWT_WF_INVALID;

		cwt_post_event(CWT_EVT_WIDGET_PAINT, widget, NULL);
	}
}

void cwt_widget_moveto(CWT_WIDGET_PTR widget, int left, int top )
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	_cwt_widget_moveto(widget, left, top);
	_cwt_widget_recalc_abs(widget);
	cwt_widget_update(widget);
}

void cwt_widget_box_layout(CWT_WIDGET_PTR widget, int left, int top, int width, int height)
{
	CwtStylePtr style;

	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	style = _CWT_WIDGET_STYLE(widget);

	if( style->align & CWT_ALIGN_LEFT )         ;
	else if( style->align & CWT_ALIGN_RIGHT )   left = left + width - widget->geometry.width;
	else if( style->align & CWT_ALIGN_CENTER )  left = left + (width - widget->geometry.width)/2;

	if( style->align & CWT_VALIGN_TOP )         ;
	else if( style->align & CWT_VALIGN_BOTTOM ) top = top + height - widget->geometry.height;
	else if( style->align & CWT_VALIGN_CENTER ) top = top +(height - widget->geometry.height)/2;

	cwt_widget_moveto(widget, left, top);
}


void cwt_widget_center_about_parent(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	if( widget->parent ) {
		CWT_SIZE sz;
		cwt_widget_inner_size(widget->parent, &sz);
		cwt_widget_moveto(widget, (sz.width - cwt_widget_width(widget))/2,  (sz.height - cwt_widget_height(widget))/2);
	}
}


void cwt_widget_resize(CWT_WIDGET_PTR widget, int width, int height)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	_cwt_widget_resize(widget, width, height);
	cwt_widget_update(widget);
}

/**
 * Recalculate widget's geometry with insets (paddings and border)
 *
 * @param widget
 * @param sz
 */
void cwt_widget_resize_insets(CWT_WIDGET_PTR widget, int width, int height)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	_cwt_widget_resize_insets(widget, width, height);
	cwt_widget_update(widget);
}


static int _cwt_propagate_visible(void *widget, void *data)
{
	CWT_WIDGET_PTR w = (CWT_WIDGET_PTR)widget;
	JQ_UNUSED(data);

	JQ_ASSERT(w);
	JQ_ASSERT(w->magic == CWT_MAGIC);

	w->flags |= CWT_WF_VISIBLE/*visible = true*/;

	if( w->nchilds > 0 ) {
		dlist_traverse(&w->children, _cwt_propagate_visible, NULL);
	}
	return 0;
}

void cwt_widget_show(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	widget->flags &= ~CWT_WF_HIDDEN & 0xFFFF;

	if( !cwt_widget_visible(widget) ) {
		widget->flags |= CWT_WF_VISIBLE;

		if( widget->nchilds > 0 ) {
			dlist_traverse(&widget->children, _cwt_propagate_visible, NULL);
		}

		cwt_widget_update(widget);
	}
}


static int _cwt_propagate_hidden(void *widget, void *data)
{
	CWT_WIDGET_PTR w = (CWT_WIDGET_PTR)widget;
	JQ_UNUSED(data);

	JQ_ASSERT(w);
	JQ_ASSERT(w->magic == CWT_MAGIC);

	w->flags &= ~CWT_WF_VISIBLE & 0xFFFF;

	if( w->children.first ) {
		dlist_traverse(&w->children, _cwt_propagate_hidden, NULL);
	}
	return 0;
}


void cwt_widget_hide(CWT_WIDGET_PTR widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	widget->flags |= CWT_WF_HIDDEN;

	if( cwt_widget_visible(widget) ) {
		widget->flags &= ~CWT_WF_VISIBLE & 0xFFFF;

		if( widget->nchilds > 0 ) {
			dlist_traverse(&widget->children, _cwt_propagate_hidden, NULL);
		}

		if( widget->parent )
			cwt_widget_update(widget->parent);
		else
			cwt_widget_update(widget);
	}
}


CWT_WIDGET_PTR cwt_widget_first_child(CWT_WIDGET_PTR widget)
{
	DListNode *child;

	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	child = widget->children.first;
	while( child ) {
		if( !_cwt_widget_deleting(CWT_CAST(child, CwtWidgetPtr)))
			break;
		child = child->next;
	}

	return CWT_CAST(child, CwtWidgetPtr);
}

CWT_WIDGET_PTR cwt_widget_last_child(CWT_WIDGET_PTR widget)
{
	DListNode *child;

	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	child = widget->children.last;
	while( child ) {
		if( !_cwt_widget_deleting(CWT_CAST(child, CwtWidgetPtr)))
			break;
		child = child->prev;
	}

	return CWT_CAST(child, CwtWidgetPtr);
}


CwtWidgetPtr cwt_widget_next(CwtWidgetPtr widget)
{
	DListNode *child;

	JQ_ASSERT(widget);

	child = widget->base.next;
	while( child ) {
		if( !_cwt_widget_deleting(CWT_CAST(child, CwtWidgetPtr)))
			break;
		child = child->next;
	}

	return CWT_CAST(child, CwtWidgetPtr);
}

CwtWidgetPtr cwt_widget_prev(CwtWidgetPtr widget)
{
	DListNode *child;

	JQ_ASSERT(widget);

	child = widget->base.prev;
	while( child ) {
		if( !_cwt_widget_deleting(CWT_CAST(child, CwtWidgetPtr)))
			break;
		child = child->prev;
	}

	return CWT_CAST(child, CwtWidgetPtr);
}


void cwt_quit(void)
{
	_cwt_set_quit_state();
	cwt_delete_widget(cwt_screen());
	cwt_post_event(CWT_EVT_QUIT, NULL, NULL);
}
