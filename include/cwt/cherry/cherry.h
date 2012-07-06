/*
 * cherry.h
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 */

#ifndef _CWT_CHERRY_H_
#define _CWT_CHERRY_H_

#include <cwt/string.h>
#include <cwt/global.h>
#include <cwt/types.h>
#include <cwt/list.h>
#include <cwt/cherry/config.h>
#include <cwt/cherry/event.h>
#include <cwt/cherry/color.h>

#define CWT_MAGIC 0xABCD

/* CherryCWT - Cross-platform Widget (GUI, windowing) Toolkit
 * */
#define CWT_CAST(w,T) ((T)w)

struct CWT_POINT
{
	int x;
	int y;
};
typedef struct CWT_POINT CWT_POINT;

struct CWT_SIZE
{
	int width;
	int height;
};
typedef struct CWT_SIZE CWT_SIZE;
typedef struct CWT_SIZE CwtSize;

struct CWT_RECT
{
	int left;
	int top;
	int width;
	int height;
};
typedef struct CWT_RECT CWT_RECT;

struct CWT_VIEWPORT
{
	int left;
	int top;
	int width;
	int height;
	BOOL clip;
};
typedef struct CWT_VIEWPORT CWT_VIEWPORT;
typedef struct CWT_VIEWPORT CwtViewport;

typedef int CWT_COLOR;

struct CWT_FONT {
	int font;
	int factor;
};
typedef struct CWT_FONT CWT_FONT;

struct CWT_PADDING
{
	int left;
	int top;
	int right;
	int bottom;
};
typedef struct CWT_PADDING CWT_PADDING;

struct CWT_BORDER
{
	CWT_COLOR color;
	int left;
	int top;
	int right;
	int bottom;
};
typedef struct CWT_BORDER  CWT_BORDER;
typedef struct CWT_BORDER  CwtBorder;
typedef struct CWT_BORDER* CwtBorderPtr;
typedef const struct CWT_BORDER* CwtBorderConstPtr;



struct CWT_STYLE
{
	int ref;
	UINT16		magic;
	CWT_COLOR   color;
	CWT_COLOR   bgcolor;
	CWT_FONT    font;
	CWT_BORDER  border;
	CWT_PADDING padding;
	UINT16      align;
	UINT16      line_height; /* in percents */
};
typedef struct CWT_STYLE  CWT_STYLE;
typedef struct CWT_STYLE* CWT_STYLE_PTR;
typedef struct CWT_STYLE  CwtStyle;
typedef struct CWT_STYLE* CwtStylePtr;
typedef const struct CWT_STYLE* CwtStyleConstPtr;


enum CWT_WIDGET_TYPE
{
/* 0*/	  CWT_WT_SCREEN
/* 1*/	, CWT_WT_MAINWINDOW
/* 2*/	, CWT_WT_DIALOG
/* 3*/	, CWT_WT_FORM
/* 4*/	, CWT_WT_LABEL
/* 5*/	, CWT_WT_BUTTON		/* push button */
/* 6*/	, CWT_WT_TEXTVIEW
/* 7*/	, CWT_WT_INPUT
/* 8*/	, CWT_WT_CHECKBOX
/* 9*/	, CWT_WT_SPINBOX
/*10*/	, CWT_WT_MENU	 /* pop-up menu */
/*11*/	, CWT_WT_MENUBAR /* menu */
/*12*/	, CWT_WT_GROUPBOX
/*13*/  , CWT_WT_TEXTEDIT
/*14*/	, CWT_WT_CANVAS
/*15*/	, CWT_WT_MSGBOX
/*16*/	, CWT_WT_COUNT
};
typedef enum   CWT_WIDGET_TYPE CWT_WIDGET_TYPE;

enum CWT_ALIGN {
	  CWT_ALIGN_LEFT    = 0x01
	, CWT_ALIGN_RIGHT   = 0x02
	, CWT_ALIGN_CENTER  = 0x04
	, CWT_VALIGN_TOP    = 0x10
	, CWT_VALIGN_BOTTOM = 0x20
	, CWT_VALIGN_CENTER = 0x40
};
typedef enum CWT_ALIGN CWT_ALIGN;

enum CWT_STATE
{
	  CWT_STATE_NORMAL
	, CWT_STATE_HOVER
	, CWT_STATE_FOCUS
	, CWT_STATE_INACTIVE
	, CWT_STATE_COUNT
};
typedef enum CWT_STATE CWT_STATE;
typedef enum CWT_STATE CwtState;


