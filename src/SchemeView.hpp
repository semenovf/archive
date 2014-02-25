/**
 * @file StencilListWidget.hpp
 * @author wladt
 * @date Feb 25, 2014
 */

#ifndef __STENCILLISTWIDGET_HPP__
#define __STENCILLISTWIDGET_HPP__

#include <pfs.hpp>
#include <QGraphicsView>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class QResizeEvent;
class QGraphicsSceneDragDropEvent;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class SchemeScene : public QGraphicsScene
{
public:
	SchemeScene (QObject * parent = 0);

protected:
    virtual void dragEnterEvent  (QGraphicsSceneDragDropEvent * event) override;
    virtual void dragLeaveEvent  (QGraphicsSceneDragDropEvent * event) override;
    virtual void dragMoveEvent   (QGraphicsSceneDragDropEvent * event) override;
    virtual void dropEvent       (QGraphicsSceneDragDropEvent * event) override;
    virtual void mousePressEvent (QGraphicsSceneMouseEvent  * event) override;
};

class SchemeView : public QGraphicsView
{
	SchemeScene _scene;

public:
    SchemeView (QWidget * parent = 0);

protected:
    virtual void resizeEvent (QResizeEvent * event) override;
};


#endif /* __STENCILLISTWIDGET_HPP__ */
