/**
 * @file SchemeCanvas.hpp
 * @author wladt
 * @date Feb 19, 2014
 */

#ifndef __SCHEMECANVAS_HPP__
#define __SCHEMECANVAS_HPP__

#include <QGraphicsView>
#include <QGraphicsScene>
#include <pfs/string.hpp>
#include <pfs/vector.hpp>

//class QPaintEvent;
class QPainter;
//class QPainterPath;
class SchemeSymbol;

class SchemeCanvas : public QGraphicsView
{
	typedef pfs::vector<pfs::string, SchemeSymbol *> symbol_vector_type;
	QGraphicsScene _scene;
	symbol_vector_type _symbols;

	int _nativeWidth;
	int _nativeHeight;

public:
	SchemeCanvas (SchemeDom & scheme, QWidget * parent = 0);
	void loadSymbols ();
};


#endif /* __SCHEMECANVAS_HPP__ */
