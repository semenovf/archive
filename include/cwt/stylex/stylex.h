/*
 * stylex.h
 *
 *  Created on: 14.06.2012
 *      Author: user
 *
 * Widget's style
 *
 */
#ifndef _CWT_STYLEX_H_
#define _CWT_STYLEX_H_

#include <cwt/global.h>
#include <cwt/types.h>
#include <cwt/list.h>
#include <cwt/stylex/colorx.h>

enum Cwt_StateType {
	  Cwt_State_Normal
	, Cwt_State_Hover
	, Cwt_State_Focus
	, CWT_State_Inactive

	, CWT_State_Count
	, Cwt_State_Default = Cwt_State_Normal
};
typedef enum Cwt_StateType Cwt_StateType;

enum Cwt_StyleAttrType {
	/* Background */
	  Cwt_StyleAttr_BackgroundColor /* Cwt_Color */

	/* Border Left */
	, Cwt_StyleAttr_BorderLeftWidth /* unsigned integer */
	, Cwt_StyleAttr_BorderLeftColor /* Cwt_Color */
	, Cwt_StyleAttr_BorderLeftStyle /* Cwt_BorderStyle */

	/* Border Top */
	, Cwt_StyleAttr_BorderTopWidth /* unsigned integer */
	, Cwt_StyleAttr_BorderTopColor /* Cwt_Color */
	, Cwt_StyleAttr_BorderTopStyle /* Cwt_BorderStyle */

	/* Border Right */
	, Cwt_StyleAttr_BorderRightWidth /* unsigned integer */
	, Cwt_StyleAttr_BorderRightColor /* Cwt_Color */
	, Cwt_StyleAttr_BorderRightStyle /* Cwt_BorderStyle */

	/* Border Bottom */
	, Cwt_StyleAttr_BorderBottomWidth /* unsigned integer */
	, Cwt_StyleAttr_BorderBottomColor /* Cwt_Color */
	, Cwt_StyleAttr_BorderBottomStyle /* Cwt_BorderStyle */

	/* Geometry */
	, Cwt_StyleAttr_Left            /* signed int */
	, Cwt_StyleAttr_Top             /* signed int */
	, Cwt_StyleAttr_Width           /* signed int */
	, Cwt_StyleAttr_Height          /* signed int */

	/* Color */
	, Cwt_StyleAttr_Color           /* Cwt_Color */

	/* Font */
	, Cwt_StyleAttr_FontFamily      /* string */
	, Cwt_StyleAttr_FontSize        /* unsigned integer */
	, Cwt_StyleAttr_FontStyle       /* Cwt_FontStyle */
	, Cwt_StyleAttr_FontWeight      /* Cwt_FontWeight */

	/* Margin */
	, Cwt_StyleAttr_MarginLeft      /* signed integer */
	, Cwt_StyleAttr_MarginTop       /* signed integer */
	, Cwt_StyleAttr_MarginRight     /* signed integer */
	, Cwt_StyleAttr_MarginBottom    /* signed integer */

	/* Padding */
	, Cwt_StyleAttr_PaddingLeft     /* unsigned integer */
	, Cwt_StyleAttr_PaddingTop      /* unsigned integer */
	, Cwt_StyleAttr_PaddingRight    /* unsigned integer */
	, Cwt_StyleAttr_PaddingBottom   /* unsigned integer */

	/* Max/Min width/height */
	, Cwt_StyleAttr_MaxWidth		/* unsigned integer */
	, Cwt_StyleAttr_MaxHeight		/* unsigned integer */
	, Cwt_StyleAttr_MinWidth		/* unsigned integer */
	, Cwt_StyleAttr_MinHeight		/* unsigned integer */

	/* Opacity */
	, Cwt_StyleAttr_Opacity         /* float: [0.0, 1.0] */

	/* Position */
	, Cwt_StyleAttr_Position		/* Cwt_Position */

