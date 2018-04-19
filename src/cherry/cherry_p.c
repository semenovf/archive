/*
 * cherry_p.c
 *
 *  Created on: 22.01.2012
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>
#include <cwt/cherry/keys.h>
#include <cwt/algo/hash_tab.h>
#include <cwt/algo/hash_int.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/cherry/txted.h>

int __total_widgets = 0;
static HashTable *__cwt_hashtable = NULL;
static BOOL __cwt_quit_state = FALSE;

static CwtWidgetPtr __cwt_active_window = NULL;
static CwtWidgetPtr __cwt_hover_widget  = NULL;

struct _CWT_METHOD_VTABLE
{
	size_t widget_alloc_size;
	void (*init)(CwtWidgetPtr widget);
	void (*destroy)(CwtWidgetPtr widget);
	CwtWidgetPtr (*clone)(CwtWidgetPtr orig, CwtWidgetPtr w);
	void (*size_min)(CwtWidgetPtr orign, CWT_SIZE*);
	void (*set_style)(CwtWidgetPtr widget, CWT_STATE state, CWT_STYLE_PTR pstyle);
	void (*render)(CwtWidgetPtr widget);
};

struct _CWT_METHOD_VTABLE __cwt_vtable[CWT_WT_COUNT] = {
	   {   sizeof(CWT_WIDGET) /* Screen */
	    , NULL
	    , _cwt_destroy_screen
		, NULL
		, NULL
		, NULL
		, NULL
	    }

	  , { sizeof(CWT_WIDGET) /* MainWindow */
		, NULL
		, NULL
		, NULL
		, NULL
		, NULL
		, NULL }

	, {   sizeof(CwtDialog) /* Dialog */
		, _cwt_init_dialog
		, NULL
		, NULL
		, NULL
		, NULL
		, NULL }


	   , { sizeof(CWT_WIDGET) /* Form */
		, NULL
		, NULL
		, NULL
		, NULL
		, NULL
		, NULL }

	  , {  sizeof(CWT_LABEL)
		, _cwt_init_label
		, _cwt_destroy_label
		, _cwt_clone_label
		, _cwt_size_min_label
		, NULL
		, _cwt_render_label }

	, {   sizeof(CWT_BUTTON)
		, _cwt_init_button
		, _cwt_destroy_button
		, _cwt_clone_button
		, _cwt_size_min_button
		, NULL
		, _cwt_render_button }

	, {   sizeof(CWT_TEXTVIEW)
		, _cwt_init_textview
		, _cwt_destroy_textview
		, _cwt_clone_textview
		, _cwt_size_min_textview
		, NULL
		, _cwt_render_textview }

	, {   sizeof(CWT_INPUT)
		, _cwt_init_input
		, _cwt_destroy_input
		, _cwt_clone_input
		, _cwt_size_min_input
		, NULL
		, _cwt_render_input }

	, {   sizeof(CWT_CHECKBOX)
		, _cwt_init_checkbox
		, _cwt_destroy_checkbox
		, _cwt_clone_checkbox
		, _cwt_size_min_checkbox
		, NULL
		, _cwt_render_checkbox }

	, {   sizeof(CWT_SPINBOX)
		, _cwt_init_spinbox
		, _cwt_destroy_spinbox
		, _cwt_clone_spinbox
		, NULL
		, _cwt_set_style_spinbox
		, _cwt_render_spinbox }

	, {   sizeof(CWT_MENU) /* Pop-up Menu */
		, _cwt_init_menu
		, _cwt_destroy_menu
		, NULL
		, NULL
		, NULL
		, _cwt_render_menu }

	, {   sizeof(CWT_MENUBAR)
		, _cwt_init_menubar
		, _cwt_destroy_menubar
		, NULL
		, NULL
		, NULL
		, _cwt_render_menubar }

	, {   sizeof(CWT_GROUPBOX)
		, _cwt_init_groupbox
		, NULL
		, NULL
		, NULL
		, NULL
		, NULL }

   , {   sizeof(CwtTextEdit)
		, _cwt_init_textedit
		, _cwt_destroy_textedit
		, NULL /*_cwt_clone_textedit*/
		, NULL /*_cwt_size_min_textedit*/
		, NULL
		, _cwt_render_textedit }

#if 0


	, {   sizeof(CWT_CANVAS)
		, _cwt_init_canvas
		, _cwt_destroy_canvas
		, _cwt_clone_canvas
		, NULL
		, _cwt_render_canvas }



	, {   sizeof(CWT_MSGBOX) /* MsgBox */
		, _cwt_init_msgbox
		, _cwt_destroy_msgbox
		, NULL
		, NULL
		, _cwt_render_msgbox }
#endif
};


