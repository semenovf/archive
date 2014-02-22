/**
 * @file SchemeCanvas.cpp
 * @author wladt
 * @date Feb 19, 2014
 */

#include "SchemeCanvas.hpp"
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPointF>
#include <cwt/svg/path.hpp>
#include <iostream>

SchemeCanvas::SchemeCanvas (SchemeDom & scheme, QWidget * parent)
	: QGraphicsView(parent)
	, _nativeWidth(0)
	, _nativeHeight(0)
{}

void SchemeCanvas::loadSymbols ()
{

}
/*
void SchemeCanvas::attachScheme (SchemeDom & scheme)
{
	cwt::dom::node svg = _scheme.document().elementsByTagName(_l1("svg")).item(0);
	if(svg.isNull()) {
		cwt::log::error(_u8(_Tr("Invalid scheme: root <svg> element not found")));
		return;
	}

	_nativeWidth = svg.attributes().getNamedItem(_l1("width")).nodeValue().toInt();
	_nativeHeight = svg.attributes().getNamedItem(_l1("height")).nodeValue().toInt();
}
*/
