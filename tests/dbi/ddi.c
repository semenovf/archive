/*
 * ddi.c
 *
 *  Created on: 02.08.2012
 *      Author: user
 */


#include <cwt/test.h>
#include <cwt/dbi/dbi.h>
#include <cwt/dbi/ddi.h>
#include <cwt/strlist.h>

static CwtDBI* __dbi = NULL;
static CwtStrListNS* __slNS = NULL;

static const CWT_CHAR *__username = _T("root");
static const CWT_CHAR *__password = _T("rdflhfnehf");
static const CWT_CHAR *__driver   = _T("mysql");
static const CWT_CHAR *__dsn      = _T("DBI:mysql:host=127.0.0.1;port=3306");

/*
CREATE DATABASE sampdb;

DROP TABLE IF EXISTS student;
CREATE TABLE student
(
  name       VARCHAR(20) NOT NULL,
  sex        ENUM('F','M') NOT NULL,
  student_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (student_id)
) ENGINE = InnoDB;


DROP TABLE IF EXISTS grade_event;
CREATE TABLE grade_event
(
  date     DATE NOT NULL,
  category ENUM('T','Q') NOT NULL,
  event_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (event_id)
) ENGINE = InnoDB;


DROP TABLE IF EXISTS score;
CREATE TABLE score
(
  student_id INT UNSIGNED NOT NULL,
  event_id   INT UNSIGNED NOT NULL,
  score      INT NOT NULL,
  PRIMARY KEY (event_id, student_id),
  INDEX (student_id),
  FOREIGN KEY (event_id) REFERENCES grade_event (event_id),
  FOREIGN KEY (student_id) REFERENCES student (student_id)
) ENGINE = InnoDB;

DROP TABLE IF EXISTS absence;
CREATE TABLE absence
(
  student_id INT UNSIGNED NOT NULL,
  date       DATE NOT NULL,
  PRIMARY KEY (student_id, date),
  FOREIGN KEY (student_id) REFERENCES student (student_id)
) ENGINE = InnoDB;

 */

static CwtDDI* __create_ddi(void)
{
	CwtDDI       *ddi;
	CwtDDITable  *tab;
	CwtDDITable  *studentTab;
	CwtDDITable  *eventTab;
	CwtDDIColumn *col;

	ddi = __dbi->createDDI(_T("sampdb"), _T("utf8"));

/*
	CREATE TABLE student (
	  name       VARCHAR(20) NOT NULL,
	  sex        ENUM('F','M') NOT NULL,
	  student_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	  PRIMARY KEY (student_id)
	) ENGINE = InnoDB;
*/
	tab = __dbi->newTable(ddi, _T("student"));

	/*name       VARCHAR(20) NOT NULL*/
	col = __dbi->newColumn(tab, _T("name"));
	__dbi->cTypeText(col, 20);
	__dbi->cNull(col, FALSE); /* by default */

	/*sex        ENUM('F','M') NOT NULL*/
	/* ENUM - MySQL specific type, we have no that type in DDI, so replace it by CHAR */
	col = __dbi->newColumn(tab, _T("sex"));
	__dbi->cTypeText(col, 1);
	__dbi->cNull(col, FALSE); /* by default */

	/*student_id INT UNSIGNED NOT NULL AUTO_INCREMENT*/
	col = __dbi->newColumn(tab, _T("student_id"));
	__dbi->cTypeInt(col, 0ll, (ULONGLONG)CWT_UINT_MAX); /* minimum value is 0 => automatically set unsigned */
	__dbi->cAutoinc(col, 1);  /* AUTO_INCREMENT */
	__dbi->cNull(col, FALSE); /* by default */
	__dbi->cPK(col);          /* PRIMARY KEY (student_id) */

	studentTab = tab;

/*
	CREATE TABLE grade_event (
	  date     DATE NOT NULL,
	  category ENUM('T','Q') NOT NULL,
	  event_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	  PRIMARY KEY (event_id)
	) ENGINE = InnoDB;
*/
	tab = __dbi->newTable(ddi, _T("grade_event"));

	/*date     DATE NOT NULL*/
	col = __dbi->newColumn(tab, _T("date"));
	__dbi->cTypeTime(col, CwtType_DATE, FALSE);

	/*category ENUM('T','Q') NOT NULL*/
	col = __dbi->newColumn(tab, _T("category"));
	__dbi->cTypeText(col, 1);
	__dbi->cNull(col, FALSE); /* by default */

	/*event_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
	  PRIMARY KEY (event_id) */
	col = __dbi->newColumn(tab, _T("event_id"));
	__dbi->cTypeInt(col, 0ll, (ULONGLONG)CWT_UINT_MAX); /* minimum value is 0 => automatically set unsigned */
	__dbi->cAutoinc(col, 1);  /* AUTO_INCREMENT */
	__dbi->cPK(col);          /* PRIMARY KEY (student_id) */
	eventTab = tab;



/*
	CREATE TABLE score (
	  student_id INT UNSIGNED NOT NULL,
	  event_id   INT UNSIGNED NOT NULL,
	  score      INT NOT NULL,
	  PRIMARY KEY (event_id, student_id),
	  INDEX (student_id),
	  FOREIGN KEY (event_id) REFERENCES grade_event (event_id),
	  FOREIGN KEY (student_id) REFERENCES student (student_id)
	) ENGINE = InnoDB;
*/
	tab = __dbi->newTable(ddi, _T("score"));

	col = __dbi->newColumn(tab, _T("student_id"));
	/* student_id INT UNSIGNED NOT NULL */
	/* FOREIGN KEY (student_id) REFERENCES student (student_id) */
	__dbi->cTypeRef(col, studentTab);
	__dbi->cIndex(col, TRUE); /* INDEX (student_id) */

	/*event_id   INT UNSIGNED NOT NULL,
	  FOREIGN KEY (event_id) REFERENCES grade_event (event_id) */
	col = __dbi->newColumn(tab, _T("event_id"));
	__dbi->cTypeRef(col, eventTab);

	col = __dbi->newColumn(tab, _T("score"));
	__dbi->cTypeInt(col, (LONGLONG)CWT_INT_MIN, (ULONGLONG)CWT_INT_MAX); /*score INT NOT NULL*/

/*
	CREATE TABLE absence (
	  student_id INT UNSIGNED NOT NULL,
	  date       DATE NOT NULL,
	  PRIMARY KEY (student_id),
	  FOREIGN KEY (student_id) REFERENCES student (student_id) )
*/
	tab = __dbi->newTable(ddi, _T("absence"));
	col = __dbi->newColumn(tab, _T("student_id"));
	/* student_id INT UNSIGNED NOT NULL */
	/* FOREIGN KEY (student_id) REFERENCES student (student_id) */
	__dbi->cTypeRef(col, studentTab);
	__dbi->cPK(col);

	col = __dbi->newColumn(tab, _T("date")); /* date DATE NOT NULL */
	__dbi->cTypeTime(col, CwtType_DATE, FALSE);


	return ddi;
}

