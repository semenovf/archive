/**
 * @file chardata.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __CWT_DOM_CHARDATA_HPP__
#define __CWT_DOM_CHARDATA_HPP__

#include <cwt/dom/node.hpp>

namespace cwt { namespace dom {

class chardata : public node
{
	pfs::string _data;
	size_t      _length; // readonly

public:
	chardata () : node () {}

	const pfs::string & data () const; // // raises(DOMException) on retrieval
	void setData (const pfs::string & data) const; // raises(DOMException) on setting
	size_t length () const { return _length; }

	pfs::string  substringData (size_t offset, size_t count); // raises(DOMException);
	void appendData (const pfs::string & arg); // raises(DOMException);
	void insertData (size_t offset, const pfs::string & arg); // raises(DOMException);
	void deleteData (size_t offset, size_t count);  // raises(DOMException);
	void replaceData (size_t offset, size_t count, const pfs::string & arg); // raises(DOMException);
};

}} // cwt::dom

#endif /* __CWT_DOM_CHARDATA_HPP__ */
