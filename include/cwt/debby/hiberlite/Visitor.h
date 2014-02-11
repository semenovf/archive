#ifndef __HIBERLITE_VISITOR_H__
#define __HIBERLITE_VISITOR_H__

#include <stack>

namespace hiberlite {

class access
{
public:
	template<class A, class C>
	static void hibernate(A & a, C & c) { c.hibernate(a); }

	template<class A, class C>
	static void construct(A & /*a*/, C ** c) { *c = new C(); }

	template<class A, class C>
	static void destroy(A & /*a*/, C * c) { delete c; }
};

template<class A, class C>
void hibernate (A & a, C & c, const long unsigned int bogus)
{
	access::hibernate(a,c);
}

template<class A, class C>
void construct (A & a, C ** c, const long unsigned int bogus)
{
	access::construct(a,c);
}

template<class A, class C>
void destroy (A & a, C * c, const long unsigned int bogus)
{
	access::destroy(a,c);
}

/*
class Actor{
	public:
		template<class Vis, class E, class Stream>
		void act(Vis* vis, collection_nvp<C,Stream> nvp);

		template<class Vis, class C>
		void act(Vis* vis, db_atom<C> atom);

		template<class AV>
		void notifyInitWalk(AV& av);

		template<class AV>
		void notifyDoneWalk(AV& av);
};
*/

template<class C>
class AVisitor
{
public:
	static const int LOADING	= 1;
	static const int SAVING		= 2;
	static const int MODEL		= 3;
	static const int DELETING	= 4;

protected:
	C * actor;
	int purpose;
	std::stack<Scope> stack;
	Scope scope;
	bean_key rootKey;

public:
	bool is_loading ();
	bool is_saving ();
	bool is_model ();
	bool is_deleting ();

	AVisitor (C * _actor, int _purp);
	~AVisitor ();

	cwt::debby::database getConnection ();
	sqlid_t getRootId ();
	inline Scope getScope () { return scope; }

	template<class Y>
	void startWalk (Y & obj, bean_key _k);

	template<class Y>
	void walk (Y & obj);

	template<class X>
	AVisitor<C> & operator & (sql_nvp<X> nvp);

	template<class E, class S>
	AVisitor<C> & operator & (collection_nvp<E,S> nvp);

	template<class X>
	AVisitor<C> & operator & (db_atom<X> atom);

	void divePrefix (const pfs::string & name);
	void diveTable (const pfs::string & name);
	void pop();

protected:
	void popScope ();
	void pushAndSubTable (const pfs::string & name);
	void pushAndSubPrefix (const pfs::string & prefix);
};

//class DropChildren;
//class UpdateBean;
//class BeanUpdater;
//
//class ExtractModel;
//class ModelExtractor;

} //namespace hiberlite

#endif // __HIBERLITE_VISITOR_H__
