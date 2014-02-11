#ifndef __HIBERLITE_BEANLOADER_H__
#define __HIBERLITE_BEANLOADER_H__

namespace hiberlite {

class LoadBean
{
protected:
	std::stack< std::pair<cwt::debby::statement, sqlid_t> > stmt;

public:
	template<class AV>
	inline void notifyInitWalk (AV & av);

	template<class AV>
	inline void notifyDoneWalk (AV & av);

	template<class AV, class C>
	inline void act (AV & av, db_atom<C> atom)
	{
		int col_indx = locateCol(av.getScope().prefix());
		atom.loadValue(stmt.top().first, col_indx);
	}

	template<class AV, class E, class S>
	inline void act (AV & av, collection_nvp<E,S> nvp);

protected:
	inline int locateCol (const pfs::string & name)
	{
		cwt::debby::statement & sel = stmt.top().first;
		int n = sel.columnCount();

		for (int i = 0; i< n; ++i)
			if (sel.columnName(size_t(i)) == name)
				return i;

		// FIXME
		//throw database_error(name + ": column not found in SELECT query results");
		return -1;
	}
};

class BeanLoader : AVisitor<LoadBean>
{
public:
	BeanLoader();

	template<class C>
	C * loadBean (bean_key k);

protected:
	LoadBean actor;
};

} //namespace hiberlite

#endif // __HIBERLITE_BEANLOADER_H__
