#ifndef __CWT_DEBBY_HPP__
#define __CWT_DEBBY_HPP__

#include <cwt/cwt.hpp>
#include <cwt/string.hpp>
#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

class Debby
{
	CWT_PIMPL_IMPL(Debby);
public:
	Debby();

	bool parse(const String & src);
	bool parseFromFile(const String & path);
};

CWT_NS_END
 
#endif /* __CWT_DEBBY_HPP__ */