static void test_00(void)
{
	CwtDBIDriver *dbd;
	CwtDDI       *ddi;
	CwtStrList   *deploySpec;
	CwtStrList   *recallSpec;

	dbd = __dbi->load(_T("DBI"), __driver);
	CWT_TEST_FAIL2(dbd, "Unable to load driver represented by DSN");

	ddi = __create_ddi();

	CWT_TEST_OK(deploySpec = dbd->specForDeploy(ddi, CWT_DDI_DEPLOY_DROP_DB | CWT_DDI_DEPLOY_DROP_TAB));
	CWT_TEST_OK(recallSpec = dbd->specForRecall(ddi, 0 /*CWT_DDI_RECALL_DROP_DB*/));

	{
		CwtStrListIterator it;
		__slNS->begin(deploySpec, &it);
		while( __slNS->hasMore(&it) ) {
			printf_debug(_T("Spec: %s"), __slNS->next(&it));
		}
	}

	{
		CwtStrListIterator it;
		__slNS->begin(recallSpec, &it);
		while( __slNS->hasMore(&it) ) {
			printf_debug(_T("Spec: %s"), __slNS->next(&it));
		}
	}

	__slNS->free(deploySpec);
	__slNS->free(recallSpec);
	__dbi->freeDDI(ddi);
}



static void test_01(void)
{
	CwtDDI       *ddi;
	CwtDBHandler *dbh;

	dbh = __dbi->connect(__dsn, __username, __password, NULL);

	CWT_TEST_FAIL2(dbh, "May be you forgot to start MySQL service?");

	ddi = __create_ddi();

	CWT_TEST_OK(__dbi->deploy(dbh, ddi, CWT_DDI_DEPLOY_DROP_DB | CWT_DDI_DEPLOY_DROP_TAB));
	CWT_TEST_OK(__dbi->recall(dbh, ddi, 0)); /*CWT_DDI_RECALL_DROP_DB*/

	__dbi->disconnect(dbh);
	__dbi->freeDDI(ddi);
}



int main(int argc, char *argv[])
{
	__dbi = cwtDBI();
	__slNS = cwtStrListNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(7);

	test_00();
	test_01();

	CWT_END_TESTS;
}


