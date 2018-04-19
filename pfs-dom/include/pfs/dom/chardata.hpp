/**
 * @file chardata.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_CHARDATA_HPP__
#define __PFS_DOM_CHARDATA_HPP__

#include <pfs.hpp>

namespace pfs { namespace dom {

class chardata_impl;
class node;

class chardata : public node
{
protected:
	chardata (chardata_impl *);

public:
	chardata ();
	chardata (const chardata & other);
	chardata & operator = (const chardata & other);

	pfs::string data () const;
	void set_data (const pfs::string & data);
	size_t length () const;

	pfs::string  substring_data (size_t offset, size_t count) const;
	void append_data (const pfs::string & arg);
	void insert_data (size_t offset, const pfs::string & arg);
	void delete_data (size_t offset, size_t count);
	void replace_data (size_t offset, size_t count, const pfs::string & arg);

	// overridden
	node::type node_type () const;
};

}} // pfs::dom

#endif /* __PFS_DOM_CHARDATA_HPP__ */
