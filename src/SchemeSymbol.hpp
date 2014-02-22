/*
 * SchemeSymbol.hpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */

#ifndef __SCHEMSYMBOL_HPP__
#define __SCHEMSYMBOL_HPP__

#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <cwt/xml/dom.hpp>

class QPainterPath;

class SchemeSymbol
{
	cwt::dom::document _svgDoc;

private:
	typedef pfs::map<pfs::string, SchemeSymbol> symbol_map_type;
	static symbol_map_type _symbols;
	static cwt::dom::document _dummyDoc;

protected:
	SchemeSymbol (const cwt::dom::document & svgDoc) : _svgDoc(svgDoc) {}

public:
	SchemeSymbol () {}

	QPainterPath toPainterPath () const;
	bool loadFromString (const pfs::string & str);
	bool loadFromFile (const pfs::string & path);

	static bool loadSymbols (const pfs::string & symbolDirecory);
	static SchemeSymbol getSymbolByName (const pfs::string & name);
};


#endif /* __SCHEMSYMBOL_HPP__ */
