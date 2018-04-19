/**
 * @file   bitset.hpp
 * @author wladt
 * @date   May 29, 2016
 *
 * @brief
 */

#ifndef __PFS_BITSET_HPP__
#define __PFS_BITSET_HPP__

#include <bitset>
#include <pfs.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/ostream.hpp>

// STL  std::bitset
// Qt   bitarray
// C#   bitarray
// Java BitSet

namespace pfs {

template <size_t N>
class bitset
{
	typedef std::bitset<N> rep_type;

public:
	typedef typename rep_type::reference reference;

    typedef pfs::shared_ptr<bitset> shared_type;
    
private:
    rep_type  _d;

public:
#if __cplusplus < 201103L
    bitset () 
        : _d() 
    {}
#else
    constexpr bitset ()
        : _d()
    {}
#endif

#if __cplusplus < 201103L
    bitset (long unsigned val) 
        : _d(val) 
    {}
#else
    constexpr bitset (long long unsigned val) 
        : _d(val) 
    {}
#endif

    bitset (bitset const & other)
        : _d(other._d)
    {}

    bitset & operator = (bitset const & other)
    {
        _d = other._d;
        return *this;
    }
    
    ~bitset () {}

#if __cplusplus < 201103L
    bool operator [] (size_t index) const
    {
        return _d[index];
    }
#else
    constexpr bool operator [] (size_t index) const
    {
        return _d[index];
    }
#endif
    
    reference operator [] (size_t index)
    {
        return _d[index];
    }
    
    
#if __cplusplus < 201103L
    size_t size () const 
    {
        return N;
    }
#else
    constexpr size_t size() const
    {
        return N;
    }
#endif    
        
    /**
     * Returns the number of bits that are set to @c true.
     */
    size_t count () const
    {
        return _d.count();
    }

    /**
     * Returns the number of bits that are set to @a val.
     */
    size_t count (bool val) const
    {
        return val ? _d.count() : _d.size() - _d.count();
    }
    
    bool test (size_t index) const
    {
        PFS_ASSERT(index < N);
        return _d.test(index);
    }
    
    bitset & set ()
    {
        _d.set();
        return *this;
    }
    
    bitset & set (size_t pos, bool val = true)
    {
        PFS_ASSERT(pos < N);
        _d.set(pos, val);
        return *this;
    }
    
    /**
     * @brief Sets every bit to false.
     */
    bitset & reset ()
    {
        _d.reset();
        return *this;
    }
    
    bitset & flip ()
    {
        _d.flip();
        return *this;
    }

    bitset & flip (size_t index)
    {
        PFS_ASSERT(index < N);
        _d.flip(index);
        return *this;
    }
};

template <size_t N>
bitset<N> make_bitset (byte_string const & s
        , byte_string::size_type pos = 0
        , byte_string::size_type n = byte_string::npos
        , byte_string::value_type zero = byte_string::value_type('0')
        , byte_string::value_type one = byte_string::value_type('1')
        , bool * ok = 0)
{
    bool okk = true;
    bitset<N> result;

    size_t nbits = pfs::min(n, size_t(s.size() - pos));
    nbits = pfs::min(N, nbits);

	for (size_t i = nbits; i > 0; --i) {
	    byte_string::value_type const c = s[pos + nbits - i];
        
	    if (c == zero)
            ;
	    else if (c == one)
            result.set(i - 1);
	    else {
            okk = false;
            break;
        }
	}

    if (ok)
        *ok = okk;
    
    return result;
}

template <size_t N>
bitset<N> make_bitset (byte_string const & s, bool * ok)
{
    return make_bitset<N>(s, 0, byte_string::npos
            , byte_string::value_type('0')
            , byte_string::value_type('1')
            , ok);
}

template <size_t N>
bitset<N> make_bitset (string::const_iterator begin
        , string::const_iterator end
        , string::value_type zero = string::value_type('0')
        , string::value_type one = string::value_type('1')
        , bool * ok = 0)
{
    bool okk = true;
    bitset<N> result;

    if (begin < end) {
        string::const_iterator it = end;
        --it;

        for (size_t i = 0; it >= begin; --it) {
            if (i == N)
                break;

            string::value_type const c = *it;

            if (c == zero)
                ;
            else if (c == one)
                result.set(i);
            else {
                okk = false;
                break;
            }
        }
    }

    if (ok)
        *ok = okk;
    
    return result;
}

template <size_t N>
bitset<N> make_bitset (string::const_iterator begin
        , string::const_iterator end
        , bool * ok)
{
    return make_bitset<N>(begin, end
            , string::value_type('0')
            , string::value_type('1')
            , ok);
}


#if __COMMENT__

class DLL_API bit_array : public nullable<bit_array_pdata>
{
protected:
	typedef nullable<bit_array_pdata> base_class;
	typedef bit_array self_class;
	typedef size_t    size_type;

public:
	bit_array () : base_class() {}
	bit_array (size_type size, bool value = false);

	bool isEmpty () const { return base_class::isNull() || size() == 0; }
	bool empty () const { return isEmpty(); }
	void clear ();

	size_type  count () const { return size(); }
	size_type  size () const;
	size_type  count (bool on) const;

	bool	at        (size_type i) const { return testBit(i); }
	void	clearBit  (size_type i);
	void	setBit    (size_type i);
	void	setBit    (size_type i, bool value);
	bool	testBit   (size_type i) const;
	bool	toggleBit (size_type i);

	void	fill (bool value) { fill(value, 0, size()); }
	void	fill (bool value, size_t size);
	void	fill (bool value, size_type begin, size_type end);
	void	resize (size_t size);
	void	truncate (size_t pos) { if (pos < size()) resize(pos); }

	bool	    operator [] (size_t i ) const { return testBit(i); }
	bool	    operator != (const bit_array & other) const;
	bool	    operator == (const bit_array & other) const;
	bit_array & operator &= (const bit_array & other);
	bit_array & operator ^= (const bit_array & other);
	bit_array & operator |= (const bit_array & other);
	bit_array   operator ~  () const;

	friend DLL_API bit_array operator & (const bit_array & a1, const bit_array & a2);
	friend DLL_API bit_array operator ^ (const bit_array & a1, const bit_array & a2);
	friend DLL_API bit_array operator | (const bit_array & a1, const bit_array & a2);
};

#endif

} // pfs

//#ifdef PFS_CC_MSVC
//#	pragma warning(pop)
//#endif

#endif /* __PFS_BITSET_HPP__ */
