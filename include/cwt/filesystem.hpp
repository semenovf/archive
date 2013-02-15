/**
 * @file   filesystem.hpp
 * @author wladt
 * @date   Feb 15, 2013 9:02:52 AM
 *
 * @brief
 */

#ifndef __CWT_FILESYSTEM_HPP__
#define __CWT_FILESYSTEM_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>

CWT_NS_BEGIN

class FileSystem
{
private:
	FileSystem() {}
	~FileSystem() {}
public:
	static Char separator();
	static bool isAbsolute(const String & path);
	static bool isRelative(const String & path) { return !isAbsolute(path); }
	static bool exists(const String& path);
	static bool rename(const String& from, const String& to);
	static bool remove(const String& from);
	static bool simpleBackup(const String& path);
};

CWT_NS_END

#endif /* __CWT_FILESYSTEM_HPP__ */
