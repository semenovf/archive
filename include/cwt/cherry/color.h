/*
 * color.h
 *
 *  Created on: 09.12.2011
 *      Author: wladt
 */

#ifndef _CWT_COLOR_H_
#define _CWT_COLOR_H_

/*#if defined(__GRAPHICS_H) && defined(JQ_CC_BORLAND)*/
#	define CWT_Transparent          -1
#   define CWT_White                15  /* constant is better than WHITE (WHITE in libbcc is not a constant, it's function's call), see libbcc.h */
#   define CWT_Black                BLACK
#   define CWT_Red                  RED
#   define CWT_LightRed             LIGHTRED
#   define CWT_Green                GREEN
#   define CWT_LightGreen           LIGHTGREEN
#   define CWT_Cyan                 CYAN
#   define CWT_LightCyan            LIGHTCYAN
#	define CWT_Blue					BLUE
#	define CWT_LightBlue			LIGHTBLUE
#	define CWT_Yellow       		YELLOW
#	define CWT_LightGray			LIGHTGRAY
#	define CWT_DarkGray				DARKGRAY
#	define CWT_Magenta				MAGENTA
#	define CWT_LightMagenta			LIGHTMAGENTA
#	define CWT_Brown				BROWN


#if 0 /*defined(__BCC2GRX__)*/
#	define CWT_Transparent          -1
#   define CWT_White                15
#   define CWT_Black                BLACK
#   define CWT_Red                  RED
#   define CWT_LightRed             LIGHTRED
#   define CWT_Green                GREEN
#   define CWT_LightGreen           LIGHTGREEN
#   define CWT_Cyan                 CYAN
#   define CWT_LightCyan            LIGHTCYAN
#	define CWT_Blue					BLUE
#	define CWT_LightBlue			LIGHTBLUE
#	define CWT_Yellow       		YELLOW
#	define CWT_LightGray			LIGHTGRAY
#	define CWT_DarkGray				DARKGRAY
#	define CWT_Magenta				MAGENTA
#	define CWT_LightMagenta			LIGHTMAGENTA
#	define CWT_Brown				BROWN
#endif

