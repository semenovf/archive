/*
 * threadls_win.cpp
 *
 *  Created on: Sep 19, 2013
 *      Author: wladt
 */


#include "../../include/cwt/thread.hpp"
#include "../../include/cwt/threadls.hpp"

namespace cwt {

thread::tls_implementation_type thread::tls_implementation ()
{
#ifdef CWT_HAVE_TLS
	return thread::TlsCompilerSpecific;
#else
	return thread::TlsWin32;
#endif
}

// FIXME Implement these !

/*
DLL_API void cwt_set_tls_data (void const * key, pfs::shared_ptr<cleanup_functor> func, void * tss_data, bool cleanup_existing)
{

}

DLL_API void * cwt_get_tls_data (void const * key)
{

}
*/

} // cwt



