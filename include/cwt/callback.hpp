/**
 * @file   callback.hpp
 * @author wladt
 * @date   Feb 18, 2013 9:47:05 AM
 *
 * @brief
 */

#ifndef __CWT_CALLBACK_HPP__
#define __CWT_CALLBACK_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

class Callback
{
	typedef bool (*callback_type)();
public:
	Callback(callback_type cb = NULL) : callback(cb ? cb : &default_cb) {}
	bool operator ()() { return callback(); }
private:
	callback_type callback;
	static bool default_cb() {return true;}
};

template <typename arg1>
class Callback1
{
	typedef bool (*callback_type)(arg1);
public:
	Callback1(callback_type cb = NULL) : callback(cb ? cb : &default_cb) {}
	bool operator ()(arg1 a1) { return callback(a1); }
private:
	callback_type callback;
	static bool default_cb(arg1) { return true;}
};

template <typename arg1, typename arg2>
class Callback2
{
	typedef bool (*callback_type)(arg1, arg2);
public:
	Callback2(callback_type cb = NULL) : callback(cb ? cb : &default_cb) {}
	bool operator ()(arg1 a1, arg2 a2) { return callback(a1, a2); }
private:
	callback_type callback;
	static bool default_cb(arg1, arg2) {return true;}
};

template <typename arg1, typename arg2, typename arg3>
class Callback3
{
	typedef bool (*callback_type)(arg1, arg2, arg3);
public:
	Callback3(callback_type cb = NULL) : callback(cb ? cb : &default_cb) {}
	bool operator ()(arg1 a1, arg2 a2, arg3 a3) { return callback(a1, a2, a3); }
private:
	callback_type callback;
	static bool default_cb(arg1, arg2, arg3) {return true;}
};

template <typename arg1, typename arg2, typename arg3, typename arg4>
class Callback4
{
	typedef bool (*callback_type)(arg1, arg2, arg3, arg4);
public:
	Callback4(callback_type cb = NULL) : callback(cb ? cb : &default_cb) {}
	bool operator ()(arg1 a1, arg2 a2, arg3 a3, arg4 a4) { return callback(a1, a2, a3, a4); }
private:
	callback_type callback;
	static bool default_cb(arg1, arg2, arg3, arg4) {return true;}
};

template <typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
class Callback5
{
	typedef bool (*callback_type)(arg1, arg2, arg3, arg4, arg5);
public:
	Callback5(callback_type cb = NULL) : callback(cb ? cb : &default_cb) {}
	bool operator ()(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5) { return callback(a1, a2, a3, a4, a5); }
private:
	callback_type callback;
	static bool default_cb(arg1, arg2, arg3, arg4, arg5) {return true;}
};

template <typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
class Callback6
{
	typedef bool (*callback_type)(arg1, arg2, arg3, arg4, arg5, arg6);
public:
	Callback6(callback_type cb = NULL) : callback(cb ? cb : &default_cb) {}
	bool operator ()(arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6) { return callback(a1, a2, a3, a4, a5, a6); }
private:
	callback_type callback;
	static bool default_cb(arg1, arg2, arg3, arg4, arg5, arg6) {return true;}
};


CWT_NS_END

#endif /* __CWT_CALLBACK_HPP__ */
