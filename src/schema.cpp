/*
 * scheme.cpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

//#include "../include/cwt/debby/dbh.hpp"
#include "../include/cwt/debby/record.hpp"
#include "../include/cwt/debby/schema.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>

CWT_NS_BEGIN

namespace debby
{

bool Schema::deploy(const cwt::String & uri)
{
	cwt::shared_ptr<DbHandler> dbh(DbHandler::open(uri));
	if(dbh.get()) {
		return dbh->deploySchema(*dbh.get(), *this);
	}

	return false;
}


bool Schema::deploy(DbHandler & dbh)
{
	return dbh.deploySchema(dbh, *this);
}

/**
 * @brief Drop schema.
 *
 * @param name Schema (database) name.
 * @return @c true if scheme (database) successfully dropped, otherwise @c false.
 */
bool Schema::drop (const cwt::String & uri)
{
	cwt::shared_ptr<DbHandler> dbh(DbHandler::open(uri));
	if(dbh.get()) {
		return dbh->dropSchema(*dbh.get(), *this);
	}

	return false;
}

bool Schema::drop (DbHandler & dbh)
{
	return dbh.dropSchema(dbh, *this);
}


#ifdef __COMMENT__

//static const cwt::String SchemaObjectKey ("__debby_schema__");

static const cwt::String VersionMajorKey ("__version_major__");
static const cwt::String VersionMinorKey ("__version_minor__");
static const int __version_major = 1;
static const int __version_minor = 0;

static const cwt::String SchemaTablesKey     ("__tables__");
static const cwt::String SchemaFieldsKey     ("__fields__");
static const cwt::String SchemaPkKey         ("__pk__");
static const cwt::String SchemaAutoincField  ("__autoinc_field__");
static const cwt::String SchemaAutoincValue  ("__autoinc_value__");


inline cwt::JsonValue & __parentTable (cwt::JsonValue & column)
{
	return column.parent().parent();
}

inline const cwt::JsonValue & __parentTable (const cwt::JsonValue & column)
{
	return column.parent().parent();
}

Schema::Schema()
{
	_schema.insertObject(SchemaTablesKey);
	_schema.insert(VersionMajorKey, __version_major);
	_schema.insert(VersionMinorKey, __version_minor);
}

Table Schema::add (const cwt::String & name)
{
	JsonValue & jv = _schema[SchemaTablesKey].insertObject(name);
	jv.insertObject(SchemaFieldsKey);
	jv.insertArray(SchemaPkKey);
/*
	jv.insert(SchemaAutoincField, cwt::String());
	jv.insert(SchemaAutoincValue, 0);
*/
	return Table(jv);
}


// Version in format xxx.yyy.
// So, 1.0 is 1000, 10.0 is 10000, 12.03 is 12003
uint_t Schema::version (ushort_t * major, ushort_t * minor) const
{
	CWT_ASSERT(!_schema[VersionMajorKey].isInvalid());
	CWT_ASSERT(!_schema[VersionMinorKey].isInvalid());

	long_t j = _schema[VersionMajorKey].integer(-1);
	long_t n = _schema[VersionMinorKey].integer(-1);

	CWT_ASSERT(j >= 0 && j < 1000);
	CWT_ASSERT(n >= 0 && n < 1000);

	if (major)
		*major = ushort_t(j);
	class DLL_API Schema
	{
	public:
		Schema();

		Table add (const cwt::String & name);

		bool deploy(const cwt::String & uri);
		bool deploy(DbHandler & dbh);
		bool drop(const cwt::String & uri);
		bool drop(DbHandler & dbh);

		const JsonValue & json () const { return _schema; }
		uint_t version (ushort_t * major = nullptr, ushort_t * minor = nullptr) const;

		size_t count () const;
		cwt::Vector<cwt::String> tableNames() const;
		Table table(const cwt::String & name) const;

	private:
		Json _schema;
	};

	if (minor)
		*minor = ushort_t(n);

	return uint_t(j * 1000 + n);
}

size_t Schema::count () const
{
	return _schema[SchemaTablesKey].count();
}

cwt::Vector<cwt::String> Schema::tableNames() const
{
	return _schema[SchemaTablesKey].keys();
}

Table Schema::table(const cwt::String & name) const
{
	return Table(_schema[SchemaTablesKey][name]);
}

void Table::addPrimaryKey (const cwt::String & fieldname)
{
	_table[SchemaPkKey].append(fieldname);
}

void Table::setPrimaryKey (const cwt::String & fieldname)
{
	clearPrimaryKey();
	addPrimaryKey(fieldname);
}

void Table::clearPrimaryKey ()
{
	_table.insertArray(SchemaPkKey); // destroys previous value if set
}

Field Table::addBoolean (const cwt::String & name)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::BoolValue));
	return Field(name, jv);
}

Field Table::addInteger (const cwt::String & name, size_t size, bool isUnsigned)
{
	return addDecimal(name, size, 0, isUnsigned);
}

Field Table::addDecimal (const cwt::String & name, size_t size, size_t decimals, bool isUnsigned)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::IntegerValue));
	if (size)
		jv.insert("size", size);

	if (decimals)
		jv.insert("decimals", decimals);

	if (isUnsigned)
		jv.insert("unsigned", isUnsigned);

	return Field(name, jv);
}

Field Table::addFloat (const cwt::String & name, bool isUnsigned)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::FloatValue));
	if (isUnsigned)
		jv.insert("unsigned", isUnsigned);
	return Field(name, jv);
}

Field Table::addDouble (const cwt::String & name, bool isUnsigned)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::DoubleValue));
	if (isUnsigned)
		jv.insert("unsigned", isUnsigned);
	return Field(name, jv);
}

