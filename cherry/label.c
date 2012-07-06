/*
 * label.c
 *
 *  Created on: 08.12.2011
 *      Author: wladt
 */

#include <cwt/cherry/cherry.h>

void _cwt_init_label(CWT_WIDGET_PTR widget)
{
	CWT_LABEL_PTR label = CWT_CAST(widget, CWT_LABEL_PTR);
	label->textblock = JQ_MALLOC(CWT_TEXTBLK);
	_cwt_init_textblk(label->textblock);
}

void _cwt_destroy_label(CWT_WIDGET_PTR widget)
{
	CWT_LABEL_PTR label = CWT_CAST(widget, CWT_LABEL_PTR);

	JQ_ASSERT(widget);
	JQ_ASSERT(label->textblock);

	_cwt_destroy_textblk(label->textblock);
	JQ_FREE(label->textblock);
}

CWT_WIDGET_PTR _cwt_clone_label(CWT_WIDGET_PTR orig, CWT_WIDGET_PTR clone)
{
	CWT_LABEL_PTR orig_label = CWT_CAST(orig, CWT_LABEL_PTR);
	CWT_LABEL_PTR clone_label = CWT_CAST(clone, CWT_LABEL_PTR);
	JQ_ASSERT(orig_label);
	JQ_ASSERT(clone_label);
	_cwt_clone_textblk(orig_label->textblock, clone_label->textblock);
	return clone;
}


void _cwt_render_label(CWT_WIDGET_PTR widget)
{
	CWT_LABEL_PTR label = CWT_CAST(widget, CWT_LABEL_PTR);
	CWT_VIEWPORT vp;
	CWT_SIZE sz;

	JQ_ASSERT(widget);

	_cwt_get_viewport(&vp);

	label->textblock->style = _CWT_WIDGET_STYLE(widget);
	sz.width = vp.width;
	sz.height = vp.height;
	_cwt_render_textblk(label->textblock, &sz);
}


/**
 * For supporting old code only
 *
 * @param label
 */
void _cwt_raw_render_label(CwtWidgetPtr widget)
{
	CwtLabelPtr label = CWT_CAST(widget, CwtLabelPtr);
	CwtSize     sz;
	CWT_SETTINGS settings;

	JQ_ASSERT(widget);

	_cwt_save_settings(&settings);

	_cwt_set_viewport_wh(widget->abs.x, widget->abs.y, widget->geometry.width, widget->geometry.height, TRUE);

	label->textblock->style = _CWT_WIDGET_STYLE(widget);
	sz.width  = widget->geometry.width;
	sz.height = widget->geometry.height;
	_cwt_render_textblk(label->textblock, &sz);

	_cwt_restore_settings(&settings);
}

void cwt_label_set_text(CWT_WIDGET_PTR widget, const CHAR *text)
{
	CWT_LABEL_PTR label = CWT_CAST(widget, CWT_LABEL_PTR);
	_cwt_textblk_set_text(label->textblock, text);
	cwt_widget_update(widget);
}

const StringListPtr cwt_label_text(CWT_WIDGET_PTR widget)
{
	CWT_LABEL_PTR label = CWT_CAST(widget, CWT_LABEL_PTR);
	JQ_ASSERT(widget);
	return label->textblock->lines;
}


void _cwt_size_min_label(CWT_WIDGET_PTR widget, CWT_SIZE *sz)
{
	CWT_LABEL_PTR label = CWT_CAST(widget, CWT_LABEL_PTR);
	JQ_ASSERT(widget);
	label->textblock->style = _CWT_WIDGET_STYLE(widget);
	_cwt_size_min_textblk(label->textblock, sz);
}
