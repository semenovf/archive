/*
 * singleton.hpp
 *
 *  Created on: Dec 17, 2008
 *      Author: wladt
 */

#ifndef __PFS_SINGLETON_HPP__
#define __PFS_SINGLETON_HPP__

#include <pfs/mt.hpp>
#include <pfs/noncopyable.hpp>

namespace pfs {

/**
 * @details Использованы материалы:
 * 		@arg Паттерн singleton (Одиночка) [http://www.rsdn.ru/article/patterns/singleton.xml]
 *
 * @arg В классе singleton конструктор должен быть объявлен в защищенной секции
 *      для предотвращения создания объекта способом, отличным от вызова метода
 *      instance().
 * @arg Деструктор также следует поместить в защищенную секцию класса, чтобы
 *      исключить возможность удаления объекта оператором delete.
 * @arg Для автоматического подсчета ссылок при освобождении объекта следует
 *      применять специальный метод, такой как free().
 * @arg При наследовании от класса, реализующего паттерн singleton, конструктор
 *      класса-потомка также должен быть объявлен в защищенной секции.
 *      Деструктор должен быть объявлен как виртуальный.
 *      Класс-потомок должен переопределить метод instance(), так, чтобы он
 *      создавал объект нужного типа.
 * @arg Если предполагается полиморфная работа с классами, наследуемыми от
 *      одного базового класса, причем некоторые классы потомки реализуют
 *      паттерн singleton, а некоторые нет, следует в базовом классе определить
 *      метод free() как виртуальный. Базовый класс предоставляет реализацию
 *      по умолчанию этого метода, просто вызывая оператор delete this.
 *      В классах-потомках, реализующих паттерн singleton, при реализации метода
 *      free(), используйте механизм подсчета ссылок.
 * @arg Если используется параметризированная версия singleton, то в производных
 *      классах следует объявить базовый класс (singleton) дружественным.
 *
 * Пример использования:
 * @code
 * class Derived : public singleton<Derived>
 * {
 * protected:
 * 		Derived(){}
 * 		friend class singleton<Derived>;
 * };
 *
 * int main(int argc, char* argv[])
 * {
 * 		Derived *p = Derived::instance();
 * 		...
 * 		...
 *      ...
 *      p->free();
 *      return 0;
 * }
 * @endcode
 */

// TODO C++11 : For the singleton pattern, double-checked locking is not needed:
// 		If control enters the declaration concurrently while the variable is being initialized,
// 		the concurrent execution shall wait for completion of the initialization.
//																	—§6.7 [stmt.dcl] p4
// static singleton& instance()
// {
//     	static singleton s;
//		return s;
// }
//
// See http://en.wikipedia.org/wiki/Double-checked_locking

template <class T>
class singleton : pfs::noncopyable
{
protected:
	singleton() {}
	virtual ~singleton() { _self = 0; }

public:
	static T *  instance();
	static void free();
	static void forceFree();

private:
	static T * _self;
	static int _refs;
};

template <class T>
T * singleton<T>::_self = 0;

template <class T>
int singleton<T>::_refs = 0;

template <class T>
T * singleton<T>::instance()
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(&__mutex);
	if( !_self )
		_self = new T;
	++_refs;
	return _self;
}

template <class T>
void singleton<T>::free()
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(& __mutex);
	if( --_refs == 0 ) {
		if (_self) {
			delete _self;
			_self = 0;
		}
	}
}

template <class T>
void singleton<T>::forceFree()
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(& __mutex);
	_refs = 0;
	if (_self) {
		delete _self;
		_self = 0;
	}
}

} // pfs

#endif /* __PFS_SINGLETON_HPP__ */
