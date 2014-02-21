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
#include <cwt/svg/path.hpp>
#include <iostream>

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
//     painter.setPen(Qt::blue);
     painter.setPen(QPen(QColor(79, 106, 25), 1, Qt::SolidLine,
                          Qt::FlatCap, Qt::MiterJoin));
     painter.setFont(QFont("Arial", 30));
     painter.drawText(rect(), Qt::AlignCenter, "Qt");

     if (_scheme.isGood()) {
    	 SchemePainterData userData(*this);
    	 _scheme.traverse(_scheme.document(), __on_start, __on_end, & userData);
     }
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

QPainterPath SchemeCanvas::toPainterPath (const pfs::string & path)
{
	QPainterPath r;
	PathParseHandlers pph(r);
	return cwt::svg::path_seg::parse(path, pph) ? r : QPainterPath();
}

void SchemeCanvas::drawPath (const pfs::string & path)
{
	QPainter painter(this);
	QPainterPath ppath(toPainterPath(path));
	painter.drawPath(ppath);
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
