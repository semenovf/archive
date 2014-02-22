/*
 * SchemeItem.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */

#include "SchemeItem.hpp"
#include "SchemeSymbol.hpp"
#include <QPainter>

SchemeItem::SchemeItem (const SchemeSymbol & symbol)
	: QPainterPath(symbol.toPainterPath())
{}

//void SchemeItem::paint (QPainter * painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
//{
//	painter->drawPath(_ppath);
//////     painter.setPen(Qt::blue);
////    painter->setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine,
////                         Qt::FlatCap, Qt::MiterJoin));
////    painter->setFont(QFont("Arial", 30));
//////    painter->drawText(rect(), Qt::AlignCenter, "Qt");
////
//}

