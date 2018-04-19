/**
 * @file element.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_ELEMENT_HPP__
#define __PFS_DOM_ELEMENT_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace dom {

class element_impl;
class node;
class attr;
class document;
class nodelist;

class element : public node
{
	friend class node;
	friend class attr;
	friend class document;

protected:
	element (element_impl * n);

public:
	element ();
	element (const element & other);
	element & operator = (const element & other);

	pfs::string tagname () const;

	pfs::string attribute (const pfs::string & name, const pfs::string & defaultvalue = pfs::string()) const
	{
		return get_attribute(name, defaultvalue);
	}

	pfs::string get_attribute (const pfs::string & name, const pfs::string & defaultvalue = pfs::string()) const;

	void set_attribute (const pfs::string & name, const pfs::string & value);

	void remove_attribute (const pfs::string & name);
	attr attribute_node (const pfs::string & name)
	{
		return get_attribute_node(name);
	}

	attr get_attribute_node (const pfs::string & name);
	attr set_attribute_node (const attr & newattr);
	attr remove_attribute_node (const attr & oldattr);

	nodelist elements_by_tagname (const pfs::string & name) const { return get_elements_by_tagname(name); }
	nodelist get_elements_by_tagname (const pfs::string & name) const ;

	pfs::string attribute_ns (const pfs::string & namespace_uri
			, const pfs::string & localname
			, const pfs::string & defaultvalue = pfs::string()) const
	{
		return get_attribute_ns(namespace_uri, localname, defaultvalue);
	}

	pfs::string get_attribute_ns (const pfs::string & namespaceURI
			, const pfs::string & localName
			, const pfs::string & defaultValue = pfs::string()) const;

	void set_attribute_ns (const pfs::string & namespace_uri
			, const pfs::string & qualifiedname
			, const pfs::string & value);

	void remove_attribute_ns (const pfs::string & namespace_uri
			, const pfs::string & localname);


	attr attribute_node_ns (const pfs::string & namespace_uri
			, const pfs::string & localname) const
	{
		return get_attribute_node_ns(namespace_uri, localname);
	}

	attr get_attribute_node_ns (const pfs::string & namespace_uri
			, const pfs::string & localname) const;

	attr set_attribute_node_ns (const attr & newattr);

	nodelist elements_by_tagname_ns (const pfs::string & namespace_uri
			, const pfs::string & localname) const
	{
		return get_elements_by_tagname_ns (namespace_uri, localname);
	}

	nodelist get_elements_by_tagname_ns (const pfs::string & namespace_uri
			, const pfs::string & localname) const;

	bool has_attribute (const pfs::string & name) const;

	bool has_attribute_ns (const pfs::string & namespace_uri
			, const pfs::string & localname) const;
};

}} // pfs

#endif /* __PFS_DOM_ELEMENT_HPP__ */