Field Table::addString (const cwt::String & name, size_t size)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::StringValue));
	if (size)
		jv.insert("size", size);
	return Field(name, jv);
}

Field Table::addBlob (const cwt::String & name)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::BlobValue));
	return Field(name, jv);
}

Field Table::addDate (const cwt::String & name, bool isTimeStamp)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::DateValue));
	if (isTimeStamp)
		jv.insert("timestamp", true);
	return Field(name, jv);
}

Field Table::addTime (const cwt::String & name, bool isTimeStamp)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::TimeValue));
	if (isTimeStamp)
		jv.insert("timestamp", true);
	return Field(name, jv);
}

Field Table::addDateTime (const cwt::String & name, bool isTimeStamp)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::DateTimeValue));
	if (isTimeStamp)
		jv.insert("timestamp", true);
	return Field(name, jv);
}

Field Table::addRef (const cwt::String & name, const cwt::String & reftable)
{
	cwt::JsonValue & jv = _table[SchemaFieldsKey].insertObject(name);
	jv.insert("type", cwt::UniType::toStringType(cwt::UniType::ObjectValue));
	jv.insert("ref", reftable);
	return Field(name, jv);
}

size_t Table::count () const
{
	return _table[SchemaFieldsKey].count();
}

cwt::Vector<cwt::String> Table::fieldNames() const
{
	return _table[SchemaFieldsKey].keys();
}

Field Table::field(const cwt::String & name) const
{
	return Field(name, _table[SchemaFieldsKey][name]);
}

cwt::Vector<cwt::String> Table::primaryKeyNames() const
{
	cwt::Vector<cwt::String> r;
	const cwt::JsonValue & pknames = _table[SchemaPkKey];

	if (pknames.isArray()) {
		size_t n = pknames.size();
		for (size_t i = 0; i < n; ++i)
			r.append(pknames[i].string());
	}
	return r;
}

Field & Field::setNullable(bool b)
{
	_column.insert("nullable", b ? true : false);
	return *this;
}

bool Field::isInvalid() const
{
	return _name.isEmpty() || _column.isInvalid();
}

cwt::UniType::TypeEnum Field::type () const
{
	return cwt::UniType::fromStringType(_column["type"].string());
}

Field & Field::setUnique(bool b)
{
	_column.insert("unique", b ? true : false);
	return *this;
}

/**
 * @brief Enables/disables AUTOINCREMENT flag for field and sets it's value.
 *
 * @param inc Increment value. Enables flag if @c inc is greater than 0, otherwise disables it.
 * @return This field.
 *
 * @note Do not use increment value greater than 1 for compatibility, except you know
 *       that DB engine support it.
 *
 * @note MySQL: there can be only one AUTO_INCREMENT column per table, it must be indexed,
 *       and it cannot have a DEFAULT value. An AUTO_INCREMENT column works properly
 *       only if it contains only positive values. Inserting a negative number
 *       is regarded as inserting a very large positive number.
 *       This is done to avoid precision problems when numbers “wrap” over from positive
 *       to negative and also to ensure that you do not accidentally
 *       get an AUTO_INCREMENT column that contains 0.
 *
 * @note Sqlite3: It seems there is now support for value greater than 1
 *       (in the standard manner, via AUTOINCREMENT keyword).
 *
 */
Field & Field::setAutoinc (uint_t inc)
{
	JsonValue & parentTable = __parentTable(_column);
	if (inc) {
		parentTable.insert(SchemaAutoincField, _name);
	} else {
		parentTable.insert(SchemaAutoincField, String());
	}
	parentTable.insert(SchemaAutoincValue, inc);

	return *this;
}

bool Field::isNullable() const
{
	return _column["nullable"].boolean(false);
}

bool Field::isUnique() const
{
	return _column["unique"].boolean(false);
}

bool Field::isUnsigned() const
{
	return _column["unsigned"].boolean(false);
}

bool Field::isRef () const
{
	return !_column["ref"].isInvalid();
}

bool Field::isAutoinc () const
{
	const cwt::JsonValue & parentTable = __parentTable(_column);
	return parentTable[SchemaAutoincField].string() == _name;
}

bool Field::isPk () const
{
	const JsonValue & parentTable = __parentTable(_column);

	cwt::Vector<cwt::String> r;
	const cwt::JsonValue & pknames = parentTable[SchemaPkKey];

	if (pknames.isArray()) {
		size_t n = pknames.size();

		if (n != 1) // no primary key or it is compound
			return false;

		if (_name == pknames[0].string())
			return true;
	}
	return false;
}


/**
 * @brief Dereferences the field if it is a reference to the table.
 *
 * @details Finds primary key field (recursively) for referenced table and returns this field.
 *          If field is not a reference this method returns shallow copy of field itself.
 *
 * @return Primary key field for referenced table.
 */
Field Field::deref() const
{
	cwt::String reftable = _column["ref"].string(cwt::String());
	if (reftable.isEmpty())
		return Field(_name, _column);

	const JsonValue & parentSchema = _column.parent().parent().parent().parent();

	if (! parentSchema.isInvalid()) {
		size_t limit = parentSchema.count();
		size_t i = 0; // total iterations made while dereferencing

		while (i < limit) {
			const cwt::JsonValue & table = parentSchema[SchemaTablesKey][reftable];

			// Check if table has primary key and get first field from it.
			const JsonValue & field = table[SchemaFieldsKey][table[SchemaPkKey][0].string()];

			if (field.isInvalid())
				break;

			// Field is not a reference
			if (field["ref"].isInvalid())
				return Field(_name, field);

			reftable = field["ref"].string();
			++i;
		}
	}

	Logger::error(_Fr("Too many references or reference '%s' is inconsistent") % reftable);

	return Field();
}
#endif

} // namespace debby

CWT_NS_END


