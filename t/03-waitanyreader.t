/*
 * 03-waitanyreader.t
 *
 *  Created on: Sep 1, 2011
 *      Author: wladt
 */
JQ_TEST_DEF(waitanyreader)
	jq::SmartCardContext sc_context;
	bool timedout = false;

	JQ_TEST_INLINE2(sc_context.isValid(), _Tr("smart card context is valid"));

	JQ_INFO(_Tr("Do three removal/insertion iterations of any reader..."));
	int i = 3;
	while( i-- ) {
		JQ_TEST_INLINE2(sc_context.waitAnyReader(INFINITE, &timedout), _Tr("wait for reader in infinite loop"));
		if( timedout ) {
			JQ_WARN(_Tr("waitAnyReader() timed out"));
		}
	}
	if( sc_context.isError() ) {
		JQ_ERROR(sc_context.getLastErrorText().c_str());
	}

JQ_TEST_END
