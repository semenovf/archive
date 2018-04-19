/**
 * @file   mt.c
 * @author wladt
 * @date   Feb 21, 2013 4:02:42 PM
 *
 * @brief
 */


#include "../include/cwt/mt.hpp"

CWT_NS_BEGIN

#if !defined(CWT_SINGLE_THREADED)
cwt_mutex_t multi_threaded_global::g_mutex;
#endif /* CWT_SINGLE_THREADED */

CWT_NS_END
