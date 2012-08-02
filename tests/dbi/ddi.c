/*
 * ddi.c
 *
 *  Created on: 02.08.2012
 *      Author: user
 */


#include <cwt/test.h>
#include <cwt/dbi/dbi.h>
#include <cwt/dbi/ddi.h>

static CwtDBI* __dbi = NULL;

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

static void test_00(void)
{
	CwtDDI       *ddi;
	CwtDDITable  *tab;
	CwtDDIColumn *col;

	ddi = __dbi->createDDI(_T("sampdb"));

	tab = __dbi->newTable(ddi, _T("student"));

	/*name       VARCHAR(20) NOT NULL*/
	col = __dbi->newColumn(tab, _T("name"));

	/*sex        ENUM('F','M') NOT NULL*/

	/*student_id INT UNSIGNED NOT NULL AUTO_INCREMENT*/

	/*PRIMARY KEY (student_id)*/


	tab = __dbi->newTable(ddi, _T("grade_event"));
	tab = __dbi->newTable(ddi, _T("score"));
	tab = __dbi->newTable(ddi, _T("absence"));

	__dbi->freeDDI(ddi);
}


int main(int argc, char *argv[])
{
	__dbi = cwtDBI();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(1);

	CWT_END_TESTS;
}


