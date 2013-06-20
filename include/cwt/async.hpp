/*
 * async.hpp
 *
 *  Created on: May 27, 2013
 *      Author: wladt
 */

#ifndef __CWT_ASYNC_HPP__
#define __CWT_ASYNC_HPP__

#include <cwt/cwt.h>
#include <cwt/sigslot.hpp>
#include <QtCore>
#if QT_VERSION >= 0x050000
#include	<QtConcurrent/QtConcurrent>
#endif


CWT_NS_BEGIN

template <typename result_type = void>
class future {
public:
	future() : m_future() {}
	explicit future(const QFuture<result_type>& future) : m_future(future) {}
	result_type result()    { return m_future.result(); }
	void cancel()           { m_future.cancel(); }
	void pause()            { m_future.pause(); }
	void resume()           { m_future.resume(); }
	bool isFinished() const { return m_future.isFinished(); }
	bool isCanceled() const { return m_future.isCanceled(); }
	bool isPaused()   const { return m_future.isPaused(); }
	void wait()             { m_future.waitForFinished(); }
private:
	QFuture<result_type> m_future;
};

template<typename T>
future<T> async(T (*func_ptr)())
{
	return future<T>(QtConcurrent::run(func_ptr));
}

template<typename T, typename Arg1>
future<T> async(T (*func_ptr)(Arg1), const Arg1 &arg1)
{
	return future<T>(QtConcurrent::run(func_ptr, arg1));
}

template<typename T, typename Arg1, typename Arg2>
future<T> async(T (*func_ptr)(Arg1, Arg2), const Arg1 &arg1, const Arg2 &arg2)
{
	return future<T>(QtConcurrent::run(func_ptr, arg1, arg2));
}

template<typename T, typename Arg1, typename Arg2, typename Arg3>
future<T> async(T (*func_ptr)(Arg1, Arg2, Arg3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
	return future<T>(QtConcurrent::run(func_ptr, arg1, arg2, arg3));
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
future<T> async(T (*func_ptr)(Arg1, Arg2, Arg3, Arg4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
	return future<T>(QtConcurrent::run(func_ptr, arg1, arg2, arg3, arg4));
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
future<T> async(T (*func_ptr)(Arg1, Arg2, Arg3, Arg4, Arg5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
	return future<T>(QtConcurrent::run(func_ptr, arg1, arg2, arg3, arg4, arg5));
}

template<typename T, typename Class>
future<T> async(const Class *object, T (Class::*method)() const)
{
	return future<T>(QtConcurrent::run(object, method));
}

template<typename T, typename Class, typename Arg1>
future<T> async(const Class *object, T (Class::*method)(Arg1) const, const Arg1 &arg1)
{
	return future<T>(QtConcurrent::run(object, method, arg1));
}

template<typename T, typename Class, typename Arg1, typename Arg2>
future<T> async(const Class *object, T (Class::*method)(Arg1, Arg2) const, const Arg1 &arg1, const Arg2 &arg2)
{
	return future<T>(QtConcurrent::run(object, method, arg1, arg2));
}

template<typename T, typename Class, typename Arg1, typename Arg2, typename Arg3>
future<T> async(const Class *object, T (Class::*method)(Arg1, Arg2, Arg3) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
	return future<T>(QtConcurrent::run(object, method, arg1, arg2, arg3));
}

template<typename T, typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
future<T> async(const Class *object, T (Class::*method)(Arg1, Arg2, Arg3, Arg4) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
	return future<T>(QtConcurrent::run(object, method, arg1, arg2, arg3, arg4));
}

template<typename T, typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
future<T> async(const Class *object, T (Class::*method)(Arg1, Arg2, Arg3, Arg4, Arg5) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
	return future<T>(QtConcurrent::run(object, method, arg1, arg2, arg3, arg4, arg5));
}

template<typename Emitter>
future<void> async_emit(Emitter &emitter)
{
	return future<void>(QtConcurrent::run(&emitter, &Emitter::emit_));
}

template<typename Emitter, typename Arg1>
future<void> async_emit(Emitter &emitter, const Arg1 &arg1)
{
	return future<void>(QtConcurrent::run(&emitter, &Emitter::emit_, arg1));
}

template<typename Emitter, typename Arg1, typename Arg2>
future<void> async_emit(Emitter &emitter, const Arg1 &arg1, const Arg2 &arg2)
{
	return future<void>(QtConcurrent::run(&emitter, &Emitter::emit_, arg1, arg2));
}

template<typename Emitter, typename Arg1, typename Arg2, typename Arg3>
future<void> async_emit(Emitter &emitter, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
	return future<void>(QtConcurrent::run(&emitter, &Emitter::emit_, arg1, arg2, arg3));
}

template<typename Emitter, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
future<void> async_emit(Emitter &emitter, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
	return future<void>(QtConcurrent::run(&emitter, &Emitter::emit_, arg1, arg2, arg3, arg4));
}

template<typename Emitter, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
future<void> async_emit(Emitter &emitter, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
	return future<void>(QtConcurrent::run(&emitter, &Emitter::emit_, arg1, arg2, arg3, arg4, arg5));
}

CWT_NS_END

#endif /* __CWT_ASYNC_HPP__ */
