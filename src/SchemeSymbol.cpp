/*
 * SchemeItem.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */

#include "SchemeSymbol.hpp"
#include "Application.hpp"
#include "SchemeCanvas.hpp"
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
	int nodeCount;
	int balance;
	int indent;
	//SchemeSymbol & symbol;
	QPainterPath ppath;

	SchemePainterData (/*SchemeSymbol & s*/)
		: nodeCount(0), balance(0), indent(0)/*, symbol(s)*/ { }
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
	return it == _symbols.cend() ? SchemeSymbol(_dummyDoc) : it->second;
}

struct PathParseHandlers : public cwt::svg::path_parse_handlers
{
	QPainterPath & _ppath;

	PathParseHandlers (QPainterPath & ppath) : _ppath(ppath) {}

	virtual void begin () override {}
	virtual void end (bool /*success*/) override {}
	virtual void onClosePath (const cwt::svg::path_seg_closepath & /*pseg*/) override
		{ _ppath.closeSubpath(); }
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

QPainterPath SchemeSymbol::toPainterPath () const
{
	if (!_svgDoc.isNull()) {
		SchemePainterData userData;
		_svgDoc.traverse(__on_start, __on_end, & userData);
		return userData.ppath;
	}

	return QPainterPath();
}

static void __on_start (const cwt::dom::node & n, void * userData)
{
	SchemePainterData * d = static_cast<SchemePainterData *>(userData);
	if (n.nodeType() == cwt::dom::node::ElementNode) {
		//std::cout << std::string(d->indent * 4, ' ') << '<' <<  n.nodeName() << '>' << std::endl;
		++d->nodeCount;
		++d->balance;
		++d->indent;

		cwt::dom::namednodemap atts = n.attributes();

		if (n.localName() == _l1("path")) {
			if (n.hasAttributes()) {
				for (size_t i = 0; i < atts.length(); ++i) {
					cwt::dom::attr attr = atts.item(i).toAttr();
					if (attr.localName() == _l1("d")) {
						std::cout << attr.localName() << "=" << attr.value() << std::endl;

						QPainterPath pp;
						PathParseHandlers pph(pp);
						if (cwt::svg::path_seg::parse(attr.value(), pph)) {
							d->ppath.addPath(pp);
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
			d->ppath.addEllipse(center, rx, ry);
		} else if (n.localName() == _l1("line")) {
			double x1 = atts.getNamedItem(_l1("x1")).toAttr().value().toDouble();
			double y1 = atts.getNamedItem(_l1("y1")).toAttr().value().toDouble();
			double x2 = atts.getNamedItem(_l1("x2")).toAttr().value().toDouble();
			double y2 = atts.getNamedItem(_l1("y2")).toAttr().value().toDouble();;
			d->ppath.moveTo(x1, y1);
			d->ppath.lineTo(x2, y2);
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