void _cwt_widget_change_state(CwtWidgetPtr widget, CWT_STATE state)
{
	JQ_ASSERT(widget);

	if( state != widget->state ) {
	 	CWT_STYLE_PTR style = _CWT_WIDGET_STYLE(widget);

		widget->state = state;

		/*_cwt_widget_dump(widget, "_cwt_widget_change_state: ");*/

		if( style != _CWT_WIDGET_STYLE(widget) ) {
			cwt_widget_update(widget);
			printf_trace("Widget's style changed to: %d", state);
		}
	}
}

/* used by HashTable */
static int _cwt_id_equals(HashTableKey value1, HashTableKey value2)
{
	return *((UINT16*)value1) == *((UINT16*)value2) ? 1 : 0;
}

void _cwt_set_quit_state(void)
{
	__cwt_quit_state = TRUE;
}

#if 0
/* FIXME replace by hash table */
struct widget_by_id {
	CwtWidgetPtr widget;
	UINT16 id;
};
static CwtWidgetPtr _cwt_widget_by_id(CwtWidgetPtr widget, UINT16 id);
static int _cwt_widget_by_id_helper(void *widget, void *extra)
{
	struct widget_by_id* d = (struct widget_by_id*)extra;

	if( CWT_CAST(widget, CwtWidgetPtr)->id == d->id ) {
		d->widget = CWT_CAST(widget, CwtWidgetPtr);
	} else
		d->widget = _cwt_widget_by_id((CwtWidgetPtr)widget, d->id);

	return d->widget ? 1 : 0;
}

CwtWidgetPtr _cwt_widget_by_id(CwtWidgetPtr widget, UINT16 id)
{
	struct widget_by_id d = { NULL, 0 };
	d.id = id;
	dlist_traverse(&widget->children, _cwt_widget_by_id_helper, &d);
	return d.widget;
}
#endif

CwtWidgetPtr cwt_widget_by_id(UINT16 id)
{
/*    UINT16 _id = id;*/
	CwtWidgetPtr widget = (CwtWidgetPtr)hash_table_lookup(__cwt_hashtable, &id);
	if( widget ) {
		JQ_ASSERT(widget->base.data == widget);
	}
	return widget;
#if 0
	return _cwt_widget_by_id(cwt_screen(), id);
#endif
}


CwtWidgetPtr cwt_new_custom_widget(UINT16 id, CWT_WIDGET_TYPE wtype, CwtWidgetPtr parent, size_t wsize)
{
	CwtWidgetPtr w = NULL;
	int i;


	if( !__cwt_hashtable ) {
		__cwt_hashtable = hash_table_new(uint16_hash, _cwt_id_equals);
		hash_table_register_free_functions(__cwt_hashtable, NULL, NULL);
	}

	if( id != 0 )
		JQ_ASSERT(!cwt_widget_by_id(id));

	w = (CwtWidgetPtr)jq_malloc(wsize/*__cwt_vtable[wtype].widget_alloc_size*/);
	JQ_ASSERT(w);

	if( !parent && wtype != CWT_WT_SCREEN )
		parent = cwt_screen();

	if( wtype == CWT_WT_SCREEN )
		parent = NULL;

	w->base.next = NULL;
	w->base.prev = NULL;
	w->base.data = w;
	w->magic = CWT_MAGIC;
	w->wtype   = wtype;
	w->id      = id;
	w->parent  = parent;
	w->focused = NULL;
	w->state   = CWT_STATE_NORMAL;
	w->flags   = 0;
	w->flags  |= CWT_WF_LAYOUT;
/*
	w->visible = FALSE;
	w->invalid = FALSE;
*/
	w->nchilds = 0;
	w->children.first = NULL;
	w->children.last  = NULL;

	for( i = 0; i < CWT_STATE_COUNT; i++ ) {
		w->styles[i] = _cwt_style_default();
	}

	w->focus_policy = CWT_FP_NOFOCUS;

	w->geometry.left
		= w->geometry.top
		= 0;
	w->geometry.width
		= w->geometry.height
		= 10;

	/* left-top corner in absolute coordinates
	 *
	 * must be recalculated
	 * - while creating widget
	 * - after its moving
	 * - after moving parent
	 * - after setting style of parent (when padding and border changed)
	 * */
	w->abs.x = w->abs.y = 0;

	w->layout = NULL;
	w->event_handler = NULL;

	w->child_event = NULL;
	w->mouse_over_event = NULL;
	w->mouse_out_event  = NULL;
	w->mouse_down_event = NULL;
	w->mouse_up_event   = NULL;
	w->key_press_event  = NULL;
	w->focus_in_event   = NULL;
	w->focus_out_event  = NULL;
	w->activated_window_event = NULL;
	w->deactivated_window_event = NULL;

	if( __cwt_vtable[wtype].init )
		__cwt_vtable[wtype].init(w);

	if( parent ) {
		dlist_insert_last(&parent->children, (DListNode*)w);
		parent->nchilds++;

		_cwt_widget_recalc_abs(w);
		cwt_post_child_event(w, TRUE);
	}

	if( w->id ) {
		hash_table_insert(__cwt_hashtable, &w->id, w);
	}

	if( w->wtype == CWT_WT_MAINWINDOW ) {
		_cwt_set_active_window(w);
	}

	/*_cwt_widget_dump(w, "WIDGET_NEW:");*/

	/* TODO may be there is better decision */
	__cwt_quit_state = FALSE;

	if( w->id != 0)
		JQ_ASSERT(cwt_widget_by_id(w->id) == w);

	return w;
}


