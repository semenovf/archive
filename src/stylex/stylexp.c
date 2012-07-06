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
	  "bg_color"
	, "bg_left_width"
	, "bg_left_color"
	, "bg_left_style"

	, "bg_top_width"
	, "bg_top_color"
	, "bg_top_style"

	, "bg_right_width"
	, "bg_right_color"
	, "bg_right_style"

	, "bg_bottom_width"
	, "bg_bottom_color"
	, "bg_bottom_style"

	, "left"
	, "top"
	, "width"
	, "height"

	, "color"

	, "font_family"
	, "font_size"
	, "font_style"
	, "font_weight"

	, "margin_left"
	, "margin_top"
	, "margin_right"
	, "margin_bottom"

	, "padding_left"
	, "padding_top"
	, "padding_right"
	, "padding_bottom"

	, "max_width"
	, "max_height"
	, "min_width"
	, "min_height"

	, "opacity"
	, "position"
	, "text_indent"
	, "text_align"
	, "valign"
	, "halign"
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

