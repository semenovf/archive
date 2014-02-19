/**
 * @file SchemeCanvas.cpp
 * @author wladt
 * @date Feb 19, 2014
 */

#include "SchemeCanvas.hpp"
#include <QFont>
#include <QPainter>
#include <iostream>

struct TraverseData
{
	int nodeCount;
	int balance;
	int indent;
	QPainter * painter;

	TraverseData(QPainter * p) : nodeCount(0), balance(0), indent(0), painter(p) {}
};

static void __on_start (const cwt::dom::node & n, void * userData);
static void __on_end (const cwt::dom::node & n, void * userData);

SchemeCanvas::SchemeCanvas (SchemeDom & scheme, QWidget * parent)
	: QWidget(parent)
	, _scheme(scheme)
{}

void SchemeCanvas::attachScheme (SchemeDom & scheme)
{
	_scheme = scheme;
}


void SchemeCanvas::paintEvent (QPaintEvent *)
 {
     QPainter painter(this);
     painter.setPen(Qt::blue);
     painter.setFont(QFont("Arial", 30));
     painter.drawText(rect(), Qt::AlignCenter, "Qt");

     if (_scheme.isGood()) {
    	 TraverseData userData(& painter);
    	 _scheme.traverse(_scheme.document(), __on_start, __on_end, & userData);
     }
 }


static void __on_start (const cwt::dom::node & n, void * userData)
{
	TraverseData * d = static_cast<TraverseData *>(userData);
	if (n.nodeType() == cwt::dom::node::ElementNode) {
		//std::cout << std::string(d->indent * 4, ' ') << '<' <<  n.nodeName() << '>' << std::endl;
		++d->nodeCount;
		++d->balance;
		++d->indent;

		if (n.localName() == _l1("path")) {
			if (n.hasAttributes()) {
				cwt::dom::namednodemap atts = n.attributes();

				for (size_t i = 0; i < atts.length(); ++i) {
					cwt::dom::node attr = atts.item(i);
					std::cout <<  dynamic_cast<cwt::dom::attr &>(attr)->localName() << std::endl;
				}
			}
		}
	}
}

static void __on_end (const cwt::dom::node & n, void * userData)
{
	TraverseData * d = static_cast<TraverseData *>(userData);
	if (n.nodeType() == cwt::dom::node::ElementNode) {
		--d->balance;
		--d->indent;
		//std::cout << std::string(d->indent * 4, ' ') << "</" <<  n.nodeName() << '>' << std::endl;
	}
}
