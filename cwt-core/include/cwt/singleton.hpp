/*
 * Singleton.hpp
 *
 *  Created on: Dec 17, 2008
 *      Author: wladt
 */

#ifndef __CWT_SINGLETON_HPP__
#define __CWT_SINGLETON_HPP__

#include <cwt/mt.hpp>

CWT_NS_BEGIN

/**
 * @details Использованы материалы:
 * 		@arg Паттерн Singleton (Одиночка) [http://www.rsdn.ru/article/patterns/singleton.xml]
 *
 * @arg В классе Singleton конструктор должен быть объявлен в защищенной секции
 *      для предотвращения создания объекта способом, отличным от вызова метода
 *      instance().
 * @arg Деструктор также следует поместить в защищенную секцию класса, чтобы
 *      исключить возможность удаления объекта оператором delete.
 * @arg Для автоматического подсчета ссылок при освобождении объекта следует
 *      применять специальный метод, такой как free().
 * @arg При наследовании от класса, реализующего паттерн Singleton, конструктор
 *      класса-потомка также должен быть объявлен в защищенной секции.
 *      Деструктор должен быть объявлен как виртуальный.
 *      Класс-потомок должен переопределить метод instance(), так, чтобы он
 *      создавал объект нужного типа.
 * @arg Если предполагается полиморфная работа с классами, наследуемыми от
 *      одного базового класса, причем некоторые классы потомки реализуют
 *      паттерн Singleton, а некоторые нет, следует в базовом классе определить
 *      метод free() как виртуальный. Базовый класс предоставляет реализацию
 *      по умолчанию этого метода, просто вызывая оператор delete this.
 *      В классах-потомках, реализующих паттерн Singleton, при реализации метода
 *      free(), используйте механизм подсчета ссылок.
 * @arg Если используется параметризированная версия Singleton, то в производных
 *      классах следует объявить базовый класс (Singleton) дружественным.
 *
 * Пример использования:
 * @code
 * class Derived : public Singleton<Derived>
 * {
 * protected:
 * 		Derived(){}
 * 		friend class Singleton<Derived>;
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
// static Singleton& instance()
// {
//     	static Singleton s;
//		return s;
// }
//
// See http://en.wikipedia.org/wiki/Double-checked_locking

template <class T>
class Singleton
{
	CWT_DENY_COPY(Singleton);
protected:
	Singleton() {}
	virtual ~Singleton() { _self = 0; }

public:
	static T *  instance();
	static void free();
	static void forceFree();

private:
	static T * _self;
	static int _refs;
};

template <class T>
T * Singleton<T>::_self = 0;

template <class T>
int Singleton<T>::_refs = 0;

template <class T>
T * Singleton<T>::instance()
{
	static CWT_DEFAULT_MT_POLICY g_mutex;
	AutoLock<> locker(&g_mutex);
	if( !_self )
		_self = new T;
	++_refs;
	return _self;
}

template <class T>
void Singleton<T>::free()
{
	static CWT_DEFAULT_MT_POLICY g_mutex;
	AutoLock<> locker(& g_mutex);
	if( --_refs == 0 ) {
		if (_self) {
			delete _self;
			_self = 0;
		}
	}
}

template <class T>
void Singleton<T>::forceFree()
{
	static CWT_DEFAULT_MT_POLICY g_mutex;
	AutoLock<> locker(& g_mutex);
	_refs = 0;
	if (_self) {
		delete _self;
		_self = 0;
	}
}

CWT_NS_END

#endif /* __CWT_SINGLETON_HPP__ */
