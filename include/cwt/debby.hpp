#ifndef __CWT_DEBBY_HPP__
#define __CWT_DEBBY_HPP__

#include <cwt/cwt.hpp>
#include <cwt/string.hpp>
#include <cwt/pimpl.hpp>
#include <cwt/hash.hpp>
#include <cwt/unitype.hpp>

CWT_NS_BEGIN

class Debby
{
	CWT_PIMPL_IMPL(Debby);

public:
	Debby();

	bool parse(const String & src);
	bool parseFromFile(const String & path);
};


/*
class Scheme
{

};
*/

class Entity : public Hash<String, UniType>
{
	typedef Hash<String, UniType> base_class;
public:
	Entity() : base_class() {}
	bool has (const String & field) { return contains(field); }
	void reset();
	bool save();
};


CWT_NS_END
 
#endif /* __CWT_DEBBY_HPP__ */
