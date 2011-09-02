/*
 * 06-pkcs11.t
 *
 *  Created on: Sep 2, 2011
 *      Author: wladt
 */
JQ_TEST_DEF(pkcs11)
	jq::SmartCardContext sc_context;
	int index;
	char path[128];

	JQ_TEST_INLINE_IF(sc_context.isValid())
		sc_context.updateReaders();

		COUT << _Tr("Readers available:") << std::endl;
		int nreaders = sc_context.readersCount();
		for( int i = 0; i < nreaders; i++ ) {
			COUT << i << ": " << sc_context.readerAt(i).c_str() << std::endl;
		}
		COUT << _Tr("Select reader: ");
		scanf("%d", &index);
		COUT << _Tr("Enter pkcs11 module path: ");
		scanf("%127s", path);
		path[127] = '\0';

		JQ_TEST_INLINE_IF( index >= 0 && index < nreaders )
			JQ_DEBUGF("Your choice is %d", index);
			JQ_DEBUGF("PKCS11 module path is %s", path);
			jq::SmartCard sc_card(sc_context, sc_context.readerAt(index));
			sc_card.connect(jq::SmartCard::SM_Shared, jq::SmartCard::Proto_Any);

			jq::P11 *p11 = sc_card.p11Create(path);
			JQ_TEST_INLINE_IF( p11 )
				uchar_t ver_major, ver_minor;
				p11->version(ver_major, ver_minor);
				JQ_DEBUGF("Version: %d.%d", ver_major, ver_minor);
				sc_card.p11Destroy(p11);
			JQ_TEST_ENDIF
		JQ_TEST_ENDIF

		if( sc_context.isError() ) {
			int nerrors = sc_context.errorCount();
			for( int i = 0; i < nerrors; i++ )
				JQ_ERROR(sc_context.errorTextAt(i).c_str());
		}
	JQ_TEST_ENDIF

JQ_TEST_END
