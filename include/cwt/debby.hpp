#ifndef __CWT_DEBBY_HPP__
#define __CWT_DEBBY_HPP__

#ifdef __COMMENT__

#include <cwt/string.hpp>
#include <cwt/pimpl.hpp>
#include <cwt/unitype.hpp>
#include <cwt/debby/dbh.hpp>

#endif

CWT_NS_BEGIN

namespace debby
{

enum TypeEnum
{
	  Null
	, Bool
	, Integer
	, Float
	, Double
	, String
	, Blob
	, Binary = Blob
	, Date
	, Time
	, DateTime
	, TimeStamp
};

#ifdef __COMMENT__



class Debby
{
	CWT_PIMPL_IMPL(Debby);

public:
	Debby();

	bool parse(const String & src);
	bool parseFromFile(const String & path);
protected:
	bool process();
};

class Table : public Hash<String, UniType>
{
	typedef Hash<String, UniType> base_class;
public:
	Table() : base_class() {}
	bool has (const String & field) { return contains(field); }
};

#endif

} // namespace debby

CWT_NS_END
 
#endif /* __CWT_DEBBY_HPP__ */
