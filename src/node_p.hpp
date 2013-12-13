/**
 * @file node_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_NODE_P_HPP__
#define __CWT_DOM_NODE_P_HPP__

#include "../include/cwt/dom/node.hpp"
#include <pfs/atomic.hpp>

namespace cwt { namespace dom {

class document::impl;

class node::impl
{
public:
    impl(document::impl*, impl* parent = 0);
    impl(impl* n, bool deep);
    virtual ~impl();

    pfs::string nodeName() const { return name; }
    pfs::string nodeValue() const { return value; }
    virtual void setNodeValue(const pfs::string& v) { value = v; }

    document::impl* ownerDocument();
    void setOwnerDocument(document::impl* doc);

    virtual impl* insertBefore(impl* newChild, impl* refChild);
    virtual impl* insertAfter(impl* newChild, impl* refChild);
    virtual impl* replaceChild(impl* newChild, impl* oldChild);
    virtual impl* removeChild(impl* oldChild);
    virtual impl* appendChild(impl* newChild);

    impl* namedItem(const pfs::string& name);

    virtual impl* cloneNode(bool deep = true);
    virtual void normalize();
    virtual void clear();

    inline impl* parent() const { return hasParent ? ownerNode : 0; }
    inline void setParent(impl *p) { ownerNode = p; hasParent = true; }

    void setNoParent() {
        ownerNode = hasParent ? (impl*)ownerDocument() : 0;
        hasParent = false;
    }

    // Dynamic cast
    bool isAttr() const                     { return nodeType() == node::AttributeNode; }
    bool isCDATASection() const             { return nodeType() == node::CDATASectionNode; }
    bool isDocumentFragment() const         { return nodeType() == node::DocumentFragmentNode; }
    bool isDocument() const                 { return nodeType() == node::DocumentNode; }
    bool isDocumentType() const             { return nodeType() == node::DocumentTypeNode; }
    bool isElement() const                  { return nodeType() == node::ElementNode; }
    bool isEntityReference() const          { return nodeType() == node::EntityReferenceNode; }
    bool isText() const                     { const node::type nt = nodeType();
                                              return (nt == node::TextNode)
                                                  || (nt == node::CDATASectionNode); }
    bool isEntity() const                   { return nodeType() == node::EntityNode; }
    bool isNotation() const                 { return nodeType() == node::NotationNode; }
    bool isProcessingInstruction() const    { return nodeType() == node::ProcessingInstructionNode; }
    bool isCharacterData() const            { const node::type nt = nodeType();
                                              return (nt == node::CharacterDataNode)
                                                  || (nt == node::TextNode)
                                                  || (nt == node::CommentNode); }
    bool isComment() const                  { return nodeType() == node::CommentNode; }

    virtual node::type nodeType() const { return node::BaseNode; }

    void setLocation(int lineNumber, int columnNumber);

    // Variables
    pfs::atomic_integer ref;
    impl* prev;
    impl* next;
    impl* ownerNode; // either the node's parent or the node's owner document
    impl* first;
    impl* last;

    pfs::string name; // this is the local name if prefix != null
    pfs::string value;
    pfs::string prefix; // set this only for ElementNode and AttributeNode
    pfs::string namespaceURI; // set this only for ElementNode and AttributeNode
    bool createdWithDom1Interface : 1;
    bool hasParent                : 1;

    int lineNumber;
    int columnNumber;
};


}} // cwt

#endif /* __CWT_DOM_NODE_P_HPP__ */
