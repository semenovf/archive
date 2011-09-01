/*
 * 05-features.t
 *
 *  Created on: Sep 1, 2011
 *      Author: wladt
 */
JQ_TEST_DEF(features)
	jq::SmartCardContext sc_context;

	JQ_TEST_INLINE_IF(sc_context.isValid())
		sc_context.updateReaders();

		int nreaders = sc_context.readersCount();
		for( int i = 0; i < nreaders; i++ ) {
			COUT << "Reader " << i << ": " << sc_context.readerAt(i).c_str() << std::endl;
			jq::SmartCard sc_card(sc_context, sc_context.readerAt(i));

			JQ_TEST_INLINE_IF(sc_card.connect(jq::SmartCard::SM_Shared, jq::SmartCard::Proto_Any))

				bool b = sc_card.verifyPIN();
				b = b;
				jq::SmartCardFeatures sc_features;
				JQ_TEST_INLINE_IF(sc_card.features(sc_features))
					sc_features.dump(COUT);
				JQ_TEST_ENDIF
			JQ_TEST_ENDIF
		}

		if( sc_context.isError() ) {
			int nerrors = sc_context.errorCount();
			for( int i = 0; i < nerrors; i++ )
				JQ_ERROR(sc_context.errorTextAt(i).c_str());
		}
	JQ_TEST_ENDIF

JQ_TEST_END
