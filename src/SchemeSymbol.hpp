/*
 * SchemeItem.hpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */

#ifndef __SCHEMEITEM_HPP__
#define __SCHEMEITEM_HPP__

#include <QGraphicsItem>
#include <cwt/xml/dom.hpp>

class SchemeDom;

class SchemeSymbol : public QGraphicsItem
{
	cwt::dom::document _svgDoc;

public:
	SchemeSymbol (SchemeSymbol * parent = 0) : QGraphicsItem(parent) {}
	void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 ) override;

	bool loadFromString (const pfs::string & str);
	bool loadFromFile (const pfs::string & path);

protected:
	void drawPath (const pfs::string & path);
	static QPainterPath toPainterPath (const pfs::string & path);
};


#endif /* __SCHEMEITEM_HPP__ */
