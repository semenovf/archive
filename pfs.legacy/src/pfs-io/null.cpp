/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <pfs/io/null.hpp>

namespace pfs { namespace io { namespace details {

struct null : public bits::device
{
    null () {}

    virtual error_code reopen ()
    {
    	return error_code();
    }

    virtual open_mode_flags open_mode () const
    {
    	return read_write | non_blocking;
    }

    virtual size_t  bytes_available () const
    {
    	 return 0;
    }

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex) override
    {
        PFS_UNUSED(ex);
        return 0;
    }

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex) override
    {
        PFS_UNUSED(ex);
        return integral_cast_check<ssize_t>(n);
    }

    virtual error_code close ()
    {
    	return error_code();
    }

    virtual bool opened () const
    {
    	return true;
    }

    virtual void flush ()
    { }

    virtual bool is_nonblocking() const
    {
        return true;
    }
    
    virtual bool set_nonblocking (bool on)
    {
    	PFS_UNUSED(on);
    	return true;
    }

    virtual native_handle_type native_handle () const
    {
    	return -1;
    }

    virtual device_type type () const
    {
    	return device_null;
    }
    
    virtual string url () const override
    {
        return _u8("null");
    }
};

}}} // pfs::io::details

namespace pfs { namespace io {

template <>
device open_device<null> (open_params<null> const & op, error_code & ex)
{
    PFS_UNUSED(op);
    ex.reset();
    return device(new details::null);
}

}} // pfs::io
