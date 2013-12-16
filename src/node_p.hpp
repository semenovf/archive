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

class document_impl;
class nodelist_impl;

class node_impl
{
	friend class nodelist_impl;

public:
    pfs::atomic_int ref;
    node_impl * prev;
    node_impl * next;
    node_impl * ownerNode; // either the node's parent or the node's owner document
    node_impl * first;
    node_impl * last;

    pfs::string name; // this is the local name if prefix != null
    pfs::string value;
    pfs::string prefix; // set this only for ElementNode and AttributeNode
    pfs::string namespaceURI; // set this only for ElementNode and AttributeNode
    bool createdWithDom1Interface : 1;
    bool hasParent                : 1;

public:
//    node_impl (document_impl *, node_impl * parent = nullptr);
    node_impl (node_impl * n, bool deep);
    virtual ~node_impl ();

    pfs::string nodeName () const { return name; }
    pfs::string nodeValue () const { return value; }
    virtual void setNodeValue (const pfs::string & v) { value = v; }

    document_impl * ownerDocument ();
    void setOwnerDocument (document_impl * doc);

    virtual node_impl * insertBefore (node_impl * newChild, node_impl * refChild);
    virtual node_impl * insertAfter  (node_impl * newChild, node_impl * refChild);
    virtual node_impl * replaceChild (node_impl * newChild, node_impl * oldChild);
    virtual node_impl * removeChild  (node_impl * oldChild);
    virtual node_impl * appendChild  (node_impl * newChild);

//    node_impl * namedItem (const pfs::string & name);

    virtual node_impl * cloneNode (bool deep = true);
    virtual void normalize ();
//    virtual void clear ();

    node_impl * parent () const { return hasParent ? ownerNode : 0; }
    void setParent (node_impl * p) { ownerNode = p; hasParent = true; }

    void setNoParent ()
    {
        ownerNode = hasParent ? (node_impl *)ownerDocument() : 0;
        hasParent = false;
    }

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
};


}} // cwt

#endif /* __CWT_DOM_NODE_P_HPP__ */