#if 0
#	define CWT_AliceBlue  	0xF0F8FF
#	define CWT_AntiqueWhite   0xFAEBD7
#	define CWT_Aqua   0x00FFFF
#	define CWT_Aquamarine   0x7FFFD4
#	define CWT_Azure   0xF0FFFF
#	define CWT_Beige   0xF5F5DC
#	define CWT_Bisque   0xFFE4C4
#	define CWT_Black   0x000000
#	define CWT_BlanchedAlmond   0xFFEBCD
#	define CWT_Blue   0x0000FF
#	define CWT_BlueViolet   0x8A2BE2
#	define CWT_Brown   0xA52A2A
#	define CWT_BurlyWood   0xDEB887
#	define CWT_CadetBlue   0x5F9EA0
#	define CWT_Chartreuse   0x7FFF00
#	define CWT_Chocolate   0xD2691E
#	define CWT_Coral   0xFF7F50
#	define CWT_CornflowerBlue   0x6495ED
#	define CWT_Cornsilk   0xFFF8DC
#	define CWT_Crimson   0xDC143C
#	define CWT_Cyan   0x00FFFF
#	define CWT_DarkBlue   0x00008B
#	define CWT_DarkCyan   0x008B8B
#	define CWT_DarkGoldenRod   0xB8860B
#	define CWT_DarkGray   0xA9A9A9
#	define CWT_DarkGrey   0xA9A9A9
#	define CWT_DarkGreen   0x006400
#	define CWT_DarkKhaki   0xBDB76B
#	define CWT_DarkMagenta   0x8B008B
#	define CWT_DarkOliveGreen   0x556B2F
#	define CWT_Darkorange   0xFF8C00
#	define CWT_DarkOrchid   0x9932CC
#	define CWT_DarkRed   0x8B0000
#	define CWT_DarkSalmon   0xE9967A
#	define CWT_DarkSeaGreen   0x8FBC8F
#	define CWT_DarkSlateBlue   0x483D8B
#	define CWT_DarkSlateGray   0x2F4F4F
#	define CWT_DarkSlateGrey   0x2F4F4F
#	define CWT_DarkTurquoise   0x00CED1
#	define CWT_DarkViolet   0x9400D3
#	define CWT_DeepPink   0xFF1493
#	define CWT_DeepSkyBlue   0x00BFFF
#	define CWT_DimGray   0x696969
#	define CWT_DimGrey   0x696969
#	define CWT_DodgerBlue   0x1E90FF
#	define CWT_FireBrick   0xB22222
#	define CWT_FloralWhite   0xFFFAF0
#	define CWT_ForestGreen   0x228B22
#	define CWT_Fuchsia   0xFF00FF
#	define CWT_Gainsboro   0xDCDCDC
#	define CWT_GhostWhite   0xF8F8FF
#	define CWT_Gold   0xFFD700
#	define CWT_GoldenRod   0xDAA520
#	define CWT_Gray   0x808080
#	define CWT_Grey   0x808080
#	define CWT_Green   0x008000
#	define CWT_GreenYellow   0xADFF2F
#	define CWT_HoneyDew   0xF0FFF0
#	define CWT_HotPink   0xFF69B4
#	define CWT_IndianRed    0xCD5C5C
#	define CWT_Indigo    0x4B0082
#	define CWT_Ivory                  0xFFFFF0
#	define CWT_Khaki                  0xF0E68C
#	define CWT_Lavender               0xE6E6FA
#	define CWT_LavenderBlush          0xFFF0F5
#	define CWT_LawnGreen              0x7CFC00
#	define CWT_LemonChiffon           0xFFFACD
#	define CWT_LightBlue              0xADD8E6
#	define CWT_LightCoral             0xF08080
#	define CWT_LightCyan              0xE0FFFF
#	define CWT_LightGoldenRodYellow   0xFAFAD2
#	define CWT_LightGray   0xD3D3D3
#	define CWT_LightGrey   0xD3D3D3
#	define CWT_LightGreen   0x90EE90
#	define CWT_LightPink   0xFFB6C1
#	define CWT_LightSalmon   0xFFA07A
#	define CWT_LightSeaGreen   0x20B2AA
#	define CWT_LightSkyBlue   0x87CEFA
#	define CWT_LightSlateGray   0x778899
#	define CWT_LightSlateGrey   0x778899
#	define CWT_LightSteelBlue   0xB0C4DE
#	define CWT_LightYellow   0xFFFFE0
#	define CWT_Lime   0x00FF00
#	define CWT_LimeGreen   0x32CD32
#	define CWT_Linen   0xFAF0E6
#	define CWT_Magenta   0xFF00FF
#	define CWT_Maroon   0x800000
#	define CWT_MediumAquaMarine   0x66CDAA
#	define CWT_MediumBlue   0x0000CD
#	define CWT_MediumOrchid   0xBA55D3
#	define CWT_MediumPurple   0x9370D8
#	define CWT_MediumSeaGreen   0x3CB371
#	define CWT_MediumSlateBlue   0x7B68EE
#	define CWT_MediumSpringGreen   0x00FA9A
#	define CWT_MediumTurquoise   0x48D1CC
#	define CWT_MediumVioletRed   0xC71585
#	define CWT_MidnightBlue   0x191970
#	define CWT_MintCream   0xF5FFFA
#	define CWT_MistyRose   0xFFE4E1
#	define CWT_Moccasin   0xFFE4B5
#	define CWT_NavajoWhite   0xFFDEAD
#	define CWT_Navy   0x000080
#	define CWT_OldLace   0xFDF5E6
#	define CWT_Olive   0x808000
#	define CWT_OliveDrab   0x6B8E23
#	define CWT_Orange   0xFFA500
#	define CWT_OrangeRed   0xFF4500
#	define CWT_Orchid   0xDA70D6
#	define CWT_PaleGoldenRod   0xEEE8AA
#	define CWT_PaleGreen   0x98FB98
#	define CWT_PaleTurquoise   0xAFEEEE
#	define CWT_PaleVioletRed   0xD87093
#	define CWT_PapayaWhip   0xFFEFD5
#	define CWT_PeachPuff   0xFFDAB9
#	define CWT_Peru   0xCD853F
#	define CWT_Pink   0xFFC0CB
#	define CWT_Plum   0xDDA0DD
#	define CWT_PowderBlue   0xB0E0E6
#	define CWT_Purple   0x800080
#	define CWT_Red   0xFF0000
#	define CWT_RosyBrown   0xBC8F8F
#	define CWT_RoyalBlue   0x4169E1
#	define CWT_SaddleBrown   0x8B4513
#	define CWT_Salmon   0xFA8072
#	define CWT_SandyBrown   0xF4A460
#	define CWT_SeaGreen   0x2E8B57
#	define CWT_SeaShell   0xFFF5EE
#	define CWT_Sienna   0xA0522D
#	define CWT_Silver   0xC0C0C0
#	define CWT_SkyBlue   0x87CEEB
#	define CWT_SlateBlue   0x6A5ACD
#	define CWT_SlateGray   0x708090
#	define CWT_SlateGrey   0x708090
#	define CWT_Snow   0xFFFAFA
#	define CWT_SpringGreen   0x00FF7F
#	define CWT_SteelBlue   0x4682B4
#	define CWT_Tan   0xD2B48C
#	define CWT_Teal   0x008080
#	define CWT_Thistle   0xD8BFD8
#	define CWT_Tomato   0xFF6347
#	define CWT_Turquoise   0x40E0D0
#	define CWT_Violet   0xEE82EE
#	define CWT_Wheat   0xF5DEB3
#	define CWT_White   0xFFFFFF
#	define CWT_WhiteSmoke   0xF5F5F5
#	define CWT_Yellow        0xFFFF00
#	define CWT_YellowGreen   0x9ACD32

#endif

#endif /* _CWT_COLOR_H_ */
