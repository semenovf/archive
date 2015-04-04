// alignof library test suite

// Copyright (C) 2003 Martin Buchholz
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
//

// ----------------------------------------------------------------
// This code is known to work with the following compilers:
// Linux x86 g++ 2.95
// Linux x86 g++ 3.2.1
// Linux x86 g++ 3.3 20030122 pre-release
// Linux x86 g++ 3.4 20030122 pre-release
// Linux x86 Intel compiler Version 7.0
// Windows x86 MS Visual C++ .NET Version 13.00

// To work with MS Visual C++, we avoid partial template specialization.
// ----------------------------------------------------------------

#include <typeinfo>
#include <pfs/test.hpp>
#include <pfs/string.hpp>
#include <pfs/bits/alignof.h>
#include <iostream>

template <typename T>
struct TypeInfo
{
	static std::string name ()
	{
		return std::string (typeid (T).name());
	}
};

class DI { DI () {} double d; int i; };
class DS { DS () {} double d; };
class X { X () {} int i; char c; };

static void check_native_alignof_helper (bool predicate
    , const char * typestr
    , size_t sz1
    , size_t sz2)
{
    const char * cmpstr = predicate ? "==" : "!=";
    pfs::string s;

    s << "ALIGNOF(" << typestr << ") "
	<< cmpstr << " alignof(" << typestr << ')'
	<< " => " << pfs::string::toString(sz1) 
	<< ' ' << cmpstr << ' ' << pfs::string::toString(sz2);

    TEST_OK2(predicate, s.c_str());
}

template <typename T>
inline void check_native_alignof (const char * typestr)
{
    check_native_alignof_helper(ALIGNOF(T) == alignof(T)
	, typestr
	, ALIGNOF(T)
	, alignof(T));
}


int main (int argc, char *argv[])
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);
	int ntests = 24;

#ifdef PFS_HAVE_LONGLONG
	ntests += 2;
#endif

#ifdef PFS_HAVE_LONG_DOUBLE
	ntests += 2;
#endif

#ifdef PFS_HAVE_ALIGNOF
	ntests += 12;
#	ifdef PFS_HAVE_LONGLONG
	ntests += 1;
#	endif

#	ifdef PFS_HAVE_LONG_DOUBLE
	ntests += 1;
#	endif
#endif

	BEGIN_TESTS(ntests);

//  using Alignment::POD::types;
//  using Alignment::POD::TYPES_COUNT;

#define CHECK_ALIGNOF_LE_SIZEOF(type) \
		TEST_OK(ALIGNOF(type) <= sizeof (type))

	CHECK_ALIGNOF_LE_SIZEOF (char);
	CHECK_ALIGNOF_LE_SIZEOF (short);
	CHECK_ALIGNOF_LE_SIZEOF (int);
	CHECK_ALIGNOF_LE_SIZEOF (long);
#ifdef PFS_HAVE_LONGLONG
	CHECK_ALIGNOF_LE_SIZEOF (long long);
#endif
	CHECK_ALIGNOF_LE_SIZEOF (float);
	CHECK_ALIGNOF_LE_SIZEOF (double);
#ifdef PFS_HAVE_LONG_DOUBLE
	CHECK_ALIGNOF_LE_SIZEOF (long double);
#endif
	CHECK_ALIGNOF_LE_SIZEOF (void *);
	CHECK_ALIGNOF_LE_SIZEOF (void (*)());
	CHECK_ALIGNOF_LE_SIZEOF (X (X::*)());
	CHECK_ALIGNOF_LE_SIZEOF (DI);
	CHECK_ALIGNOF_LE_SIZEOF (DS);
	CHECK_ALIGNOF_LE_SIZEOF (X);

#define CHECK_ALIGNOF_DIVIDES_SIZEOF(type) \
		TEST_OK((sizeof (type) % ALIGNOF (type)) == 0)

	CHECK_ALIGNOF_DIVIDES_SIZEOF (char);
	CHECK_ALIGNOF_DIVIDES_SIZEOF (short);
	CHECK_ALIGNOF_DIVIDES_SIZEOF (int);
	CHECK_ALIGNOF_DIVIDES_SIZEOF (long);
#ifdef PFS_HAVE_LONGLONG
	CHECK_ALIGNOF_DIVIDES_SIZEOF (long long);
#endif
	CHECK_ALIGNOF_DIVIDES_SIZEOF (float);
	CHECK_ALIGNOF_DIVIDES_SIZEOF (double);
