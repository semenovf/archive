/**
 * @file StencilListWidget.cpp
 * @author wladt
 * @date Feb 25, 2014
 */

#include "SchemeView.hpp"
#include "Stencil.hpp"
#include <QtGui>
#include <QPixmap>

SchemeScene::SchemeScene (QObject * parent) : QGraphicsScene(parent)
{}

void SchemeScene::dropEvent (QGraphicsSceneDragDropEvent * event)
{
    if (event->mimeData()->hasFormat(Stencil::MimeType)) {
        QByteArray stencilData = event->mimeData()->data(Stencil::MimeType);
        QDataStream stream(& stencilData, QIODevice::ReadOnly);

        QPixmap pixmap;
        stream >> pixmap;

        QGraphicsPixmapItem * pitem = this->addPixmap(pixmap);
        QRectF boundRect =  pitem->boundingRect();

/*
        qDebug() << "viewport pos: x=" << event->pos().x() << "; y=" << event->pos().y();
        qDebug() << "scene pos: x=" << event->scenePos().x() << "; y=" << event->scenePos().y();
        qDebug() << "[0] bounding rect: x=" << boundRect.x()
        		<< "; y=" << boundRect.y()
        		<< "; width=" << boundRect.width()
        		<< "; height=" << boundRect.height();
*/

        pitem->setPos(event->scenePos().x() - boundRect.width()/2
        		, event->scenePos().y() - boundRect.height()/2);
        pitem->setFlag(QGraphicsItem::ItemIsMovable, true);

/*
        qDebug() << "scene pos: x=" << pitem->scenePos().x() << "; y=" << pitem->scenePos().y();
        qDebug() << "[1] bounding rect: x=" << boundRect.x()
        		<< "; y=" << boundRect.y()
        		<< "; width=" << boundRect.width()
        		<< "; height=" << boundRect.height();
*/

        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void SchemeScene::dragEnterEvent  (QGraphicsSceneDragDropEvent * event)
{
    if (event->mimeData()->hasFormat(Stencil::MimeType))
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

    if (event->mimeData()->hasFormat(Stencil::MimeType)) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void SchemeScene::mousePressEvent (QGraphicsSceneMouseEvent * event)
{
	//qDebug() << "press: pos: x=" << event->pos().x() << "; y=" << event->pos().y(); // NOTE always zero
    qDebug() << "press: scene pos: x=" << event->scenePos().x() << "; y=" << event->scenePos().y();

    QGraphicsItem * pitem = itemAt(event->scenePos());

    if (pitem) {
         qDebug() << "You clicked on item" << pitem;
     } else {
         qDebug() << "You didn't click on an item.";
     }

    QGraphicsScene::mousePressEvent(event);
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
	_scene.setSceneRect(QRect(QPoint(0,0), QPoint(1000,1000)));
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