CwtWidgetPtr cwt_new_widget(UINT16 id, CWT_WIDGET_TYPE wtype, CwtWidgetPtr parent)
{
	return cwt_new_custom_widget(id, wtype, parent, __cwt_vtable[wtype].widget_alloc_size);
}



static int _cwt_delete_widget_helper(void *widget, void *extra)
{
	JQ_UNUSED(extra);
	_cwt_delete_widget((CwtWidgetPtr)widget);
	return 0;
}

void _cwt_delete_widget(CwtWidgetPtr widget)
{
	int i;

	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	if( widget == __cwt_hover_widget )
		__cwt_hover_widget = NULL;

/*	JQ_ASSERT(widget != __cwt_focus_widget);*/

	dlist_back_traverse(&widget->children, _cwt_delete_widget_helper, NULL);

	if( widget->parent ) {
		JQ_ASSERT(widget->parent->nchilds > 0);
		widget->parent->nchilds--;
		dlist_remove(&widget->parent->children, (DListNode*)widget);
	}

/*	hash_table_remove(__cwt_hashtable, &widget->id);*/

	if( __cwt_vtable[widget->wtype].destroy ) {
		__cwt_vtable[widget->wtype].destroy(widget);
	}

	for( i = 0; i < CWT_STATE_COUNT; i++ ) {
		cwt_delete_style(widget->styles[i]);
	}

	/* destroy hash table of widgets if it is empty */
/*	if( __cwt_hashtable && widget == cwt_screen() && !hash_table_num_entries(__cwt_hashtable) ) {
		hash_table_free(__cwt_hashtable);
		__cwt_hashtable = NULL;
	}
*/
	/*_cwt_widget_dump(widget, "WIDGET_DELETE:");*/

	widget->magic = 0;
	JQ_FREE(widget);
}

void cwt_delete_widget_now(CwtWidgetPtr widget)
{
	JQ_ASSERT(widget);
	_cwt_delete_widget(widget);

/*	if( parent )
		cwt_widget_update(parent);*/
}



static void _cwt_mark_deleting(CwtWidgetPtr widget)
{
	DListIterator it;
	CwtWidgetPtr w;
	BOOL bHasActive = FALSE;

	if( cwt_is_active_window(widget) )
		bHasActive = TRUE;

	widget->flags |= CWT_WF_DELETING;
	/*_cwt_widget_dump(widget, "WIDGET_DELETING:");*/

	if( widget->id != 0 ) {
		hash_table_remove(__cwt_hashtable, &widget->id);
	}

	dlist_begin(&widget->children, &it);
	while( dlist_has_more(&it) ) {
		w = CWT_CAST(dlist_next(&it), CwtWidgetPtr);
		JQ_ASSERT(w);

		if( cwt_is_active_window(w) )
			bHasActive = TRUE;

		w->flags |= CWT_WF_DELETING;
		/*_cwt_widget_dump(w, "WIDGET_DELETING:");*/

		if( w->id != 0 ) {
			hash_table_remove(__cwt_hashtable, &w->id);
		}

		if( w->nchilds > 0 )
			_cwt_mark_deleting(w);
	}

	/* TODO костыль */
	if( widget->wtype == CWT_WT_DIALOG )
		return;

	if( bHasActive && !__cwt_quit_state ) {
		JQ_ASSERT(widget->parent);
		cwt_set_active_window(widget->parent);
	}
}


/**
 * @brief Deferred widget deletion
 *
 * @param widget widget to delete
 */
void cwt_delete_widget(CwtWidgetPtr widget)
{
	JQ_ASSERT(widget);

	if( _cwt_widget_deleting(widget) )
		return;

	if( widget == cwt_screen() ) {
		_cwt_set_quit_state();
		__cwt_active_window = NULL;
	}


	_cwt_mark_deleting(widget);
	cwt_post_event(CWT_EVT_WIDGET_DELETE, widget, NULL);

	/* TODO костыль */
	if( widget->wtype == CWT_WT_DIALOG )
		return;


	if( __cwt_active_window && cwt_widget_is_focused(widget) ) {
		CwtWidgetPtr focused_widget;

		JQ_ASSERT(widget->parent);

		focused_widget = _cwt_widget_next_focused(widget->parent, TRUE);

		if( !focused_widget )
			_cwt_widget_next_focused(widget->parent, FALSE);

		if( !focused_widget )
			focused_widget = widget->parent;

		JQ_ASSERT(focused_widget);
		_cwt_widget_set_focused(focused_widget, CWT_FOCUS_REASON_OTHER);
	}
}

