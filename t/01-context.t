/*
 * 01-context.t
 *
 *  Created on: Sep 1, 2011
 *      Author: wladt
 */
JQ_TEST_DEF(context)

	jq::SmartCardContext sc_context;
	sc_context.updateReaders();

	int nreaders = sc_context.readersCount();
	for( int i = 0; i < nreaders; i++ ) {
		COUT << "Reader " << i << ": " << sc_context.readerAt(i).c_str() << std::endl;
	}

	JQ_TEST_INLINE2(sc_context.isValid(), "smartcard context is valid");
JQ_TEST_END