#ifdef PFS_HAVE_LONG_DOUBLE
	CHECK_ALIGNOF_DIVIDES_SIZEOF (long double);
#endif
	CHECK_ALIGNOF_DIVIDES_SIZEOF (void *);
	CHECK_ALIGNOF_DIVIDES_SIZEOF (void (*)());
	CHECK_ALIGNOF_DIVIDES_SIZEOF (X (X::*)());
	CHECK_ALIGNOF_DIVIDES_SIZEOF (DI);
	CHECK_ALIGNOF_DIVIDES_SIZEOF (DS);
	CHECK_ALIGNOF_DIVIDES_SIZEOF (X);

#ifdef PFS_HAVE_ALIGNOF // have native implementation (gnuc's or C++11)
#define CHECK_NATIVE_ALIGNOF(type) check_native_alignof<type>(#type)

	CHECK_NATIVE_ALIGNOF(char);
	CHECK_NATIVE_ALIGNOF(short);
	CHECK_NATIVE_ALIGNOF(int);
	CHECK_NATIVE_ALIGNOF(long);
#ifdef PFS_HAVE_LONGLONG
	CHECK_NATIVE_ALIGNOF(long long);
#endif
	CHECK_NATIVE_ALIGNOF(float);
	CHECK_NATIVE_ALIGNOF(double);
#ifdef PFS_HAVE_LONG_DOUBLE
	CHECK_NATIVE_ALIGNOF(long double);
#endif
	CHECK_NATIVE_ALIGNOF(void *);
	CHECK_NATIVE_ALIGNOF(void (*)());
	CHECK_NATIVE_ALIGNOF(X (X::*)());
	CHECK_NATIVE_ALIGNOF(DI);
	CHECK_NATIVE_ALIGNOF(DS);
	CHECK_NATIVE_ALIGNOF(X);
#endif

#define PRINT_ALIGN_POD(type) \
		std::cout << "ALIGN_POD_TYPE (" #type ") ==> " \
		<< TypeInfo<ALIGN_POD_TYPE(type)>::name() << std::endl
	PRINT_ALIGN_POD (char);
	PRINT_ALIGN_POD (short);
	PRINT_ALIGN_POD (int);
	PRINT_ALIGN_POD (long);
#ifdef PFS_HAVE_LONGLONG
	PRINT_ALIGN_POD (long long);
#endif
	PRINT_ALIGN_POD (float);
	PRINT_ALIGN_POD (double);
#ifdef PFS_HAVE_LONG_DOUBLE
	PRINT_ALIGN_POD (long double);
#endif
	PRINT_ALIGN_POD (char *);
	PRINT_ALIGN_POD (long *);
	PRINT_ALIGN_POD (void *);
	PRINT_ALIGN_POD (void (*)());
	PRINT_ALIGN_POD (DI);
	PRINT_ALIGN_POD (DS);
	PRINT_ALIGN_POD (X);

  //   cout << TypeInfo<types< 0>::Type>::name() << endl;
  //   cout << TypeInfo<types<10>::Type>::name() << endl;
  //   cout << TypeInfo<types<20>::Type>::name() << endl;
  //   cout << TypeInfo<types< 1>::Type>::name() << endl;
  //   cout << TypeInfo<types<11>::Type>::name() << endl;
  //   cout << TypeInfo<types<21>::Type>::name() << endl;
  //   cout << TypeInfo<types< 9>::Type>::name() << endl;
  //   cout << TypeInfo<types<19>::Type>::name() << endl;
  //   cout << TypeInfo<types<29>::Type>::name() << endl;

  //   using Alignment::POD::equally_aligned;
  //   cout << equally_aligned<char,int>::value << endl;
  //   cout << equally_aligned<void*,int>::value << endl;

//  check_types<0>("char");
//  check_types<1>("short");
//  check_types<2>("int");
//  check_types<3>("long");
//  check_types<4>("float");
//  check_types<5>("double");
//  check_types<6>("long double");

//#ifdef __INTEL_COMPILER
//  assert (typeid(types<0>            ::Type).name() == string ("c"));
//  assert (typeid(types<0+TYPES_COUNT>::Type).name() == string ("Pc"));
//  assert (typeid(types<1>            ::Type).name() == string ("s"));
//  assert (typeid(types<1+TYPES_COUNT>::Type).name() == string ("Ps"));
//#endif

  END_TESTS;
}
