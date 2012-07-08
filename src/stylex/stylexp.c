/*
 * stylexp.c
 *
 *  Created on: 15.06.2012
 *      Author: wladt
 *
 *  Parse stylex definitions string
 */

#include <cwt/stylex/stylex.h>
#include <ctype.h>

static const CWT_CHAR* __attr_names[] = {
	  _T("bg_color")
	, _T("bg_left_width")
	, _T("bg_left_color")
	, _T("bg_left_style")

	, _T("bg_top_width")
	, _T("bg_top_color")
	, _T("bg_top_style")

	, _T("bg_right_width")
	, _T("bg_right_color")
	, _T("bg_right_style")

	, _T("bg_bottom_width")
	, _T("bg_bottom_color")
	, _T("bg_bottom_style")

	, _T("left")
	, _T("top")
	, _T("width")
	, _T("height")

	, _T("color")

	, _T("font_family")
	, _T("font_size")
	, _T("font_style")
	, _T("font_weight")

	, _T("margin_left")
	, _T("margin_top")
	, _T("margin_right")
	, _T("margin_bottom")

	, _T("padding_left")
	, _T("padding_top")
	, _T("padding_right")
	, _T("padding_bottom")

	, _T("max_width")
	, _T("max_height")
	, _T("min_width")
	, _T("min_height")

	, _T("opacity")
	, _T("position")
	, _T("text_indent")
	, _T("text_align")
	, _T("valign")
	, _T("halign")
};

/**
 * Загружает стили из строки
 *
 * Формат строки стилей:
 *
 * style-defs := style-def+
 * style-def  := selector '{' attr-defs '}'
 * selector   := '#' id
 * 	| '@' media
 * 	| '.' class
 * 	| wtype
 * attr-defs : = attr-def
 * 	| attr-def ';' attr-defs
 * attr-def  := attr-name ':' attr-value
 *
 *
 * @param stylexStr
 */
enum _parse_states {
	  _STATE_BEGIN
	, _STATE_END
	, _STATE_SELECTOR
	, _STATE_BEGIN_ATTR_DEFS
	, _STATE_END_ATTR_DEFS
	, _STATE_ATTR_NAME
	, _STATE_ATTR_VALUE
};

void cwt_stylex_loadFromString(const CWT_CHAR *stylexStr)
{
	const CWT_CHAR *ptr;
	int state = _STATE_BEGIN;


	ptr = stylexStr;

	while( state != _STATE_END ) {

		while( isspace(*ptr) )
			ptr++;

		if( *ptr == '\x0' ) {
			state = _STATE_END;
			continue;
		}

		switch( state ) {
		case _STATE_BEGIN:
			break;

		default:
			break;
		}

		ptr++;
	}

}

/**
 * Загружает стили из файла
 */
void cwt_stylex_loadFromFile(const CWT_CHAR *stylexPath)
{
	CWT_ASSERT(stylexPath);
}

