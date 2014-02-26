/**
 * @file StationName.cpp
 * @author wladt
 * @date Feb 26, 2014
 */

#ifdef __COMMENT__
#include "StationName.hpp"

static const pfs::string __pattern("<svg><g class=\"station-name\">"
"<text x=\"185.71429\" y=\"215.21933\""
"  style=\"font-size:40px;font-style:normal;font-weight:normal;line-height:100%;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;font-family:Sans\">"
"  ${station_name}"
"</text>"
"<rect width=\"299.45261\" height=\"113.82869\" x=\"168.57143\" y=\"143.79076\""
"  style=\"fill:none;stroke:#000000;stroke-width:2.31976819;stroke-miterlimit:4;stroke-opacity:1;stroke-dasharray:none\" />"
"</g></svg>");

StationName::StationName (QGraphicsItem * parent)
	: Stencil (parent)
{}

StationName::StationName (const pfs::string & name, QGraphicsItem * parent)
	: Stencil (parent)
{
	rename(name);
}

void StationName::rename (const pfs::string & newName)
{
	_name = newName;
	pfs::string svg(__pattern);
	svg.replace(_l1("${station_name}"), _name);
	setFromSvg(svg);
}

pfs::string StationName::stencilIconSvg () const
{
	pfs::string svg(__pattern);
	svg.replace(_l1("${station_name}"), _l1(_Tr("Station Name")));
	return svg;
}
#endif
