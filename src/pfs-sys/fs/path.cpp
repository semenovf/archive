/*
 * path.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#include <pfs/deque.hpp>
#include <pfs/fs/path.hpp>

namespace pfs { namespace fs {

path path::canonical (const string_type & separator) const
{
	pfs::deque<string_type> comp_deque;
	range rn(_path, separator);
	iterator it = rn.begin();
	iterator it_end = rn.end();

	while (it != it_end) {
		if (*it == ".") {
			; // ignore
		} else if (*it == "..") {
			if (comp_deque.size() > 0)
				comp_deque.pop_front();
		} else {
			comp_deque.push_front(*it);
		}

		++it;
	}

	pathlist pl;

	while (comp_deque.size() > 0) {
		pl.push_back(comp_deque.back());
		comp_deque.pop_back();
	}

	if (this->is_absolute())
		return join(path(), join(pl, separator), separator);

	return join(pl, separator);
}

path join (const path & p1, const path & p2, const string & separator)
{
	string r(p1.native());
	r.append(separator);
	r.append(p2.native());
	return path(r);
}

path join (const pathlist & pl, const string & separator)
{
	string r;

	pathlist::const_iterator it = pl.cbegin();
	pathlist::const_iterator it_end = pl.cend();

	if (it != it_end) {
		r.append(it->native());
		++it;
	}

	while (it != it_end) {
		r.append(separator);
		r.append(it->native());
		++it;
	}

	return path(r);
}

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
		, const pathlist & searchdirs
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
		path p = join(*it, file);

		if (exists(p))
			return p;
		++it;
	}

	return path();
}

}} // pfs::fs
