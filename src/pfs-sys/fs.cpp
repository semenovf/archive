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

pfs::string fs::join (const pfs::string & dir, const pfs::string filename)
{
	pfs::string r;
	r << dir << separator() << filename;
	return r;
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

} // pfs

