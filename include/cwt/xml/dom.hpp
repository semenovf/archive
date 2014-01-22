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
#include <cwt/xml/sax.hpp>

namespace cwt { namespace xml {

class DLL_API dom : public reader
{
public:
	dom ();
	cwt::dom::document createDocument (const pfs::string & xml_source);
};

}} // cwt::xml

#endif /* __CWT_XML_DOM_HPP__ */
