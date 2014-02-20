/**
 * @file SchemeCanvas.cpp
 * @author wladt
 * @date Feb 19, 2014
 */

#include "SchemeCanvas.hpp"
#include "SchemeDom.hpp"
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPointF>
#include <iostream>

struct SchemePainterData
{
	int nodeCount;
	int balance;
	int indent;
	QPainter * painter;

	SchemePainterData (QPainter * p) : nodeCount(0), balance(0), indent(0), painter(p) {}
};

static void __on_start (const cwt::dom::node & n, void * userData);
static void __on_end (const cwt::dom::node & n, void * userData);

SchemeCanvas::SchemeCanvas (SchemeDom & scheme, QWidget * parent)
	: QWidget(parent)
	, _scheme(scheme)
	, _nativeWidth(0)
	, _nativeHeight(0)
{}

void SchemeCanvas::attachScheme (SchemeDom & scheme)
{
	_scheme = scheme;
	cwt::dom::node svg = _scheme.document().elementsByTagName(_l1("svg")).item(0);
	if(svg.isNull()) {
		cwt::log::error(_u8(_Tr("Invalid scheme: root <svg> element not found")));
		return;
	}

	_nativeWidth = svg.attributes().getNamedItem(_l1("width")).nodeValue().toInt();
	_nativeHeight = svg.attributes().getNamedItem(_l1("height")).nodeValue().toInt();
}


void SchemeCanvas::paintEvent (QPaintEvent *)
{
     QPainter painter(this);
     painter.setPen(Qt::blue);
     painter.setFont(QFont("Arial", 30));
     painter.drawText(rect(), Qt::AlignCenter, "Qt");

     if (_scheme.isGood()) {
    	 SchemePainterData userData(& painter);
    	 _scheme.traverse(_scheme.document(), __on_start, __on_end, & userData);
     }
}

void SchemeCanvas::drawPath (QPainter * painter, const pfs::string & path)
{
	QPainterPath ppath;
	pfs::vector<pfs::string> tokens = path.split(pfs::ucchar(' '), false);
	pfs::vector<pfs::string>::const_iterator it = tokens.cbegin();
	pfs::vector<pfs::string>::const_iterator itEnd = tokens.cend();

	for (; it != itEnd; ++it) {
		if (it->toLower() == _l1("m")) {
			QPointF point;
			++it;
			if (it != itEnd)
				point.setX(it->toDouble());
			++it;
			if (it != itEnd)
				point.setY(it->toDouble());
		} else if ()
	}
}


static void __on_start (const cwt::dom::node & n, void * userData)
{
	SchemePainterData * d = static_cast<SchemePainterData *>(userData);
	if (n.nodeType() == cwt::dom::node::ElementNode) {
		//std::cout << std::string(d->indent * 4, ' ') << '<' <<  n.nodeName() << '>' << std::endl;
		++d->nodeCount;
		++d->balance;
		++d->indent;

		if (n.localName() == _l1("path")) {
			if (n.hasAttributes()) {
				cwt::dom::namednodemap atts = n.attributes();
				for (size_t i = 0; i < atts.length(); ++i) {
					cwt::dom::attr attr = atts.item(i).toAttr();
					std::cout << attr.localName() << "=" << attr.value() << std::endl;
				}
			}
		}
	}
}

static void __on_end (const cwt::dom::node & n, void * userData)
{
	SchemePainterData * d = static_cast<SchemePainterData *>(userData);
	if (n.nodeType() == cwt::dom::node::ElementNode) {
		--d->balance;
		--d->indent;
		//std::cout << std::string(d->indent * 4, ' ') << "</" <<  n.nodeName() << '>' << std::endl;
	}
}
