/*
 * cwt/eevnt/kbd.h
 *
 *  Created on: 24.06.2012
 *      Author: wladt
 */

#ifndef __CWT_EVENT_KBD_H__
#define __CWT_EVENT_KBD_H__

#include <cwt/event/types.h>

enum CwtKbdKey {
	  CwtKey_Escape
	, CwtKey_Tilde
	, CwtKey_F1
	, CwtKey_F2
	, CwtKey_F3
	, CwtKey_F4
	, CwtKey_F5
	, CwtKey_F6
	, CwtKey_F7
	, CwtKey_F8
	, CwtKey_F9
	, CwtKey_F10
	, CwtKey_F11
	, CwtKey_F12
	, CwtKey_0
	, CwtKey_1
	, CwtKey_2
	, CwtKey_3
	, CwtKey_4
	, CwtKey_5
	, CwtKey_6
	, CwtKey_7
	, CwtKey_8
	, CwtKey_9
	, CwtKey_A
	, CwtKey_B
	, CwtKey_C
	, CwtKey_D
	, CwtKey_E
	, CwtKey_F
	, CwtKey_G
	, CwtKey_H
	, CwtKey_I
	, CwtKey_J
	, CwtKey_K
	, CwtKey_L
	, CwtKey_M
	, CwtKey_N
	, CwtKey_O
	, CwtKey_P
	, CwtKey_Q
	, CwtKey_R
	, CwtKey_S
	, CwtKey_T
	, CwtKey_U
	, CwtKey_V
	, CwtKey_W
	, CwtKey_X
	, CwtKey_Y
	, CwtKey_Z
	, CwtKey_Tab
	, CwtKey_Backspace
	, CwtKey_Enter
	, CwtKey_Insert
	, CwtKey_Delete
	, CwtKey_Home
	, CwtKey_End
	, CwtKey_Left
	, CwtKey_Up
	, CwtKey_Right
	, CwtKey_Down
	, CwtKey_PageUp
	, CwtKey_PageDown
	, CwtKey_Break	/*The Pause/Break key */
	, CwtKey_Meta	/* On Windows keyboards, this key is mapped to the Windows key */
	, CwtKey_Space
	, CwtKey_Minus
	, CwtKey_Hypen = CwtKey_Minus
	, CwtKey_Dash = CwtKey_Minus
	, CwtKey_Equal
	, CwtKey_Backslash
	, CwtKey_BracketLeft    /*[*/
	, CwtKey_BracketRight   /*]*/
	, CwtKey_Semicolon
	, CwtKey_Quote
	, CwtKey_Comma
	, CwtKey_Period
	, CwtKey_Slash
};
typedef enum CwtKbdKey CwtKbdKey;

enum CwtKbdMod {
	  CwtKbdMod_NoMod      = 0x0000
	, CwtKbdMod_LeftShift  = 0x0001
	, CwtKbdMod_RightShift = 0x0002
	, CwtKbdMod_Shift      = CwtKbdMod_LeftShift | CwtKbdMod_RightShift
	, CwtKbdMod_LeftCtrl   = 0x0004
	, CwtKbdMod_RightCtrl  = 0x0008
	, CwtKbdMod_Ctrl       = CwtKbdMod_LeftCtrl  | CwtKbdMod_RightCtrl
	, CwtKbdMod_LeftAlt    = 0x0010
	, CwtKbdMod_RightAlt   = 0x0020
	, CwtKbdMod_Alt        = CwtKbdMod_LeftAlt   | CwtKbdMod_RightAlt
	, CwtKbdMod_ScrollLock = 0x0040
	, CwtKbdMod_NumLock    = 0x0080
	, CwtKbdMod_CapsLock   = 0x0100
	, CwtKbdMod_InsertOn   = 0x0200
	, CwtKbdMod_SysRq      = 0x0400
};
typedef enum CwtKbdMod CwtKbdMod;

/*
#define CWT_Key_Backspace 0x0E
#define CWT_Key_Delete    0x53
#define CWT_Key_Insert    0x52
#define CWT_Key_Esc       0x01
#define CWT_Key_Enter     0x1C
#define CWT_Key_Left      0x4B
#define CWT_Key_Right     0x4D
#define CWT_Key_Up        0x48
#define CWT_Key_Down      0x50
#define CWT_Key_Home      0x47
#define CWT_Key_End       0x4F
#define CWT_Key_Tab       0x0F
#define CWT_Key_PgUp      0x49
#define CWT_Key_PgDown    0x51
#define CWT_Key_Space     0x39

#define CWT_Key_F1        0x3B
#define CWT_Key_F2        0x3C
#define CWT_Key_F3        0x3D
#define CWT_Key_F4        0x3E
#define CWT_Key_F10	      0x44

#define CWT_Key_Q         0x10
#define CWT_Key_P	      0x19
*/


EXTERN_C_BEGIN
#ifdef __COMMENT__
DLL_API_EXPORT CwtEventSourcePtr cwtEventKbdSource(void);
DLL_API_EXPORT Stack* cwtEventKbdHandler(void);

DLL_API_EXPORT void cwtEventPeekKbd(CwtEventPtr pevt, CwtKbdKey *key, UINT16 *mods);
DLL_API_EXPORT void cwtEventPostKbd();
#endif
EXTERN_C_END


#endif /* __CWT_EVENT_KBD_H__ */
