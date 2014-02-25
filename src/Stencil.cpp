/**
 * @file Stencil.cpp
 * @author wladt
 * @date Feb 25, 2014
 */


#include "Stencil.hpp"
#include "Application.hpp"
#include <QDir>
#include <QStringList>
#include <QSvgRenderer>
#include <QPainter>
#include <cwt/logger.hpp>
#include <cwt/fs.hpp>

const char * Stencil::MimeType = "image/x-rw-meccano-stencil";
Stencil::map_type Stencil::_stencils;

Stencil Stencil::getStencilByName (const pfs::string & name)
{
	map_type::const_iterator it = _stencils.find(name);
	if (it == _stencils.cend()) {
		return Stencil();
	}

	return it->second;
}


bool Stencil::loadStencils (const pfs::string & stencilDirecory)
{
	bool r = true;
	QDir dir(qcast(stencilDirecory));
	cwt::fs fs;

	if (!dir.exists()) {
		cwt::log::error(_Fr("%s: symbols' directory not found") % stencilDirecory);
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
		pfs::string filepath(fs.join(stencilDirecory, filename));
		pfs::string name(filename.substr(0, filename.length() - 4)); // exclude suffix '.svg'

		cwt::log::debug(_Fr("Load symbol from file: %s") % filepath);

		_stencils.insert(name, Stencil(filepath));
		cwt::log::debug(_Fr("Inserted new symbol '%s' from file: %s") % name % filepath);
	}
	return r;
}

QPixmap Stencil::toPixmap () const
{
	QSvgRenderer renderer(qcast(_filepath));
	QPixmap r(50, 50);
	r.fill(Qt::transparent);

	QPainter painter(& r);
	renderer.render(& painter);
    return r;
}