void cwt_delete_children(CwtWidgetPtr widget)
{
	DListIterator it;
	CwtWidgetPtr w;

	dlist_begin(&widget->children, &it);
	while( dlist_has_more(&it) ) {
		w = CWT_CAST(dlist_next(&it), CwtWidgetPtr);
		JQ_ASSERT(w);
		if( !_cwt_widget_deleting(w) ) {
			_cwt_mark_deleting(w);
			cwt_post_event(CWT_EVT_WIDGET_DELETE, w, NULL);
		}
	}
}



static CwtWidgetPtr _cwt_core_clone_widget(UINT16 id, CwtWidgetPtr orig, CwtWidgetPtr *clone_ptr)
{
	CwtWidgetPtr clone;
	int i;

	JQ_ASSERT(orig);
	JQ_ASSERT(clone_ptr);

	if( id != 0 && orig->id != 0 )
		JQ_ASSERT(id != orig->id);

	*clone_ptr = cwt_new_widget(id, orig->wtype, orig->parent);
	clone = *clone_ptr;

	for( i = 0; i < CWT_STATE_COUNT; i++ ) {
		cwt_assign_style(&clone->styles[i], orig->styles[i]);
	}

	clone->focus_policy = orig->focus_policy;

	clone->geometry.left   = orig->geometry.left;
	clone->geometry.top    = orig->geometry.top;
	clone->geometry.width  = orig->geometry.width;
	clone->geometry.height = orig->geometry.height;

	clone->abs.x = orig->abs.x;
	clone->abs.y = orig->abs.y;

	return clone;
}


/**
 * @brief Clone widget if it is not root widget (Screen)
 *
 * @param id
 * @param widget
 * @return
 *
 */
CwtWidgetPtr cwt_clone_widget(UINT16 id, CwtWidgetPtr widget)
{
	CwtWidgetPtr clone;

	if( widget->wtype == CWT_WT_SCREEN ) {
		clone = cwt_screen();
	} else if( ! __cwt_vtable[widget->wtype].clone ) {
		_cwt_core_clone_widget(id, widget, &clone);
	} else {
		_cwt_core_clone_widget(id, widget, &clone);
		__cwt_vtable[widget->wtype].clone(widget, clone);
	}

	clone->flags  |= CWT_WF_LAYOUT;
	return clone;
}



static int _cwt_widget_propagate_render(void *widget, void *data)
{
	CwtWidgetPtr w = (CwtWidgetPtr)widget;
	JQ_UNUSED(data);

	if( cwt_widget_visible(w)) {
		_cwt_widget_repaint(w);
	}
	return 0;
}


void _cwt_widget_render(CwtWidgetPtr widget)
{
	CWT_VIEWPORT vp;
	int left, top, width, height;
	CWT_STYLE_PTR style;
	CWT_SETTINGS settings;

	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	/* draw border */
	style = _CWT_WIDGET_STYLE(widget);
	left = 0;
	top = 0;
	width = widget->geometry.width;
	height = widget->geometry.height;

	_cwt_save_settings(&settings);

	_cwt_set_fill_style(SOLID_FILL, style->border.color);
	_cwt_bar(left, top, width, height);

	/* draw box include padding and exclude border */
	left   += style->border.left;
	top    += style->border.top;
	width  -= style->border.right + style->border.left;
	height -= style->border.bottom + style->border.top;

	_cwt_set_fill_style(SOLID_FILL, _cwt_widget_bgcolor(widget));
	_cwt_bar(left, top, width, height);

	_cwt_get_viewport(&vp);

	left   = vp.left   + style->border.left + style->padding.left;
	top    = vp.top    + style->border.top + style->padding.top;
	width  = vp.width
		- style->border.left
		- style->padding.left
		- style->border.right
		- style->padding.right;
	height = vp.height
		- style->border.top
		- style->padding.top
		- style->border.bottom
		- style->padding.bottom;

	/* save and set viewport*/
	_cwt_set_viewport_wh(left, top, width, height, TRUE);

	if(__cwt_vtable[widget->wtype].render )
		__cwt_vtable[widget->wtype].render(widget);

	if( widget->nchilds > 0 ) {
		dlist_traverse(&widget->children, _cwt_widget_propagate_render, NULL);
	}

	_cwt_restore_settings(&settings);
}


