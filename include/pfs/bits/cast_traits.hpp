/**
 * @file cast_trait.hpp
 * @author wladt
 * @date Jul 2, 2014
 */

#ifndef __PFS_CAST_TRAITS_HPP__
#define __PFS_CAST_TRAITS_HPP__

namespace pfs {

template <typename R, typename T> R cast_trait (const T &) { PFS_ASSERT_X(false, "Can't cast"); return R(); }

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
template <> inline bool cast_trait<bool, long double> (const long double & v) { return v ? true : false; }
#endif

template <> inline char cast_trait<char, bool> (const bool & v)                     { return static_cast<char>(v ? 1 : 0); }
template <> inline char cast_trait<char, char> (const char & v)                     { return static_cast<char>(v); }
template <> inline char cast_trait<char, unsigned char> (const unsigned char & v)   { return static_cast<char>(v); }
template <> inline char cast_trait<char, short> (const short & v)                   { return static_cast<char>(v); }
template <> inline char cast_trait<char, unsigned short> (const unsigned short & v) { return static_cast<char>(v); }
template <> inline char cast_trait<char, int> (const int & v)                       { return static_cast<char>(v); }
template <> inline char cast_trait<char, unsigned int> (const unsigned int & v)     { return static_cast<char>(v); }
template <> inline char cast_trait<char, long> (const long & v)                     { return static_cast<char>(v); }
template <> inline char cast_trait<char, unsigned long> (const unsigned long & v)   { return static_cast<char>(v); }
template <> inline char cast_trait<char, float> (const float & v)                   { return static_cast<char>(v); }
template <> inline char cast_trait<char, double> (const double & v)                 { return static_cast<char>(v); }
#ifdef HAVE_LONGLONG
template <> inline char cast_trait<char, long long> (const long long & v)           { return static_cast<char>(v); }
template <> inline char cast_trait<char, unsigned long long> (const unsigned long long & v) { return static_cast<char>(v); }
template <> inline char cast_trait<char, long double> (const long double & v)       { return static_cast<char>(v); }
#endif

template <> inline unsigned char cast_trait<unsigned char, bool> (const bool & v)                     { return static_cast<unsigned char>(v ? 1 : 0); }
template <> inline unsigned char cast_trait<unsigned char, char> (const char & v)                     { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, unsigned char> (const unsigned char & v)   { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, short> (const short & v)                   { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, unsigned short> (const unsigned short & v) { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, int> (const int & v)                       { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, unsigned int> (const unsigned int & v)     { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, long> (const long & v)                     { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, unsigned long> (const unsigned long & v)   { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, float> (const float & v)                   { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, double> (const double & v)                 { return static_cast<unsigned char>(v); }
#ifdef HAVE_LONGLONG
template <> inline unsigned char cast_trait<unsigned char, long long> (const long long & v)           { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, unsigned long long> (const unsigned long long & v) { return static_cast<unsigned char>(v); }
template <> inline unsigned char cast_trait<unsigned char, long double> (const long double & v) { return static_cast<unsigned char>(v); }
#endif

template <> inline short cast_trait<short, bool> (const bool & v)                     { return static_cast<short>(v ? 1 : 0); }
template <> inline short cast_trait<short, char> (const char & v)                     { return static_cast<short>(v); }
template <> inline short cast_trait<short, unsigned char> (const unsigned char & v)   { return static_cast<short>(v); }
template <> inline short cast_trait<short, short> (const short & v)                   { return static_cast<short>(v); }
template <> inline short cast_trait<short, unsigned short> (const unsigned short & v) { return static_cast<short>(v); }
template <> inline short cast_trait<short, int> (const int & v)                       { return static_cast<short>(v); }
template <> inline short cast_trait<short, unsigned int> (const unsigned int & v)     { return static_cast<short>(v); }
template <> inline short cast_trait<short, long> (const long & v)                     { return static_cast<short>(v); }
template <> inline short cast_trait<short, unsigned long> (const unsigned long & v)   { return static_cast<short>(v); }
template <> inline short cast_trait<short, float> (const float & v)                   { return static_cast<short>(v); }
template <> inline short cast_trait<short, double> (const double & v)                 { return static_cast<short>(v); }
#ifdef HAVE_LONGLONG
template <> inline short cast_trait<short, long long> (const long long & v)           { return static_cast<short>(v); }
template <> inline short cast_trait<short, unsigned long long> (const unsigned long long & v) { return static_cast<short>(v); }
template <> inline short cast_trait<short, long double> (const long double & v)       { return static_cast<short>(v); }
#endif

template <> inline unsigned short cast_trait<unsigned short, bool> (const bool & v)                     { return static_cast<unsigned short>(v ? 1 : 0); }
template <> inline unsigned short cast_trait<unsigned short, char> (const char & v)                     { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, unsigned char> (const unsigned char & v)   { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, short> (const short & v)                   { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, unsigned short> (const unsigned short & v) { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, int> (const int & v)                       { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, unsigned int> (const unsigned int & v)     { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, long> (const long & v)                     { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, unsigned long> (const unsigned long & v)   { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, float> (const float & v)                   { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, double> (const double & v)                 { return static_cast<unsigned short>(v); }
#ifdef HAVE_LONGLONG
template <> inline unsigned short cast_trait<unsigned short, long long> (const long long & v)           { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, unsigned long long> (const unsigned long long & v) { return static_cast<unsigned short>(v); }
template <> inline unsigned short cast_trait<unsigned short, long double> (const long double & v)       { return static_cast<unsigned short>(v); }
#endif

template <> inline int cast_trait<int, bool> (const bool & v)                     { return static_cast<int>(v ? 1 : 0); }
template <> inline int cast_trait<int, char> (const char & v)                     { return static_cast<int>(v); }
template <> inline int cast_trait<int, unsigned char> (const unsigned char & v)   { return static_cast<int>(v); }
template <> inline int cast_trait<int, short> (const short & v)                   { return static_cast<int>(v); }
template <> inline int cast_trait<int, unsigned short> (const unsigned short & v) { return static_cast<int>(v); }
template <> inline int cast_trait<int, int> (const int & v)                       { return static_cast<int>(v); }
template <> inline int cast_trait<int, unsigned int> (const unsigned int & v)     { return static_cast<int>(v); }
template <> inline int cast_trait<int, long> (const long & v)                     { return static_cast<int>(v); }
template <> inline int cast_trait<int, unsigned long> (const unsigned long & v)   { return static_cast<int>(v); }
template <> inline int cast_trait<int, float> (const float & v)                   { return static_cast<int>(v); }
template <> inline int cast_trait<int, double> (const double & v)                 { return static_cast<int>(v); }
#ifdef HAVE_LONGLONG
template <> inline int cast_trait<int, long long> (const long long & v)           { return static_cast<int>(v); }
template <> inline int cast_trait<int, unsigned long long> (const unsigned long long & v) { return static_cast<int>(v); }
template <> inline int cast_trait<int, long double> (const long double & v)       { return static_cast<int>(v); }
#endif

template <> inline unsigned int cast_trait<unsigned int, bool> (const bool & v)                     { return static_cast<unsigned int>(v ? 1 : 0); }
template <> inline unsigned int cast_trait<unsigned int, char> (const char & v)                     { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, unsigned char> (const unsigned char & v)   { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, short> (const short & v)                   { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, unsigned short> (const unsigned short & v) { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, int> (const int & v)                       { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, unsigned int> (const unsigned int & v)     { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, long> (const long & v)                     { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, unsigned long> (const unsigned long & v)   { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, float> (const float & v)                   { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, double> (const double & v)                 { return static_cast<unsigned int>(v); }
#ifdef HAVE_LONGLONG
template <> inline unsigned int cast_trait<unsigned int, long long> (const long long & v)           { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, unsigned long long> (const unsigned long long & v) { return static_cast<unsigned int>(v); }
template <> inline unsigned int cast_trait<unsigned int, long double> (const long double & v)        { return static_cast<unsigned int>(v); }
#endif

template <> inline long cast_trait<long, bool> (const bool & v)                     { return static_cast<long>(v ? 1 : 0); }
template <> inline long cast_trait<long, char> (const char & v)                     { return static_cast<long>(v); }
template <> inline long cast_trait<long, unsigned char> (const unsigned char & v)   { return static_cast<long>(v); }
template <> inline long cast_trait<long, short> (const short & v)                   { return static_cast<long>(v); }
template <> inline long cast_trait<long, unsigned short> (const unsigned short & v) { return static_cast<long>(v); }
template <> inline long cast_trait<long, int> (const int & v)                       { return static_cast<long>(v); }
template <> inline long cast_trait<long, unsigned int> (const unsigned int & v)     { return static_cast<long>(v); }
template <> inline long cast_trait<long, long> (const long & v)                     { return static_cast<long>(v); }
template <> inline long cast_trait<long, unsigned long> (const unsigned long & v)   { return static_cast<long>(v); }
template <> inline long cast_trait<long, float> (const float & v)                   { return static_cast<long>(v); }
template <> inline long cast_trait<long, double> (const double & v)                 { return static_cast<long>(v); }
#ifdef HAVE_LONGLONG
template <> inline long cast_trait<long, long long> (const long long & v)           { return static_cast<long>(v); }
template <> inline long cast_trait<long, unsigned long long> (const unsigned long long & v) { return static_cast<long>(v); }
template <> inline long cast_trait<long, long double> (const long double & v)       { return static_cast<long>(v); }
#endif


template <> inline float cast_trait<float, bool> (const bool & v)                      { return static_cast<float>(v ? 1 : 0); }
template <> inline float cast_trait<float, char> (const char & v)                      { return static_cast<float>(v); }
template <> inline float cast_trait<float, unsigned char> (const unsigned char & v)    { return static_cast<float>(v); }
template <> inline float cast_trait<float, short> (const short & v)                    { return static_cast<float>(v); }
template <> inline float cast_trait<float, unsigned short> (const unsigned short & v)  { return static_cast<float>(v); }
template <> inline float cast_trait<float, int> (const int & v)                        { return static_cast<float>(v); }
template <> inline float cast_trait<float, unsigned int> (const unsigned int & v)      { return static_cast<float>(v); }
template <> inline float cast_trait<float, long> (const long & v)                      { return static_cast<float>(v); }
template <> inline float cast_trait<float, unsigned long> (const unsigned long & v)    { return static_cast<float>(v); }
template <> inline float cast_trait<float, float> (const float & v)                    { return static_cast<float>(v); }
template <> inline float cast_trait<float, double> (const double & v)                  { return static_cast<float>(v); }
#ifdef HAVE_LONGLONG
template <> inline float cast_trait<float, long long> (const long long & v)            { return static_cast<float>(v); }
template <> inline float cast_trait<float, unsigned long long> (const unsigned long long & v) { return static_cast<float>(v); }
template <> inline float cast_trait<float, long double> (const long double & v)        { return static_cast<float>(v); }
#endif

template <> inline double cast_trait<double, bool> (const bool & v)                     { return static_cast<double>(v ? 1 : 0); }
template <> inline double cast_trait<double, char> (const char & v)                     { return static_cast<double>(v); }
template <> inline double cast_trait<double, unsigned char> (const unsigned char & v)   { return static_cast<double>(v); }
template <> inline double cast_trait<double, short> (const short & v)                   { return static_cast<double>(v); }
template <> inline double cast_trait<double, unsigned short> (const unsigned short & v) { return static_cast<double>(v); }
template <> inline double cast_trait<double, int> (const int & v)                       { return static_cast<double>(v); }
template <> inline double cast_trait<double, unsigned int> (const unsigned int & v)     { return static_cast<double>(v); }
template <> inline double cast_trait<double, long> (const long & v)                     { return static_cast<double>(v); }
template <> inline double cast_trait<double, unsigned long> (const unsigned long & v)   { return static_cast<double>(v); }
template <> inline double cast_trait<double, float> (const float & v)                   { return static_cast<double>(v); }
template <> inline double cast_trait<double, double> (const double & v)                 { return static_cast<double>(v); }
#ifdef HAVE_LONGLONG
template <> inline double cast_trait<double, long long> (const long long & v)           { return static_cast<double>(v); }
template <> inline double cast_trait<double, unsigned long long> (const unsigned long long & v) { return static_cast<double>(v); }
template <> inline double cast_trait<double, long double> (const long double & v)       { return static_cast<double>(v); }
#endif


#ifdef HAVE_LONGLONG
template <> inline long long cast_trait<long long, bool> (const bool & v)                     { return static_cast<long long>(v ? 1 : 0); }
template <> inline long long cast_trait<long long, char> (const char & v)                     { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, unsigned char> (const unsigned char & v)   { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, short> (const short & v)                   { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, unsigned short> (const unsigned short & v) { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, int> (const int & v)                       { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, unsigned int> (const unsigned int & v)     { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, long> (const long & v)                     { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, unsigned long> (const unsigned long & v)   { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, float> (const float & v)                   { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, double> (const double & v)                 { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, long long> (const long long & v)           { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, unsigned long long> (const unsigned long long & v) { return static_cast<long long>(v); }
template <> inline long long cast_trait<long long, long double> (const long double & v)       { return static_cast<long long>(v); }

template <> inline unsigned long long cast_trait<unsigned long long, bool> (const bool & v)                     { return static_cast<unsigned long long>(v ? 1 : 0); }
template <> inline unsigned long long cast_trait<unsigned long long, char> (const char & v)                     { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, unsigned char> (const unsigned char & v)   { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, short> (const short & v)                   { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, unsigned short> (const unsigned short & v) { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, int> (const int & v)                       { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, unsigned int> (const unsigned int & v)     { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, long> (const long & v)                     { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, unsigned long> (const unsigned long & v)   { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, float> (const float & v)                   { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, double> (const double & v)                 { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, long long> (const long long & v)           { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, unsigned long long> (const unsigned long long & v) { return static_cast<unsigned long long>(v); }
template <> inline unsigned long long cast_trait<unsigned long long, long double> (const long double & v)       { return static_cast<unsigned long long>(v); }

template <> inline long double cast_trait<long double, bool> (const bool & v)                     { return static_cast<long double>(v ? 1 : 0); }
template <> inline long double cast_trait<long double, char> (const char & v)                     { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, unsigned char> (const unsigned char & v)   { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, short> (const short & v)                   { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, unsigned short> (const unsigned short & v) { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, int> (const int & v)                       { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, unsigned int> (const unsigned int & v)     { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, long> (const long & v)                     { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, unsigned long> (const unsigned long & v)   { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, float> (const float & v)                   { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, double> (const double & v)                 { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, long long> (const long long & v)           { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, unsigned long long> (const unsigned long long & v) { return static_cast<long double>(v); }
template <> inline long double cast_trait<long double, long double> (const long double & v)       { return static_cast<long double>(v); }

#endif

}

#endif /* __PFS_CAST_TRAITS_HPP__ */
