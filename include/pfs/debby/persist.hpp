/**
 * @file perist.hpp
 * @author wladt
 * @date Mar 14, 2014
 */

#ifndef __PFS_DEBBY_PERSIST_HPP__
#define __PFS_DEBBY_PERSIST_HPP__

namespace pfs { namespace debby {

template <typename T>
class storage_type
{
	 _typestr;
public:
	storage_type() {}

	pfs::string typestr ();
};

class deployer
{
	bool _nullable;
	int  _autoinc;

public:
	deployer ()
		: _nullable(false)
		, _autoinc(0) {}

    deployer & operator << (void (*__pf)(deployer &))
    {
    	__pf(*this);
    	return *this;
    }

	template <typename T>
	deployer & operator << (void (*__pf)(storage_type<T> &))
	{
		__pf(*this);
		return *this;
    }

	void setNullable () { _nullable = true; }
	void setAutoinc (int inc = 1) { _autoinc = inc; }
};

inline deployer & nullable (deployer & d) { d.setNullable(); return d; }
inline deployer & autoinc  (deployer & d) { d.setAutoinc(1); return d; }

// deployer() << nullable << storage_type<int>()



class abstract_persistent
{
private:
	abstract_persistent () {}

public:
	virtual ~abstract_persistent () { std::cout << std::endl; }

	virtual bool create ();
	virtual bool get ();
	virtual bool update ();
	virtual bool remove ();

	virtual bool deploy ();
	virtual bool destroy ();
};


template <typename Model>
class persistent : public abstract_persistent
{
	Model * _model;

public:
	persistent (Model * model) : _model (model) {}
};

}} // pfs::debby

#endif /* __PFS_DEBBY_PERSIST_HPP__ */
