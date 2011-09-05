/*
 * test.cpp
 *
 *  Created on: Sep 1, 2011
 *      Author: wladt
 */

#include <iostream>
#include <jq/test.h>
#include <jq/smartcard.hpp>
#include <jq/pkcs11.hpp>

#include "01-context.t"
#include "02-connect.t"
#include "03-waitanyreader.t"
#include "04-verifypin.t"
#include "05-features.t"
#include "06-pkcs11.t"
#include "07-pkcs11_slots.t"
#include "08-pkcs11_session.t"

using namespace jq;

JQ_TEST_BEGIN
	//JQ_TEST(context);
	//JQ_TEST(features);
	//JQ_TEST(connect);
	//JQ_TEST(waitanyreader);
	//JQ_TEST(verifypin);
	//JQ_TEST(pkcs11);
	//JQ_TEST(pkcs11_slots);
	JQ_TEST(pkcs11_session);
JQ_TEST_FINISH
