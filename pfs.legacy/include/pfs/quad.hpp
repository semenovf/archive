/**
 * @file quad.hpp
 * @author wladt
 * @date Apr 2, 2014
 */

#ifndef __PFS_QUAD_HPP__
#define __PFS_QUAD_HPP__

namespace pfs {

template <typename T>
class quad
{
	typedef T value_type;

	value_type _value[4];

private:
	quad ();

public:
	quad (const value_type & first
			, const value_type & second
			, const value_type & third
			, const value_type & fourth)
	{
		_value[0] = first;
		_value[1] = second;
		_value[2] = third;
		_value[3] = fourth;
	}

	quad (const value_type & all)
	{
		_value[0] = _value[1] = _value[2] = _value[3] = all;
	}

//	quad (const value_type & top_bottom, const value_type & left_right)
//	{
//		set_top(top_bottom);
//		set_bottom(top_bottom);
//		set_left(left_right);
//		set_right(left_right);
//	}

	quad (const quad<T> & other)
	{
		set(other);
	}

	quad & operator = (const quad<T> & other)
	{
		set(other);
		return *this;
	}

	const value_type & first  () const { return _value[0]; }
	value_type         first  ()       { return _value[0]; }
	const value_type & second () const { return _value[1]; }
	value_type         second ()       { return _value[1]; }
	const value_type & third  () const { return _value[2]; }
	value_type         third  ()       { return _value[2]; }
	const value_type & fourth () const { return _value[3]; }
	value_type         fourth ()       { return _value[3]; }

//	const value_type & top    () const { return first();  }
//	value_type         top    ()       { return first();  }
//	const value_type & right  () const { return second(); }
//	value_type         right  ()       { return second(); }
//	const value_type & bottom () const { return third();  }
//	value_type         bottom ()       { return third();  }
//	const value_type & left   () const { return fourth(); }
//	value_type         left   ()       { return fourth(); }

	void set (const quad<T> & other)
	{
		_value[0] = other._value[0];
		_value[1] = other._value[1];
		_value[2] = other._value[2];
		_value[3] = other._value[3];
	}

	void set_first  (const value_type & v) { _value[0] = v; }
	void set_second (const value_type & v) { _value[1] = v; }
	void set_third  (const value_type & v) { _value[2] = v; }
	void set_rourth (const value_type & v) { _value[3] = v; }

//	void set_top    (const value_type & v) { set_first(v); }
//	void set_right  (const value_type & v) { set_second(v); }
//	void set_bottom (const value_type & v) { set_third(v); }
//	void set_left   (const value_type & v) { set_fourth(v); }
};

} // pfs

#endif /* __PFS_QUAD_HPP__ */
