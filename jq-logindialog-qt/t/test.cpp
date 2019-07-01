/*
 * test.cpp
 *
<<<<<<< HEAD
 *  Created on: Sep 1, 2011
 *      Author: wladt
 */

#include <iostream>
#include <jq/test.h>
#include <jq/logger.hpp>
#include <jq/smartcard.hpp>
#include <jq/pkcs11.hpp>

#include "01-context.t"
//#include "02-connect.t"
//#include "03-waitanyreader.t"
//#include "04-verifypin.t"
//#include "05-features.t"
//#include "06-pkcs11.t"
//#include "07-pkcs11_slots.t"
//#include "08-pkcs11_waitforslot.t"
#include "09-pkcs11_login.t"

using namespace jq;

JQ_TEST_BEGIN
//	char svcName[128];
	jq::String svcName(_T("eTSrv"));
//	JQ_MEMZERO(svcName, 128);
//	COUT << _Tr("Enter service name (or leave empty): ");
//	scanf("%127s", svcName);
//	JQ_DEBUGF(_Tr("Service name is %s"), svcName);
	
	JQ_TEST_INLINE_IF(jq::SmartCardContext::serviceAvailable(svcName.c_str()))
		JQ_TEST(context);
		//JQ_TEST(features);
		//JQ_TEST(connect);
		//JQ_TEST(waitanyreader);
		//JQ_TEST(verifypin);
		//JQ_TEST(pkcs11);
		//JQ_TEST(pkcs11_slots);
		//JQ_TEST(pkcs11_waitforslot);
		JQ_TEST(pkcs11_login);
	JQ_TEST_ENDIF
=======
 *  Created on: Sep 9, 2011
 *      Author: wladt
 */

#include <jq/test.h>

#include "logindialog.t"

JQ_TEST_BEGIN
    JQ_TEST(logindialog);
>>>>>>> jq-logindialog-qt/master
JQ_TEST_FINISH
