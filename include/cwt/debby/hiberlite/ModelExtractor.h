#ifndef __HIBERLITE_MODELEXTRACTOR_H__
#define __HIBERLITE_MODELEXTRACTOR_H__

namespace hiberlite {

class ExtractModel
{
protected:
	Model model;

public:
	Model getModel();

	template<class AV>
	inline void notifyInitWalk (AV & av)
	{
		model[av.getScope().table()].name=av.getScope().table();
	}

	template <class AV>
	inline void notifyDoneWalk (AV & av) {}

	template <class AV, class X>
	inline void act (AV & av, db_atom<X> atom)
	{
		putColumn(av, atom.sqliteStorageClass());
	}

	template<class AV, class E, class S>
	inline void act (AV & av, collection_nvp<E,S> nvp );

protected:
	template <class AV>
	inline void putColumn (AV & av, const pfs::string storage)
	{
		Scope scope = av.getScope();
		putNamedColumn(scope.table(), scope.prefix(), storage);
	}

	inline void putNamedColumn (const pfs::string & table, const pfs::string & colname, const pfs::string & storClass)
	{
		model[table].name = table;
		if (model[table].contains(colname)) {
			// FIXME
			//throw std::logic_error(colname+": column name is in use");
		}
		model[table].add(Column(colname, storClass));
	}
};

class ModelExtractor : AVisitor<ExtractModel>
{
public:
	ModelExtractor();
	template<class C>
	void extractModel (cwt::debby::database _con);
	Model getModel();

protected:
	ExtractModel actor;
};

} //namespace hiberlite

#endif // __HIBERLITE_MODELEXTRACTOR_H__
