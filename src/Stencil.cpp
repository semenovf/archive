/**
 * @file Stencil.cpp
 * @author wladt
 * @date Feb 25, 2014
 */


#include "Stencil.hpp"
#include "Application.hpp"
#include "qcast.hpp"
#include <QDir>
#include <QStringList>
#include <QSvgRenderer>
#include <QSvgWidget>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSvgItem>
#include <cwt/logger.hpp>
#include <cwt/fs.hpp>
#include <cwt/io/textreader.hpp>
#include <cwt/io/file.hpp>

//const char * Stencil::MimeType = "image/x-rw-meccano-stencil";
Stencil::document_map_type Stencil::_documents;


Stencil::Stencil (const pfs::string & name)
	: _name(name)
{}

Stencil::Stencil (const Stencil & other)
	: _name(other._name)
{}

Stencil & Stencil::operator = (const Stencil & other)
{
	_name = other._name;
	return *this;
}

QPixmap Stencil::toPixmap ()
{
	// FIXME need actual size
	return toPixmap(50, 50);
}

QPixmap Stencil::toPixmap (int /*scale*/)
{
	// FIXME need actual scaled pixmap
	return toPixmap(50, 50);
}

QPixmap Stencil::toPixmap (int width, int height)
{
	if (!_name.isEmpty()) {
		cwt::xml::dom dom;
		pfs::string svgText = dom.toString(documentByName(_name));

		// calculate actual size
		QSvgWidget svgWidget;
		svgWidget.load(QByteArray(svgText.constData()));

//		PFS_DEBUG(qDebug() << "svgWidget: " << svgWidget);

		QSvgRenderer renderer(QByteArray(svgText.constData()));
		QPixmap pm(width, height);
		pm.fill(Qt::transparent);

		QPainter painter(& pm);
		renderer.render(& painter);
		return pm;
	}
	return QPixmap();
}

/*
void Stencil::setFromSvg (const pfs::string & svg)
{
	setPixmap(toPixmap(50, 50, svg)); // FIXME need real size
}

void Stencil::setFromDocument (const cwt::dom::document & doc)
{
	cwt::xml::dom dom;
	setFromSvg(dom.toString(doc));
}
*/

QPixmap Stencil::toIcon ()
{
	return toPixmap(IconWidth, IconHeight);
}

cwt::dom::document Stencil::documentByName (const pfs::string & name)
{
	return _documents.value(name, cwt::dom::document());
}

cwt::dom::document Stencil::documentFromString (const pfs::string & str)
{
	cwt::xml::dom dom;
	cwt::dom::document r = dom.createDocument(str);

	if (dom.isError()) {
		dom.logErrors();
		return cwt::dom::document();
	}

	return r;
}

cwt::dom::document Stencil::documentFromFile (const pfs::string & path)
{
	cwt::io::file file;
	file.open(path, cwt::io::device::ReadOnly);

	if (!file.opened()) {
		file.logErrors();
		return cwt::dom::document();
	}

	cwt::io::text_reader textReader(file);

	pfs::string str = textReader.read(file.size());
	return documentFromString(str);
}

bool Stencil::loadStencils (const pfs::string & stencilDirectory)
{
	bool r = true;
	QDir dir(qcast(stencilDirectory));
	cwt::fs fs;

	if (!dir.exists()) {
		cwt::log::error(_Fr("%s: symbols' directory not found") % stencilDirectory);
		return false;
	}

	QStringList nameFilters;

	nameFilters << "*.svg";
	dir.setNameFilters(nameFilters);
	dir.setFilter(QDir::Files);

	QStringList	symbolFiles = dir.entryList();

	cwt::log::debug(_Fr("Number of symbols found: %d") % symbolFiles.size());

	for (int i = 0; i < symbolFiles.size(); ++i) {
		pfs::string filename(qcast(symbolFiles[i]));
		pfs::string filepath(fs.join(stencilDirectory, filename));
		pfs::string name(filename.substr(0, filename.length() - 4)); // exclude suffix '.svg'

		cwt::dom::document doc = documentFromFile(filepath);

		if (doc.isNull()) {
			cwt::log::error(_Fr("%s: bad source") % filepath);
		} else {
			cwt::log::info(_Fr("Stencil '%s' loaded from file: %s") % name % filepath);
			_documents.insert(name, doc);
		}
	}
	return r;
}

