#ifndef __HIBERLITE_DATABASE_H__
#define __HIBERLITE_DATABASE_H__

namespace hiberlite {

class ModelExtractor;
class UpdateBean;

class Database : noncopyable
{
	ModelExtractor * mx;
	cwt::debby::database con;

private:
	friend class UpdateBean;
	friend class KillChildren;
	friend class LoadBean;

	template<class C>
	friend class real_bean;

	template<class C>
	static void dbDelete (bean_key key, C& bean);

	template<class C>
	static void dbUpdate (bean_key key, C& bean);

	template<class C>
	static C * dbLoad (bean_key key);

	static sqlid_t allocId (cwt::debby::database & dbh, const pfs::string & table);
	sqlid_t allocId (const pfs::string & table);

	static void dbExecQuery (cwt::debby::database con, const pfs::string & query);
	void dbExecQuery (const pfs::string & query);

	static std::vector<sqlid_t> dbSelectIds (cwt::debby::database & con, const pfs::string & table,
													const pfs::string & condition, const pfs::string & orderBy);

	static std::vector<sqlid_t> dbSelectChildIds (cwt::debby::database & con, const pfs::string & table, sqlid_t parent);
	inline std::vector<sqlid_t> dbSelectChildIds (const pfs::string & table, sqlid_t parent);

	template<class C>
	static void dbDeleteRows (cwt::debby::database con, const pfs::string & table, const pfs::string & column, C value);

public:
	static const sqlid_t NULL_ID = -1;

	template<class C>
	static pfs::string getClassName();

	Database ();
	Database (const pfs::string & fname);
	virtual ~Database();

	void open (const pfs::string & fname);
	void close();

	template<class C>
	inline void registerBeanClass();

	Model getModel();
	inline cwt::debby::database getConnection() { return con; }

	std::vector<pfs::string> checkModel();
	void dropModel();
	void createModel();

	template <class C>
	bean_ptr <C> loadBean(sqlid_t objId);

	template <class C>
	std::vector<sqlid_t> getBeanIds();

	template <class C>
	std::vector< bean_ptr<C> > getAllBeans();

	template <class C>
	bean_ptr<C> copyBean(const C& c);

	template <class C>
	bean_ptr<C> createBean();

	template <class C>
	bean_ptr<C> manageBean(C * ptr);
};

} // namespace hiberlite;


#endif // __HIBERLITE_DATABASE_H__
