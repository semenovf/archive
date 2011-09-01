/*
 * 04-verifypin.t
 *
 *  Created on: Sep 1, 2011
 *      Author: wladt
 */
JQ_TEST_DEF(verifypin)
	jq::SmartCardContext sc_context;
//	bool timedout = false;
	int index;

	JQ_TEST_INLINE_IF(sc_context.isValid())
		sc_context.updateReaders();

		int nreaders = sc_context.readersCount();
		for( int i = 0; i < nreaders; i++ ) {
			COUT << i << ": " << sc_context.readerAt(i).c_str() << std::endl;
		}
		printf("Select reader: ");
		scanf("%d", &index);

		JQ_TEST_INLINE_IF( index >= 0 && index < nreaders )
			JQ_DEBUGF("Your choice is %d", index);
			jq::SmartCard sc_card(sc_context, sc_context.readerAt(index));
			sc_card.connect(jq::SmartCard::SM_Shared, jq::SmartCard::Proto_Any);

			JQ_TEST_INLINE2(sc_card.verifyPIN(), _Tr("PIN verification successful"));
		JQ_TEST_ENDIF

		if( sc_context.isError() ) {
			JQ_ERROR(sc_context.getLastErrorText().c_str());
		}
	JQ_TEST_ENDIF

JQ_TEST_END
