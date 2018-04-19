/*
 * cherry_p.h
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 *
 *  Protected and private methods
 */

#ifndef _CWT_CHERRY_P_H_
#define _CWT_CHERRY_P_H_

#include <cwt/strbuf.h>
#include <cwt/list.h>
#include <cwt/strlist.h>

#define _CWT_WIDGET_STYLE(w) cwt_widget_style(w, (w)->state)


EXTERN_C_BEGIN

struct CwtDialog
{
	CWT_WIDGET base;
	CwtWidgetPtr active_window;
	BOOL accept;
	UINT16 rc;
	void (*on_dialog_done)(CwtWidgetPtr dlg);
};
typedef struct CwtDialog  CwtDialog;
typedef struct CwtDialog* CwtDialogPtr;


struct CWT_TEXTBLK
{
	StringListPtr     lines;
	CwtStylePtr       style;
	StringListElemPtr top_line;
	BOOL              bottom_overflow;
	BOOL              upwards;
};
typedef struct CWT_TEXTBLK  CWT_TEXTBLK;
typedef struct CWT_TEXTBLK* CWT_TEXTBLK_PTR;
typedef struct CWT_TEXTBLK  CwtTextBlk;
typedef struct CWT_TEXTBLK* CwtTextBlkPtr;

struct CWT_LABEL
{
	CWT_WIDGET base;
	CWT_TEXTBLK_PTR textblock;
};
typedef struct CWT_LABEL  CWT_LABEL;
typedef struct CWT_LABEL* CWT_LABEL_PTR;
typedef struct CWT_LABEL  CwtLabel;
typedef struct CWT_LABEL* CwtLabelPtr;

/* Button */
enum CWT_BUTTON_TYPE {
	CWT_BUTTON_TEXT
	, CWT_BUTTON_CANVAS
/*	, CWT_BUTTON_IMAGE*/
};
typedef enum CWT_BUTTON_TYPE CWT_BUTTON_TYPE;

struct CWT_BUTTON
{
	CwtWidget       base;
	CWT_BUTTON_TYPE type;
	void *content;
	CWT_CALLBACK clicked;
};
typedef struct CWT_BUTTON  CWT_BUTTON;
typedef struct CWT_BUTTON* CWT_BUTTON_PTR;


struct CWT_TEXTVIEW
{
	CwtWidget       base;
	CwtTextBlkPtr textblock;
};
typedef struct CWT_TEXTVIEW  CWT_TEXTVIEW;
typedef struct CWT_TEXTVIEW* CWT_TEXTVIEW_PTR;
typedef struct CWT_TEXTVIEW  CwtTextView;
typedef struct CWT_TEXTVIEW* CwtTextViewPtr;

struct CWT_INPUT
{
	CWT_WIDGET base;
	StringBufferPtr strbuf;
	size_t start_pos;            /* start position (start character ) */
	size_t pos;                  /* current position (current character) */
	size_t limit;
	BOOL (*validate)(const CWT_CHAR*);
};
typedef struct CWT_INPUT CWT_INPUT;
typedef struct CWT_INPUT* CWT_INPUT_PTR;

struct CWT_CHECKBOX
{
	CWT_BUTTON base;
	CWT_STYLE *checked_style;
	CWT_STYLE *saved_style;
	BOOL checked;
	CWT_CALLBACK on_changed;
};
typedef struct CWT_CHECKBOX  CWT_CHECKBOX;
typedef struct CWT_CHECKBOX* CWT_CHECKBOX_PTR;
typedef struct CWT_CHECKBOX  CwtCheckbox;
typedef struct CWT_CHECKBOX* CwtCheckboxPtr;

/* SPINBOX---{{{ */
struct CWT_SPINBOX_ITEM
{
	DListNode base;
	UINT16 id;
	CWT_CHAR *text;
};
typedef struct CWT_SPINBOX_ITEM CWT_SPINBOX_ITEM;

struct CWT_SPINBOX
{
	CWT_WIDGET base;
	CwtWidgetPtr label;
	CwtWidgetPtr btn_prev;
	CwtWidgetPtr btn_next;
	DList items;
	DListNode *current;
	CWT_CALLBACK on_item_changed;
};
typedef struct CWT_SPINBOX  CWT_SPINBOX;
typedef struct CWT_SPINBOX* CWT_SPINBOX_PTR;
/* }}}---SPINBOX */

struct CWT_MENUBAR
{
	CWT_WIDGET base;
	CwtWidgetPtr active_window;
};
typedef struct CWT_MENUBAR  CWT_MENUBAR;
typedef struct CWT_MENUBAR *CWT_MENUBAR_PTR;

struct CWT_MENU
{
	CWT_WIDGET base;
	CwtWidgetPtr active_window;
	CwtWidgetPtr parent_action;
	CwtBitMapPtr   saved_image;
};
typedef struct CWT_MENU CWT_MENU;
typedef struct CWT_MENU *CWT_MENU_PTR;


