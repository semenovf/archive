/**
 * @file SchemeCanvas.hpp
 * @author wladt
 * @date Feb 19, 2014
 */

#ifndef __SCHEMECANVAS_HPP__
#define __SCHEMECANVAS_HPP__

#include <QWidget>
#include "SchemeDom.hpp"

class SchemeCanvas : public QWidget
{
	SchemeDom & _scheme;

public:
	SchemeCanvas (SchemeDom & scheme, QWidget * parent = nullptr);
	void attachScheme (SchemeDom & scheme);

protected:
	virtual void paintEvent (QPaintEvent *) override;
};


#endif /* __SCHEMECANVAS_HPP__ */