void _cwt_widget_resize_min(CwtWidgetPtr widget)
{
	CWT_SIZE sz = { 10, 10};
	if( __cwt_vtable[widget->wtype].size_min )
		__cwt_vtable[widget->wtype].size_min(widget, &sz);
	_cwt_widget_resize_insets(widget, sz.width, sz.height);
}


void cwt_widget_resize_min(CwtWidgetPtr widget)
{
	_cwt_widget_resize_min(widget);
	cwt_widget_update(widget);
}

void _cwt_widget_resize_insets(CwtWidgetPtr widget, int width, int height)
{
	CWT_STYLE_PTR style;

	style = _CWT_WIDGET_STYLE(widget);

	_cwt_widget_resize(widget
		, width
			+ style->padding.left
			+ style->padding.right
			+ style->border.left
			+ style->border.right
		, height
			+ style->padding.top
			+ style->padding.bottom
			+ style->border.top
			+ style->border.bottom);
}


void _cwt_widget_repaint(CwtWidgetPtr widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	if( cwt_widget_visible(widget) && !(widget->flags & CWT_WF_HIDDEN) ) {
		int left, top, width, height;
		CWT_VIEWPORT vp;

		/*_cwt_widget_dump(widget, "WIDGET_REPAINT:");*/

		if( widget->layout ) {
			widget->layout(widget);
		}

		left = widget->abs.x;
		top = widget->abs.y;

		width  = widget->geometry.width;
		height = widget->geometry.height;

		/* save and set viewport */
		_cwt_get_viewport(&vp);
		_cwt_set_viewport_wh(left, top, width, height, TRUE);

		_cwt_widget_render(widget);

		/* restore viewport */
		_cwt_set_viewport_wh( vp.left, vp.top, vp.width, vp.height, vp.clip );

		widget->flags &= ((~CWT_WF_INVALID) & 0xFF); /*invalid = FALSE;*/
		widget->flags &= ((~CWT_WF_LAYOUT) & 0xFF);
	} /* if visible */
}


static int _cwt_widget_propagate_abs(void *widget, void *data)
{
	CwtWidgetPtr w = (CwtWidgetPtr)widget;
	JQ_UNUSED(data);

	_cwt_widget_recalc_abs(w);
	return 0;
}


/**
 * @brief Recalculates widget's absolute coordinates and it's children
 *
 * @param widget widget for recalculation absolute coordinates
 */
void _cwt_widget_recalc_abs(CwtWidgetPtr widget)
{
	JQ_ASSERT(widget);
	if( widget->parent ) {
		CwtWidgetPtr parent = widget->parent;
		CWT_STYLE_PTR parent_style = _CWT_WIDGET_STYLE(parent);

		widget->abs.x = parent->abs.x
			+ parent_style->border.left
			+ parent_style->padding.left
			+ widget->geometry.left;

		widget->abs.y = parent->abs.y
			+ parent_style->border.top
			+ parent_style->padding.top
			+ widget->geometry.top;

	}

	if( widget->nchilds > 0 ) {
		dlist_traverse(&widget->children, _cwt_widget_propagate_abs, NULL);
	}
}


/*--- {{{ _cwt_widget_find function group */
struct _CWT_WIDGET_AT_DATA
{
	CwtWidgetPtr widget;
	int x;
	int y;
};
typedef struct _CWT_WIDGET_AT_DATA _CWT_WIDGET_AT_DATA;



static BOOL _cwt_widget_has_point(CwtWidgetPtr widget, int x, int y)
{
	int x1, x2, y1, y2;

	JQ_ASSERT(widget);

	x1 = widget->abs.x;
	y1 = widget->abs.y;
	x2 = x1 + widget->geometry.width;
	y2 = y1 + widget->geometry.height;

	if( x >= x1 && x < x2 && y >= y1 && y < y2 ) {
		return TRUE;
	}
	return FALSE;
}


static int _cwt_widget_child_at_propagate(void *widget, void *data)
{
	CwtWidgetPtr w = (CwtWidgetPtr)widget;
	_CWT_WIDGET_AT_DATA *at_data = (_CWT_WIDGET_AT_DATA*)data;
	at_data->widget = NULL;

	if( cwt_widget_visible(w) && _cwt_widget_has_point(w, at_data->x, at_data->y) ) {
		at_data->widget = w;
		return 1;
	}
	return 0;
}

CwtWidgetPtr _cwt_widget_child_at(CwtWidgetPtr widget, int x, int y)
{
	_CWT_WIDGET_AT_DATA at_data;

	JQ_ASSERT(widget);

	at_data.widget = NULL;
	at_data.x = x;
	at_data.y = y;

	if( _cwt_widget_deleting(widget) )
		return NULL;

	if( !_cwt_widget_has_point(widget, x, y) )
		return NULL;

	while( widget ) {

		at_data.widget = NULL;

		dlist_back_traverse(&widget->children, _cwt_widget_child_at_propagate, &at_data);

		if( at_data.widget == NULL ) {
			break;
		}

		widget = at_data.widget;
	}

	if( _cwt_widget_deleting(widget) )
		return NULL;

	return widget;
}