enum CWT_FOCUS_POLICY
{
	  CWT_FP_NOFOCUS
	, CWT_FP_TAB = 0x01
	, CWT_FP_CLICK = 0x02
	, CWT_FP_STRONG = CWT_FP_TAB | CWT_FP_CLICK
};
typedef enum CWT_FOCUS_POLICY CWT_FOCUS_POLICY;

enum CWT_WIDGET_FLAG
{
	  CWT_WF_VISIBLE  = 0x0001
	, CWT_WF_INVALID  = 0x0002
	, CWT_WF_DELETING = 0x0004 /* set if widget marked (DELETE event posted) for deferred deleting */
	, CWT_WF_LAYOUT   = 0x0008 /* set if need layout before rendering (sense and layout method is set) */
	, CWT_WF_HIDDEN   = 0x0010
};
typedef enum CWT_WIDGET_FLAG CWT_WIDGET_FLAG;

struct CWT_WIDGET
{
	DListNode           base;
	UINT16				magic;
	CWT_WIDGET_TYPE     wtype;
	UINT16              id;
	struct CWT_WIDGET   *parent;
	struct CWT_WIDGET   *focused;
	CWT_STATE           state;
	UINT16              flags;
	int                 nchilds;
	DList               children;

	CwtStylePtr		    styles[CWT_STATE_COUNT];
	CWT_FOCUS_POLICY    focus_policy;
	struct CWT_RECT     geometry;
	struct CWT_POINT    abs; /* left-top corner in absolute coordinates */

	void (*layout)(struct CWT_WIDGET*);

	BOOL (*event_handler)(CWT_EVENT_PTR pevt);

