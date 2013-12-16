/**
 * @file document_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_DOCUMENT_P_HPP__
#define __CWT_DOM_DOCUMENT_P_HPP__

#include "../include/cwt/dom/document.hpp"
#include <pfs/shared_ptr.hpp>

namespace cwt { namespace dom {

class attr_impl;
class comment_impl;
class node_impl;
class text_impl;
class element_impl;
class entity_reference_impl;
class document_type_impl;
class document_fragment_impl;
class processing_instruction_impl;
class dom_implementation_impl;
class cdatasection_impl;

class document_impl : public node_impl
{
public:
    document_impl();
    document_impl(const pfs::string& name);
    document_impl(document_type_impl * dt);
    document_impl(document_impl * n, bool deep);
    ~document_impl();

//    bool setContent(QXmlInputSource *source, bool namespaceProcessing, pfs::string *errorMsg, int *errorLine, int *errorColumn);
//    bool setContent(QXmlInputSource *source, QXmlReader *reader, pfs::string *errorMsg, int *errorLine, int *errorColumn);

    // Attributes
    document_type_impl * doctype() { return type.get(); }
    dom_implementation_impl * implementation() { return impl.get(); }
    element_impl* documentElement();

    // Factories
    element_impl * createElement(const pfs::string& tagName);
    element_impl * createElementNS(const pfs::string& nsURI, const pfs::string& qName);
    document_fragment_impl * createDocumentFragment();
    text_impl * createTextNode(const pfs::string& data);
    comment_impl * createComment(const pfs::string& data);
    cdatasection_impl * createCDATASection(const pfs::string& data);
    processing_instruction_impl * createProcessingInstruction(const pfs::string& target, const pfs::string& data);
    attr_impl * createAttribute(const pfs::string& name);
    attr_impl * createAttributeNS(const pfs::string& nsURI, const pfs::string& qName);
    entity_reference_impl * createEntityReference(const pfs::string& name);

    node_impl * importNode(const node_impl* importedNode, bool deep);

    // Reimplemented from node_impl
    node_impl * cloneNode(bool deep = true);
    node::type nodeType() const { return node::DocumentNode; }
    void clear();

    // Variables
//    QExplicitlySharedDataPointer<dom_implementation_impl> impl;
//    QExplicitlySharedDataPointer<document_type_impl> type;
    // TODO Check equivalence to above declaration
    pfs::shared_ptr<dom_implementation_impl> impl;
    pfs::shared_ptr<document_type_impl> type;

    long nodeListTime;
};

}} // cwt

#endif /* __CWT_DOM_DOCUMENT_P_HPP__ */
