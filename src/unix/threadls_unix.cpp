/*
 * threadls_unix.cpp
 *
 *  Created on: Sep 19, 2013
 *      Author: wladt
 */


#include "../../include/cwt/threadls.hpp"

CWT_NS_BEGIN

// FIXME Implement these !

DLL_API void cwt_set_tls_data (void const * key, shared_ptr<cleanup_functor> func, void * tss_data, bool cleanup_existing)
{

}

DLL_API void * cwt_get_tls_data (void const * key)
{

}

CWT_NS_END

