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

class DLL_API node_impl
{
	friend class nodelist_impl;

public:
    pfs::atomic_int ref;
    node_impl * _prev;
    node_impl * _next;
    node_impl * _ownerNode; // either the node's parent or the node's owner document
    node_impl * _first;
    node_impl * _last;

    pfs::string _name; // this is the local name if prefix != null
    pfs::string _value;
    pfs::string _prefix; // set this only for ElementNode and AttributeNode
    pfs::string _namespaceURI; // set this only for ElementNode and AttributeNode
    bool _createdWithDom1Interface : 1;
    bool _hasParent                : 1;

public:
    node_impl (document_impl *, node_impl * parent = nullptr);
    node_impl (node_impl * n, bool deep);
    virtual ~node_impl ();

    pfs::string nodeName () const { return _name; }
    pfs::string nodeValue () const { return _value; }
    virtual void setNodeValue (const pfs::string & v) { _value = v; }

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

    node_impl * parent () const { return _hasParent ? _ownerNode : 0; }
    void setParent (node_impl * p) { _ownerNode = p; _hasParent = true; }

    void setNoParent ()
    {
        _ownerNode = _hasParent ? (node_impl *)ownerDocument() : 0;
        _hasParent = false;
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
