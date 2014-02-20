/**
 * @file dom.cpp
 * @author wladt
 * @date Jan 10, 2014
 */

#include "dom_p.hpp"
#include <cwt/logger.hpp>
#include <cwt/xml/sax.hpp>

namespace cwt { namespace xml {

static bool __accept_version(int major, int minor)
{
	return major == 1 && minor == 0;
}

dom::dom () : cwt::errorable()
{ }

//cwt::dom::document createDocument (const pfs::string & xml_source
//		, const pfs::string & /*namespaceURI*/
//		, const pfs::string & /*qualifiedName*/
//		, const cwt::dom::doctype & /*doctype*/)
//{}
cwt::dom::document dom::createDocument (const pfs::string & xml_source)
{
	document_builder h;
	cwt::xml::reader xml_reader;
	xml_reader.setHandler(& h);
	xml_reader.acceptVersion(__accept_version);

	if (!xml_reader.parse(xml_source)) {
		this->addErrors(xml_reader);
		delete h._doc;
		return cwt::dom::document();
	}

	return h._doc->createDocument(); // this call is safe: document implementation '_doc' is dynamically allocated.
}

void document_builder::startElement (const pfs::string & nsURI, const pfs::string & localName, const pfs::vector<attr_triplet> & atts)
{
    cwt::dom::node_impl * n;

    if (nsURI.isEmpty())
    	n = _doc->createElement(localName);
    else
    	n = _doc->createElementNS(nsURI, localName);

    PFS_ASSERT(n);
    _node->appendChild(n);
    _node = n;

    pfs::vector<attr_triplet>::const_iterator it = atts.cbegin();
    pfs::vector<attr_triplet>::const_iterator itEnd = atts.cend();

    for ( ; it != itEnd; ++it)
    {
    	if (it->nsURI.isEmpty()) {
    		dynamic_cast<cwt::dom::element_impl *>(_node)->setAttribute(it->localName, it->value);
    	} else {
    		dynamic_cast<cwt::dom::element_impl *>(_node)->setAttributeNS(it->nsURI, it->localName, it->value);
    	}
    }
}

void document_builder::endElement (const pfs::string & /*nsURI*/, const pfs::string & /*localName*/)
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


void dom::traverse (const cwt::dom::node & beginNode
		, void (* onStart) (const cwt::dom::node & n, void * d)
		, void (* onEnd) (const cwt::dom::node & n, void * d)
		, void * userData)
{
	if (!beginNode.pimpl())
		return;

	cwt::dom::nodelist children = beginNode.childNodes();

	// No children
	if (!children.size())
		return;

	for (size_t i = 0; i < children.size(); ++i) {
		if (onStart)
			onStart(children.item(i), userData);

		traverse(children.item(i), onStart, onEnd, userData);

		if (onEnd)
			onEnd(children.item(i), userData);
	}
}


}} // cwt::xml
