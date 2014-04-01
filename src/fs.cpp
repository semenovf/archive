/*
 * fs.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: wladt
 */


#include "../include/cwt/fs.hpp"

namespace cwt {

pfs::string fs::join (const pfs::string & dir, const pfs::string filename)
{
	pfs::string r;
	r << dir << separator() << filename;
	return r;
}

pfs::string fs::basename (const pfs::string & path) const
{
	pfs::vector<pfs::string> s = path.split(separator(), true, pfs::ucchar('"'));
	if (s.size() > 0) {
		return s[s.size() - 1];
	}
	return pfs::string();
}


} // cwt

