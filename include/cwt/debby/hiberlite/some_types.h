#ifndef SOM_TYPES_H_INCLUDED
#define SOM_TYPES_H_INCLUDED

namespace hiberlite{

template <typename E, typename C>
class stl_stream_adapter
{
	C & ct;
	typename C::iterator it;
	E xx;

public:
	stl_stream_adapter (C & _ct) : ct(_ct) { it = ct.begin(); }

	void startLoadFromDb ()
	{
		ct.clear();
		it = ct.begin();
	}

	void putNext (E & el)
	{
		ct.insert(ct.end(), el);
		it = ct.end();
	}

	E & getNext ()
	{
		xx = *it++;
		return xx;
	}

	bool done()
	{
		return it == ct.end();
	}
};

template <typename A, typename E>
void hibernate (A & ar, std::vector<E> & v, const unsigned int)
{
	collection_nvp<E,stl_stream_adapter<E,std::vector<E> > > body( "items", stl_stream_adapter<E,std::vector<E> >(v) );
	ar & body;
}

template <typename A, typename First, class Second>
void hibernate (A & ar, std::pair<First,Second> & m, const unsigned int)
{
	sql_nvp<First> f("first", m.first);
	sql_nvp<Second> s("second", m.second);
	ar & f;
	ar & s;
}

template <typename A, typename Key>
void hibernate (A & ar, std::set<Key> & m, const unsigned int)
{
	typedef Key ElType;
	collection_nvp<ElType,stl_stream_adapter<ElType,std::set<Key> > >
			body( "items", stl_stream_adapter<ElType,std::set<Key> >(m) );
	ar & body;
}

template <typename A, typename Key>
void hibernate (A & ar, std::list<Key> & m, const unsigned int)
{
	typedef Key ElType;
	collection_nvp<ElType,stl_stream_adapter<ElType,std::list<Key> > >
			body("items", stl_stream_adapter<ElType,std::list<Key> >(m) );
	ar & body;
}

template <typename K, typename D>
class stl_map_adapter
{
	typedef std::pair<const K,D> RType;
	typedef std::pair<K,D> WType;
	typedef std::map<K,D> C;

	typename C::iterator it;
	C & ct;
	WType xx;

public:
	void startLoadFromDb()
	{
		ct.clear();
		it = ct.begin();
	}

	stl_map_adapter (C & _ct) : ct(_ct) { it = ct.begin(); }

	void putNext (WType & el)
	{
		ct.insert(el);
		it = ct.end();
	}

	WType & getNext()
	{
		xx = *it++;
		return xx;
	}

	bool done() { return it == ct.end(); }
};

template <typename A, typename Key, typename Data>
void hibernate (A & ar, std::map<Key,Data> & m, const unsigned int)
{
	typedef std::pair<Key,Data> ElType;
	collection_nvp<ElType,stl_map_adapter<Key,Data> >
			body("items", stl_map_adapter<Key,Data>(m) );
	ar & body;
}

#define HIBERLITE_DEF_DB_ATOM(T, StorClass)                                                     \
template<class A> void hibernate (A & ar, T& value, const unsigned int)	                        \
	{ ar & db_atom<T>(value);	}										                        \
template<> inline pfs::string db_atom<T>::sqliteStorageClass ()					                \
	{ return pfs::string(StorClass); }											                \
template<> inline void db_atom<T>::bindValue (cwt::debby::statement & stmt, size_t col)		    \
	{ stmt.bind(col,val); }
//template <> template <class Stmt, class Arg> void db_atom<T>::loadValue (Stmt & res, Arg & arg)
//	{ val = res.GetFromDB(arg);	}

HIBERLITE_DEF_DB_ATOM(char              , "INTEGER")
HIBERLITE_DEF_DB_ATOM(unsigned char     , "INTEGER")
HIBERLITE_DEF_DB_ATOM(int               , "INTEGER")
HIBERLITE_DEF_DB_ATOM(short             , "INTEGER")
HIBERLITE_DEF_DB_ATOM(unsigned short    , "INTEGER")
HIBERLITE_DEF_DB_ATOM(unsigned int      , "INTEGER")
HIBERLITE_DEF_DB_ATOM(long long         , "INTEGER")
HIBERLITE_DEF_DB_ATOM(long int          , "INTEGER")
HIBERLITE_DEF_DB_ATOM(unsigned long int , "INTEGER")
HIBERLITE_DEF_DB_ATOM(float             , "REAL")
HIBERLITE_DEF_DB_ATOM(double            , "REAL")

#undef HIBERLITE_DEF_DB_ATOM

template <typename A>
void hibernate (A & ar, pfs::string & value, const unsigned int)
{
	ar & db_atom<pfs::string>(value);
}

/*
template<> template<typename Stmt, typename Arg>
void db_atom<pfs::string>::loadValue (Stmt & res, Arg & arg)
{
	val = pfs::string((const char*)(res.get_text(arg)));
}
*/

template<>
inline pfs::string db_atom<pfs::string>::sqliteStorageClass ()
{
	return _l1("TEXT");
}

template<>
inline void db_atom<pfs::string>::bindValue (cwt::debby::statement & stmt, size_t col)
{
	//sqlite3_bind_text(stmt,col,val.c_str(),-1,SQLITE_TRANSIENT);
	stmt.bind(size_t(col), val);
}

} //namespace hiberlite

#endif // SOM_TYPES_H_INCLUDED
