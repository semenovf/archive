/**
 * @file domimpl.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_DOMIMPL_HPP__
#define __PFS_DOM_DOMIMPL_HPP__

#include <pfs/string.hpp>

namespace pfs { namespace dom {

class document;
class doctype;
class dom_implementation_impl;

class DLL_API dom_implementation
{
	friend class document;

protected:
	dom_implementation_impl * _d;
	dom_implementation (dom_implementation_impl * p);

public:
	dom_implementation ();
	dom_implementation (const dom_implementation & other);
	dom_implementation & operator = (const dom_implementation & other);
	~dom_implementation ();

	bool has_feature (const pfs::string & feature, const pfs::string & version = pfs::string()) const;

	bool has_feature (const char * feature, const char * version = 0) const
	{
		return has_feature(pfs::string(feature), pfs::string(version));
	}

	document create_document (const pfs::string & namespace_uri
			, const pfs::string & qualifiedname
			, const doctype & doctype);   // raises(DOMException)

	doctype create_document_type (const pfs::string & qualifiedname
			, const pfs::string & public_id
			, const pfs::string & system_id); // raises(DOMException)

	// non-standard
	pfs::string idname () const; // used by node::get_element_by_id() e.g.
};

}} // pfs::dom

#endif /* __PFS_DOM_DOMIMPL_HPP__ */
