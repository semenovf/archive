/**
 * @file condition_variable.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 *
 * @brief Implements Conditional Variable
 */

#ifndef __PFS_CONDITION_VARIABLE_HPP__
#define __PFS_CONDITION_VARIABLE_HPP__

#include <pfs.hpp>
#include <pfs/mutex.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class condition_variable_impl;

class DLL_API condition_variable
{
	condition_variable_impl * _d;

private:
	condition_variable (const condition_variable &);
	condition_variable & operator = (const condition_variable &);

public:
	condition_variable ();
    ~condition_variable ();

    bool wait (pfs::mutex & mx, uintmax_t time = max_value<uintmax_t>());

    void notify_one ();
    void notify_all ();

//    template <typename predicate_type>
//    void wait (pfs::mutex & lockedMutex, predicate_type predicate)
//    {
//        while (!predicate())
//        	wait(lockedMutex);
//    }
//
//    template <typename predicate_type>
//    bool wait (pfs::mutex & lockedMutex, uintmax_t timeout, predicate_type predicate)
//    {
//        while (!predicate()) {
//            if(!wait(lockedMutex, timeout))
//                return predicate();
//        }
//        return true;
//    }
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_CONDITION_VARIABLE_HPP__ */
