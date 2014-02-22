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

} // cwt

