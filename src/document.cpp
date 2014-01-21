/**
 * @file document.cpp
 * @author wladt
 * @date Dec 11, 2013
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "docfragment_p.hpp"
#include "chardata_p.hpp"
#include "text_p.hpp"
#include "comment_p.hpp"
#include "cdatasection_p.hpp"
#include "pinstruction_p.hpp"
#include "entity_p.hpp"
#include "entityref_p.hpp"
#include "notation_p.hpp"
#include "domimpl_p.hpp"
#include "doctype_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {

document_impl::document_impl ()
    : node_impl(nullptr)
    , _impl(new dom_implementation_impl)
	, _type(new doctype_impl(this, this))
    , _nodeListTime(1)
{
    //_type->ref.deref();
    _name = pfs::string("#document");
}

document_impl::document_impl (const pfs::string & name)
    : node_impl(nullptr)
    , _impl(new dom_implementation_impl)
	, _type(new doctype_impl(this, this))
	, _nodeListTime(1)
{
    //_type->ref.deref();
    _type->_name = name;
    _name = pfs::string("#document");
}

document_impl::document_impl (doctype_impl * dt)
    : node_impl(nullptr)
    , _impl(new dom_implementation_impl)
    , _nodeListTime(1)
{
    if (dt != 0) {
        _type.reset(dt);
    } else {
        _type.reset(new doctype_impl(this, this));
        _type->ref.deref();
    }

    _name = pfs::string("#document");
}

document_impl::document_impl (document_impl * n, bool deep)
    : node_impl(n, deep)
	, _impl(n->_impl->clone())
	, _type(static_cast<doctype_impl*>(n->_type->cloneNode()))
	, _nodeListTime(1)
{
    _type->setParent(this);
}

element_impl* document_impl::documentElement()
{
    node_impl * p = _first;

    while (p && !p->isElement())
        p = p->_next;

    return static_cast<element_impl *>(p);
}


document_impl::~document_impl ()
{
	PFS_TRACE("~document_impl()");
//	_type.reset(nullptr);
}

node_impl * document_impl::cloneNode (bool deep)
{
    node_impl *p = new document_impl(this, deep);
    p->ref.deref();
    return p;
}

element_impl * document_impl::createElement (const pfs::string & tagName)
{
/*
    bool ok;
    pfs::string fixedName = fixedXmlName(tagName, &ok);
    if (!ok)
        return 0;
*/

    element_impl * e = new element_impl(this, 0, tagName/*fixedName*/);
    e->ref.deref();
    return e;
}



element_impl * document_impl::createElementNS (const pfs::string & nsURI, const pfs::string & qName)
{
/*
    bool ok;
    pfs::string fixedName = fixedXmlName(qName, &ok, true);
    if (!ok)
        return 0;
*/
    element_impl * e = new element_impl(this, 0, nsURI, qName/*fixedName*/);
    e->ref.deref();
    return e;
}

document_fragment_impl * document_impl::createDocumentFragment ()
{
    document_fragment_impl * f = new document_fragment_impl(this, nullptr);
    f->ref.deref();
    return f;
}

text_impl * document_impl::createTextNode (const pfs::string & data)
{
/*
    bool ok;
    pfs::string fixedData = fixedCharData(data, &ok);
    if (!ok)
        return 0;
*/
    text_impl * t = new text_impl(this, 0, data/*fixedData*/);
    t->ref.deref();
    return t;
}


comment_impl * document_impl::createComment (const pfs::string & data)
{
/*
    bool ok;
    pfs::string fixedData = fixedComment(data, &ok);
    if (!ok)
        return 0;
*/
    comment_impl * c = new comment_impl(this, 0, data/*fixedData*/);
    c->ref.deref();
    return c;
}

cdatasection_impl * document_impl::createCDATASection (const pfs::string & data)
{
/*
    bool ok;
    pfs::string fixedData = fixedCDataSection(data, &ok);
    if (!ok)
        return 0;
*/
    cdatasection_impl * c = new cdatasection_impl(this, 0, data/*fixedData*/);
    c->ref.deref();
    return c;
}

pinstruction_impl * document_impl::createProcessingInstruction (const pfs::string & target, const pfs::string & data)
{
/*
    bool ok;
    pfs::string fixedData = fixedPIData(data, &ok);
    if (!ok)
        return 0;
    // [17] PITarget ::= Name - (('X' | 'x') ('M' | 'm') ('L' | 'l'))
    pfs::string fixedTarget = fixedXmlName(target, &ok);
    if (!ok)
        return 0;
*/

    pinstruction_impl * p = new pinstruction_impl(this, 0, target/*fixedTarget*/, data/*fixedData*/);
    p->ref.deref();
    return p;
}


attr_impl * document_impl::createAttribute(const pfs::string & name)
{
/*
    bool ok;
    pfs::string fixedName = fixedXmlName(aname, &ok);
    if (!ok)
        return 0;
*/
    attr_impl * a = new attr_impl(this, 0, name/*fixedName*/);
    a->ref.deref();
    return a;
}

attr_impl * document_impl::createAttributeNS (const pfs::string & nsURI, const pfs::string & qName)
{
/*
    bool ok;
    pfs::string fixedName = fixedXmlName(qName, &ok, true);
    if (!ok)
        return 0;
*/
    attr_impl * a = new attr_impl(this, 0, nsURI, qName/*fixedName*/);
    a->ref.deref();
    return a;
}

