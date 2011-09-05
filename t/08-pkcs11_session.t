/*
 * 08-pkcs11_session.t
 *
 *  Created on: Sep 2, 2011
 *      Author: wladt
 */

JQ_TEST_DEF(pkcs11_session)
	int index;
	char path[128];

	COUT << _Tr("Enter pkcs11 module path: ");
	scanf("%127s", path);
	path[127] = '\0';
	JQ_DEBUGF(_Tr("PKCS11 module path is %s"), path);

	jq::Pkcs11* p11 = jq::Pkcs11::open(path);

	JQ_TEST_INLINE_IF(p11)

		p11->updateSlots();

		COUT << _Tr("Slots available:") << std::endl;
		int nslots = p11->slotsCount();
		for( int i = 0; i < nslots; i++ ) {
			CK_SLOT_INFO slotInfo;
			if( p11->slotInfo((uint_t)i, slotInfo) )
				COUT << i << ": " << slotInfo.slotDescription << std::endl;
		}
		COUT << _Tr("Select slot: ");
		scanf("%d", &index);

		jq::Pkcs11Session* session = p11->openSession(index);

		JQ_TEST_INLINE_IF(session)
			p11->closeSession(session);
			session = NULL;
		JQ_TEST_ENDIF

		jq::Pkcs11::close(p11);

	JQ_TEST_ENDIF

JQ_TEST_END

/*JQ_TEST_DEF(pkcs11)
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
			JQ_DEBUGF(_Tr("PKCS11 module path is %s"), path);
			jq::SmartCard sc_card(sc_context, sc_context.readerAt(index));
			sc_card.connect(jq::SmartCard::SM_Shared, jq::SmartCard::Proto_Any);

			jq::Pkcs11 *p11 = sc_card.pkcs11Create(path);
			JQ_TEST_INLINE_IF( p11 )
				uchar_t ver_major, ver_minor;
				p11->version(ver_major, ver_minor);
				JQ_DEBUGF(_Tr("Version: %d.%d"), ver_major, ver_minor);


				uchar_t cryptokiMajor;
				uchar_t cryptokiMinor;
				jq::String manufacturerId;
				jq::String libDesc;
				uchar_t libMajor;
				uchar_t libMinor;
				p11->info( &cryptokiMajor, &cryptokiMinor, &manufacturerId, &libDesc, &libMajor, &libMinor);
				JQ_DEBUGF(_Tr("Cryptoki version: %d.%d"), cryptokiMajor, cryptokiMinor);
				JQ_DEBUGF(_Tr("Manufacturer: \"%s\""), manufacturerId.c_str());
				JQ_DEBUGF(_Tr("Library: \"%s\" version %d.%d"), libDesc.c_str(), libMajor, libMinor);

				sc_card.pkcs11Destroy(p11);
			JQ_TEST_ENDIF
		JQ_TEST_ENDIF

		if( sc_context.isError() ) {
			int nerrors = sc_context.errorCount();
			for( int i = 0; i < nerrors; i++ )
				JQ_ERROR(sc_context.errorTextAt(i).c_str());
		}
	JQ_TEST_ENDIF

JQ_TEST_END*/