	/* Text */
	, Cwt_StyleAttr_TextIndent    	/* signed integer */
	, Cwt_StyleAttr_TextAlign     	/* Cwt_Style_TextAlign */

	/* Align */
	, Cwt_StyleAttr_VAlign 			/* Cwt_Style_VAlign */
	, Cwt_StyleAttr_HAlign 			/* Cwt_Style_HAlign */

	, Cwt_StyleAttr_Count
};
typedef enum Cwt_StyleAttrType Cwt_StyleAttrType;

enum Cwt_VAlign {
	  Cwt_VAlign_Inherit
	, Cwt_VAlign_Top
	, Cwt_VAlign_Middle
	, Cwt_VAlign_Bottom
};
typedef enum Cwt_Style_VAlign Cwt_Style_VAlign;

enum Cwt_HAlign {
	  Cwt_HAlign_Inherit
	, Cwt_HAlign_Left
	, Cwt_HAlign_Center
	, Cwt_HAlign_Right
};
typedef enum Cwt_Style_HAlign Cwt_Style_HAlign;
typedef enum Cwt_Style_HAlign Cwt_Style_TextAlign;

enum Cwt_Position {
	  Cwt_Position_Inherit
	, Cwt_Position_Absolute
	, Cwt_Position_Fixed
	, Cwt_Position_Relative
	, Cwt_Position_Static
};
typedef enum Cwt_Position Cwt_Position;

enum Cwt_FontStyle {
	Cwt_FontStyle_Inherit
	, Cwt_FontStyle_Normal
	, Cwt_FontStyle_Italic
	, Cwt_FontStyle_Oblique
};
typedef enum Cwt_FontStyle Cwt_FontStyle;

enum Cwt_FontWeight {
	  Cwt_FontWeight_Inherit
	, Cwt_FontWeight_Bold
	, Cwt_FontWeight_Bolder
	, Cwt_FontWeight_Lighter
	, Cwt_FontWeight_Normal
	, Cwt_FontWeight_100
	, Cwt_FontWeight_200
	, Cwt_FontWeight_300
	, Cwt_FontWeight_400
	, Cwt_FontWeight_500
	, Cwt_FontWeight_600
	, Cwt_FontWeight_700
	, Cwt_FontWeight_800
	, Cwt_FontWeight_900

};
typedef enum Cwt_FontWeight Cwt_FontWeight;

enum Cwt_BorderStyle {
	  Cwt_BorderStyle_Inherit
	, Cwt_BorderStyle_None
	, Cwt_BorderStyle_Hidden
	, Cwt_BorderStyle_Dotted
	, Cwt_BorderStyle_Dashed
	, Cwt_BorderStyle_Solid
	, Cwt_BorderStyle_Double
	, Cwt_BorderStyle_Groove
	, Cwt_BorderStyle_Ridge
	, Cwt_BorderStyle_Inset
	, Cwt_BorderStyle_Outset
};
typedef enum Cwt_BorderStyle Cwt_BorderStyle;


union Cwt_StyleAttrValue {
	Cwt_Color			bg_color;

	UINT				bg_left_width;
	Cwt_Color			bg_left_color;
	Cwt_BorderStyle		bg_left_style;

	UINT				bg_top_width;
	Cwt_Color			bg_top_color;
	Cwt_BorderStyle		bg_top_style;

	UINT				bg_right_width;
	Cwt_Color			bg_right_color;
	Cwt_BorderStyle		bg_right_style;

	UINT				bg_bottom_width;
	Cwt_Color			bg_bottom_color;
	Cwt_BorderStyle		bg_bottom_style;

	INT                 left;
	INT                 top;
	INT                 width;
	INT                 height;

	Cwt_Color           color;

	CHAR*               font_family;
	UINT                font_size;
	Cwt_FontStyle       font_style;
	Cwt_FontWeight      font_weight;

	INT                 margin_left;
	INT                 margin_top;
	INT                 margin_right;
	INT                 margin_bottom;

