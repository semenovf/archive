/**
 * @file SchemeCanvas.hpp
 * @author wladt
 * @date Feb 19, 2014
 */

#ifndef __SCHEMECANVAS_HPP__
#define __SCHEMECANVAS_HPP__

#include <QGraphicsScene>
#include <QGraphicsView>
#include <pfs/string.hpp>
#include <pfs/map.hpp>

class SchemeCanvas : public QGraphicsView
{
	QGraphicsScene _scene;

public:
	SchemeCanvas (QWidget * parent = 0);
	virtual ~SchemeCanvas();
};


#endif /* __SCHEMECANVAS_HPP__ */
