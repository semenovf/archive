#ifndef __CWT_DEBBY_HPP__
#define __CWT_DEBBY_HPP__

#include <cwt/string.hpp>
#include <cwt/pimpl.hpp>
#include <cwt/unitype.hpp>
#include <cwt/debby/dbh.hpp>

CWT_NS_BEGIN

class Debby
{
	CWT_PIMPL_IMPL(Debby);

public:
	enum TypeEnum
	{
		  TypeNull
		, TypeBool
		, TypeString
		, TypeInteger
		, TypeFloat
		, TypeDouble
		, TypeDate
		, TypeTime
		, TypeDateTime
		, TypeTimeStamp
	};

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

CWT_NS_END
 
#endif /* __CWT_DEBBY_HPP__ */
