/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   atomic.hpp
 * Author: wladt
 *
 * Created on October 17, 2016, 9:13 AM
 */

#ifndef __PFS_CXX98_ATOMIC_HPP__
#define __PFS_CXX98_ATOMIC_HPP__

namespace pfs {

// TODO Implement using src/atomic.mt

#if __COMMENT__

template <typename T>
class atomic_integer
{
	typedef T Type;
public:
    typedef atomic_integer_intrinsics<T> intrinsics;
    typename intrinsics::Type _value;

    atomic_integer () {}
    atomic_integer (Type value) { store(value); }

    // Non-atomic API
    Type load () const 
    {
        return intrinsics::load(_value);
    }
    
    void store (Type newValue) 
    {
        intrinsics::store((Type &)_value, newValue); 
    }
    
    bool ref () 
    {
        return intrinsics::ref((Type &)_value); 
    }
    
    bool deref () 
    {
        return intrinsics::deref((Type &)_value);
    }

    Type fetchAndAddRelaxed (Type valueToAdd)
    {
    	return intrinsics::fetchAndAddRelaxed((Type &)_value, valueToAdd);
    }

    Type compareAndSwap (Type oldValue, Type newValue)
    {
    	return intrinsics::compareAndSwap(& _value, oldValue, newValue);
    }

//    void swap (atomic_integer<Type> & other)
//    {
//    	other.store(compareAndSwap(_value, other.load()));
//    }
};

typedef atomic_integer<int> atomic_int;

template <typename T>
class atomic_pointer
{
	typedef T * Type;
public:
    typedef atomic_integer_intrinsics<Type> intrinsics;
    typename intrinsics::Type _value;

    atomic_pointer () {}
    atomic_pointer (Type value) { store(value); }

    // Non-atomic API
    Type load  () const { return intrinsics::load(_value); }
    void store (Type newValue) { intrinsics::store((Type &)_value, newValue); }
    bool ref   () { return intrinsics::ref((Type &)_value); }
    bool deref () { return intrinsics::deref((Type &)_value); }

    Type fetchAndAddRelaxed(Type valueToAdd)
    {
    	return intrinsics::fetchAndAddRelaxed(_value, valueToAdd);
    }

    Type compareAndSwap (Type oldValue, Type newValue)
    {
    	return intrinsics::compareAndSwap(& _value, oldValue, newValue);
    }
};

#endif

}

#endif /* __PFS_CXX98_ATOMIC_HPP__ */

