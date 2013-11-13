/*
 * csv2db.hpp
 *
 *  Created on: Nov 13, 2013
 *      Author: wladt
 */

#ifndef __CSV2DB_HPP__
#define __CSV2DB_HPP__

#include <cwt/json.hpp>
#include <cwt/csv.hpp>
#include <cwt/debby/dbh.hpp>
#include <cwt/debby/record.hpp>
#include <cwt/debby/schema.hpp>

class Csv2DbContext
{
public:
	bool convert (const cwt::Json & policy
			, cwt::CsvReader & csvreader
			, cwt::debby::DbHandler & dbh
			, cwt::debby::Schema & schema
			, const cwt::String & tableName);
};


#endif /* __CSV2DB_HPP__ */
