/**
 * @file SchemeDom.cpp
 * @author wladt
 * @date Feb 19, 2014
 */

#include "SchemeDom.hpp"
#include <cwt/logger.hpp>
#include <cwt/io/file.hpp>
#include <cwt/io/textreader.hpp>

SchemeDom::SchemeDom () : cwt::xml::dom()
{}


bool SchemeDom::loadFromString (const pfs::string & str)
{
	_svgDoc = createDocument(str);

	if (!isError())
		logErrors();

	return !_svgDoc.isNull();
}

bool SchemeDom::loadFromFile (const pfs::string & path)
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



