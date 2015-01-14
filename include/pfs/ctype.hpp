/*
 * ctype.hpp
 *
 *  Created on: Jan 14, 2015
 *      Author: wladt
 */

#ifndef __PFS_CTYPE_HPP__
#define __PFS_CTYPE_HPP__

namespace pfs {

template <typename CharT>
bool is_space (CharT v)
{
	return v == CharT(' ')
			|| v == CharT('\f')
			|| v == CharT('\n')
			|| v == CharT('\r')
			|| v == CharT('\t')
			|| v == CharT('\v');
}


}

#endif /* __PFS_CTYPE_HPP__ */
