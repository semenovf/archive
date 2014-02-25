/*
 * TrafficLights.cpp
 *
 *  Created on: Feb 24, 2014
 *      Author: wladt
 */

#include "TrafficLights.hpp"
#include <QApplication>
#include <QPen>
#include <QColor>
#include <QBrush>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QCursor>
#include <QDrag>
#include <QBitmap>

QRectF TrafficLights::boundingRect() const
{
    return QRectF(-15.5, -15.5, 34, 34);
}

void TrafficLights::paint (QPainter * painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
//	QPen pen(QColor(79, 106, 25));
//	pen.setWidth(20);
//	painter->setPen(pen);
//	painter->setBrush(QBrush(QColor("red")));
//	painter->drawRect(0, 0, 100, 100);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-12, -12, 30, 30);
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(Qt::red));
    painter->drawEllipse(-15, -15, 30, 30);
}

QPixmap TrafficLights::toPixmap () const
{
//	QPen pen(QColor(79, 106, 25));
//	pen.setWidth(20);
//	painter->setPen(pen);
//	painter->setBrush(QBrush(QColor("red")));
//	painter->drawRect(0, 0, 100, 100);

	QPixmap r(50, 50);
	r.fill(Qt::transparent);

	QPainter painter(& r);
	painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::darkGray);
    painter.drawEllipse(0, 0, 50, 50);
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(5, 5, 40, 40);

    return r;
}

void TrafficLights::mousePressEvent (QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::ClosedHandCursor);
}
//! [3]

//! [5]
void TrafficLights::mouseMoveEvent (QGraphicsSceneMouseEvent * event)
{
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
        .length() < QApplication::startDragDistance()) {
        return;
    }

    QDrag * drag = new QDrag(event->widget());
    QMimeData * mime = new QMimeData;
    drag->setMimeData(mime);
//! [5]

//! [6]
//    mime->setColorData(Qt::yellow);
//    mime->setText(QString("#%1%2%3")
//    		.arg(QColor(Qt::yellow).red(), 2, 16, QLatin1Char('0'))
//    		.arg(QColor(Qt::yellow).green(), 2, 16, QLatin1Char('0'))
//    		.arg(QColor(Qt::yellow).blue(), 2, 16, QLatin1Char('0')));

    QPixmap pixmap(boundingRect().width(), boundingRect().height());
    pixmap.fill(Qt::white);

    QPainter painter(& pixmap);
    painter.translate(15, 15);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(& painter, 0, 0);
    painter.end();

    pixmap.setMask(pixmap.createHeuristicMask());

    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(15, 20));
//! [7]

//! [8]
    drag->exec();
    setCursor(Qt::OpenHandCursor);
}
//! [8]

//! [4]
void TrafficLights::mouseReleaseEvent (QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::OpenHandCursor);
}
