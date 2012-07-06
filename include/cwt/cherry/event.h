/*
 * event.h
 *
 *  Created on: 08.01.2012
 *      Author: wladt
 */

#ifndef _EVENT_H_
#define _EVENT_H_

/* TODO depricated; use cwt/event/event.h instead */

#include <time.h> /* struct time, struct date */
#include <cwt/global.h>
#include <cwt/types.h>
#include <cwt/list.h>
#include <cwt/io/channel.h>
/*#include <cherry/cherry.h>*/

EXTERN_C_BEGIN

enum CWT_EVENT_TYPE {
/* 0*/	  CWT_EVT_NOEVENT
/* 1*/	, CWT_EVT_QUIT
/* 2*/ 	, CWT_EVT_TIMER
/* 3*/	, CWT_EVT_CLOCK_TIME
/* 4*/	, CWT_EVT_CLOCK_DATE
/* 5*/	, CWT_EVT_KEYBOARD
/* 6*/	, CWT_EVT_MOUSE_PRESS
/* 7*/	, CWT_EVT_MOUSE_RELEASE
/* 8*/ 	, CWT_EVT_MOUSE_MOVE
/* 9*/ 	, CWT_EVT_CHANNEL
/*10*/ 	, CWT_EVT_WIDGET_PAINT /* ev_data is widget object */
/*11*/ 	, CWT_EVT_WIDGET_DELETE /* deferred delete of widget */
/*12*/	, CWT_EVT_WIDGET_ENTER /* Mouse enters widget's boundaries */
/*13*/ 	, CWT_EVT_WIDGET_LEAVE /*  */
/*14*/	, CWT_EVT_INFOCUS
/*15*/	, CWT_EVT_OUTFOCUS
/*16*/	, CWT_EVT_CHILD
/*17*/	, CWT_EVT_WINDOW_ACTIVATE

/*xx*/	, CWT_EVT_USER
};
typedef enum CWT_EVENT_TYPE CWT_EVENT_TYPE;

typedef DList CWT_EVENT_QUEUE;

struct CWT_EVENT {
	CWT_EVENT_TYPE ev_type;
	void          *ev_data;
	void (*delete_data_func)(void*);
};
typedef struct CWT_EVENT CWT_EVENT;
typedef struct CWT_EVENT* CWT_EVENT_PTR;
typedef struct CWT_EVENT* CwtEventPtr;

typedef BOOL (*CWT_EVENT_HANDLER)(CWT_EVENT_PTR);

struct CWT_TIMER_EVENT
{
	UINT16 id;  /* timer id */
};
typedef struct CWT_TIMER_EVENT CWT_TIMER_EVENT;

struct CWT_CLOCK_EVENT
{
	int hour;
	int min;
	int sec;
	int day;
	int mon;
	int year;
};
typedef struct CWT_CLOCK_EVENT CWT_CLOCK_EVENT;

struct CWT_KBD_EVENT
{
	UINT16 key;
	UINT16 mods;
};
typedef struct CWT_KBD_EVENT CWT_KBD_EVENT;

enum CWT_KBD_MOD {
	  CWT_KBD_NO_MODIFIER
	, CWT_KBD_SHIFT_MODIFIER = 0x0010
	, CWT_KBD_CTRL_MODIFIER  = 0x0020
	, CWT_KBD_ALT_MODIFIER   = 0x0040
};
typedef enum CWT_KBD_MOD CWT_KBD_MOD;

enum CWT_MOUSE_BUTTON {
	  CWT_MOUSE_NO_BUTTON
	, CWT_MOUSE_LEFT_BUTTON   = 0x0001
	, CWT_MOUSE_RIGHT_BUTTON  = 0x0002
	, CWT_MOUSE_MIDDLE_BUTTON = 0x0004
};
typedef enum CWT_MOUSE_BUTTON CWT_MOUSE_BUTTON;

struct CWT_MOUSE_EVENT
{
	UINT16 status;
	int x; /* absolute x-position */
	int y; /* absolute y-position */
};
typedef struct CWT_MOUSE_EVENT CWT_MOUSE_EVENT;


struct CWT_CHANNEL_EVENT
{
	CHANNEL_PTR channel;
};
typedef struct CWT_CHANNEL_EVENT CWT_CHANNEL_EVENT;

struct CWT_CHILD_EVENT {
	void *child;
	BOOL added; /* false - removed */
};
typedef struct CWT_CHILD_EVENT CWT_CHILD_EVENT;

enum CWT_FOCUS_REASON
{
	  CWT_FOCUS_REASON_MOUSE = 0x01
	, CWT_FOCUS_REASON_TAB   = 0x02
	, CWT_FOCUS_REASON_ACTIVEWINDOW = 0x03
	, CWT_FOCUS_REASON_OTHER = 0x04
};
typedef enum CWT_FOCUS_REASON CWT_FOCUS_REASON;


#define CWT_KBD_KEY(pevt)		  (((CWT_KBD_EVENT*)(pevt)->ev_data)->key)
#define CWT_KBD_KEY_SCAN(key)	  (((key) >> 8) & 0x00FF)
#define CWT_KBD_KEY_ASCII(key)	  ((key) & 0x00FF)
#define CWT_KBD_MODS(pevt)	      (((CWT_KBD_EVENT*)(pevt)->ev_data)->mods)

