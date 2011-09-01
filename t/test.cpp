/*
 * test.cpp
 *
 *  Created on: Sep 1, 2011
 *      Author: wladt
 */

#include <iostream>
#include <jq/test.h>
#include <jq/smartcard.hpp>

#include "01-context.t"
#include "02-connect.t"
#include "03-waitanyreader.t"
#include "04-verifypin.t"
#include "05-features.t"

using namespace jq;

JQ_TEST_BEGIN
	//JQ_TEST(context);
	JQ_TEST(features);
	//JQ_TEST(connect);
	//JQ_TEST(waitanyreader);
	//JQ_TEST(verifypin);
JQ_TEST_FINISH
