/**
 * @file StencilListWidget.cpp
 * @author wladt
 * @date Feb 25, 2014
 */

#include "SchemeView.hpp"
#include "Stencil.hpp"
#include "qcast.hpp"
#include <QtGui>
#include <QPixmap>

SchemeScene::SchemeScene (QObject * parent) : QGraphicsScene(parent)
{}

void SchemeScene::dropEvent (QGraphicsSceneDragDropEvent * event)
{
    if (event->mimeData()->hasText()) {
        QString stencilName = event->mimeData()->text();
        Stencil stencil(qcast(stencilName));

        QGraphicsPixmapItem * pitem = this->addPixmap(stencil.toPixmap());
        QRectF boundRect =  pitem->boundingRect();

        pitem->setPos(event->scenePos().x() - boundRect.width()/2
        		, event->scenePos().y() - boundRect.height()/2);
        pitem->setFlag(QGraphicsItem::ItemIsMovable, true);

        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void SchemeScene::dragEnterEvent  (QGraphicsSceneDragDropEvent * event)
{
    if (event->mimeData()->hasText())
        event->accept();
    else
        event->ignore();
}

void SchemeScene::dragLeaveEvent (QGraphicsSceneDragDropEvent * event)
{
    event->accept();
}

void SchemeScene::dragMoveEvent (QGraphicsSceneDragDropEvent * event)
{
/*
	qDebug() << "move pos: x=" << event->pos().x() << "; y=" << event->pos().y();
	qDebug() << "scene pos: x=" << event->scenePos().x() << "; y=" << event->scenePos().y();
*/

    if (event->mimeData()->hasText()) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void SchemeScene::mousePressEvent (QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem * pitem = itemAt(event->scenePos());

    if (pitem) {
        qDebug() << "Mouse Press: scene pos" << event->scenePos() << "; item=" << pitem;
     }

    QGraphicsScene::mousePressEvent(event);
}

void SchemeScene::mouseReleaseEvent (QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem * pitem = itemAt(event->scenePos());

    if (pitem) {
    	qDebug() << "Mouse Release: scene pos" << event->scenePos() << "; item=" << pitem;
  		drawSelectionRect(pitem, event->scenePos());
     }

//    update();
    QGraphicsScene::mouseReleaseEvent(event);
}

void SchemeScene::drawSelectionRect (QGraphicsItem * item, QPointF /*scenePos*/)
{
	if (item) {
		views()[0]->centerOn(item);
		//QRectF brect(scenePos, item->boundingRect().size());
		QRectF brect(item->boundingRect());
		QPen pen;
		QBrush brush;

		pen.setColor(Qt::black);
		pen.setStyle(Qt::DashLine);
		pen.setWidth(1);

		qDebug() << "drawSelectionRect: " << brect;

		addRect(brect, pen, brush);
	}
}

SchemeView::SchemeView (QWidget * parent)
    : QGraphicsView(parent)
{
    setAcceptDrops(true);
	setScene(& _scene);
	_scene.setSceneRect(QRect(QPoint(0,0), QPoint(200, 200)));
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void SchemeView::resizeEvent (QResizeEvent * event)
{
//	_scene.setSceneRect(QRect(QPoint(0,0), event->size()));
//	_scene.setSceneRect(QRect(QPoint(0,0), QPoint(1000,1000)));
// _scene.setSceneRect(_scene.itemsBoundingRect());
	QGraphicsView::resizeEvent(event);
}

#ifdef __COMMENT__
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
#endif
