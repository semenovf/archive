/*
 * 02-connect.t
 *
 *  Created on: Sep 1, 2011
 *      Author: wladt
 */
JQ_TEST_DEF(connect)

	int index = 0;
	jq::SmartCardContext sc_context;
	sc_context.updateReaders();

	int nreaders = sc_context.readersCount();
	for( int i = 0; i < nreaders; i++ ) {
		COUT << "Reader " << i << ": " << sc_context.readerAt(i).c_str() << std::endl;
	}

	if( nreaders > 0 ) {
		jq::SmartCard **sc_cards = new jq::SmartCard*[nreaders];
		for( int i = 0; i < nreaders; i++ ) {
			sc_cards[i] = new jq::SmartCard(sc_context, sc_context.readerAt(i));
			sc_cards[i]->connect(jq::SmartCard::SM_Shared, jq::SmartCard::Proto_Any);
		}


		if( index >= 0 && index < nreaders ) {
			jq::SmartCardStatus sc_status;
			if( sc_cards[index]->status(sc_status) ) {
				sc_status.dump(COUT);
			}
		}

		for( int i = 0; i < nreaders; i++ ) {
			for( int i = 0; i < nreaders; i++ ) {
				delete sc_cards[i];
			}
		}
		delete[] sc_cards;
	}

	if( sc_context.isError() ) {
		JQ_ERROR(sc_context.getLastErrorText().c_str());
	}

JQ_TEST_END