	UINT				padding_left;
	UINT				padding_top;
	UINT				padding_right;
	UINT				padding_bottom;

	UINT                max_width;
	UINT                max_height;
	UINT                min_width;
	UINT                min_height;

	float               opacity;
	Cwt_Position        position;
	INT                 text_indent;
	Cwt_Style_TextAlign text_align;
	Cwt_Style_VAlign    valign;
	Cwt_Style_HAlign    halign;
};
typedef union Cwt_StyleAttrValue Cwt_StyleAttrValue;


struct Cwt_StyleAttr {
	ListNode           m_base;
	Cwt_StyleAttrType  m_type;
	Cwt_StyleAttrValue m_value;
};
typedef struct Cwt_StyleAttr  Cwt_StyleAttr;
typedef struct Cwt_StyleAttr* Cwt_StyleAttrPtr;


struct Cwt_StyleAttrSet {
	List m_states[CWT_State_Count];
};
typedef struct Cwt_StyleAttrSet Cwt_StyleAttrSet;
typedef struct Cwt_StyleAttrSet* Cwt_StyleAttrSetPtr;


enum CwtStyleSetType {
	  Cwt_StyleSet_Id
	, Cwt_StyleSet_Class
	, Cwt_StyleSet_State
	, Cwt_StyleSet_WType
	, Cwt_StyleSet_Count
};
typedef enum CwtStyleSetType CwtStyleSetType;

struct Cwt_Style {
	Cwt_StyleAttr m_attrs[Cwt_StyleAttr_Count];
};
typedef struct Cwt_Style  Cwt_Style;
typedef struct Cwt_Style* Cwt_StylePtr;
typedef const struct Cwt_Style* Cwt_StyleConstPtr;


EXTERN_C_BEGIN

void cwt_stylex_init(void);
void cwt_stylex_release(void);
void cwt_stylex_loadFromString(const CHAR *stylexStr);
void cwt_stylex_loadFromFile(const CHAR *stylexPath);

void cwt_stylex_cloneStyle(Cwt_StylePtr clone, Cwt_StyleConstPtr orig);
Cwt_StyleAttrSetPtr cwt_stylex_styleAttrSetByMedia(const CHAR *media, BOOL create);
Cwt_StyleAttrSetPtr cwt_stylex_styleAttrSetById(const CHAR *id, BOOL create);
Cwt_StyleAttrSetPtr cwt_stylex_styleAttrSetByClass(const CHAR *cls, BOOL create);
Cwt_StyleAttrSetPtr cwt_stylex_styleAttrSetByWType(const CHAR *wtype, BOOL create);

Cwt_StyleAttrSetPtr cwt_stylex_newStyleAttrSet(void);

void cwt_stylex_assembly(Cwt_StylePtr pstyle
	, const CHAR *media
	, const CHAR *wtype
	, const CHAR *cls
	, const CHAR *id
	, Cwt_StateType state);



/*
void cwt_stylex_setOpacity(Cwt_StyleAttrSetPtr attrSet, float opacity);
void cwt_stylex_setPosition(Cwt_StyleAttrSetPtr attrSet, Cwt_Position position);
void cwt_stylex_setTextIndent(Cwt_StyleAttrSetPtr attrSet, INT text_indent);
void cwt_stylex_setTextAlign(Cwt_StyleAttrSetPtr attrSet, Cwt_Style_TextAlign text_align);
void cwt_stylex_setVAlign(Cwt_StyleAttrSetPtr attrSet, Cwt_Style_VAlign valign);
void cwt_stylex_setHAlign(Cwt_StyleAttrSetPtr attrSet, Cwt_Style_HAlign halign);
*/
/*void cwt_style_attr_set(Cwt_StyleAttrSetPtr attrSet, );*/

#ifdef CWT_USE_TESTS
#	include <cwt/test.h>
	void cwt_stylex_test_parse_attrs(void);
#endif

EXTERN_C_END

#endif /* _CWT_STYLEX_H_ */