/*
static int _cwt_widget_has_focused_helper(void *widget, void *data)
{
	CwtWidgetPtr w = (CwtWidgetPtr)widget;
	BOOL *ok = (BOOL*)data;

	if( _cwt_widget_has_focused(w) ) {
		*ok = TRUE;
		return 1;
	}
	return 0;
}
*/
/**
 * Checks if widget or it's children have focus
 *
 * @param widget
 * @return
 */
/*
BOOL _cwt_widget_has_focused(CwtWidgetPtr widget)
{
	BOOL ok = FALSE;

	JQ_ASSERT(widget);
	if( widget == __cwt_focus_widget )
		return TRUE;

	dlist_traverse(&widget->children, _cwt_widget_has_focused_helper, &ok);
	return ok;
}
*/

/**
 * Get color
 *
 * @param widget
 * @return
 */
CWT_COLOR _cwt_widget_color(CwtWidgetPtr widget)
{
	CWT_COLOR color = CWT_White;
	CWT_STYLE_PTR style;

	JQ_ASSERT(widget);

	while( widget ) {
		style = _CWT_WIDGET_STYLE(widget);

		if( style->color != CWT_Transparent ) {
			color = style->color;
			break;
		}
		widget = widget->parent;
	}

	return color;
}

CWT_COLOR _cwt_widget_bgcolor(CwtWidgetPtr widget)
{
	CWT_COLOR bgcolor = CWT_Black;
	CWT_STYLE_PTR style;

	JQ_ASSERT(widget);

	while( widget ) {
		style = _CWT_WIDGET_STYLE(widget);

		if( style->bgcolor != CWT_Transparent ) {
			bgcolor = style->bgcolor;
			break;
		}
		widget = widget->parent;
	}

	return bgcolor;
}


void _cwt_widget_set_hover(CwtWidgetPtr widget)
{
	if( widget != __cwt_hover_widget ) {
		if( __cwt_hover_widget
				&& cwt_widget_visible(__cwt_hover_widget)
				&& __cwt_hover_widget->state != CWT_STATE_INACTIVE ) {
			cwt_post_event(CWT_EVT_WIDGET_LEAVE, __cwt_hover_widget, NULL);
		}

		__cwt_hover_widget = NULL;

		if( widget
				&& cwt_widget_visible(widget)
				&& widget->state != CWT_STATE_INACTIVE
				&& widget->focus_policy != CWT_FP_NOFOCUS ) {

			__cwt_hover_widget = widget;

			cwt_post_event(CWT_EVT_WIDGET_ENTER, widget, NULL);
		}
	}
}


CwtWidgetPtr cwt_active_window(void)
{
	return __cwt_active_window;
}


void _cwt_set_active_window(CwtWidgetPtr widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);

	if( widget == __cwt_active_window )
		return;

	/* TODO "костыль" {
	 * Необходимо доработать таким образом, чтобы значение active_window присваивалось
	 * в соответствующем виджету коде.
	 * Проблема заключается в том, что __cwt_active_window меняется до того как
	 * исполнение программы переходит в код виджета.
	 * Нужны новые инструменты для отслеживания изменения активного окна */
	if( widget->wtype == CWT_WT_MENU ) {
		if( CWT_CAST(widget, CWT_MENU_PTR)->active_window == NULL )
			CWT_CAST(widget, CWT_MENU_PTR)->active_window = __cwt_active_window;
	} else if( widget->wtype == CWT_WT_MENUBAR ) {
		if( CWT_CAST(widget, CWT_MENUBAR_PTR)->active_window == NULL )
			CWT_CAST(widget, CWT_MENUBAR_PTR)->active_window = __cwt_active_window;
	} else if( widget->wtype == CWT_WT_GROUPBOX ) {
		if( CWT_CAST(widget, CWT_GROUPBOX_PTR)->active_window == NULL )
			CWT_CAST(widget, CWT_GROUPBOX_PTR)->active_window = __cwt_active_window;
	}
	/* "костыль" } */

	if( __cwt_active_window && __cwt_active_window->deactivated_window_event )
		__cwt_active_window->deactivated_window_event(__cwt_active_window, NULL);

	__cwt_active_window = widget;

	if( __cwt_active_window && __cwt_active_window->activated_window_event )
		__cwt_active_window->activated_window_event(__cwt_active_window, NULL);
}

