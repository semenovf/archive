/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   active_queue.hpp
 * Author: wladt
 *
 * Created on September 22, 2016, 4:05 PM
 */

#ifndef __PFS_ACTIVE_QUEUE_1_HPP__
#define __PFS_ACTIVE_QUEUE_1_HPP__

#include <cstring> // for std::memcpy
#include <memory>  // for std::allocator
#include <pfs/atomic.hpp>
#include <pfs/binder.hpp>
#include <pfs/mutex.hpp>

#include <pfs/debug.hpp>

namespace pfs {

namespace details {

//
//    ---------------------------- 
//    | | | |x|x|x|x| | | | | | |
//    ---------------------------- 
//     ^     ^       ^           ^   
//     |     |       |           |
//  _begin _head   _tail     _capacity
//                             _end
//
//    ---------------------------- 
//    |y|y|y|y| | | | | |x|x|x|x|
//    ---------------------------- 
//     ^       ^         ^       ^
//     |       |         |       |
//  _begin   _tail    _head  _capacity
//                             _end
//
//    ---------------------------- 
//    |y|y|y|y| | | |x|x|x|x| | |
//    ---------------------------- 
//     ^       ^     ^       ^   ^   
//     |       |     |       |   |
//  _begin   _tail _head   _end _capacity
//

class ring_queue
{
public:
    static size_t const DefaultIncrementFactor = 10;

private:
	char * _begin;
	size_t _end;   // Logic end (not a real end of buffer)
	size_t _head;
	size_t _tail;
    size_t _count;
    size_t _capacity;
    size_t _max_capacity;
    size_t _increment_factor;
    
public:
	ring_queue (size_t initial = 0
            , size_t max_capacity = pfs::max_value<size_t>()
            , size_t increment_factor = DefaultIncrementFactor)
        : _begin(initial > 0 ? new char[initial] : 0)
		, _end(initial)
		, _head(0)
		, _tail(0)
		, _count(0)
        , _capacity(initial)
        , _max_capacity(max_capacity)
        , _increment_factor(increment_factor)
	{}


    bool empty () const
    {
        return _count == 0;
    }

    size_t capacity () const
    {
        return _capacity;
    }

    bool ensure_capacity (size_t nsize);

//    typename <typename T>
//    std::allocator<T> prepare_allocator ()
//    {
//        std::allocator<T> alloc;
//        string = alloc.allocate(1);
//        return alloc;
//    }
    
//      /**
//       *  Returns a read/write reference to the data at the first
//       *  element of the %queue.
//       */
//      reference
//      front()
//      {
//	__glibcxx_requires_nonempty();
//	return c.front();
//      }
//
//      /**
//       *  Returns a read-only (constant) reference to the data at the first
//       *  element of the %queue.
//       */
//      const_reference
//      front() const
//      {
//	__glibcxx_requires_nonempty();
//	return c.front();
//      }
//
//      /**
//       *  Returns a read/write reference to the data at the last
//       *  element of the %queue.
//       */
//      reference
//      back()
//      {
//	__glibcxx_requires_nonempty();
//	return c.back();
//      }
//
//      /**
//       *  Returns a read-only (constant) reference to the data at the last
//       *  element of the %queue.
//       */
//      const_reference
//      back() const
//      {
//	__glibcxx_requires_nonempty();
//	return c.back();
//      }
//
//
//
//      /**
//       *  @brief  Removes first element.
//       *
//       *  This is a typical %queue operation.  It shrinks the %queue by one.
//       *  The time complexity of the operation depends on the underlying
//       *  sequence.
//       *
//       *  Note that no data is returned, and if the first element's
//       *  data is needed, it should be retrieved before pop() is
//       *  called.
//       */
//      void
//      pop()
//      {
//	__glibcxx_requires_nonempty();
//	c.pop_front();
//      }
};

// TODO Move to .cpp file
//
inline bool ring_queue::ensure_capacity (size_t nsize)
{
    if (! _begin) {
        size_t capacity = _increment_factor * frsize;
        _begin = new char[capacity];
        _end = capacity;
        _head = 0;
        _tail = 0;
        _count.store(0);
        _capacity.store(capacity);
        
        return true;
    }
}

} // details


#if __COMMENT1__

template <typename Return, typename Mutex = pfs::fake_mutex>
class active_queue_base
{
public:
	typedef Return return_type;
	typedef binder_base<Return> binder_base_type;

protected:
    static size_t const DefaultIncrementFactor = 10;
    
protected:
	Mutex  _mutex;
	char * _begin;
	size_t _end;   // Logic end (not a real end of buffer)
	size_t _head;
	size_t _tail;
	atomic_integer<size_t> _count;
    atomic_integer<size_t> _capacity;
    atomic_integer<size_t> _max_capacity;
    size_t _increment_factor;
    
