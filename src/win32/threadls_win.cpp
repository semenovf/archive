/*
 * threadls_win.cpp
 *
 *  Created on: Sep 19, 2013
 *      Author: wladt
 */


#include "../../include/cwt/threadls.hpp"

#error "!!! Must be implemented !!!"

namespace cwt {

// FIXME Implement these !

DLL_API void cwt_set_tls_data (void const * key, pfs::shared_ptr<cleanup_functor> func, void * tss_data, bool cleanup_existing)
{

}

DLL_API void * cwt_get_tls_data (void const * key)
{

}

} // cwt



