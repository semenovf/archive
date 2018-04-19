/*
 * kbd.c
 *
 *  Created on: 24.06.2012
 *      Author: wladt
 */
#ifdef __COMMENT__

#include <cwt/event/kbd.h>
#include <cwt/event/event.h>
/*#include <cwt/string.h>*/


#ifdef CWT_OS_MSDOS
#	include <bios.h> /* bioskey */
#else
#	error "No implementation"
#endif

struct CwtEventKbd
{
	UINT16 key;
	UINT16 mods;
};
typedef struct CwtEventKbd CwtEventKbd;


/* Source: http://www.freepascal.org/docs-html/rtl/keyboard/kbdscancode.html */
/*
Key Scancodes 		Code 	Key 		Code 	Key 	Code 	Key
00 	NoKey 			3D 	F3 				70 	ALT-F9
01 	ALT-Esc 		3E 	F4 				71 	ALT-F10
02 	ALT-Space 		3F 	F5 				72 	CTRL-PrtSc
04 	CTRL-Ins 		40 	F6 				73 	CTRL-Left
05 	SHIFT-Ins 		41 	F7 				74 	CTRL-Right
06 	CTRL-Del 		42 	F8 				75 	CTRL-end
07 	SHIFT-Del 		43 	F9 				76 	CTRL-PgDn
08 	ALT-Back 		44 	F10 			77 	CTRL-Home
09 	ALT-SHIFT-Back 	47 	Home 			78 	ALT-1
0F 	SHIFT-Tab 		48 	Up 				79 	ALT-2
10 	ALT-Q 			49 	PgUp 			7A 	ALT-3
11 	ALT-W 			4B 	Left 			7B 	ALT-4
12 	ALT-E 			4C 	Center 			7C 	ALT-5
13 	ALT-R 			4D 	Right 			7D 	ALT-6
14 	ALT-T 			4E 	ALT-GrayPlus 	7E 	ALT-7
15 	ALT-Y 			4F 	end 			7F 	ALT-8
16 	ALT-U 			50 	Down 			80 	ALT-9
17 	ALT-I 			51 	PgDn 			81 	ALT-0
18 	ALT-O 			52 	Ins 			82 	ALT-Minus
19 	ALT-P 			53 	Del 			83 	ALT-Equal
1A 	ALT-LftBrack 	54 	SHIFT-F1 		84 	CTRL-PgUp
1B 	ALT-RgtBrack 	55 	SHIFT-F2 		85 	F11
1E 	ALT-A 			56 	SHIFT-F3 		86 	F12
1F 	ALT-S 			57 	SHIFT-F4 		87 	SHIFT-F11
20 	ALT-D 			58 	SHIFT-F5 		88 	SHIFT-F12
21 	ALT-F 			59 	SHIFT-F6 		89 	CTRL-F11
22 	ALT-G 			5A 	SHIFT-F7 		8A 	CTRL-F12
23 	ALT-H 			5B 	SHIFT-F8 		8B 	ALT-F11
24 	ALT-J 			5C 	SHIFT-F9 		8C 	ALT-F12
25 	ALT-K 			5D 	SHIFT-F10 		8D 	CTRL-Up
26 	ALT-L 			5E 	CTRL-F1 		8E 	CTRL-Minus
27 	ALT-SemiCol 	5F 	CTRL-F2 		8F 	CTRL-Center
28 	ALT-Quote 		60 	CTRL-F3 		90 	CTRL-GreyPlus
29 	ALT-OpQuote 	61 	CTRL-F4 		91 	CTRL-Down
2B 	ALT-BkSlash 	62 	CTRL-F5 		94 	CTRL-Tab
2C 	ALT-Z 			63 	CTRL-F6 		97 	ALT-Home
2D 	ALT-X 			64 	CTRL-F7 		98 	ALT-Up
2E 	ALT-C 			65 	CTRL-F8 		99 	ALT-PgUp
2F 	ALT-V 			66 	CTRL-F9 		9B 	ALT-Left
30 	ALT-B 			67 	CTRL-F10 		9D 	ALT-Right
31 	ALT-N 			68 	ALT-F1 			9F 	ALT-end
32 	ALT-M 			69 	ALT-F2 			A0 	ALT-Down
33 	ALT-Comma 		6A 	ALT-F3 			A1 	ALT-PgDn
34 	ALT-Period 		6B 	ALT-F4 			A2 	ALT-Ins
35 	ALT-Slash 		6C 	ALT-F5 			A3 	ALT-Del
37 	ALT-GreyAst 	6D 	ALT-F6 			A5 	ALT-Tab
3B 	F1 				6E 	ALT-F7
3C 	F2 				6F 	ALT-F8 	*/
/*
Key 	Code 	SHIFT-Key 	CTRL-Key 	Alt-Key
F1 		3B 		54 			5E 			68
F2 		3C 		55 			5F 			69
F3 		3D 		56 			60 			6A
F4 		3E 		57 			61 			6B
F5 		3F 		58 			62 			6C
F6 		40 		59 			63 			6D
F7 		41 		5A 			64 			6E
F8 		42 		5A 			65 			6F
F9 		43 		5B 			66 			70
F10 	44 		5C 			67 			71
F11 	85 		87 			89 			8B
F12 	86 		88 			8A 			8C
Home 	47 					77 			97
Up 		48 					8D 			98
PgUp 	49 					84 			99
Left 	4B 					73 			9B
Center 	4C 					8F
Right 	4D 					74 			9D
End 	4F 					75 			9F
Down 	50 					91 			A0
PgDn 	51 					76 			A1
Ins 	52 		05 			04 			A2
Del 	53 		07 			06 			A3
Tab 	8 		0F 			94 			A5
*/
UINT8 __cwt_evt_kbd_map[256] = {
/*    0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F    */
/*0*/ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F /*0*/
/*1*/,0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F /*1*/
/*2*/,0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F /*2*/
/*3*/,0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F /*3*/
/*4*/,0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F /*4*/
/*5*/,0x50, 0x51, 0x52, 0x53, 0x3B, 0x55, 0x56, 0x3E, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x44, 0x3B, 0x5F /*5*/
/*6*/,0x60, 0x3E, 0x62, 0x63, 0x64, 0x65, 0x66, 0x44, 0x3B, 0x69, 0x6A, 0x3E, 0x6C, 0x6D, 0x6E, 0x6F /*6*/
/*7*/,0x70, 0x44, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F /*7*/
/*8*/,0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F /*8*/
/*9*/,0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F /*9*/
/*A*/,0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF /*A*/
/*B*/,0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF /*B*/
/*C*/,0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF /*C*/
/*D*/,0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF /*D*/
/*E*/,0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF /*E*/
/*F*/,0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF /*F*/
};