entityref_impl * document_impl::createEntityReference (const pfs::string & name)
{
/*
    bool ok;
    pfs::string fixedName = fixedXmlName(aname, &ok);
    if (!ok)
        return 0;
*/
    entityref_impl * e = new entityref_impl(this, 0, name/*fixedName*/);
    e->ref.deref();
    return e;
}

node_impl * document_impl::importNode (const node_impl * importedNode, bool deep)
{
    node_impl * node = nullptr;

    switch (importedNode->nodeType()) {
        case node::AttributeNode:
            node = new attr_impl((attr_impl *)importedNode, true);
            break;
        case node::DocumentFragmentNode:
            node = new document_fragment_impl((document_fragment_impl *)importedNode, deep);
            break;
        case node::ElementNode:
            node = new element_impl((element_impl *)importedNode, deep);
            break;
        case node::EntityNode:
            node = new entity_impl((entity_impl *)importedNode, deep);
            break;
        case node::EntityReferenceNode:
            node = new entityref_impl((entityref_impl *)importedNode, false);
            break;
        case node::NotationNode:
            node = new notation_impl((notation_impl *)importedNode, deep);
            break;
        case node::ProcessingInstructionNode:
            node = new pinstruction_impl((pinstruction_impl *)importedNode, deep);
            break;
        case node::TextNode:
            node = new text_impl((text_impl *)importedNode, deep);
            break;
        case node::CDATASectionNode:
            node = new cdatasection_impl((cdatasection_impl *)importedNode, deep);
            break;
        case node::CommentNode:
            node = new comment_impl((comment_impl *)importedNode, deep);
            break;
        default:
            break;
    }
    if (node) {
        node->setOwnerDocument(this);
        node->ref.deref();
    }
    return node;
}

document::document (const pfs::string & name) : node(new document_impl(name))
{}

document::document (const cwt::dom::doctype & dt)
	: node(new document_impl((doctype_impl*)(dt._pimpl)))
{}

document::document (const document & other) : node(other)
{}

document::document (document_impl * x) : node(x)
{}

document & document::operator = (const document & other)
{
    return (document &) node::operator = (other);
}

cwt::dom::doctype document::doctype () const
{
	return _pimpl
			? cwt::dom::doctype(dynamic_cast<document_impl *>(_pimpl)->doctype())
			: cwt::dom::doctype();
}

dom_implementation document::implementation () const
{
	return _pimpl
			? dom_implementation(dynamic_cast<document_impl *>(_pimpl)->implementation())
			: dom_implementation();
}

element document::documentElement () const
{
	return _pimpl
			? element(dynamic_cast<document_impl *>(_pimpl)->documentElement())
			: element();
}//    QDomAttr createAttributeNS(const QString& nsURI, const QString& qName);

element document::createElement (const pfs::string & tagName)
{
	if (!_pimpl)
        _pimpl = new document_impl();

    return element(dynamic_cast<document_impl *>(_pimpl)->createElement(tagName));
}


element document::createElementNS (const pfs::string & nsURI, const pfs::string & qName)
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return element(dynamic_cast<document_impl *>(_pimpl)->createElementNS(nsURI, qName));
}


document_fragment document::createDocumentFragment()
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return document_fragment(dynamic_cast<document_impl *>(_pimpl)->createDocumentFragment());
}

text document::createTextNode (const pfs::string & value)
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return text(dynamic_cast<document_impl *>(_pimpl)->createTextNode(value));
}

comment document::createComment (const pfs::string & value)
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return comment(dynamic_cast<document_impl *>(_pimpl)->createComment(value));
}

cdatasection document::createCDATASection(const pfs::string& value)
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return cdatasection(dynamic_cast<document_impl *>(_pimpl)->createCDATASection(value));
}

pinstruction document::createProcessingInstruction (const pfs::string & target, const pfs::string & data)
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return pinstruction(dynamic_cast<document_impl *>(_pimpl)->createProcessingInstruction(target, data));
}

attr document::createAttribute (const pfs::string & name)
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return attr(dynamic_cast<document_impl *>(_pimpl)->createAttribute(name));
}

attr document::createAttributeNS (const pfs::string & nsURI, const pfs::string & qName)
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return attr(dynamic_cast<document_impl *>(_pimpl)->createAttributeNS(nsURI, qName));
}

entityref document::createEntityReference (const pfs::string & name)
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return entityref(dynamic_cast<document_impl *>(_pimpl)->createEntityReference(name));
}

node document::importNode (const node & importedNode, bool deep)
{
    if (!_pimpl)
        _pimpl = new document_impl();
    return node(dynamic_cast<document_impl *>(_pimpl)->importNode(importedNode._pimpl, deep));
}

nodelist document::getElementsByTagName (const pfs::string & tagname) const
{
    return nodelist(new nodelist_impl(_pimpl, tagname));
}

nodelist document::getElementsByTagNameNS (const pfs::string & nsURI, const pfs::string & localName) const
{
    return nodelist(new nodelist_impl(_pimpl, nsURI, localName));
}

element document::getElementById (const pfs::string & /*elementId*/) const
{
    //qWarning("elementById() is not implemented and will always return a null node.");
    return element();
}

}} // cwt::dom