void cwt_set_active_window(CwtWidgetPtr widget)
{
	JQ_ASSERT(widget);

	if( widget->magic != CWT_MAGIC )
		JQ_ASSERT(widget->magic == CWT_MAGIC);

	if( __cwt_quit_state )
		return;

	while( TRUE ) {
		while( widget->nchilds == 0 && !(widget->focus_policy | CWT_FP_TAB) ) {
			widget = widget->parent;
			JQ_ASSERT(widget);
		}

		if(  _cwt_widget_deleting(widget) )
			widget = widget->parent;
		else
			break;

		JQ_ASSERT(widget);
	}

	JQ_ASSERT(widget);
	cwt_post_event(CWT_EVT_WINDOW_ACTIVATE, widget, NULL);
	cwt_widget_set_focused(widget, CWT_FOCUS_REASON_ACTIVEWINDOW);
}

BOOL cwt_is_active_window(CwtWidgetPtr widget)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	return __cwt_active_window == widget ? TRUE : FALSE;
}


void cwt_widget_set_focused(CwtWidgetPtr widget, CWT_FOCUS_REASON reason)
{
	if( __cwt_quit_state )
		return;

	if( widget->nchilds == 0 ) {
		JQ_ASSERT(widget->parent);
		JQ_ASSERT(widget->parent->magic == CWT_MAGIC);
		_cwt_widget_set_focused(widget, reason);
	} else {
		if( !widget->focused ) {
			CwtWidgetPtr first_focused = _cwt_widget_first_focused(widget);
			if( first_focused )
				_cwt_widget_set_focused(first_focused, reason);
		} else {
            _cwt_widget_set_focused(widget->focused, reason);
        }
	}
}



CwtWidgetPtr _cwt_widget_first_focused(CwtWidgetPtr active_widget)
{
	CwtWidgetPtr focused_widget;

	JQ_ASSERT(active_widget);
	JQ_ASSERT(active_widget->magic == CWT_MAGIC);

	focused_widget = (CwtWidgetPtr)active_widget->children.first;

	while( focused_widget ) {
		if( focused_widget->focus_policy != CWT_FP_NOFOCUS ) {
			return focused_widget;
		}

		focused_widget = (CwtWidgetPtr)focused_widget->base.next;
	}

	return focused_widget;
}

CwtWidgetPtr _cwt_widget_next_focused(CwtWidgetPtr active_widget, BOOL forward)
{
	CwtWidgetPtr next_focused_widget;
	CwtWidgetPtr focused_widget;

	JQ_ASSERT(active_widget);
	JQ_ASSERT(active_widget->magic == CWT_MAGIC);

	focused_widget = active_widget->focused;

	if( !focused_widget ) {
		return _cwt_widget_first_focused(active_widget);
	}

	if( forward )
		next_focused_widget = (CwtWidgetPtr)focused_widget->base.next;
	else
		next_focused_widget = (CwtWidgetPtr)focused_widget->base.prev;

	while( next_focused_widget != focused_widget ) {

		if( !next_focused_widget ) {
			if( forward )
				next_focused_widget = (CwtWidgetPtr)active_widget->children.first;
			else
				next_focused_widget = (CwtWidgetPtr)active_widget->children.last;
			continue;
		}

		if( next_focused_widget->focus_policy & CWT_FP_TAB ) {
			break;
		}

		if( forward )
			next_focused_widget = (CwtWidgetPtr)next_focused_widget->base.next;
		else
			next_focused_widget = (CwtWidgetPtr)next_focused_widget->base.prev;
	}

	if( next_focused_widget == focused_widget )
		return NULL;

	return next_focused_widget;
}


void _cwt_widget_focus_out(CwtWidgetPtr widget)
{
	if( widget ) {
		if( !_cwt_widget_deleting(widget) ) {
			JQ_ASSERT(widget->parent);
			JQ_ASSERT(widget->parent->magic == CWT_MAGIC);

			widget->parent->focused = NULL;
			cwt_post_event(CWT_EVT_OUTFOCUS, widget, NULL);
			_cwt_widget_change_state(widget, CWT_STATE_NORMAL);
		}
	}
}

void _cwt_widget_focus_in(CwtWidgetPtr widget, CWT_FOCUS_REASON reason)
{
	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	JQ_ASSERT(!_cwt_widget_deleting(widget));

	if( widget->focus_policy == CWT_FP_NOFOCUS )
		return;

	if( widget->state == CWT_STATE_INACTIVE )
		return;


	if( (CWT_FOCUS_REASON_MOUSE == reason && !(widget->focus_policy & CWT_FP_CLICK)) )
		return;

	if( (CWT_FOCUS_REASON_TAB == reason && !(widget->focus_policy & CWT_FP_TAB)) )
		return;

	JQ_ASSERT(widget->parent);
	JQ_ASSERT(widget->parent->magic == CWT_MAGIC);

	if( reason != CWT_FOCUS_REASON_ACTIVEWINDOW ) {
		if( !cwt_is_active_window(widget->parent) ) {
			/*cwt_set_active_window(widget->parent);*/
			cwt_post_event(CWT_EVT_WINDOW_ACTIVATE, widget->parent, NULL);
		}
	}
	cwt_post_event(CWT_EVT_INFOCUS, widget, NULL);

	widget->parent->focused = widget;
	_cwt_widget_change_state(widget, CWT_STATE_FOCUS);
}

