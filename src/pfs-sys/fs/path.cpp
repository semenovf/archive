/*
 * path.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#include <pfs/fs/path.hpp>

namespace pfs { namespace fs {

//path unique_path (const path & model, notification_type * nx)
//{
//    string r;
//    pfs::random rnd;
//
//    do {
//        string s;
//        uint32_t d = rnd.rand();
//        s << prefix << string::toString(d, 16) << suffix;
//        r = join(dir, s);
//    } while (exists(r) && --nattempts);
//
//    if (nattempts <= 0)
//        return string();
//
//    return r;
//}

path search_file (const path & file
		, const pathlist searchdirs
		, error_code * ex)
{
	if (file.empty())
		return path();

	if (fs::exists(file)) {
		if (file.is_absolute())
			return file;

		/*
		 * If p is not an absolute path
		 * then prepend current directory and return result
		 */
		error_code ex1;
		path curr_dir = fs::current_directory(& ex1);

		if (ex1) {
			if (ex)
				*ex = ex1;
			return path();
		}

		return join(curr_dir, file);
	}

	// p is relative, search it in
	//
	fs::pathlist::const_iterator it = searchdirs.begin();
	fs::pathlist::const_iterator it_end = searchdirs.end();

	while (it != it_end) {
		path p = join(*it, p);

		if (exists(p))
			return p;
		++it;
	}

	return path();
}

}} // pfs::fs
