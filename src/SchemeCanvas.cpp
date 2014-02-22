/**
 * @file SchemeCanvas.cpp
 * @author wladt
 * @date Feb 19, 2014
 */

#include "SchemeCanvas.hpp"
#include "SchemeSymbol.hpp"
#include "SchemeItem.hpp"
#include <QPen>
#include <QBrush>
#include <cwt/logger.hpp>
#include <iostream>

SchemeCanvas::SchemeCanvas (QWidget * parent)
	: QGraphicsView(parent)
{
	SchemeItem p1(SchemeSymbol::getSymbolByName(_l1("circle-cross45")));
	SchemeItem p2(SchemeSymbol::getSymbolByName(_l1("circle-cross90")));

	if (p1.isEmpty()) {
		cwt::log::error(_l1("p1 is empty"));
	}

	if (p2.isEmpty()) {
		cwt::log::error(_l1("p1 is empty"));
	}

	_scene.addPath(p1, QPen(QColor(79, 106, 25)));
	_scene.addPath(p2, QPen(QColor(79, 106, 25)));

	this->setScene(& _scene);
}

SchemeCanvas::~SchemeCanvas ()
{}
