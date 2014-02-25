/**
 * @file StencilListWidget.cpp
 * @author wladt
 * @date Feb 25, 2014
 */

#include "SchemeView.hpp"
#include "Stencil.hpp"
#include "TrafficLights.hpp"
#include <QtGui>
#include <QPixmap>

SchemeView::SchemeView (QWidget * parent)
    : QGraphicsView(parent)
{
    setAcceptDrops(true);

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

//	TrafficLights * trafficLights = new TrafficLights;
//	_scene.addItem(trafficLights);

//	_scene.addPath(p1, QPen(QColor(79, 106, 25)));
//	_scene.addPath(p2, QPen(QColor(79, 106, 25)));

	this->setScene(& _scene);
}

void SchemeView::clear ()
{
    update();
}

void SchemeView::dragEnterEvent (QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(Stencil::MimeType))
        event->accept();
    else
        event->ignore();
}

void SchemeView::dragLeaveEvent (QDragLeaveEvent * event)
{
//    QRect updateRect = highlightedRect;
//    highlightedRect = QRect();
//    update(updateRect);
	update();
    event->accept();
}

void SchemeView::dragMoveEvent (QDragMoveEvent * event)
{
	qDebug() << "move pos: x=" << event->pos().x() << "; y=" << event->pos().y();
	QPointF mappedPoint = mapToScene(event->pos());
	qDebug() << "scene pos: x=" << mappedPoint.x() << "; y=" << mappedPoint.y();

    if (event->mimeData()->hasFormat(Stencil::MimeType)) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }

//    update(updateRect);
}

void SchemeView::dropEvent (QDropEvent * event)
{
    if (event->mimeData()->hasFormat(Stencil::MimeType)) {
        QByteArray stencilData = event->mimeData()->data(Stencil::MimeType);
        QDataStream stream(& stencilData, QIODevice::ReadOnly);

        QPixmap pixmap;
        stream >> pixmap;

//        QRectF sceneRect = _scene.sceneRect();
        QGraphicsPixmapItem * pitem = _scene.addPixmap(pixmap);
        QRectF boundRect =  pitem->boundingRect();

        QPointF mappedPos = mapToScene(event->pos());

        qDebug() << "drop pos: x=" << event->pos().x() << "; y=" << event->pos().y();
        qDebug() << "mapped pos: x=" << mappedPos.x() << "; y=" << mappedPos.y();
        qDebug() << "[0] bounding rect: x=" << boundRect.x()
        		<< "; y=" << boundRect.y()
        		<< "; width=" << boundRect.width()
        		<< "; height=" << boundRect.height();

        pitem->setPos(mappedPos.x() - boundRect.width()/2
        		, mappedPos.y() - boundRect.height()/2);

//        _scene.setSceneRect(sceneRect);

        qDebug() << "scene pos: x=" << pitem->scenePos().x() << "; y=" << pitem->scenePos().y();

        qDebug() << "[1] bounding rect: x=" << boundRect.x()
        		<< "; y=" << boundRect.y()
        		<< "; width=" << boundRect.width()
        		<< "; height=" << boundRect.height();

//        _scene.update(pitem->boundingRect());

        event->setDropAction(Qt::CopyAction);
        event->accept();

    } else {
        event->ignore();
    }
}
void SchemeView::mousePressEvent (QMouseEvent * event)
{
	QPointF mappedPoint = mapToScene(event->pos());
	qDebug() << "press pos: x=" << event->pos().x() << "; y=" << event->pos().y();
    qDebug() << "mapped press pos: x=" << mappedPoint.x() << "; y=" << mappedPoint.y();

#ifdef __COMMENT__
    QRect square = targetSquare(event->pos());
    int found = -1; //findStencil(square);

    if (found == -1)
        return;

//    QPoint location = _stencilLocations[found];
//    QPixmap pixmap = _stencilPixmaps[found];
//    _stencilLocations.removeAt(found);
//    _stencilPixmaps.removeAt(found);
//    _stencilRects.removeAt(found);

    update(square);

    QByteArray itemData;
    QDataStream dataStream(& itemData, QIODevice::WriteOnly);

    dataStream << pixmap;// << location;

    QMimeData * mimeData = new QMimeData;
    mimeData->setData(Stencil::MimeType, itemData);

    QDrag * drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - square.topLeft());
    drag->setPixmap(pixmap);

    if (drag->start(Qt::MoveAction) == 0) {
//    	_stencilLocations.insert(found, location);
//    	_stencilPixmaps.insert(found, pixmap);
//    	_stencilRects.insert(found, square);
        update(targetSquare(event->pos()));
    }
#endif
}
