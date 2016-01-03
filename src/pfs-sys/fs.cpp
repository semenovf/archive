/*
 * fs.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */

#if __OBSOLETE__ // XXX OBSOLETE

#include "pfs/fs.hpp"
#include "pfs/random.hpp"

namespace pfs {

pfs::string fs::join (const pfs::string & dir, const pfs::string filename) const
{
	pfs::string r;
	r << dir << separator() << filename;
	return r;
}

/**
 * @brief Concatenates list items into string represented the path.
 *
 * @details Result starts with separator if first item is explicit specified separator.
 *          Result ends with separator if last item is explicit specified separator.
 *          No repeated separator in resulting string if it not specified inside the item;
 *
 * @param items List of path elements.
 * @return Concatenated string represented the path.
 */
string fs::join (const stringlist & items) const
{
	if (items.size() == 0)
		return string();

	stringlist::const_iterator it = items.cbegin();
	stringlist::const_iterator itEnd = items.cend();

	string result(*it);
	++it;

	ucchar sep = separator();
	string sepStr(1, sep);

	for (; it != itEnd; ++it) {
		if (!result.endsWith(sep))
			result.append(1, sep);

		if (*it != sepStr)
			result.append(*it);
	}

	return result;
}

string fs::basename (const string & path) const
{
	vector<string> s = path.split(separator(), true, pfs::ucchar('"'));

	if (s.size() > 0)
		return s[s.size() - 1];

	return pfs::string();
}

string fs::tempFile (const string & prefix, const string & suffix, const string & dir, int nattempts)
{
    string r;
    pfs::random rnd;

    do {
        string s;
        uint32_t d = rnd.rand();
        s << prefix << string::toString(d, 16) << suffix;
        r = join(dir, s);
    } while (exists(r) && --nattempts);

    if (nattempts <= 0)
        return string();

    return r;
}
/**
 * @brief Normalize path, i.e. remove "." and "..".
 *
 * @param path Path to normalization.
 * @return Normalized path.
 */
string fs::normalizePath (const string & path) const
{
	if (path.isEmpty())
		return string();

	// TODO Add reserve() method for mbcs_string (reserve space in units)
	// result.reserve(path.size())
	string result;

//	stringlist sl = path.split(separatorChar(), string::KeepEmpty);
//
//
//	stringlist::const_iterator it = sl.rbegin();
//	stringlist::const_iterator itEnd = sl.rend();
//
//	while (it != itEnd) {
//		if (*it == string("..")) {
//			++it;
//		} else if (*it == string(".")) {
//			; // nothing to do
//		} else {
//			result.prepend(*it);
//		}
//	}
//
//	if (path.startsWith(fs.separator()))
//		result.prepend()

	return result;
}

} // pfs
#endif
