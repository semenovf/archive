/**
 * @file document.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_XML_DOM_P_HPP__
#define __CWT_XML_DOM_P_HPP__

#include "../include/cwt/xml/sax.hpp"
#include <cwt/dom.hpp>
/*
#include <cwt/dom/doctype.hpp>
#include <cwt/dom/exception.hpp>
*/

namespace cwt { namespace xml {

class document_builder : public handlers
{
public:
	cwt::dom::document _doc;

public:
	document_builder () : handlers() {}
	virtual ~document_builder () {}

	virtual void startElement (const pfs::string & tagname, const pfs::map<pfs::string, pfs::string> & atts);
	virtual void endElement (const pfs::string & tagname);
	virtual void characterData (const pfs::string & text);
	virtual void processingInstruction (const pfs::string & target, const pfs::string & data);
	virtual void comment (const pfs::string &);
	virtual void startCdataSection ();
	virtual void endCdataSection ();
	virtual void defaultData (const pfs::string &);
	virtual void skippedEntity (const pfs::string & entityName, bool is_parameter_entity);
	virtual void startNamespaceDecl (const pfs::string & prefix, const pfs::string & uri);
	virtual void endNamespaceDecl (const pfs::string & prefix);
	virtual void startDoctypeDecl (const pfs::string & doctypeName
			, const pfs::string & systemId
			, const pfs::string & publicId
			, bool has_internal_subset);
	virtual void endDoctypeDecl ();
	virtual void unparsedEntityDecl (
			  const pfs::string & entityName
			, const pfs::string & base
			, const pfs::string & systemId
			, const pfs::string & publicId
			, const pfs::string & notationName);
	virtual void notationDecl (
			  const pfs::string & notationName
	        , const pfs::string & base
	        , const pfs::string & systemId
	        , const pfs::string & publicId);
};

}} // cwt::xml

#endif /* __CWT_XML_DOM_P_HPP__ */
