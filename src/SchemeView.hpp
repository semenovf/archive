/**
 * @file StencilListWidget.hpp
 * @author wladt
 * @date Feb 25, 2014
 */

#ifndef __STENCILLISTWIDGET_HPP__
#define __STENCILLISTWIDGET_HPP__

#include <pfs.hpp>
#include <QList>
#include <QPoint>
#include <QPixmap>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
QT_END_NAMESPACE

class SchemeView : public QGraphicsView
{
	QGraphicsScene _scene;

    Q_OBJECT

public:
    SchemeView (QWidget * parent = 0);
    void clear ();

protected:
    virtual void dragEnterEvent  (QDragEnterEvent * event) override;
    virtual void dragLeaveEvent  (QDragLeaveEvent * event) override;
    virtual void dragMoveEvent   (QDragMoveEvent  * event) override;
    virtual void dropEvent       (QDropEvent      * event) override;
    virtual void mousePressEvent (QMouseEvent     * event) override;
};


#endif /* __STENCILLISTWIDGET_HPP__ */
