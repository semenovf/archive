/*
 * SchemeItem.hpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */

#ifndef __SCHEMEITEM_HPP__
#define __SCHEMEITEM_HPP__

#include <pfs.hpp>
#include <QGraphicsItemGroup>
#include <QPoint>

class SchemeSymbol;
class QWidget;
class QPainter;
class QGraphicsSceneMouseEvent;
class QStyleOptionGraphicsItem;

class SchemeItem : public QGraphicsItemGroup
{
	QPoint _dragStartPosition;

public:
	SchemeItem (SchemeItem * parent = nullptr) : QGraphicsItemGroup(parent) {}
	SchemeItem (const SchemeSymbol & symbol, SchemeItem * parent);

protected:
	virtual void paint (QPainter * /*painter*/, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/ = nullptr) override {};
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * event) override { QGraphicsItemGroup::mousePressEvent(event); }
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent * event) override { QGraphicsItemGroup::mouseMoveEvent(event); }
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event) override { QGraphicsItemGroup::mouseReleaseEvent(event); }
};




#endif /* __SCHEMEITEM_HPP__ */
