/**
 * @file document.cpp
 * @author wladt
 * @date Dec 11, 2013
 */

#include <pfs/stack.hpp>
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

namespace pfs { namespace dom {

document_impl::document_impl ()
    : node_impl(0)
    , _impl(new dom_implementation_impl)
    , _type(new doctype_impl(this, this))
    , _nodeListTime(1)
{
    //_type->ref.deref();
    _name = pfs::string("#document");
}

document_impl::document_impl (const pfs::string & name)
    : node_impl(0)
    , _impl(new dom_implementation_impl)
	, _type(new doctype_impl(this, this))
	, _nodeListTime(1)
{
    //_type->ref.deref();
    _type->_name = name;
    _name = pfs::string("#document");
}

document_impl::document_impl (doctype_impl * dt)
    : node_impl(0)
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
	, _type(static_cast<doctype_impl*>(n->_type->clone_node()))
	, _nodeListTime(1)
{
    _type->setParent(this);
}

element_impl* document_impl::document_element()
{
    node_impl * p = _first;

    while (p && !p->is_element())
        p = p->_next;

    return static_cast<element_impl *>(p);
}


document_impl::~document_impl ()
{
//	_type.reset(0);
}

node_impl * document_impl::clone_node (bool deep)
{
    node_impl *p = new document_impl(this, deep);
    p->ref.deref();
    return p;
}

element_impl * document_impl::create_element (const pfs::string & tagName)
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



element_impl * document_impl::create_element_ns (const pfs::string & nsURI, const pfs::string & qName)
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

document_fragment_impl * document_impl::create_document_fragment ()
{
    document_fragment_impl * f = new document_fragment_impl(this, 0);
    f->ref.deref();
    return f;
}

text_impl * document_impl::create_text_node (const pfs::string & data)
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


comment_impl * document_impl::create_comment (const pfs::string & data)
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

cdatasection_impl * document_impl::create_cdata_section (const pfs::string & data)
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

pinstruction_impl * document_impl::create_processing_instruction (const pfs::string & target, const pfs::string & data)
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


attr_impl * document_impl::create_attribute(const pfs::string & name)
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

attr_impl * document_impl::create_attribute_ns (const pfs::string & nsURI, const pfs::string & qName)
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

entityref_impl * document_impl::create_entity_reference (const pfs::string & name)
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

node_impl * document_impl::import_node (const node_impl * importedNode, bool deep)
{
    node_impl * node = 0;

    switch (importedNode->node_type()) {
        case node::attribute_node:
            node = new attr_impl((attr_impl *)importedNode, true);
            break;
        case node::document_fragment_node:
            node = new document_fragment_impl((document_fragment_impl *)importedNode, deep);
            break;
        case node::element_node:
            node = new element_impl((element_impl *)importedNode, deep);
            break;
        case node::entity_node:
            node = new entity_impl((entity_impl *)importedNode, deep);
            break;
        case node::entity_reference_node:
            node = new entityref_impl((entityref_impl *)importedNode, false);
            break;
        case node::notation_node:
            node = new notation_impl((notation_impl *)importedNode, deep);
            break;
        case node::processing_instruction_node:
            node = new pinstruction_impl((pinstruction_impl *)importedNode, deep);
            break;
        case node::text_node:
            node = new text_impl((text_impl *)importedNode, deep);
            break;
        case node::cdata_section_node:
            node = new cdatasection_impl((cdatasection_impl *)importedNode, deep);
            break;
        case node::comment_node:
            node = new comment_impl((comment_impl *)importedNode, deep);
            break;
        default:
            break;
    }
    if (node) {
        node->set_owner_document(this);
        node->ref.deref();
    }
    return node;
}

document::document (const pfs::string & name) : node(new document_impl(name))
{}

document::document (const pfs::dom::doctype & dt)
	: node(new document_impl((doctype_impl*)(dt._d)))
{}

document::document (const document & other) : node(other)
{}

document::document (document_impl * x) : node(x)
{}

document & document::operator = (const document & other)
{
    return (document &) node::operator = (other);
}

pfs::dom::doctype document::doctype () const
{
	return _d
			? pfs::dom::doctype(dynamic_cast<document_impl *>(_d)->doctype())
			: pfs::dom::doctype();
}

dom_implementation document::implementation () const
{
	return _d
			? dom_implementation(dynamic_cast<document_impl *>(_d)->implementation())
			: dom_implementation();
}

element document::document_element () const
{
	return _d
			? element(dynamic_cast<document_impl *>(_d)->document_element())
			: element();
}//    QDomAttr createAttributeNS(const QString& nsURI, const QString& qName);

element document::create_element (const pfs::string & tagName)
{
	if (!_d)
        _d = new document_impl();

    return element(dynamic_cast<document_impl *>(_d)->create_element(tagName));
}


element document::create_element_ns (const pfs::string & nsURI, const pfs::string & qName)
{
    if (!_d)
        _d = new document_impl();
    return element(dynamic_cast<document_impl *>(_d)->create_element_ns(nsURI, qName));
}


document_fragment document::create_document_fragment()
{
    if (!_d)
        _d = new document_impl();
    return document_fragment(dynamic_cast<document_impl *>(_d)->create_document_fragment());
}

text document::create_text_node (const pfs::string & value)
{
    if (!_d)
        _d = new document_impl();
    return text(dynamic_cast<document_impl *>(_d)->create_text_node(value));
}

comment document::create_comment (const pfs::string & value)
{
    if (!_d)
        _d = new document_impl();
    return comment(dynamic_cast<document_impl *>(_d)->create_comment(value));
}

cdatasection document::create_cdata_section(const pfs::string& value)
{
    if (!_d)
        _d = new document_impl();
    return cdatasection(dynamic_cast<document_impl *>(_d)->create_cdata_section(value));
}

pinstruction document::create_processing_instruction (const pfs::string & target, const pfs::string & data)
{
    if (!_d)
        _d = new document_impl();
    return pinstruction(dynamic_cast<document_impl *>(_d)->create_processing_instruction(target, data));
}

attr document::create_attribute (const pfs::string & name)
{
    if (!_d)
        _d = new document_impl();
    return attr(dynamic_cast<document_impl *>(_d)->create_attribute(name));
}

attr document::create_attribute_ns (const pfs::string & nsURI, const pfs::string & qName)
{
    if (!_d)
        _d = new document_impl();
    return attr(dynamic_cast<document_impl *>(_d)->create_attribute_ns(nsURI, qName));
}

entityref document::create_entity_reference (const pfs::string & name)
{
    if (!_d)
        _d = new document_impl();
    return entityref(dynamic_cast<document_impl *>(_d)->create_entity_reference(name));
}

node document::import_node (const node & importedNode, bool deep)
{
    if (!_d)
        _d = new document_impl();
    return node(dynamic_cast<document_impl *>(_d)->import_node(importedNode._d, deep));
}

nodelist document::get_elements_by_tagname (const pfs::string & tagname) const
{
    return nodelist(new nodelist_impl(_d, tagname));
}

nodelist document::get_elements_by_tagname_ns (const pfs::string & nsURI, const pfs::string & localName) const
{
    return nodelist(new nodelist_impl(_d, nsURI, localName));
}

// The DOM implementation must have information that says which attributes are of type ID.
// Attributes with the name "ID" are not of type ID unless so defined.
// Implementations that do not know whether attributes are of type ID or not are expected to return null.
element document::get_element_by_id (const pfs::string & elementId) const
{
	pfs::string idName = dynamic_cast<document_impl *>(_d)->implementation()->idname();

	if (idName.isEmpty())
		return element();

	pfs::dom::nodelist children = child_nodes();

	// No children
	if (!children.size())
		return element();

	pfs::stack<pfs::dom::nodelist> stack;
	stack.push(children);

	while (stack.size() > 0) {
		children = stack.top();
		stack.pop();

		for (size_t i = 0; i < children.size(); ++i) {
			node n(children.item(i));

			if (n.node_type() == node::element_node) {
				element e(n.to_element());

				if (e.attribute(idName) == elementId)
					return e;
			}

			if (n.has_child_nodes()) {
				stack.push(n.child_nodes());
			}
		}
	}
	return element();
}

}} // pfs::dom
