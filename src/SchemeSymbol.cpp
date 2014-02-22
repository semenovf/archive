/*
 * SchemeItem.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */

#include "SchemeItem.hpp"
#include <cwt/io/file.hpp>
#include <cwt/io/textreader.hpp>

struct SchemePainterData
{
	int nodeCount;
	int balance;
	int indent;
	SchemeCanvas & canvas;

	SchemePainterData (SchemeCanvas & c) : nodeCount(0), balance(0), indent(0), canvas(c) {}
};

static void __on_start (const cwt::dom::node & n, void * userData);
static void __on_end (const cwt::dom::node & n, void * userData);

void SchemeSymbol::paint ( QPainter * painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
//     painter.setPen(Qt::blue);
    painter->setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine,
                         Qt::FlatCap, Qt::MiterJoin));
    painter->setFont(QFont("Arial", 30));
//    painter->drawText(rect(), Qt::AlignCenter, "Qt");

    if (!_svgDoc.isNull()) {
		 SchemePainterData userData(*this);
		_svgDoc.traverse(__on_start, __on_end, & userData);
    }
}

bool SchemeSymbol::loadFromString (const pfs::string & str)
{
	cwt::xml::dom dom;
	_svgDoc = dom.createDocument(str);

	if (!dom.isError())
		dom.logErrors();

	return !_svgDoc.isNull();
}

bool SchemeSymbol::loadFromFile (const pfs::string & path)
{
	cwt::io::file file;
	file.open(path, cwt::io::device::ReadOnly);

	if (!file.opened()) {
		file.logErrors();
		return false;
	}

	cwt::io::text_reader textReader(file);

	pfs::string str = textReader.read(file.size());
	return loadFromString(str);
}

struct PathParseHandlers : public cwt::svg::path_parse_handlers
{
	QPainterPath & _ppath;

	PathParseHandlers (QPainterPath & ppath) : _ppath(ppath) {}

	virtual void begin () override {}
	virtual void end (bool /*success*/) override {}
	virtual void onClosePath (const cwt::svg::path_seg_closepath & /*pseg*/) override {}
	virtual void onMoveToAbs (const cwt::svg::path_seg_moveto_abs & p) override
		{ _ppath.moveTo(p.x(), p.y()); }
	virtual void onMoveToRel (const cwt::svg::path_seg_moveto_rel & p) override
		{ _ppath.moveTo(_ppath.currentPosition().x() + p.x(), _ppath.currentPosition().y() + p.y()); }
	virtual void onLineToAbs (const cwt::svg::path_seg_lineto_abs & p) override
		{ _ppath.lineTo(p.x(), p.y()); }
	virtual void onLineToRel (const cwt::svg::path_seg_lineto_rel & p) override
		{ _ppath.lineTo(_ppath.currentPosition().x() + p.x(), _ppath.currentPosition().y() + p.y());}
	virtual void onCurveToCubicAbs (const cwt::svg::path_seg_curveto_cubic_abs & /*pseg*/) override {}
	virtual void onCurveToCubicRel (const cwt::svg::path_seg_curveto_cubic_rel & /*pseg*/) override {}
	virtual void onCurveToQuadraticAbs (const cwt::svg::path_seg_curveto_quadratic_abs & /*pseg*/) override {}
	virtual void onCurveToQuadraticRel (const cwt::svg::path_seg_curveto_quadratic_rel & /*pseg*/) override {}
	virtual void onArcAbs (const cwt::svg::path_seg_arc_abs & /*pseg*/) override {}
	virtual void onArcRel (const cwt::svg::path_seg_arc_rel & /*pseg*/) override {}
	virtual void onLineToHorizontalAbs (const cwt::svg::path_seg_lineto_horizontal_abs & /*pseg*/) override {}
	virtual void onLineToHorizontalRel (const cwt::svg::path_seg_lineto_horizontal_rel & /*pseg*/) override {}
	virtual void onLineToVerticalAbs (const cwt::svg::path_seg_lineto_vertical_abs & /*pseg*/) override {}
	virtual void onLineToVerticalRel (const cwt::svg::path_seg_lineto_vertical_rel & /*pseg*/) override {}
	virtual void onCurveToCubicSmoothAbs (const cwt::svg::path_seg_curveto_cubic_smooth_abs & /*pseg*/) override {}
	virtual void onCurveToCubicSmoothRel (const cwt::svg::path_seg_curveto_cubic_smooth_rel & /*pseg*/) override {}
	virtual void onCurveToQuadraticSmoothAbs (const cwt::svg::path_seg_curveto_quadratic_smooth_abs & /*pseg*/) override {}
	virtual void onCurveToQuadraticSmoothRel (const cwt::svg::path_seg_curveto_quadratic_smooth_rel & /*pseg*/) override {}
};

void SchemeSymbol::drawPath (const pfs::string & path)
{
	QPainter painter(this);
	QPainterPath ppath(toPainterPath(path));
	painter.drawPath(ppath);
}


QPainterPath SchemeSymbol::toPainterPath (const pfs::string & path)
{
	QPainterPath r;
	PathParseHandlers pph(r);
	return cwt::svg::path_seg::parse(path, pph) ? r : QPainterPath();
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
					if (attr.localName() == _l1("d")) {
						std::cout << attr.localName() << "=" << attr.value() << std::endl;
						d->canvas.drawPath(attr.value());
					}

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
