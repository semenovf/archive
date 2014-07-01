/**
 * @file unitype.hpp
 * @author wladt
 * @date Jul 1, 2014
 */

#ifndef __PFS_DETAIL_UNITYPE_HPP__
#define __PFS_DETAIL_UNITYPE_HPP__

#include "../pimpl.hpp"

namespace pfs { namespace unitype1 {

//template <typename T> bool cast_boolean_trait (const T & v);

template <typename R, typename T> R cast_trait (const T & v);
template <> inline bool cast_trait<bool, bool> (const bool & v)	{ return v; }
template <> inline bool cast_trait<bool, char> (const char & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, unsigned char> (const unsigned char & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, short> (const short & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, unsigned short> (const unsigned short & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, int> (const int & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, unsigned int> (const unsigned int & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, long> (const long & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, unsigned long> (const unsigned long & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, float> (const float & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, double> (const double & v) { return v ? true : false; }
#ifdef HAVE_LONGLONG
template <> inline bool cast_trait<bool, long long> (const long long & v) { return v ? true : false; }
template <> inline bool cast_trait<bool, unsigned long long> (const unsigned long long & v) { return v ? true : false; }
#endif


template <> inline int cast_trait<int, bool> (const bool & v) { return v ? int(1) : int(0); }
template <> inline int cast_trait<int, char> (const char & v) { return int(v); }
template <> inline int cast_trait<int, unsigned char> (const unsigned char & v) { return int(v); }
template <> inline int cast_trait<int, short> (const short & v) { return int(v); }
template <> inline int cast_trait<int, unsigned short> (const unsigned short & v) { return int(v); }
template <> inline int cast_trait<int, int> (const int & v) { return int(v); }
template <> inline int cast_trait<int, unsigned int> (const unsigned int & v) { return int(v); }
template <> inline int cast_trait<int, long> (const long & v) { return int(v); }
template <> inline int cast_trait<int, unsigned long> (const unsigned long & v) { return int(v); }
template <> inline int cast_trait<int, float> (const float & v) { return int(v); }
template <> inline int cast_trait<int, double> (const double & v) { return int(v); }
#ifdef HAVE_LONGLONG
template <> inline int cast_trait<int, long long> (const long long & v) { return int(v); }
template <> inline int cast_trait<int, unsigned long long> (const unsigned long long & v) { return int(v); }
#endif

template <> inline long cast_trait<long, bool> (const bool & v) { return v ? long(1) : long(0); }
template <> inline long cast_trait<long, char> (const char & v) { return long(v); }
template <> inline long cast_trait<long, unsigned char> (const unsigned char & v) { return long(v); }
template <> inline long cast_trait<long, short> (const short & v) { return long(v); }
template <> inline long cast_trait<long, unsigned short> (const unsigned short & v) { return long(v); }
template <> inline long cast_trait<long, int> (const int & v) { return long(v); }
template <> inline long cast_trait<long, unsigned int> (const unsigned int & v) { return long(v); }
template <> inline long cast_trait<long, long> (const long & v) { return long(v); }
template <> inline long cast_trait<long, unsigned long> (const unsigned long & v) { return long(v); }
template <> inline long cast_trait<long, float> (const float & v) { return long(v); }
template <> inline long cast_trait<long, double> (const double & v) { return long(v); }
#ifdef HAVE_LONGLONG
template <> inline long cast_trait<long, long long> (const long long & v) { return long(v); }
template <> inline long cast_trait<long, unsigned long long> (const unsigned long long & v) { return long(v); }
#endif

#ifdef HAVE_LONGLONG
template <> inline long long cast_trait<long long, bool> (const bool & v) { return v ? (long long)1 : (long long)0; }
template <> inline long long cast_trait<long long, char> (const char & v) { return (long long)v; }
template <> inline long long cast_trait<long long, unsigned char> (const unsigned char & v) { return (long long)v; }
template <> inline long long cast_trait<long long, short> (const short & v) { return (long long)v; }
template <> inline long long cast_trait<long long, unsigned short> (const unsigned short & v) { return (long long)v; }
template <> inline long long cast_trait<long long, int> (const int & v) { return (long long)v; }
template <> inline long long cast_trait<long long, unsigned int> (const unsigned int & v) { return (long long)v; }
template <> inline long long cast_trait<long long, long> (const long & v) { return (long long)v; }
template <> inline long long cast_trait<long long, unsigned long> (const unsigned long & v) { return (long long)v; }
template <> inline long long cast_trait<long long, float> (const float & v) { return (long long)v; }
template <> inline long long cast_trait<long long, double> (const double & v) { return (long long)v; }
template <> inline long long cast_trait<long long, long long> (const long long & v) { return (long long)v; }
template <> inline long long cast_trait<long long, unsigned long long> (const unsigned long long & v) { return (long long)v; }
#endif

template <> inline float cast_trait<float, bool> (const bool & v) { return v ? float(1) : float(0); }
template <> inline float cast_trait<float, char> (const char & v) { return float(v); }
template <> inline float cast_trait<float, unsigned char> (const unsigned char & v) { return float(v); }
template <> inline float cast_trait<float, short> (const short & v) { return float(v); }
template <> inline float cast_trait<float, unsigned short> (const unsigned short & v) { return float(v); }
template <> inline float cast_trait<float, int> (const int & v) { return float(v); }
template <> inline float cast_trait<float, unsigned int> (const unsigned int & v) { return float(v); }
template <> inline float cast_trait<float, long> (const long & v) { return float(v); }
template <> inline float cast_trait<float, unsigned long> (const unsigned long & v) { return float(v); }
template <> inline float cast_trait<float, float> (const float & v) { return float(v); }
template <> inline float cast_trait<float, double> (const double & v) { return float(v); }
#ifdef HAVE_LONGLONG
template <> inline float cast_trait<float, long long> (const long long & v) { return float(v); }
template <> inline float cast_trait<float, unsigned long long> (const unsigned long long & v) { return float(v); }
#endif

template <> inline double cast_trait<double, bool> (const bool & v) { return v ? double(1) : double(0); }
template <> inline double cast_trait<double, char> (const char & v) { return double(v); }
template <> inline double cast_trait<double, unsigned char> (const unsigned char & v) { return double(v); }
template <> inline double cast_trait<double, short> (const short & v) { return double(v); }
template <> inline double cast_trait<double, unsigned short> (const unsigned short & v) { return double(v); }
template <> inline double cast_trait<double, int> (const int & v) { return double(v); }
template <> inline double cast_trait<double, unsigned int> (const unsigned int & v) { return double(v); }
template <> inline double cast_trait<double, long> (const long & v) { return double(v); }
template <> inline double cast_trait<double, unsigned long> (const unsigned long & v) { return double(v); }
template <> inline double cast_trait<double, float> (const float & v) { return double(v); }
template <> inline double cast_trait<double, double> (const double & v) { return double(v); }
#ifdef HAVE_LONGLONG
template <> inline double cast_trait<double, long long> (const long long & v) { return double(v); }
template <> inline double cast_trait<double, unsigned long long> (const unsigned long long & v) { return double(v); }
#endif

template <typename T>
class unitype
{
	pfs::pimpl _d;

public:
	unitype () : _d() {}
	unitype (const T & d) : _d(new T(d)) {}

	bool isNull () { return _d.isNull(); }

	template <typename R>
	R get () const { return cast_trait<R,T>(*_d.cast<T>()); }
};

template <typename T>
unitype<T> make_unitype (const T & x) { return unitype<T>(x); }

template <typename T>
unitype<T> make_unitype () { return unitype<T>(); }

}} // pfs::unitype1

#endif /* __PFS_DETAIL_UNITYPE_HPP__ */
