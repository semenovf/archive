/**
 * @file SchemeDom.hpp
 * @author wladt
 * @date Feb 19, 2014
 */

#ifndef __SCHEMEDOM_HPP__
#define __SCHEMEDOM_HPP__

#include <cwt/dom.hpp>
#include <cwt/xml/dom.hpp>

class SchemeDom : public cwt::xml::dom
{
	cwt::dom::document _svgDoc;

public:
	SchemeDom ();
	bool loadFromString (const pfs::string & str);
	bool loadFromFile (const pfs::string & path);

	const cwt::dom::document & document () const { return _svgDoc; }
	cwt::dom::document & document () { return _svgDoc; }
};

#endif /* __SCHEMEDOM_HPP__ */
