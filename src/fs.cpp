/*
 * fs.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */


#include "pfs/fs.hpp"
#include "pfs/random.hpp"

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

string fs::tempFile (const string & prefix, const string & suffix)
{
    string r;
    string tmpDir = tempDirectory();
    size_t ntries = 100;

    do {
        random rand;
        string s;
        s << prefix << string::toString(rand.rand(), 16) << suffix;
        r = join(tmpDir, s);
    } while (!exists(r) && --ntries);

    if (!ntries) {
        return string();
    }

    return r;
}

} // pfs

