/*
 * dbs.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DBS_HPP__
#define __CWT_DBS_HPP__

#include <cwt/unitype.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

class DbHandler;

class DbStatement
{

public:
	DbStatement();
	~DbStatement();

	void            close();
	bool            exec ();
	ulong_t         lastId ();
	long_t          errno  ();
	String          strerror ();
	bool            bindByIndex (size_t index, const UniType & ut);
	size_t          bindParmsCount ();
	bool            isBindParmBounds (int index);
	bool            setParm (const UniType & ut, const void * copy, size_t sz);
	ulong_t         rows ();
	ulong_t         size ();
	bool            fetchNext ();
	bool            fetchColumn (const String & col, UniType & value);

private:
	shared_ptr<DbHandler> dbh;
	//Vector<UniType>       bind_params;
} CwtStatement;

CWT_NS_END


#endif /* __CWT_DBS_HPP__ */
