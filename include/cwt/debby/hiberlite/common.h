#ifndef __HIBERLITE_COMMON_H__
#define __HIBERLITE_COMMON_H__

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include <stdexcept>

//#include <sqlite3.h>
#include <pfs.hpp>

namespace hiberlite {

class noncopyable
{
protected:
	noncopyable() {}
	~noncopyable() {}

private:
	noncopyable (const noncopyable &);
	const noncopyable operator = (const noncopyable &);
};

//typedef sqlite_int64 sqlid_t;
typedef int64_t sqlid_t;

} //namespace hiberlite

#define HIBERLITE_HL_DBG_DO(x) ;

#endif // __HIBERLITE_COMMON_H__
