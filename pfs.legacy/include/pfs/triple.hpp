/* 
 * File:   triple.hpp
 * Author: wladt
 *
 * Created on July 12, 2016, 3:57 PM
 */

#ifndef __PFS_TRIPLE_HPP__
#define __PFS_TRIPLE_HPP__

namespace pfs {

template <typename T1, typename T2, typename T3>
struct triple
{
    typedef T1 first_type;    /// @c first_type is the first bound type
    typedef T2 second_type;   /// @c second_type is the second bound type
    typedef T3 third_type;    /// @c third_type is the third bound type

    T1 first;                 /// @c first is a copy of the first object
    T2 second;                /// @c second is a copy of the second object
    T3 third;                 /// @c third is a copy of the third object
    

    triple ()
        : first(), second(), third()
    {}

    
    triple (T1 const & a, T2 const & b, T3 const & c)
        : first(a), second(b), third(c)
    {}

    // TODO Must replaced with code below
    //---------------------------------    
    //        template<class _U1, class _U2, class = typename
    //	       enable_if<__and_<is_convertible<const _U1&, _T1>,
    //				is_convertible<const _U2&, _T2>>::value>::type>
    //	constexpr pair(const pair<_U1, _U2>& __p)
    //	: first(__p.first), second(__p.second) { }

    template <typename U1, typename U2, typename U3>
	triple (triple<U1, U2, U3> const & t)
        : first(t.first), second(t.second), third(t.third)
    {}
    
#if __FIXME__
    template<class _U1, class _U2, class = typename
	       enable_if<__and_<is_convertible<const _U1&, _T1>,
				is_convertible<const _U2&, _T2>>::value>::type>
	constexpr pair(const pair<_U1, _U2>& __p)
	: first(__p.first), second(__p.second) { }

      constexpr pair(const pair&) = default;
      constexpr pair(pair&&) = default;

      // DR 811.
      template<class _U1, class = typename
	       enable_if<is_convertible<_U1, _T1>::value>::type>
	constexpr pair(_U1&& __x, const _T2& __y)
	: first(std::forward<_U1>(__x)), second(__y) { }

      template<class _U2, class = typename
	       enable_if<is_convertible<_U2, _T2>::value>::type>
	constexpr pair(const _T1& __x, _U2&& __y)
	: first(__x), second(std::forward<_U2>(__y)) { }

      template<class _U1, class _U2, class = typename
	       enable_if<__and_<is_convertible<_U1, _T1>,
				is_convertible<_U2, _T2>>::value>::type>
	constexpr pair(_U1&& __x, _U2&& __y)
	: first(std::forward<_U1>(__x)), second(std::forward<_U2>(__y)) { }

      template<class _U1, class _U2, class = typename
	       enable_if<__and_<is_convertible<_U1, _T1>,
				is_convertible<_U2, _T2>>::value>::type>
	constexpr pair(pair<_U1, _U2>&& __p)
	: first(std::forward<_U1>(__p.first)),
	  second(std::forward<_U2>(__p.second)) { }

      template<typename... _Args1, typename... _Args2>
        pair(piecewise_construct_t, tuple<_Args1...>, tuple<_Args2...>);

      pair&
      operator=(const pair& __p)
      {
	first = __p.first;
	second = __p.second;
	return *this;
      }

      pair&
      operator=(pair&& __p)
      noexcept(__and_<is_nothrow_move_assignable<_T1>,
	              is_nothrow_move_assignable<_T2>>::value)
      {
	first = std::forward<first_type>(__p.first);
	second = std::forward<second_type>(__p.second);
	return *this;
      }

      template<class _U1, class _U2>
	pair&
	operator=(const pair<_U1, _U2>& __p)
	{
	  first = __p.first;
	  second = __p.second;
	  return *this;
	}

      template<class _U1, class _U2>
	pair&
	operator=(pair<_U1, _U2>&& __p)
	{
	  first = std::forward<_U1>(__p.first);
	  second = std::forward<_U2>(__p.second);
	  return *this;
	}

      void
      swap(pair& __p)
      noexcept(noexcept(swap(first, __p.first))
	       && noexcept(swap(second, __p.second)))
      {
	using std::swap;
	swap(first, __p.first);
	swap(second, __p.second);
      }

    private:
      template<typename... _Args1, std::size_t... _Indexes1,
               typename... _Args2, std::size_t... _Indexes2>
        pair(tuple<_Args1...>&, tuple<_Args2...>&,
             _Index_tuple<_Indexes1...>, _Index_tuple<_Indexes2...>);
#endif
};

} // pfs

#endif /* __PFS_TRIPLE_HPP__ */

