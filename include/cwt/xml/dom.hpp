/**
 * @file dom.hpp
 * @author wladt
 * @date Jan 22, 2014
 */

#ifndef __CWT_XML_DOM_HPP__
#define __CWT_XML_DOM_HPP__

#include <pfs/string.hpp>
#include <cwt/errorable.hpp>
#include <cwt/dom.hpp>

namespace cwt { namespace xml {

class DLL_API dom : public cwt::errorable
{
public:
	dom ();

	cwt::dom::document createDocument (const pfs::string & xml_source);
	pfs::string toString () const;
};

}} // cwt::xml

#endif /* __CWT_XML_DOM_HPP__ */
