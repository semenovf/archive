/*
 * colorx.h
 *
 *  Created on: 15.06.2012
 *      Author: wladt
 */

#ifndef _CWT_COLORX_H_
#define _CWT_COLORX_H_

#include <cwt/types.h>

typedef UINT32 Cwt_Color;

#define CWT_InvalidColor           ((Cwt_Color)0xF0000000L)
#define CWT_Transparent            ((Cwt_Color)0xE0000000L)
#define CWT_AliceBlue              ((Cwt_Color)0xF0F8FFL)
#define CWT_AntiqueWhite           ((Cwt_Color)0xFAEBD7L)
#define CWT_Aqua                   ((Cwt_Color)0x00FFFFL)
#define CWT_Aquamarine             ((Cwt_Color)0x7FFFD4L)
#define CWT_Azure                  ((Cwt_Color)0xF0FFFFL)
#define CWT_Beige                  ((Cwt_Color)0xF5F5DCL)
#define CWT_Bisque                 ((Cwt_Color)0xFFE4C4L)
#define CWT_Black                  ((Cwt_Color)0x000000L)
#define CWT_BlanchedAlmond         ((Cwt_Color)0xFFEBCDL)
#define CWT_Blue                   ((Cwt_Color)0x0000FFL)
#define CWT_BlueViolet             ((Cwt_Color)0x8A2BE2L)
#define CWT_Brown                  ((Cwt_Color)0xA52A2AL)
#define CWT_BurlyWood              ((Cwt_Color)0xDEB887L)
#define CWT_CadetBlue              ((Cwt_Color)0x5F9EA0L)
#define CWT_Chartreuse             ((Cwt_Color)0x7FFF00L)
#define CWT_Chocolate              ((Cwt_Color)0xD2691EL)
#define CWT_Coral                  ((Cwt_Color)0xFF7F50L)
#define CWT_CornflowerBlue         ((Cwt_Color)0x6495EDL)
#define CWT_Cornsilk               ((Cwt_Color)0xFFF8DCL)
#define CWT_Crimson                ((Cwt_Color)0xDC143CL)
#define CWT_Cyan                   ((Cwt_Color)0x00FFFFL)
#define CWT_DarkBlue               ((Cwt_Color)0x00008BL)
#define CWT_DarkCyan               ((Cwt_Color)0x008B8BL)
#define CWT_DarkGoldenRod          ((Cwt_Color)0xB8860BL)
#define CWT_DarkGray               ((Cwt_Color)0xA9A9A9L)
#define CWT_DarkGrey               ((Cwt_Color)0xA9A9A9L)
#define CWT_DarkGreen              ((Cwt_Color)0x006400L)
#define CWT_DarkKhaki              ((Cwt_Color)0xBDB76BL)
#define CWT_DarkMagenta            ((Cwt_Color)0x8B008BL)
#define CWT_DarkOliveGreen         ((Cwt_Color)0x556B2FL)
#define CWT_Darkorange             ((Cwt_Color)0xFF8C00L)
#define CWT_DarkOrchid             ((Cwt_Color)0x9932CCL)
#define CWT_DarkRed                ((Cwt_Color)0x8B0000L)
#define CWT_DarkSalmon             ((Cwt_Color)0xE9967AL)
#define CWT_DarkSeaGreen           ((Cwt_Color)0x8FBC8FL)
#define CWT_DarkSlateBlue          ((Cwt_Color)0x483D8BL)
#define CWT_DarkSlateGray          ((Cwt_Color)0x2F4F4FL)
#define CWT_DarkSlateGrey          ((Cwt_Color)0x2F4F4FL)
#define CWT_DarkTurquoise          ((Cwt_Color)0x00CED1L)
#define CWT_DarkViolet             ((Cwt_Color)0x9400D3L)
#define CWT_DeepPink               ((Cwt_Color)0xFF1493L)
#define CWT_DeepSkyBlue            ((Cwt_Color)0x00BFFFL)
#define CWT_DimGray                ((Cwt_Color)0x696969L)
#define CWT_DimGrey                ((Cwt_Color)0x696969L)
#define CWT_DodgerBlue             ((Cwt_Color)0x1E90FFL)
#define CWT_FireBrick              ((Cwt_Color)0xB22222L)
#define CWT_FloralWhite            ((Cwt_Color)0xFFFAF0L)
#define CWT_ForestGreen            ((Cwt_Color)0x228B22L)
#define CWT_Fuchsia                ((Cwt_Color)0xFF00FFL)
#define CWT_Gainsboro              ((Cwt_Color)0xDCDCDCL)
#define CWT_GhostWhite             ((Cwt_Color)0xF8F8FFL)
#define CWT_Gold                   ((Cwt_Color)0xFFD700L)
#define CWT_GoldenRod              ((Cwt_Color)0xDAA520L)
#define CWT_Gray                   ((Cwt_Color)0x808080L)
#define CWT_Grey                   ((Cwt_Color)0x808080L)
#define CWT_Green                  ((Cwt_Color)0x008000L)
#define CWT_GreenYellow            ((Cwt_Color)0xADFF2FL)
#define CWT_HoneyDew               ((Cwt_Color)0xF0FFF0L)
#define CWT_HotPink                ((Cwt_Color)0xFF69B4L)
#define CWT_IndianRed              ((Cwt_Color)0xCD5C5CL)
#define CWT_Indigo                 ((Cwt_Color)0x4B0082L)
#define CWT_Ivory                  ((Cwt_Color)0xFFFFF0L)
#define CWT_Khaki                  ((Cwt_Color)0xF0E68CL)
#define CWT_Lavender               ((Cwt_Color)0xE6E6FAL)
#define CWT_LavenderBlush          ((Cwt_Color)0xFFF0F5L)
#define CWT_LawnGreen              ((Cwt_Color)0x7CFC00L)
#define CWT_LemonChiffon           ((Cwt_Color)0xFFFACDL)
#define CWT_LightBlue              ((Cwt_Color)0xADD8E6L)
#define CWT_LightCoral             ((Cwt_Color)0xF08080L)
#define CWT_LightCyan              ((Cwt_Color)0xE0FFFFL)
#define CWT_LightGoldenRodYellow   ((Cwt_Color)0xFAFAD2L)
#define CWT_LightGray              ((Cwt_Color)0xD3D3D3L)
#define CWT_LightGrey              ((Cwt_Color)0xD3D3D3L)
#define CWT_LightGreen             ((Cwt_Color)0x90EE90L)
#define CWT_LightPink              ((Cwt_Color)0xFFB6C1L)
#define CWT_LightSalmon            ((Cwt_Color)0xFFA07AL)
#define CWT_LightSeaGreen          ((Cwt_Color)0x20B2AAL)
#define CWT_LightSkyBlue           ((Cwt_Color)0x87CEFAL)
#define CWT_LightSlateGray         ((Cwt_Color)0x778899L)
#define CWT_LightSlateGrey         ((Cwt_Color)0x778899L)
#define CWT_LightSteelBlue         ((Cwt_Color)0xB0C4DEL)
#define CWT_LightYellow            ((Cwt_Color)0xFFFFE0L)
#define CWT_Lime                   ((Cwt_Color)0x00FF00L)
#define CWT_LimeGreen              ((Cwt_Color)0x32CD32L)
#define CWT_Linen                  ((Cwt_Color)0xFAF0E6L)
#define CWT_Magenta                ((Cwt_Color)0xFF00FFL)
#define CWT_Maroon                 ((Cwt_Color)0x800000L)
#define CWT_MediumAquaMarine       ((Cwt_Color)0x66CDAAL)
#define CWT_MediumBlue             ((Cwt_Color)0x0000CDL)
#define CWT_MediumOrchid           ((Cwt_Color)0xBA55D3L)
#define CWT_MediumPurple           ((Cwt_Color)0x9370D8L)
#define CWT_MediumSeaGreen         ((Cwt_Color)0x3CB371L)
#define CWT_MediumSlateBlue        ((Cwt_Color)0x7B68EEL)
#define CWT_MediumSpringGreen      ((Cwt_Color)0x00FA9AL)
#define CWT_MediumTurquoise        ((Cwt_Color)0x48D1CCL)
#define CWT_MediumVioletRed        ((Cwt_Color)0xC71585L)
#define CWT_MidnightBlue           ((Cwt_Color)0x191970L)
#define CWT_MintCream              ((Cwt_Color)0xF5FFFAL)
#define CWT_MistyRose              ((Cwt_Color)0xFFE4E1L)
#define CWT_Moccasin               ((Cwt_Color)0xFFE4B5L)
#define CWT_NavajoWhite            ((Cwt_Color)0xFFDEADL)
#define CWT_Navy                   ((Cwt_Color)0x000080L)
#define CWT_OldLace                ((Cwt_Color)0xFDF5E6L)
#define CWT_Olive                  ((Cwt_Color)0x808000L)
#define CWT_OliveDrab              ((Cwt_Color)0x6B8E23L)
#define CWT_Orange                 ((Cwt_Color)0xFFA500L)
#define CWT_OrangeRed              ((Cwt_Color)0xFF4500L)
#define CWT_Orchid                 ((Cwt_Color)0xDA70D6L)
#define CWT_PaleGoldenRod          ((Cwt_Color)0xEEE8AAL)
#define CWT_PaleGreen              ((Cwt_Color)0x98FB98L)
#define CWT_PaleTurquoise          ((Cwt_Color)0xAFEEEEL)
#define CWT_PaleVioletRed          ((Cwt_Color)0xD87093L)
#define CWT_PapayaWhip             ((Cwt_Color)0xFFEFD5L)
#define CWT_PeachPuff              ((Cwt_Color)0xFFDAB9L)
#define CWT_Peru                   ((Cwt_Color)0xCD853FL)
#define CWT_Pink                   ((Cwt_Color)0xFFC0CBL)
#define CWT_Plum                   ((Cwt_Color)0xDDA0DDL)
#define CWT_PowderBlue             ((Cwt_Color)0xB0E0E6L)
#define CWT_Purple                 ((Cwt_Color)0x800080L)
#define CWT_Red                    ((Cwt_Color)0xFF0000L)
#define CWT_RosyBrown              ((Cwt_Color)0xBC8F8FL)
#define CWT_RoyalBlue              ((Cwt_Color)0x4169E1L)
#define CWT_SaddleBrown            ((Cwt_Color)0x8B4513L)
#define CWT_Salmon                 ((Cwt_Color)0xFA8072L)
#define CWT_SandyBrown             ((Cwt_Color)0xF4A460L)
#define CWT_SeaGreen               ((Cwt_Color)0x2E8B57L)
#define CWT_SeaShell               ((Cwt_Color)0xFFF5EEL)
#define CWT_Sienna                 ((Cwt_Color)0xA0522DL)
#define CWT_Silver                 ((Cwt_Color)0xC0C0C0L)
#define CWT_SkyBlue                ((Cwt_Color)0x87CEEBL)
#define CWT_SlateBlue              ((Cwt_Color)0x6A5ACDL)
#define CWT_SlateGray              ((Cwt_Color)0x708090L)
#define CWT_SlateGrey              ((Cwt_Color)0x708090L)
#define CWT_Snow                   ((Cwt_Color)0xFFFAFAL)
#define CWT_SpringGreen            ((Cwt_Color)0x00FF7FL)
#define CWT_SteelBlue              ((Cwt_Color)0x4682B4L)
#define CWT_Tan                    ((Cwt_Color)0xD2B48CL)
#define CWT_Teal                   ((Cwt_Color)0x008080L)
#define CWT_Thistle                ((Cwt_Color)0xD8BFD8L)
#define CWT_Tomato                 ((Cwt_Color)0xFF6347L)
#define CWT_Turquoise              ((Cwt_Color)0x40E0D0L)
#define CWT_Violet                 ((Cwt_Color)0xEE82EEL)
#define CWT_Wheat                  ((Cwt_Color)0xF5DEB3L)
#define CWT_White                  ((Cwt_Color)0xFFFFFFL)
#define CWT_WhiteSmoke             ((Cwt_Color)0xF5F5F5L)
#define CWT_Yellow                 ((Cwt_Color)0xFFFF00L)
#define CWT_YellowGreen            ((Cwt_Color)0x9ACD32L)
#define CWT_MaxColor               CWT_White


Cwt_Color cwt_stylex_colorByName(const CWT_CHAR *name);
BOOL cwt_stylex_parseColor(const CWT_CHAR *str, Cwt_Color *pcolor);

#ifdef CWT_USE_TESTS
#	include <cwt/test.h>
	void cwt_stylex_test_parse_color(void);
#endif


#endif /* _CWT_COLORX_H_ */