	BOOL (*child_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
	BOOL (*mouse_over_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
	BOOL (*mouse_out_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
	BOOL (*mouse_down_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
	BOOL (*mouse_up_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
	BOOL (*key_press_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
	BOOL (*focus_in_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
	BOOL (*focus_out_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
	BOOL (*activated_window_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
	BOOL (*deactivated_window_event)(struct CWT_WIDGET*, CWT_EVENT_PTR);
};

typedef struct CWT_WIDGET  CWT_WIDGET;
typedef struct CWT_WIDGET* CWT_WIDGET_PTR;
typedef struct CWT_WIDGET  CwtWidget;
typedef struct CWT_WIDGET* CwtWidgetPtr;

#define _cwt_widget_invalid(widget) ((widget)->flags & CWT_WF_INVALID)
#define _cwt_widget_deleting(widget) ((widget)->flags & CWT_WF_DELETING)

typedef void (*CWT_CALLBACK)(CwtWidgetPtr widget);
typedef BOOL (*CWT_LOOP_PROC)(CWT_EVENT_PTR pevt);


#include <jq/cherry/core_p.h>
#include <jq/cherry/cherry_p.h>
#include <jq/cherry/txted.h>


EXTERN_C_BEGIN

BOOL cwt_init(void);
void cwt_finish(void);
void cwt_quit(void);

CWT_FONT* cwt_default_font(void);
void      cwt_set_default_font(const CWT_FONT *font);
CwtWidgetPtr cwt_screen(void);
CwtWidgetPtr cwt_active_window(void);
void cwt_set_active_window(CwtWidgetPtr widget);
BOOL cwt_is_active_window(CwtWidgetPtr widget);

CwtWidgetPtr cwt_new_widget(UINT16 id, CWT_WIDGET_TYPE wtype, CwtWidgetPtr parent);
CwtWidgetPtr cwt_new_custom_widget(UINT16 id, CWT_WIDGET_TYPE wtype, CwtWidgetPtr parent, size_t wsize);
void         cwt_delete_widget(CwtWidgetPtr widget);
void         cwt_delete_widget_now(CwtWidgetPtr widget);
CwtWidgetPtr cwt_clone_widget(UINT16 id, CwtWidgetPtr widget);
void         cwt_delete_children(CwtWidgetPtr widget);
CwtWidgetPtr cwt_widget_by_id(UINT16 id);
void         cwt_widget_set_focused(CwtWidgetPtr widget, CWT_FOCUS_REASON reason);
BOOL         cwt_widget_is_focused(CwtWidgetPtr widget);
#define      cwt_widget_visible(widget)	((widget)->flags & CWT_WF_VISIBLE)


CwtStylePtr  cwt_new_style(void);
void         cwt_delete_style(CwtStylePtr style);
CwtStylePtr  cwt_assign_style(CwtStylePtr* pstyle, CwtStylePtr style);
CwtStylePtr  cwt_clone_style(CwtStylePtr style);
BOOL         cwt_style_eq(const CwtStylePtr style0, const CwtStylePtr style1);
CwtBorderConstPtr cwt_style_border(const CwtStylePtr style);
void         cwt_style_set_border(CwtStylePtr style, int color, int left, int top, int right, int bottom);
void         cwt_style_setx_border(CwtStylePtr style, CWT_BORDER *border);
void         cwt_style_set_padding(CwtStylePtr style, int left, int top, int right, int bottom);
void         cwt_style_setx_padding(CwtStylePtr style, CWT_PADDING *padding);
void         cwt_style_set_color(CwtStylePtr style, CWT_COLOR color, CWT_COLOR bgcolor );
void         cwt_style_set_font(CwtStylePtr style, CWT_FONT *font);
void         cwt_style_set_align(CwtStylePtr style, UINT16 align);
void         cwt_style_set_line_height(CwtStylePtr style, UINT16 line_height);

void cwt_widget_set_focus_policy(CwtWidgetPtr widget, CWT_FOCUS_POLICY focus_policy);
void cwt_widget_set_style(CwtWidgetPtr widget, CWT_STATE state, CwtStylePtr pstyle);
CwtStylePtr cwt_widget_style(CwtWidgetPtr widget, CWT_STATE state);
/*void cwt_widget_set_styles(CwtWidgetPtr widget, CwtStylePtr pstyle);*/
void cwt_widget_set_width(CwtWidgetPtr widget, int width);
void cwt_widget_set_height(CwtWidgetPtr widget, int height);
void cwt_widget_set_layout(CwtWidgetPtr widget, void (*layout)(CwtWidgetPtr));

void cwt_widget_size(CwtWidgetPtr widget, CWT_SIZE *sz);
int cwt_widget_left(CwtWidgetPtr widget);
int cwt_widget_top(CwtWidgetPtr widget);
int cwt_widget_width(CwtWidgetPtr widget);
int cwt_widget_height(CwtWidgetPtr widget);
int cwt_widget_abs_left(CwtWidgetPtr widget);
int cwt_widget_abs_top(CwtWidgetPtr widget);
void cwt_widget_inner_size(CwtWidgetPtr widget, CWT_SIZE *sz);
void cwt_widget_geometry(CwtWidgetPtr widget, CWT_RECT *rect);
void cwt_widget_abs_geometry(CwtWidgetPtr widget, CWT_RECT *rect);


CwtWidgetPtr cwt_widget_first_child(CwtWidgetPtr widget);
CwtWidgetPtr cwt_widget_last_child(CwtWidgetPtr widget);
CwtWidgetPtr cwt_widget_next(CwtWidgetPtr widget);
CwtWidgetPtr cwt_widget_prev(CwtWidgetPtr widget);

void cwt_widget_show(CwtWidgetPtr widget);
void cwt_widget_hide(CwtWidgetPtr widget);
void cwt_widget_repaint(CwtWidgetPtr widget);
void cwt_widget_update(CwtWidgetPtr widget);
void cwt_widget_resize(CwtWidgetPtr widget, int width, int height);
void cwt_widget_resize_insets(CwtWidgetPtr widget, int width, int height);
void cwt_widget_resize_min(CwtWidgetPtr widget);
void cwt_widget_moveto(CwtWidgetPtr widget, int left, int top);
void cwt_widget_box_layout(CwtWidgetPtr widget, int left, int top, int width, int height);
void cwt_widget_center_about_parent(CwtWidgetPtr widget);
void cwt_widget_quick_show(CwtWidgetPtr widget);
void cwt_widget_quick_hide(CwtWidgetPtr widget);


/* Dialog */
void cwt_dialog_open(CwtWidgetPtr widget);
UINT16 cwt_dialog_result(CwtWidgetPtr dialog);
BOOL cwt_dialog_accepted(CwtWidgetPtr dialog);
BOOL cwt_dialog_rejected(CwtWidgetPtr dialog);
void cwt_dialog_done(CwtWidgetPtr dialog, UINT16 rc);
void cwt_dialog_accept(CwtWidgetPtr dialog);
void cwt_dialog_reject(CwtWidgetPtr dialog);
void cwt_dialog_set_hook(CwtWidgetPtr dialog, void (*hook)(CwtWidgetPtr));


/* Label */
void cwt_label_set_text(CwtWidgetPtr widget, const CHAR *text);
const StringListPtr cwt_label_text(CwtWidgetPtr widget);

/* Button */
void cwt_button_set_text(CwtWidgetPtr widget, const CHAR *text);
void cwt_button_set_painter(CwtWidgetPtr widget, void (*painter)(CwtWidgetPtr));
CHAR* cwt_button_text(CwtWidgetPtr widget);
void* cwt_button_painter(CwtWidgetPtr widget);

/* TextView */
void cwt_textview_clear(CwtWidgetPtr widget);
void cwt_textview_prepend_text(CwtWidgetPtr widget, const CHAR *text);
void cwt_textview_append_text(CwtWidgetPtr widget, const CHAR *text);
void cwt_textview_set_text(CwtWidgetPtr widget, const CHAR *text);
void cwt_textview_remove_first(CwtWidgetPtr widget);
void cwt_textview_remove_last(CwtWidgetPtr widget);
void cwt_textview_scroll_top(CwtWidgetPtr widget);
void cwt_textview_scroll_bottom(CwtWidgetPtr widget);
int cwt_textview_nlines(CwtWidgetPtr widget);

/* ReachView */
CwtTextBlkPtr cwt_reachview_prepend_block(CwtWidgetPtr widget);
CwtTextBlkPtr cwt_reachview_append_block(CwtWidgetPtr widget);

/* Input */
void cwt_input_set_text(CwtWidgetPtr widget, const CHAR *text);
const CHAR* cwt_input_text(CwtWidgetPtr widget);
void cwt_input_set_validator(CwtWidgetPtr widget, BOOL (*validate)(const CHAR*));
void cwt_input_set_limit(CwtWidgetPtr widget, int limit);

/* Checkbox */
void cwt_checkbox_set_checked_style(CwtWidgetPtr widget, CwtStylePtr style);
void cwt_checkbox_set_checked(CwtWidgetPtr widget, BOOL checked);
BOOL cwt_checkbox_checked(CwtWidgetPtr widget);
void cwt_checkbox_toggle(CwtWidgetPtr widget);
void cwt_checkbox_set_text(CwtWidgetPtr widget, const CHAR *text);
CHAR* cwt_checkbox_text(CwtWidgetPtr widget);


/* SpinBox */
void cwt_spinbox_add_item(CwtWidgetPtr widget, UINT16 id, const CHAR* text);
const CHAR* cwt_spinbox_text(CwtWidgetPtr widget);
UINT16 cwt_spinbox_id(CwtWidgetPtr widget);

/* Menu */
/*CwtWidgetPtr cwt_new_menu(UINT16 id);*/
void cwt_menu_set_parent_action(CwtWidgetPtr menu, CwtWidgetPtr action);
void cwt_menu_show(CwtWidgetPtr widget);

/* Action */
/*
CwtWidgetPtr cwt_new_action(UINT16 id, CWT_ACTION_TYPE type, CwtWidgetPtr parent, CWT_ACTION_PTR parent_action);
void cwt_action_set_callback(CwtWidgetPtr action, CWT_CALLBACK callback);
*/

EXTERN_C_END

#endif /* _CWT_CHERRY_H_ */




#if 0
enum CWT_MSGBOX_STANDARD_BUTTON {
	  MSGBOX_OK      = 0x0001
	, MSGBOX_CANCEL  = 0x0002
	, MSGBOX_YES     = 0x0004
	, MSGBOX_NO      = 0x0008
/*
	, MSGBOX_ABORT   = 0x0010
	, MSGBOX_RETRY   = 0x0020
	, MSGBOX_IGNORE  = 0x0040
*/
};

enum CWT_TEXT_WRAP {
	  CWT_TEXT_NOWRAP
	, CWT_TEXT_WRAP_SYMBOL
	, CWT_TEXT_WRAP_WORD
};
typedef enum CWT_TEXT_WRAP CWT_TEXT_WRAP;

/* Canvas */
void cwt_canvas_set_painter(CwtWidgetPtr w, void (*painter)(CwtWidgetPtr));
void* cwt_canvas_painter(CwtWidgetPtr w);

/* MsgBox */
void cwt_msgbox_set_msg(CwtWidgetPtr widget, const CHAR* msg);
void cwt_msgbox_set_buttons(CwtWidgetPtr widget, UINT16 buttons, CWT_ALIGN align, UINT16 default_button);
#endif
