#include <cwt/debby/schema.hpp>

#include <pfs/string.hpp>
#include <vector>
#include <iostream>

class Person
{
	friend class hiberlite::access;

	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(name);
		ar & HIBERLITE_NVP(age);
		ar & HIBERLITE_NVP(bio);
	}

public:
	pfs::string name;
	double age;
	vector<pfs::string> bio;
};

HIBERLITE_EXPORT_CLASS(Person)

void createDB()
{
	hiberlite::Database db("sample.db");
	//register bean classes
	db.registerBeanClass<Person>();
	//drop all tables beans will use
	db.dropModel();
	//create those tables again with proper schema
	db.createModel();

	const char* names[5] = {"Stanley Marsh", "Kyle Broflovski", "Eric Theodore Cartman", "Kenneth McCormick", "Leopold Stotch"};

	for(unsigned int i=0;i<5;i++) {
		Person x;
		x.name = _u8(names[i%5]);
		x.age = 14 + i * 0.1;
		x.bio.push_back(_l1("Hello"));
		x.bio.push_back(_l1("world"));
		x.bio.push_back(_l1("!"));

		hiberlite::bean_ptr<Person> p=db.copyBean(x);	//create a managed copy of the object
	}
}

void printDB ()
{
	hiberlite::Database db("sample.db");
	db.registerBeanClass<Person>();

	std::cout << pfs::string(15, '=') << "\nreading the DB\n";

	std::vector< hiberlite::bean_ptr<Person> > v = db.getAllBeans<Person>();

	std::cout << "found " << v.size() << " persons in the database:\n";

	for(size_t j=0;j<v.size();j++){
		std::cout << "[name=" << v[j]->name << "\t";
		std::cout << "age=" << v[j]->age << "\t";
		std::cout << "bio={";

		for (size_t i = 0; i < v[j]->bio.size(); ++i)
			i && cout << ", ", cout << v[j]->bio[i];

		std::cout << "}]\n";
	}
}

void modifyDB()
{
	hiberlite::Database db("sample.db");
	db.registerBeanClass<Person>();

	std::vector< hiberlite::bean_ptr<Person> > v = db.getAllBeans<Person>();
	std::cout << v[0]->name << " will be deleted.\n";
	v[0].destroy();
	std::cout << v[1]->name << " becomes 1 year older.\n\n";
	v[1]->age+=1;
}

int main()
{
	createDB();
	printDB();
	modifyDB();
	printDB();
	modifyDB();
	printDB();
	return 0;
}
