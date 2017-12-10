/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   null.hpp
 * Author: wladt
 *
 * Created on April 20, 2016, 11:01 AM
 */

#ifndef __PFS_IO_NULL_HPP__
#define __PFS_IO_NULL_HPP__

#include <pfs/io/device.hpp>

namespace pfs { namespace io {

/**
 * @struct pfs::io::null
 * @brief Null device implementation.
 * @see pfs::io::device.
 */
struct null {};

template <>
struct open_params<null>
{
	open_params () {}
};

/**
 * @fn device open_device<null> (const open_params<null> & op, error_code & ex)
 *
 * @brief Open null device.
 *
 * @param op Open device parameters.
 *      @li open_params<null>()
 *      	Open null device.
 * @param ex Reference to store error code.
 * 
 * @return Device instance.
 */
template <>
device open_device<null> (open_params<null> const & op, error_code & ex);

}}

#endif /* __PFS_IO_NULL_HPP__ */

