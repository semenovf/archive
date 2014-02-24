/**
 * @file SchemeCanvas.cpp
 * @author wladt
 * @date Feb 19, 2014
 */

#include "SchemeCanvas.hpp"
#include "SchemeSymbol.hpp"
#include "TrafficLights.hpp"
#include <QPen>
#include <QBrush>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <cwt/logger.hpp>
#include <iostream>

SchemeCanvas::SchemeCanvas (QWidget * parent)
	: QGraphicsView(parent)
{
//	SchemeItem p1(SchemeSymbol::getSymbolByName(_l1("trafficlight-mast")));
//	SchemeItem p2(SchemeSymbol::getSymbolByName(_l1("trafficlight-dwarf")));
//
//	if (p1.isEmpty()) {
//		cwt::log::error(_l1("p1 is empty"));
//	}
//
//	if (p2.isEmpty()) {
//		cwt::log::error(_l1("p2 is empty"));
//	}

	TrafficLights * trafficLights = new TrafficLights;
	_scene.addItem(trafficLights);
//	_scene.addPath(p1, QPen(QColor(79, 106, 25)));
//	_scene.addPath(p2, QPen(QColor(79, 106, 25)));

	this->setScene(& _scene);
}

SchemeCanvas::~SchemeCanvas ()
{}

void SchemeCanvas::dragEnterEvent (QGraphicsSceneDragDropEvent * event)
{
   if (event->mimeData()->hasColor()) {
		event->setAccepted(true);
		//dragOver = true;
		update();
	} else {
		event->setAccepted(false);
	}
}

void SchemeCanvas::dragLeaveEvent (QGraphicsSceneDragDropEvent * /*event*/)
{
//    dragOver = false;
    update();
}

void SchemeCanvas::dropEvent (QGraphicsSceneDragDropEvent * /*event*/)
{
    //dragOver = false;
//    if (event->mimeData()->hasColor())
//        color = qvariant_cast<QColor>(event->mimeData()->colorData());
    update();
}

