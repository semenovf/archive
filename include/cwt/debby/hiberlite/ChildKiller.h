#ifndef __HIBERNATE_CHILDKILLER_H__
#define __HIBERNATE_CHILDKILLER_H__

namespace hiberlite {

class KillChildren
{
protected:
	std::stack<sqlid_t> rowId;

public:
	template <class AV>
	inline void notifyInitWalk (AV & av);

	template <class AV>
	inline void notifyDoneWalk (AV & av);

	template <class AV, class C>
	inline void act (AV & av, db_atom<C> nvp ){}

	template <class AV, class E, class S>
	inline void act (AV & av, collection_nvp<E,S> nvp );
};

class ChildKiller : AVisitor<KillChildren>
{
protected:
	KillChildren actor;

public:
	ChildKiller ();

	template<class C>
	void killChildren (bean_key k, C & bean);
};

} //namespace hiberlite

#endif // __HIBERNATE_CHILDKILLER_H__