    PFS_DEBUG(bool _realloced);

protected:
	void pop (binder_base_type & fr);
    void pull (binder_base_type * & fr);
	bool prepare_push (size_t frsize);

	void reset ()
	{
		_head = 0;   // Move Head and Tail to the begin of queue
		_tail = 0;
		_end  = _capacity.load();
	}

public:
	active_queue_base (size_t initial = 0
            , size_t max_capacity = pfs::max_value<size_t>()
            , size_t increment_factor = DefaultIncrementFactor)
        : _begin(initial > 0 ? new char[initial] : 0)
		, _end(initial)
		, _head(0)
		, _tail(0)
		, _count(0)
        , _capacity(initial)
        , _max_capacity(max_capacity)
        , _increment_factor(increment_factor)
        , PFS_DEBUG(_realloced(false))
	{}

	virtual ~active_queue_base ()
	{
		while (_head != _tail)
			pop();
        delete [] _begin;
	}

	bool empty () const
	{
		return _count.load() == 0;
	}

	size_t count () const
	{
		return _count.load();
	}

	size_t capacity () const
	{
		return _capacity.load();
	}

	bool push_function (return_type (* f) ())
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_function0<return_type>))) {
			binder_base_type * fr = new (_begin + _tail) binder_function0<return_type>(f);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Arg1>
	bool push_function (return_type (*f) (Arg1), Arg1 a1)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_function1<return_type, Arg1>))) {
			binder_base_type * fr = new (_begin + _tail) binder_function1<return_type, Arg1>(f, a1);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	template <typename Arg1, typename Arg2>
	bool push_function (return_type (*f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		lock_guard<Mutex> locker(_mutex);

		if (prepare_push(sizeof(binder_function2<return_type, Arg1, Arg2>))) {
			binder_base_type * fr = new (_begin + _tail) binder_function2<return_type, Arg1, Arg2>(f, a1, a2);
			_tail += fr->size();
			_count.ref(); //++_count;
			return true;
		}
		return false;
	}

	void pop ();
};

template <typename Return, typename Mutex>
void active_queue_base<Return, Mutex>::pop ()
{
    lock_guard<Mutex> locker(_mutex);
	binder_base<Return> * fr = 0;

	pull(fr);

	if (fr)
		pop(*fr);
}

template <typename Return, typename Mutex>
bool active_queue_base<Return, Mutex>::prepare_push (size_t frsize)
{
    if (! _begin) {
        size_t capacity = _increment_factor * frsize;
        _begin = new char[capacity];
        _end = capacity;
        _head = 0;
        _tail = 0;
        _count.store(0);
        _capacity.store(capacity);
        
        PFS_DEBUG(
            std::cout << "INITIAL ALLOCATION: " << std::endl
                      << "\t_end=" << this->_end << std::endl
                      << "\t_head=" << this->_head << std::endl
                      << "\t_tail=" << this->_tail << std::endl
                      << "\t_count=" << this->_count.load() << std::endl
                      << "\t_capacity=" << this->_capacity.load() << std::endl
                      << "\t_max_capacity=" << this->_max_capacity.load() << std::endl
        );
        
        return true;
    }
    
    if (_tail == _head && empty()) {
        reset();   // Move Head and Tail to the begin of queue
    }

    if (_tail == _head && ! empty()) {      // Queue is full
        ;
    } if (_tail >= _head) {                 // Tail is at the right side of Head or Queue is empty (_tail == _head)
        size_t end  = _capacity.load();

        if (_tail + frsize <= end) {        // There is enough space before the real end of queue
            _end = end;                     // Logic End must be moved to real end of queue
            return true;
        } else {                            // There is no enough space before the real end of queue
            end = _tail;                    // Save Tail position

            if (frsize <= _head) {          // There is enough space before Head from Begin
                _tail = 0;                  // Move Tail to Begin
                _end = end;                 // Move Logic End
                return true;
            }
        }
    } else {                                // Tail is at the left side of Head
        if (_tail + frsize <= _head) {      // There is enough space before Head
            return true;
        }
    }

    //
	// There is no enough space to push function
    //
    
    size_t capacity = _capacity.load();
    size_t increment = _increment_factor * frsize;

    PFS_DEBUG(
        std::cout << "BEFORE RELOCATION: " << std::endl
                  << "\t_end=" << this->_end << std::endl
                  << "\t_head=" << this->_head << std::endl
                  << "\t_tail=" << this->_tail << std::endl
                  << "\t_count=" << this->_count.load() << std::endl
                  << "\t_capacity=" << this->_capacity.load() << std::endl
                  << "\t_max_capacity=" << this->_max_capacity.load() << std::endl
                  << "\tcapacity=" << capacity << std::endl
                  << "\tincrement=" << increment << std::endl;
    );

    // Check for overflow
    //
    if (_max_capacity.load() >= increment && _max_capacity.load() - increment > capacity) {
        capacity += increment;
        char * begin = new char[capacity];
        
        if (_tail >= _head) {
            //
            //          |---n---|                |---n---|
            //    -------------------------       -----------------------------
            //    | | | |x|x|x|x| | | | | | -->  |x|x|x|x| | | | | | | | | | |
            //    -------------------------       -----------------------------
            //     ^     ^       ^     ^          ^       ^                   ^
            //     |     |       |     |          |       |                   |
            //  _begin _head   _tail _end      _begin   _tail               _end
            //                                 _head
            //
            size_t n = _tail - _head;
            std::memcpy(begin, _begin + _head, n);
            _head = 0;
            _tail = n;
            _end  = capacity;
        } else {
#if __COMMENT__            
            //                        |-n3|                                |-n3|
            //    |n1-|         |---n2----|      |n1-|               |---n2----|
            //    -------------------------       ------------------------------
            //    |y|y| | | | | |x|x|x| | | -->  |y|y| | | | | | | | |x|x|x| | |
            //    -------------------------       ------------------------------
            //     ^   ^         ^     ^          ^   ^               ^     ^
            //     |   |         |     |          |   |               |     |
            // _begin _tail  _head  _end       _begin _tail         _head  _end
            //
            size_t n1 = _tail;
            size_t n2 = _capacity.load() - _head; // _end - _head;
            size_t n3 = _capacity.load() - _end;
            
            std::memcpy(begin, _begin, n1);
            std::memcpy(begin + capacity - n2, _begin + _head, n2);
            
            _head = capacity - n2;
            _tail = n1;
            _end  = capacity - n3;
#endif
            //
            //    |n1-|         |-n2--|          |-n2--|n1-|
            //    -------------------------       ------------------------------
            //    |y|y| | | | | |x|x|x| | | -->  |x|x|x|y|y| | | | | | | | | | 
            //    -------------------------       ------------------------------
            //     ^   ^         ^     ^          ^         ^                 ^
            //     |   |         |     |          |         |                 |
            // _begin _tail  _head  _end       _begin     _tail             _end
            //                                 _head
            //
            size_t n1 = _tail;
            size_t n2 = _end - _head;
            
            std::memcpy(begin, _begin + _head, n2);
            std::memcpy(begin + n2, _begin, n1);
            
            _head = 0;
            _tail = n2 + n1;
            _end  = capacity;
            
        }
        
        delete [] _begin;
        
      	_begin = begin;
        _capacity.store(capacity);
        
        PFS_DEBUG(
            std::cout << "AFTER RELOCATION: " << std::endl
                      << "\t_end=" << this->_end << std::endl
                      << "\t_head=" << this->_head << std::endl
                      << "\t_tail=" << this->_tail << std::endl
                      << "\t_count=" << this->_count.load() << std::endl
                      << "\t_capacity=" << this->_capacity.load() << std::endl
        );
        
        PFS_DEBUG(_realloced = true);

        return true;
    }
    
	return false;
}

template <typename Return, typename Mutex>
void active_queue_base<Return, Mutex>::pull (binder_base_type * & fr)
{
	if (!empty()) {
		if (_head == _end) { // Head exceeds Tail, but Queue is not empty
			_head = 0;
			_end = _capacity.load();
		}

		fr = reinterpret_cast<binder_base_type *>(_begin + _head);
	}
}

template <typename Return, typename Mutex>
void active_queue_base<Return, Mutex>::pop (binder_base_type & fr)
{
	_head += fr.size(); // Supposed Head position
	_count.deref();
	fr.~binder_base_type();
}

template <typename Return, typename Mutex = pfs::fake_mutex>
class active_queue : public active_queue_base<Return, Mutex>
{
public:
	typedef active_queue_base<Return, Mutex> base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::binder_base_type binder_base_type;

    /**
     * @brief Constructs instance of @c active_queue with the given traits. 
     * 
     * @param initial Initial capacity of internal buffer in bytes.
     * @param max_capacity Maximum capacity of internal buffer in bytes.
     * @param increment_factor Increment factor. Multiplier value for increment . 
     *        This value used to increment capacity of internal buffer while pushing
     *        new binder and no room to store it. Real increment value calculated
     *        from @a increment_factor multiplying by size of pushing binder.
     */
	active_queue (size_t initial = 0
            , size_t max_capacity = pfs::max_value<size_t>()
            , size_t increment_factor = base_class::DefaultIncrementFactor)
        : base_class(initial, max_capacity, increment_factor)
    {}

	return_type call ();
	return_type call_all ();
};

template <typename Mutex>
class active_queue<void, Mutex> : public active_queue_base<void, Mutex>
{
public:
	typedef active_queue_base<void, Mutex>   base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::binder_base_type binder_base_type;

	active_queue (size_t initial = 0
            , size_t max_capacity = pfs::max_value<size_t>()
            , size_t increment_factor = base_class::DefaultIncrementFactor)
        : base_class(initial, max_capacity, increment_factor) 
    {}

	return_type call ();
	return_type call_all ();
};

template <typename Return, typename Mutex>
typename active_queue<Return, Mutex>::return_type
	active_queue<Return, Mutex>::call ()
{
    PFS_ASSERT(!this->empty());
    
	return_type r;
	binder_base<Return> * fr = 0;

	unique_lock<Mutex> locker(this->_mutex);

	this->pull(fr);

	locker.unlock();

	// To avoid this assert need to check for empty of queue before this call
	PFS_ASSERT(fr);

	r = (*fr)();

	locker.lock();
	this->pop(*fr);
	locker.unlock();

	return r;
}

template <typename Return, typename Mutex>
typename active_queue<Return, Mutex>::return_type
	active_queue<Return, Mutex>::call_all ()
{
	return_type r;

	while (!this->empty())
		r = call();
	return r;
}

template <typename Mutex>
typename active_queue<void, Mutex>::return_type active_queue<void, Mutex>::call ()
{
    lock_guard<Mutex> locker(this->_mutex);
    PFS_ASSERT(!this->empty());
    
	binder_base<void> * fr = 0;
    
//	unique_lock<Mutex> locker(this->_mutex);
	this->pull(fr);

//	locker.unlock();

	// To avoid this assert need to check for empty of queue before this call
	PFS_ASSERT(fr);
    
//    if (fr->magic() != 0xDEADBEAF) {
        PFS_DEBUG(
            std::cout << "_end=" << this->_end << std::endl
                      << "_head=" << this->_head << std::endl
                      << "_tail=" << this->_tail << std::endl
                      << "_count=" << this->_count.load() << std::endl
                      << "_capacity=" << this->_capacity.load() << std::endl
                      << "_magic=" << std::hex << fr->magic() << std::dec << std::endl
                      << "_realloced=" << this->_realloced << std::endl
        );
//    }
        
    PFS_ASSERT(fr->magic() == 0xDEADBEAF);
    PFS_ASSERT(this->_end == this->_capacity.load());
	
    (*fr)();

//	locker.lock();
	this->pop(*fr);
//	locker.unlock();
}

template <typename Mutex>
inline typename active_queue<void, Mutex>::return_type active_queue<void, Mutex>::call_all ()
{
	while (!this->empty())
		call();
}

#endif

} // pfs

#endif /* __PFS_ACTIVE_QUEUE_HPP__ */

