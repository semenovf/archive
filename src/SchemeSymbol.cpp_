/*
 * SchemeItem.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */

#include "SchemeSymbol.hpp"
#include "Application.hpp"
#include "SchemeView.hpp"
#include <pfs/stack.hpp>
#include <cwt/fs.hpp>
#include <cwt/io/file.hpp>
#include <cwt/io/textreader.hpp>
#include <cwt/svg.hpp>
#include <iostream>
#include <QDir>
#include <QStringList>

SchemeSymbol::symbol_map_type SchemeSymbol::_symbols;
cwt::dom::document SchemeSymbol::_dummyDoc;

struct SchemePainterData
{
	pfs::stack<QPainterPath> stack;
};

static void __on_start (const cwt::dom::node & n, void * userData);
static void __on_end (const cwt::dom::node & n, void * userData);


bool SchemeSymbol::loadFromString (const pfs::string & str)
{
	cwt::xml::dom dom;
	_svgDoc = dom.createDocument(str);

	if (dom.isError()) {
		dom.logErrors();
		return false;
	}

	return true;
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

bool SchemeSymbol::loadSymbols (const pfs::string & symbolDirectory)
{
	bool r = true;
	QDir dir(qcast(symbolDirectory));
	cwt::fs fs;

	if (!dir.exists()) {
		cwt::log::error(_Fr("%s: symbols' directory not found") % symbolDirectory);
		return false;
	}

	QStringList nameFilters;

	nameFilters << "*.svg";
	dir.setNameFilters(nameFilters);
	dir.setFilter(QDir::Files);

	QStringList	symbolFiles = dir.entryList();

	cwt::log::debug(_Fr("Number of symbols found: %d") % symbolFiles.size());

	for (int i = 0; i < symbolFiles.size(); ++i) {
		SchemeSymbol symbol;
		pfs::string filename(qcast(symbolFiles[i]));
		pfs::string path(fs.join(symbolDirectory, filename));
		pfs::string name(filename.substr(0, filename.length() - 4)); // exclude suffix '.svg'

		cwt::log::debug(_Fr("Load symbol from file: %s") % path);

		if (symbol.loadFromFile(path)) {
			_symbols.insert(name, symbol);
			cwt::log::debug(_Fr("Inserted new symbol '%s' from file: %s") % name % path);
			//PFS_ASSERT(_symbols.find(name) != _symbols.cend());
		} else {
			cwt::log::error(_Fr("Failed to load scheme symbol from file %s") % path);
			r = false;
		}
	}
	return r;
}

SchemeSymbol SchemeSymbol::getSymbolByName (const pfs::string & name)
{
	symbol_map_type::const_iterator it = _symbols.find(name);
	if (it == _symbols.cend()) {
		return SchemeSymbol(_dummyDoc);
	}

	return it->second;
}

struct PathParseHandlers : public cwt::svg::path_parse_handlers
{
	QPainterPath & _ppath;

	PathParseHandlers (QPainterPath & ppath) : _ppath(ppath) {}

	virtual void begin () override {}
	virtual void end (bool /*success*/) override {}

	virtual void onClosePath (const cwt::svg::path_seg_closepath & /*pseg*/) override
	{
		_ppath.closeSubpath();
	}

	virtual void onMoveToAbs (const cwt::svg::path_seg_moveto_abs & p) override
	{
		_ppath.moveTo(_ppath.currentPosition().x() + p.x(), _ppath.currentPosition().y() + p.y());
	}

	virtual void onMoveToRel (const cwt::svg::path_seg_moveto_rel & p) override
	{
		_ppath.moveTo(p.x(), p.y());
	}

	virtual void onLineToAbs (const cwt::svg::path_seg_lineto_abs & p) override
	{
		_ppath.lineTo(_ppath.currentPosition().x() + p.x(), _ppath.currentPosition().y() + p.y());
	}

	virtual void onLineToRel (const cwt::svg::path_seg_lineto_rel & p) override
	{
		_ppath.lineTo(p.x(), p.y());
	}

	virtual void onCurveToCubicAbs (const cwt::svg::path_seg_curveto_cubic_abs & p) override
	{
		std::cout << "Cubic(abs): x1=" << p.x1() << "; y1=" << p.y1() << "; x2=" << p.x2() << "; y2=" << p.y2() << "; x=" << p.x() << "; y=" << p.y() << std::endl;
		double curX = _ppath.currentPosition().x();
		double curY = _ppath.currentPosition().y();
		_ppath.cubicTo(curX + p.x1(), curY + p.y1(), curX + p.x2(), curY + p.y2(), curX + p.x(), curY + p.y());
	}

	virtual void onCurveToCubicRel (const cwt::svg::path_seg_curveto_cubic_rel & p) override
	{
		std::cout << "Cubic(rel): x1=" << p.x1() << "; y1=" << p.y1() << "; x2=" << p.x2() << "; y2=" << p.y2() << "; x=" << p.x() << "; y=" << p.y() << std::endl;
		_ppath.cubicTo(p.x1(), p.y1(), p.x2(), p.y2(), p.x(), p.y());
	}

	virtual void onCurveToQuadraticAbs (const cwt::svg::path_seg_curveto_quadratic_abs & /*p*/) override {}
	virtual void onCurveToQuadraticRel (const cwt::svg::path_seg_curveto_quadratic_rel & /*p*/) override {}
	virtual void onArcAbs (const cwt::svg::path_seg_arc_abs & /*pseg*/) override {}
	virtual void onArcRel (const cwt::svg::path_seg_arc_rel & /*pseg*/) override {}
	virtual void onLineToHorizontalAbs (const cwt::svg::path_seg_lineto_horizontal_abs & /*p*/) override {}
	virtual void onLineToHorizontalRel (const cwt::svg::path_seg_lineto_horizontal_rel & /*p*/) override {}
	virtual void onLineToVerticalAbs (const cwt::svg::path_seg_lineto_vertical_abs & /*p*/) override {}
	virtual void onLineToVerticalRel (const cwt::svg::path_seg_lineto_vertical_rel & /*p*/) override {}
	virtual void onCurveToCubicSmoothAbs (const cwt::svg::path_seg_curveto_cubic_smooth_abs & /*p*/) override {}
	virtual void onCurveToCubicSmoothRel (const cwt::svg::path_seg_curveto_cubic_smooth_rel & /*p*/) override {}
	virtual void onCurveToQuadraticSmoothAbs (const cwt::svg::path_seg_curveto_quadratic_smooth_abs & /*p*/) override {}
	virtual void onCurveToQuadraticSmoothRel (const cwt::svg::path_seg_curveto_quadratic_smooth_rel & /*p*/) override {}
};

QPainterPath SchemeSymbol::toPainterPath () const
{
	if (!_svgDoc.isNull()) {
		SchemePainterData userData;
		userData.stack.push(QPainterPath());
		_svgDoc.traverse(__on_start, __on_end, & userData);
		PFS_ASSERT(userData.stack.size() == 1);
		return userData.stack.top();
	}

	return QPainterPath();
}

static void __on_start (const cwt::dom::node & n, void * userData)
{
	SchemePainterData * d = static_cast<SchemePainterData *>(userData);
	if (n.nodeType() == cwt::dom::node::ElementNode) {
		cwt::dom::namednodemap atts = n.attributes();

		if (n.localName() == _l1("g")) {
			d->stack.push(QPainterPath());
		} else if (n.localName() == _l1("path")) {
			if (n.hasAttributes()) {
				for (size_t i = 0; i < atts.length(); ++i) {
					cwt::dom::attr attr = atts.item(i).toAttr();
					if (attr.localName() == _l1("d")) {
						std::cout << attr.localName() << "=" << attr.value() << std::endl;

						QPainterPath pp;
						PathParseHandlers pph(pp);
						if (cwt::svg::path_seg::parse(attr.value(), pph)) {
							d->stack.top().addPath(pp);
						}
					}
				}
			}
		} else if (n.localName() == _l1("ellipse")) {
			QPointF center;
			center.setX(atts.getNamedItem(_l1("cx")).toAttr().value().toDouble());
			center.setY(atts.getNamedItem(_l1("cy")).toAttr().value().toDouble());
			double rx = atts.getNamedItem(_l1("rx")).toAttr().value().toDouble();
			double ry = atts.getNamedItem(_l1("ry")).toAttr().value().toDouble();;
			d->stack.top().addEllipse(center, rx, ry);
		} else if (n.localName() == _l1("line")) {
			double x1 = atts.getNamedItem(_l1("x1")).toAttr().value().toDouble();
			double y1 = atts.getNamedItem(_l1("y1")).toAttr().value().toDouble();
			double x2 = atts.getNamedItem(_l1("x2")).toAttr().value().toDouble();
			double y2 = atts.getNamedItem(_l1("y2")).toAttr().value().toDouble();;
			d->stack.top().moveTo(x1, y1);
			d->stack.top().lineTo(x2, y2);
		}
	}
}

static void __on_end (const cwt::dom::node & n, void * userData)
{
	SchemePainterData * d = static_cast<SchemePainterData *>(userData);
	if (n.nodeType() == cwt::dom::node::ElementNode) {
		if (n.localName() == _l1("g")) {
			QPainterPath g(d->stack.top());
			d->stack.pop();
			d->stack.top().addPath(g);
		}
	}
}