void _cwt_widget_set_focused(CwtWidgetPtr widget, CWT_FOCUS_REASON reason)
{
	CwtWidgetPtr parent;

	JQ_ASSERT(widget);
	JQ_ASSERT(widget->magic == CWT_MAGIC);
	JQ_ASSERT(widget->parent);
	JQ_ASSERT(widget->parent->magic == CWT_MAGIC);
/*	JQ_ASSERT(widget->focus_policy & reason);*/

	parent = widget->parent;

	if( parent->focused != widget ) {
		_cwt_widget_focus_out(parent->focused);
	}

	_cwt_widget_focus_in(widget, reason);
}

BOOL cwt_widget_is_focused(CwtWidgetPtr widget)
{
	 JQ_ASSERT(widget);
	 JQ_ASSERT(widget->magic == CWT_MAGIC);
	 JQ_ASSERT(widget->parent);
	 JQ_ASSERT(widget->parent->magic == CWT_MAGIC);
	 return widget->parent->focused == widget ? TRUE : FALSE;
}


void _cwt_widget_set_geometry(CwtWidgetPtr widget, int left, int top, int width, int height )
{
	widget->geometry.left = left;
	widget->geometry.top = top;
	widget->geometry.width = width;
	widget->geometry.height = height;
	widget->flags  |= CWT_WF_LAYOUT;
}

void _cwt_widget_moveto(CwtWidgetPtr widget, int left, int top)
{
	widget->geometry.left = left;
	widget->geometry.top = top;
	widget->flags  |= CWT_WF_LAYOUT;
}

void _cwt_widget_resize(CwtWidgetPtr widget, int width, int height)
{
	widget->geometry.width = width;
	widget->geometry.height = height;
	widget->flags  |= CWT_WF_LAYOUT;
}

void _cwt_widget_set_width(CwtWidgetPtr widget, int width)
{
	widget->geometry.width = width;
	widget->flags  |= CWT_WF_LAYOUT;
}

void _cwt_widget_set_height(CwtWidgetPtr widget, int height)
{
	widget->geometry.height = height;
	widget->flags  |= CWT_WF_LAYOUT;
}


void _cwt_widget_set_style(CwtWidgetPtr widget, CWT_STATE state, CWT_STYLE_PTR style)
{
	if(__cwt_vtable[widget->wtype].set_style ) {
		__cwt_vtable[widget->wtype].set_style(widget, state, style);
	} else {
		cwt_assign_style(&widget->styles[state], style);
	}
	widget->flags  |= CWT_WF_LAYOUT;
}


BOOL _cwt_intersect_widgets(CwtWidgetPtr w0, CwtWidgetPtr w1)
{
	int x00 = w0->abs.x;
	int x01 = x00 + w0->geometry.width - 1;
	int y00 = w0->abs.y;
	int y01 = y00 + w0->geometry.height - 1;

	int x10 = w1->abs.x;
	int x11 = x10 + w1->geometry.width - 1;
	int y10 = w1->abs.y;
	int y11 = y10 + w1->geometry.height - 1;

	if( JQ_MIN(x01, x11) > JQ_MAX(x00, x10) && JQ_MIN(y01, y11) > JQ_MAX(y00, y10) )
		return TRUE;

	return FALSE;
}

/*
void _cwt_widget_raise(CwtWidgetPtr widget)
{
	JQ_ASSERT(widget);
	dlist_remove(&widget->parent->children, (DListNode*)widget);
	dlist_insert_last(&widget->parent->children, (DListNode*)widget);
	cwt_widget_update(widget);
}
*/


void _cwt_widget_dump(CwtWidgetPtr widget, const CWT_CHAR* prefix)
{
	printf_trace("%s ptr=%p; wtype=%d; id=%d; flags=%04X; parent=%p; state=%d; visible=%s; nchilds=%d; focus_policy=%d; geometry=(%d, %d, %d, %d); abs=(%d, %d)"
		, prefix ? prefix : ""
		, widget
		, widget->wtype
		, widget->id
		, widget->flags
		, widget->parent
		, widget->state
		, cwt_widget_visible(widget) ? "TRUE" : "FALSE"
		, widget->nchilds
		, widget->focus_policy
		, widget->geometry.left
		, widget->geometry.top
		, widget->geometry.width
		, widget->geometry.height
		, widget->abs.x
		, widget->abs.y
	);
}

