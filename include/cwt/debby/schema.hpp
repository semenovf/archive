/*
 * scheme.hpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_SCHEME_HPP__
#define __CWT_DEBBY_SCHEME_HPP__

#include <cwt/string.hpp>
#include <cwt/map.hpp>
#include <cwt/debby.hpp>
#include <cwt/debby/dbh.hpp>

CWT_NS_BEGIN

namespace debby
{

class Record;

class DLL_API Schema
{
	typedef cwt::Map<cwt::String, Record *> RecordMap;
public:
	Schema() {}
	~Schema();

	Record & add (const cwt::String & name);

	bool deploy (const cwt::String & uri);
	bool deploy (DbHandler & dbh);
	bool drop   (const cwt::String & uri);
	bool drop   (DbHandler & dbh);

	size_t count () const { return _schema.size(); }
	cwt::Vector<cwt::String> names() const { return _schema.keys(); }

	const Record & operator [] (const cwt::String & name) const;
	Record & operator [] (const cwt::String & name);
	bool containes (const cwt::String & name) { return _schema.contains(name); }

	bool load (DbHandler & dbh);

private:
	RecordMap _schema;
	Record    _dummy;
};

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEME_HPP__ */
