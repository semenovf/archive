/**
 * @file dom.cpp
 * @author wladt
 * @date Jan 10, 2014
 */

#include "dom_p.hpp"
#include <cwt/logger.hpp>

namespace cwt { namespace xml {

static bool __accept_version(int major, int minor)
{
	return major == 1 && minor == 0;
}

dom::dom () : reader()
{ }

//cwt::dom::document createDocument (const pfs::string & xml_source
//		, const pfs::string & /*namespaceURI*/
//		, const pfs::string & /*qualifiedName*/
//		, const cwt::dom::doctype & /*doctype*/)
cwt::dom::document dom::createDocument (const pfs::string & xml_source)
{
	document_builder h;
	this->setHandler(& h);
	this->acceptVersion(__accept_version);

	if (!this->parse(xml_source)) {
		delete h._doc;
		return cwt::dom::document();
	}

	return h._doc->createDocument(); // this call is safe: document implementation '_doc' is dynamically allocated.
}

void document_builder::startElement (const pfs::string & tagname, const pfs::map<pfs::string, pfs::string> & atts)
{
    cwt::dom::node_impl * n;

    // TODO need to support namespace (createElementNS)

    n = _doc->createElement(tagname);
    PFS_ASSERT(n);
    _node->appendChild(n);
    _node = n;


    pfs::map<pfs::string, pfs::string>::const_iterator it = atts.cbegin();
    pfs::map<pfs::string, pfs::string>::const_iterator itEnd = atts.cend();

    for ( ; it != itEnd; ++it)
    {
    	// TODO need to support namespace (setAttributeNS)

    	dynamic_cast<cwt::dom::element_impl *>(_node)->setAttribute(it->first, it->second);
    }
}

void document_builder::endElement (const pfs::string & /*tagname*/)
{
    PFS_ASSERT(_node && _node != _doc);
    _node = _node->parent();
}

void document_builder::characterData (const pfs::string & text)
{
	PFS_ASSERT(_node && _node != _doc);

    cwt::dom::node_impl * n = nullptr;

    if (_isCdata) {
        n = _doc->createCDATASection(text);
    } else {
        n = _doc->createTextNode(text);
    }

    PFS_ASSERT(n);
    _node->appendChild(n);
}

void document_builder::processingInstruction (const pfs::string & target, const pfs::string & data)
{
    cwt::dom::node_impl * n;
    n = _doc->createProcessingInstruction(target, data);
    PFS_ASSERT(n);
    _node->appendChild(n);
}

void document_builder::comment (const pfs::string & text)
{
    cwt::dom::node_impl * n = _doc->createComment(text);
    PFS_ASSERT(n);
    _node->appendChild(n);
}

void document_builder::startCdataSection ()
{
	_isCdata = true;
}

void document_builder::endCdataSection ()
{
	_isCdata = false;
}

void document_builder::defaultData (const pfs::string &)
{}

void document_builder::skippedEntity (const pfs::string & entityName, bool /*is_parameter_entity*/)
{
    cwt::dom::node_impl * n = _doc->createEntityReference(entityName);
    PFS_ASSERT(n);
    _node->appendChild(n);
}

void document_builder::startNamespaceDecl (const pfs::string & /*prefix*/, const pfs::string & /*uri*/)
{}

void document_builder::endNamespaceDecl (const pfs::string & /*prefix*/)
{}

void document_builder::startDoctypeDecl (const pfs::string & doctypeName
		, const pfs::string & systemId
		, const pfs::string & publicId
		, bool /*has_internal_subset*/)
{
    _doc->doctype()->_name = doctypeName;
    _doc->doctype()->_publicId = publicId;
    _doc->doctype()->_systemId = systemId;
}

void document_builder::endDoctypeDecl ()
{}

void document_builder::unparsedEntityDecl (
		  const pfs::string & entityName
		, const pfs::string & /*base*/
		, const pfs::string & systemId
		, const pfs::string & publicId
		, const pfs::string & notationName)
{
    cwt::dom::entity_impl * n = new cwt::dom::entity_impl(_doc
    		, nullptr
    		, entityName
            , publicId
            , systemId
            , notationName);
    n->ref.deref();
    _doc->doctype()->appendChild(n);
}

void document_builder::notationDecl (
		  const pfs::string & notationName
        , const pfs::string & /*base*/
        , const pfs::string & systemId
        , const pfs::string & publicId)
{
    cwt::dom::notation_impl * n = new cwt::dom::notation_impl(_doc
    		, nullptr
    		, notationName
    		, publicId
    		, systemId);
    n->ref.deref();
    _doc->doctype()->appendChild(n);
}

}} // cwt::xml
