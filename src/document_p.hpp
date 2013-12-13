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

class attr::impl;
class comment::impl;
class node::impl;
class text::impl;
class element::impl;
class entity_reference::impl;
class document_type::impl;
class document_fragment::impl;
class processing_instruction::impl;
class dom_implementation::impl;
class cdatasection::impl;

class document::impl : public node::impl
{
public:
    document::impl();
    document::impl(const pfs::string& name);
    document::impl(document_type::impl* dt);
    document::impl(document::impl* n, bool deep);
    ~document::impl();

    bool setContent(QXmlInputSource *source, bool namespaceProcessing, pfs::string *errorMsg, int *errorLine, int *errorColumn);
    bool setContent(QXmlInputSource *source, QXmlReader *reader, pfs::string *errorMsg, int *errorLine, int *errorColumn);

    // Attributes
    document_type::impl * doctype() { return type.data(); }
    dom_implementation::impl * implementation() { return impl.data(); }
    element::impl* documentElement();

    // Factories
    element::impl * createElement(const pfs::string& tagName);
    element::impl *        createElementNS(const pfs::string& nsURI, const pfs::string& qName);
    document_fragment::impl* createDocumentFragment();
    text::impl* createTextNode(const pfs::string& data);
    comment::impl* createComment(const pfs::string& data);
    cdatasection::impl * createCDATASection(const pfs::string& data);
    processing_instruction::impl * createProcessingInstruction(const pfs::string& target, const pfs::string& data);
    attr::impl* createAttribute(const pfs::string& name);
    attr::impl* createAttributeNS(const pfs::string& nsURI, const pfs::string& qName);
    entity_reference::impl * createEntityReference(const pfs::string& name);

    node::impl * importNode(const node::impl* importedNode, bool deep);

    // Reimplemented from node::impl
    node::impl * cloneNode(bool deep = true);
    node::type nodeType() const { return node::DocumentNode; }
    void clear();

    // Variables
//    QExplicitlySharedDataPointer<dom_implementation::impl> impl;
//    QExplicitlySharedDataPointer<document_type::impl> type;
    // TODO Check equivalence to above declaration
    pfs::shared_ptr<dom_implementation::impl> impl;
    pfs::shared_ptr<document_type::impl> type;

    long nodeListTime;
};

}} // cwt

#endif /* __CWT_DOM_DOCUMENT_P_HPP__ */
