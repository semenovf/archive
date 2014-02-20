/**
 * @file SchemeCanvas.hpp
 * @author wladt
 * @date Feb 19, 2014
 */

#ifndef __SCHEMECANVAS_HPP__
#define __SCHEMECANVAS_HPP__

#include <QWidget>

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
	SchemeCanvas (SchemeDom & scheme, QWidget * parent = nullptr);
	void attachScheme (SchemeDom & scheme);

protected:
	virtual void paintEvent (QPaintEvent *) override;
	QPainterPath toPainterPath (const pfs::string & path) const;
	void drawPath (QPainter * painter, const pfs::string & path);
};


#endif /* __SCHEMECANVAS_HPP__ */
