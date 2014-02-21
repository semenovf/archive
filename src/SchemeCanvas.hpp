/**
 * @file SchemeCanvas.hpp
 * @author wladt
 * @date Feb 19, 2014
 */

#ifndef __SCHEMECANVAS_HPP__
#define __SCHEMECANVAS_HPP__

#include <QWidget>
#include <pfs/string.hpp>

class SchemeDom;
class QPaintEvent;
class QPainter;
class QPainterPath;

class SchemeCanvas : public QWidget
{
	SchemeDom & _scheme;
	int _nativeWidth;
	int _nativeHeight;

public:
	SchemeCanvas (SchemeDom & scheme, QWidget * parent = 0);
	void attachScheme (SchemeDom & scheme);

	void drawPath (const pfs::string & path);

protected:
	virtual void paintEvent (QPaintEvent *) override;
	static QPainterPath toPainterPath (const pfs::string & path);
};


#endif /* __SCHEMECANVAS_HPP__ */
