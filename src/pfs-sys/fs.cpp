/*
 * fs.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */


#include "pfs/fs.hpp"
#include "pfs/random.hpp"

#include <iostream>

namespace pfs {

pfs::string fs::join (const pfs::string & dir, const pfs::string filename) const
{
	pfs::string r;
	r << dir << separator() << filename;
	return r;
}

string fs::join (const stringlist & items) const
{
	stringlist::const_iterator it = items.cbegin();
	stringlist::const_iterator itEnd = items.cend();

	string result;

	for (; it != itEnd; ++it) {
		if (!result.isEmpty() && !result.endsWith(separator()))
			result.append(1, separator());

		result.append(*it);
	}

	return result;
}

string fs::basename (const string & path) const
{
	vector<string> s = path.split(separator(), true, pfs::ucchar('"'));
	if (s.size() > 0) {
		return s[s.size() - 1];
	}
	return pfs::string();
}

string fs::tempFile (const string & prefix, const string & suffix, const string & dir, int nattempts)
{
    string r;
    pfs::random rnd;

    std::cout << nattempts << std::endl;

    do {
        string s;
        uint32_t d = rnd.rand();
        s << prefix << string::toString(d, 16) << suffix;
        r = join(dir, s);
    } while (exists(r) && --nattempts);

    std::cout << nattempts << std::endl;

    if (nattempts <= 0) {
        return string();
    }

    return r;
}

#if __FIXME__

/**
 * @brief Searches file in directories listed by @c dirs
 *        and returns normalized absolute path for file @c filename.
 *
 * @param filename File name to search. May be absolute or relative.
 * @param dirs List of directories for search.
 * @return Absolute file path.
 *         If @c filename is empty or if error occurred returns empty string.
 *         Error can be raised if currentDirectory() failed.
 */
string fs::findFile (const string & filename, const stringlist & dirs)
{
	pfs::fs fs;

	if (filename.isEmpty())
		return string();

	if (fs.exists(filename)) {
		/*
		 * If filename is not an absolute path
		 * and does not start with current directory prefix (i.e. "./" on unix)
		 * then append prefix and return result
		 */
		if (!fs.isAbsolute(filename)) {

			string cwd = fs.currentDirectory();
			if (cwd.isEmpty())
				return string();

			string result(".");
			result.append(1, fs.separator());

			if (!filename.startsWith(result)) {
				result.append(filename);
				return result;
			}
		}
		return string(filename);
	}

	if (!fs.isAbsolute(filename)) {
		stringlist::const_iterator it = _searchPath.cbegin();
		stringlist::const_iterator itEnd = _searchPath.cend();

		while (it != itEnd) {
			string r(*it);
			r += fs.separator();
			r += filename;

			if (fs.exists(r))
				return r;
			++it;
		}
	}

	return string();
}

/**
 * @brief Normalize path, i.e. remove "." and "..".
 *
 * @param path Path to normalization.
 * @return Normalized path.
 */
string normalizePath (const string & path) const
{
	if (path.isEmpty())
		return string();

	string result;
	stringlist sl = path.split(fs.separator(), string::KeepEmpty);

	// TODO Add reserve() method for mbcs_string (reserve space in units)
	// result.reserve(path.size())

	stringlist::const_iterator it = sl.rbegin();
	stringlist::const_iterator itEnd = sl.rend();

	while (it != itEnd) {
		if (*it == string("..")) {
			++it;
		} else if (*it == string(".")) {
			; // nothing to do
		} else {
			result.prepend(*it);
		}
	}

	if (path.startsWith(fs.separator()))
		result.prepend()

}

#endif

} // pfs

