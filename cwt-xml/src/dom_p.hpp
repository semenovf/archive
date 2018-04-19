/**
 * @file document.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_XML_DOM_P_HPP__
#define __CWT_XML_DOM_P_HPP__

#include "../include/cwt/xml/dom.hpp"
#include "../include/cwt/xml/sax.hpp"
#include "../../cwt-dom/src/node_p.hpp"
#include "../../cwt-dom/src/nodelist_p.hpp"
#include "../../cwt-dom/src/namednodemap_p.hpp"
#include "../../cwt-dom/src/attr_p.hpp"
#include "../../cwt-dom/src/element_p.hpp"
#include "../../cwt-dom/src/element_p.hpp"
#include "../../cwt-dom/src/chardata_p.hpp"
#include "../../cwt-dom/src/text_p.hpp"
#include "../../cwt-dom/src/cdatasection_p.hpp"
#include "../../cwt-dom/src/comment_p.hpp"
#include "../../cwt-dom/src/entity_p.hpp"
#include "../../cwt-dom/src/entityref_p.hpp"
#include "../../cwt-dom/src/notation_p.hpp"
#include "../../cwt-dom/src/pinstruction_p.hpp"
#include "../../cwt-dom/src/doctype_p.hpp"
#include "../../cwt-dom/src/document_p.hpp"

namespace cwt { namespace xml {

class document_builder : public handlers
{
public:
	cwt::dom::document_impl * _doc;
	cwt::dom::node_impl * _node;
	bool _isCdata;

public:
	document_builder ()
		: handlers()
		, _doc(new cwt::dom::document_impl)
		, _node(nullptr)
		, _isCdata(false)
	{
		_node = _doc;
	}
	virtual ~document_builder () {}

	virtual void startElement (const pfs::string & nsURI, const pfs::string & localName, const pfs::vector<attr_triplet> & atts);
	virtual void endElement (const pfs::string & nsURI, const pfs::string & localName);
	virtual void characterData (const pfs::string & text);
	virtual void processingInstruction (const pfs::string & target, const pfs::string & data);
	virtual void comment (const pfs::string & text);
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
