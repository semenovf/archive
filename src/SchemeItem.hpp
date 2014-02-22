/*
 * SchemeItem.hpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */

#ifndef __SCHEMEITEM_HPP__
#define __SCHEMEITEM_HPP__

#include <pfs.hpp>
#include <QGraphicsItem>
#include <QRectF>

class SchemeSymbol;
class QPainter;

class SchemeItem : public QPainterPath
{
public:
	SchemeItem (const QPainterPath & ppath) : QPainterPath(ppath) {}
	SchemeItem (const SchemeSymbol & symbol);

//	virtual void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
};




#endif /* __SCHEMEITEM_HPP__ */