#define CWT_KBD_RIGHT_SHIFT(mods) ((mods) & 0x0001)
#define CWT_KBD_LEFT_SHIFT(mods)  ((mods) & 0x0002)
#define CWT_KBD_SHIFT(mods)       (CWT_KBD_RIGHT_SHIFT(mods) || CWT_KBD_LEFT_SHIFT(mods))
#define CWT_KBD_LEFT_CTRL(mods)   ((mods) & 0x0100)
#define CWT_KBD_RIGHT_CTRL(mods)  ((mods) & 0x0400)
#define CWT_KBD_CTRL(mods)        (((mods) & 0x0004) || CWT_KBD_LEFT_CTRL(mods) || CWT_KBD_RIGHT_CTRL(mods))
#define CWT_KBD_LEFT_ALT(mods)    ((mods) & 0x0200)
#define CWT_KBD_RIGHT_ALT(mods)   ((mods) & 0x0800)
#define CWT_KBD_ALT(mods)         (((mods) & 0x0008) || CWT_KBD_LEFT_ALT(mods) || CWT_KBD_RIGHT_ALT(mods))
#define CWT_KBD_SCROLL_LOCK(mods) (((mods) & 0x0010) || ((mods) & 0x1000))
#define CWT_KBD_NUM_LOCK(mods)    (((mods) & 0x0020) || ((mods) & 0x2000))
#define CWT_KBD_CAPS_LOCK(mods)   (((mods) & 0x0040) || ((mods) & 0x4000))
#define CWT_KBD_INSERT_ON(mods)   ((mods) & 0x0080)
#define CWT_KBD_SYS_REQ(mods)     ((mods) & 0x8000)
#define CWT_KBD_NO_MODS(mods)     (!(CWT_KBD_SHIFT(mods) || CWT_KBD_CTRL(mods) || CWT_KBD_ALT(mods)))

#define CWT_KBD_TAB(pevt)        (CWT_KBD_NO_MODS(CWT_KBD_MODS(pevt)) && (CWT_KBD_KEY_SCAN(CWT_KBD_KEY(pevt)) == CWT_Key_Tab))
#define CWT_KBD_SHIFT_TAB(pevt)  (CWT_KBD_SHIFT(CWT_KBD_MODS(pevt)) && (CWT_KBD_KEY_SCAN(CWT_KBD_KEY(pevt)) == CWT_Key_Tab))
#define CWT_KBD_ALT_TAB(pevt)    (CWT_KBD_ALT(CWT_KBD_MODS(pevt)) && (CWT_KBD_KEY_SCAN(CWT_KBD_KEY(pevt)) == CWT_Key_Tab))
#define CWT_KBD_CTRL_TAB(pevt)   (CWT_KBD_CTRL(CWT_KBD_MODS(pevt)) && (CWT_KBD_KEY_SCAN(CWT_KBD_KEY(pevt)) == CWT_Key_Tab))


#define CWT_MOUSE_POS_X(pevt)               (((CWT_MOUSE_EVENT*)(pevt)->ev_data)->x)
#define CWT_MOUSE_POS_Y(pevt)               (((CWT_MOUSE_EVENT*)(pevt)->ev_data)->y)
#define CWT_MOUSE_STATUS(pevt)              (((CWT_MOUSE_EVENT*)(pevt)->ev_data)->status)
#define CWT_MOUSE_LEFT_BUTTON_EVENT(pevt)   (CWT_MOUSE_STATUS(pevt) & CWT_MOUSE_LEFT_BUTTON)
#define CWT_MOUSE_RIGHT_BUTTON_EVENT(pevt)  (CWT_MOUSE_STATUS(pevt) & CWT_MOUSE_RIGHT_BUTTON)
#define CWT_MOUSE_MIDDLE_BUTTON_EVENT(pevt) (CWT_MOUSE_STATUS(pevt) & CWT_MOUSE_MIDDLE_BUTTON)
#define CWT_MOUSE_KBD_ALT_MODIFIER(pevt)    (CWT_MOUSE_STATUS(pevt) & CWT_KBD_ALT_MODIFIER)
#define CWT_MOUSE_KBD_SHIFT_MODIFIER(pevt)  (CWT_MOUSE_STATUS(pevt) & CWT_KBD_SHIFT_MODIFIER)
#define CWT_MOUSE_KBD_CTRL_MODIFIER(pevt)   (CWT_MOUSE_STATUS(pevt) & CWT_KBD_CTRL_MODIFIER)

#define CWT_DEFAULT_FREE jq_free

int cwt_main_loop(CWT_EVENT_HANDLER);
void cwt_set_idle_proccess(void (*idle_process)(void));
void cwt_push_event_handler(CWT_EVENT_HANDLER ehandler);
CWT_EVENT_HANDLER cwt_pop_event_handler(void);
void cwt_post_event(CWT_EVENT_TYPE type, void *data, void (*delete_data_func)(void*));
#define cwt_post_quit_event() cwt_post_event(CWT_EVT_QUIT, NULL, NULL)
void cwt_post_kbd_event(UINT16 key, UINT16 mods);
void cwt_post_mouse_move_event(UINT16 status, int x, int y);
void cwt_post_mouse_press_event(UINT16 status, int x, int y);
void cwt_post_mouse_release_event(UINT16 status, int x, int y);
void cwt_post_child_event(void *child, BOOL added);

void cwt_add_channel_listener(CHANNEL_PTR chan);
void cwt_remove_channel_listener(CHANNEL_PTR chan);

void cwt_update_clock(void);
UINT16 cwt_kbd_key(CWT_EVENT_PTR pevt);
void cwt_start_timer(UINT16 id, long interval);

EXTERN_C_END

#endif /* _EVENT_H_ */