struct CWT_GROUPBOX
{
	CWT_WIDGET base;
	CwtWidgetPtr active_window;
};
typedef struct CWT_GROUPBOX  CWT_GROUPBOX;
typedef struct CWT_GROUPBOX *CWT_GROUPBOX_PTR;
typedef struct CWT_GROUPBOX  CwtGroupbox;
typedef struct CWT_GROUPBOX *CwtGroupboxPtr;


void _cwt_delete_widget(CwtWidgetPtr widget);
/*CwtWidgetPtr _cwt_core_clone_widget(UINT16 id, CwtWidgetPtr orig, CwtWidgetPtr *clone_ptr);*/
void _cwt_widget_render(CwtWidgetPtr widget);
void _cwt_widget_repaint(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_widget_child_at(CwtWidgetPtr widget, int x, int y);
/*BOOL _cwt_widget_has_point(CwtWidgetPtr widget, int x, int y);*/
void _cwt_widget_set_geometry(CwtWidgetPtr widget, int left, int top, int width, int height );
void _cwt_widget_set_width(CwtWidgetPtr widget, int width);
void _cwt_widget_set_height(CwtWidgetPtr widget, int height);
void _cwt_widget_set_style(CwtWidgetPtr widget, CWT_STATE state, CwtStylePtr style);
void _cwt_widget_moveto(CwtWidgetPtr widget, int left, int top);
void _cwt_widget_resize(CwtWidgetPtr widget, int width, int height);
void _cwt_widget_resize_insets(CwtWidgetPtr widget, int width, int height);
void _cwt_widget_resize_min(CwtWidgetPtr widget);
void _cwt_widget_recalc_abs(CwtWidgetPtr widget);
void _cwt_widget_restore_active(void);
BOOL _cwt_intersect_widgets(CwtWidgetPtr w0, CwtWidgetPtr w1);
/*void _cwt_widget_raise(CwtWidgetPtr widget);*/
void _cwt_set_quit_state(void);

const CwtStylePtr _cwt_style_default(void);
BOOL _cwt_style_eq_default(const CwtStylePtr style);

CWT_COLOR _cwt_widget_color(CwtWidgetPtr widget);
CWT_COLOR _cwt_widget_bgcolor(CwtWidgetPtr widget);
void _cwt_widget_set_hover(CwtWidgetPtr widget);

void _cwt_set_active_window(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_widget_first_focused(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_widget_next_focused(CwtWidgetPtr widget, BOOL forward);
void _cwt_widget_set_focused(CwtWidgetPtr widget, CWT_FOCUS_REASON reason);
void _cwt_widget_focus_out(CwtWidgetPtr widget);
void _cwt_widget_focus_in(CwtWidgetPtr widget, CWT_FOCUS_REASON reason);

/*BOOL _cwt_widget_has_focused(CwtWidgetPtr widget);*/
void _cwt_widget_dump(CwtWidgetPtr widget, const CWT_CHAR* prefix);

void _cwt_widget_change_state(CwtWidgetPtr widget, CWT_STATE state);
BOOL _cwt_handle_events(CWT_EVENT_PTR evt);
/*
BOOL _cwt_widget_focus_in_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_widget_focus_out_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_widget_mouse_over_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_widget_mouse_out_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
*/
BOOL _cwt_textview_key_press_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_textedit_key_press_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_button_key_press_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_button_mouse_up_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_input_key_press_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_checkbox_key_press_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_action_key_press_event(CwtWidgetPtr widget, CWT_EVENT_PTR pevt);
BOOL _cwt_groupbox_focus_in_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt);
BOOL _cwt_groupbox_focus_out_event(CWT_WIDGET_PTR widget, CWT_EVENT_PTR pevt);
BOOL _cwt_groupbox_key_press_event(CwtWidgetPtr widget, CwtEventPtr pevt);

/* Screen */
void _cwt_destroy_screen(CwtWidgetPtr widget);

/* MainWin */
/*void _cwt_init_mainwin(CwtWidgetPtr widget);*/

/* Dialog */
void _cwt_init_dialog(CwtWidgetPtr widget);


/* TextBlock */
void _cwt_init_textblk(CWT_TEXTBLK_PTR tb);
void _cwt_destroy_textblk(CWT_TEXTBLK_PTR tb);
CWT_TEXTBLK_PTR _cwt_clone_textblk(CWT_TEXTBLK_PTR orig, CWT_TEXTBLK_PTR clone);
void _cwt_render_textblk(CWT_TEXTBLK_PTR tb, CWT_SIZE *sz);
void _cwt_size_min_textblk(CWT_TEXTBLK_PTR tb, CWT_SIZE *sz);
void _cwt_textblk_clear(CWT_TEXTBLK_PTR tb);
void _cwt_textblk_append_text(CWT_TEXTBLK_PTR tb, const CWT_CHAR *text);
void _cwt_textblk_prepend_text(CWT_TEXTBLK_PTR tb, const CWT_CHAR *text);
void _cwt_textblk_set_text(CWT_TEXTBLK_PTR tb, const CWT_CHAR *text);
void _cwt_textblk_remove_first(CWT_TEXTBLK_PTR tb);
void _cwt_textblk_remove_last(CWT_TEXTBLK_PTR tb);
void _cwt_textblk_scroll_top(CWT_TEXTBLK_PTR tb);
void _cwt_textblk_scroll_bottom(CWT_TEXTBLK_PTR tb, CWT_SIZE *sz);
int _cwt_textblk_nlines(CWT_TEXTBLK_PTR tb);

/* Label */
void _cwt_init_label(CwtWidgetPtr widget);
void _cwt_destroy_label(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_clone_label(CwtWidgetPtr orig, CwtWidgetPtr clone);
void _cwt_size_min_label(CwtWidgetPtr widget, CWT_SIZE *sz);
void _cwt_render_label(CwtWidgetPtr widget);
void _cwt_raw_render_label(CwtWidgetPtr label);

/* Button */
void _cwt_init_button(CwtWidgetPtr widget);
void _cwt_destroy_button(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_clone_button(CwtWidgetPtr orig, CwtWidgetPtr clone);
void _cwt_size_min_button(CwtWidgetPtr widget, CWT_SIZE *sz);
void _cwt_render_button(CwtWidgetPtr widget);

/* TextView */
void _cwt_init_textview(CwtWidgetPtr widget);
void _cwt_destroy_textview(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_clone_textview(CwtWidgetPtr orig, CwtWidgetPtr clone);
void _cwt_size_min_textview(CwtWidgetPtr widget, CWT_SIZE *sz);
void _cwt_render_textview(CwtWidgetPtr widget);

/* Input */
void _cwt_init_input(CwtWidgetPtr widget);
void _cwt_destroy_input(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_clone_input(CwtWidgetPtr orig, CwtWidgetPtr clone);
void _cwt_size_min_input(CwtWidgetPtr widget, CWT_SIZE *sz);
void _cwt_render_input(CwtWidgetPtr widget);

/* Checkbox */
void _cwt_init_checkbox(CwtWidgetPtr widget);
void _cwt_destroy_checkbox(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_clone_checkbox(CwtWidgetPtr orig, CwtWidgetPtr clone);
void _cwt_size_min_checkbox(CwtWidgetPtr widget, CWT_SIZE *sz);
void _cwt_render_checkbox(CwtWidgetPtr widget);

/* Spinbox */
void _cwt_init_spinbox(CwtWidgetPtr widget);
void _cwt_destroy_spinbox(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_clone_spinbox(CwtWidgetPtr orig, CwtWidgetPtr clone);
void _cwt_set_style_spinbox(CwtWidgetPtr widget, CWT_STATE state, CwtStylePtr pstyle);
void _cwt_render_spinbox(CwtWidgetPtr widget);


/* Menu */
void _cwt_init_menu(CwtWidgetPtr widget);
void _cwt_destroy_menu(CwtWidgetPtr widget);
void _cwt_layout_menu(CwtWidgetPtr widget);
void _cwt_render_menu(CwtWidgetPtr widget);

/* MenuBar*/
void _cwt_init_menubar(CwtWidgetPtr widget);
void _cwt_destroy_menubar(CwtWidgetPtr widget);
void _cwt_render_menubar(CwtWidgetPtr widget);

/* GroupBox */
void _cwt_init_groupbox(CwtWidgetPtr widget);

#if 0
struct CWT_CANVAS
{
	CWT_WIDGET base;
	void (*painter)(CwtWidgetPtr);
};
typedef struct CWT_CANVAS CWT_CANVAS;
typedef struct CWT_CANVAS* CWT_CANVAS_PTR;

struct CWT_MSGBOX
{
	CWT_DIALOG base;
	int vspace;
	int hspace;
	UINT16 default_button;
	CWT_ALIGN buttons_align; /* valid values are: CWT_ALIGN_LEFT, CWT_ALIGN_RIGHT and CWT_ALIGN_CENTER */
};
typedef struct CWT_MSGBOX  CWT_MSGBOX;
typedef struct CWT_MSGBOX* CWT_PMSGBOX;

void _cwt_init_canvas(CwtWidgetPtr widget);
void _cwt_init_msgbox(CwtWidgetPtr widget);
void _cwt_destroy_canvas(CwtWidgetPtr widget);
void _cwt_destroy_msgbox(CwtWidgetPtr widget);
CwtWidgetPtr _cwt_clone_canvas(CwtWidgetPtr orig, CwtWidgetPtr clone);
void _cwt_render_canvas(CwtWidgetPtr widget);
void _cwt_render_msgbox(CwtWidgetPtr widget);
#endif



EXTERN_C_END

#endif /* _CWT_CHERRY_P_H_ */
