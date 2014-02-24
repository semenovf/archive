/*
 * TrafficLights.hpp
 *
 *  Created on: Feb 24, 2014
 *      Author: wladt
 */

#ifndef __TRAFFICLIGHTS_HPP__
#define __TRAFFICLIGHTS_HPP__

#include "SchemeItem.hpp"

class TrafficLights : public SchemeItem
{
public:
	TrafficLights () : SchemeItem() {}
	virtual QRectF boundingRect() const override;

protected:
	virtual void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr) override;
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *event) override;
};

#endif /* __TRAFFICLIGHTS_HPP__ */
