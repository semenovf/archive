/*
 * aligned_storage.hpp
 *
 *  Created on: 05 апр. 2015 г.
 *      Author: wladt
 */

#ifndef __PFS_BITS_ALIGNED_STORAGE_HPP__
#define __PFS_BITS_ALIGNED_STORAGE_HPP__

#include <cstdlib>
#include <pfs/bits/typelist.hpp>
#include <pfs/bits/type_traits.hpp>

namespace pfs { namespace bits {

template <typename T> struct structify { T _dummy; };

class unknown_class;

typedef cons<
          char
        , short int
        , int
        , long int
        , float
        , double
        , char *
        , short int *
        , int *
        , long int*
        , float *
        , double *
        , void *
        , unknown_class (*)(unknown_class)
        , unknown_class* unknown_class::*
        , unknown_class (unknown_class::*)(unknown_class)
        , structify<char>
        , structify<short int>
        , structify<int>
        , structify<long int>
        , structify<float>
        , structify<double>
        , structify<char *>
        , structify<short int *>
        , structify<int *>
        , structify<long int *>
        , structify<float *>
        , structify<double *>
        , structify<void *>
        , structify<unknown_class (*)(unknown_class)>
        , structify<unknown_class* unknown_class::*>
        , structify<unknown_class (unknown_class::*)(unknown_class)>
#ifdef PFS_HAVE_LONGLONG
		, long long int
		, long long int *
		, structify<long long int>
		, structify<long long int *>
#endif
#ifdef PFS_HAVE_LONG_DOUBLE
		, long double
		, long double *
        , structify<long double>
		, structify<long double *>
#endif
>::type typelist_all; //TypesOfAllAlignments;


//inline size_t alignment_of_max_align ()
//{
//	return //      std::size_t
////    , alignment_of_max_align = ::boost::alignment_of<max_align>::value
////    );

//
// How can we guarantee that 'buffer' is properly aligned?
// Well, that's exactly what union does — it guarantees
// enough size for its largest member and alignment
// for its most restrictive member. This is what we need,
// so our storage should look something like this:
//
//union
//{
//    unsigned char buffer[neededSize];
//    Align dummy; // never used; it's there just for alignment purposes
//};
//

//template <class TList, size_t size> struct ComputeAlignBound;
//
//template <size_t size>
//struct ComputeAlignBound<null_typelist, size>
//{
//  typedef null_typelist type;
//};
//
//template <class Head, class Tail, size_t Size>
//struct ComputeAlignBound<typelist<Head, Tail>, Size>
//{
//  typedef typename ComputeAlignBound<Tail, Size>::type result;
//  typedef typename if_else<  // details below
//		  sizeof(Head) <= Size
//		  , typelist<Head, result>::type
//		  , result>::type type;
//};
//
//template <typename TypeList>
//class AlignedPOD
//{
//	enum { maxSize = typelist_max_size<TypeList>::result };
//	typedef ComputeAlignBound<typelist_all, maxSize>::type AlignTypes;
//public:
//	typedef ConfigurableUnion<AlignTypes> result;
//};
//
//template <size_t Len, size_t Align>
//struct aligned_storage {
//	union type {
//		unsigned char data[Len];
//		Align dummy;
//	};
//};


//template <size_t Size, size_t Alignment>
//struct aligned_storage
//{
//    union type
//    {
//    	unsigned char data[Size];
////        typename if_else<
////              Alignment == size_t(-1)
////            , ::boost::mpl::identity< ::boost::detail::max_align >
////            , ::boost::type_with_alignment<Alignment>
////            >::type align;
//    };
//};

//template< std::size_t Alignment>
//struct aligned_storage_impl<0u,Alignment>
//{
//    /* intentionally empty */
//    void * address() const { return 0; }
//};
//
//template <std::size_t Size, std::size_t Alignment = std::size_t(-1)>
//class aligned_storage :
//#ifndef __BORLANDC__
//   private
//#else
//   public
//#endif
//   aligned_storage_imp<Size, Alignment>
//{
//
//public: // constants
//
//    typedef aligned_storage_imp<Size, Alignment> type;
//
//    static const std::size_t size = Size;
//    static std::size_t aligment ()
//    {
//        return Alignment == std::size_t(-1)
//            ? alignment_of_max_align()
//            : Alignment;
//    }
//
//private: // noncopyable
//
//    aligned_storage(const aligned_storage&);
//    aligned_storage& operator=(const aligned_storage&);
//
//public: // structors
//    aligned_storage() {}
//    ~aligned_storage() {}
//
//public: // accessors
//
//    void* address()
//    {
//        return static_cast<type*>(this)->address();
//    }
//
//    const void * address() const
//    {
//        return static_cast<const type*>(this)->address();
//    }
//};

}} // pfs::bits

#endif /* __PFS_BITS_ALIGNED_STORAGE_HPP__ */