static void __cwtEventKbdPoll(void);

static CwtEventSource __cwt_evt_kbd_source = { __cwtEventKbdPoll };
static Stack __cwt_evt_kbd_handlers = { NULL };

CwtEventSourcePtr cwtEventKbdSource(void)
{
	return &__cwt_evt_kbd_source;
}

Stack* cwtEventKbdHandler(void)
{
	return &__cwt_evt_kbd_handlers;
}

static void __cwtEventKbdPoll(void)
{
	UINT16 key;
	UINT16 mods;

#ifdef JQ_OS_DOS
	if( bioskey(_KEYBRD_READY) ) { /* is equivalent to kbhit() but last resides in conio.h header file */
		key = bioskey(_KEYBRD_READ);
		mods = bioskey(_KEYBRD_SHIFTSTATUS);
		key = ((__cwt_evt_kbd_map[ ((key >> 8) & 0x00FF) ] << 8) & 0xFF00) | (key & 0xFF);
	}
#endif

	cwtEventPostKbd(key, mods);
}


void cwtEventPeekKbd(CwtEventPtr pevt, UINT16 *key, UINT16 *mods)
{
	CwtEventKbd *pevt_kbd = (CwtEventKbd*)pevt;
	JQ_ASSERT(pevt);

	if( key )  *key  = pevt_kbd->key;
	if( mods ) *mods = pevt_kbd->mods;
}


void cwtEventPostKbd(UINT16 key, UINT16 mods)
{
	CwtEventKbd *pevt = JQ_MALLOC(CwtEventKbd);

	pevt->key = key;
	pevt->mods = mods;

	/* FIXME remove line below */
	pevt->mods &= ~0x200;

	__cwt_evtInitEvent((CwtEventPtr)pevt, &__cwt_evt_kbd_handlers, cwtEventDefaultDestructor);
	cwtEventPost((CwtEventPtr)pevt);
}


#ifdef __COMMENT__
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
#endif

#endif
