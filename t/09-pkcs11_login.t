/*
 * 09-pkcs11_login.t
 *
 *  Created on: Sep 6, 2011
 *      Author: wladt
 */


JQ_TEST_DEF(pkcs11_login)
	int index;
	char path[128];

	COUT << _Tr("Enter pkcs11 module path: ");
	scanf("%127s", path);
	path[127] = '\0';
	JQ_DEBUGF(_Tr("PKCS11 module path is %s"), path);

	jq::Pkcs11* p11 = jq::Pkcs11::open(path);

	JQ_TEST_INLINE_IF(p11)

		p11->updateSlots();

		JQ_TEST_INLINE_IF(p11->slotsCount())

			COUT << _Tr("Slots available:") << std::endl;
			int nslots = p11->slotsCount();
			for( int i = 0; i < nslots; i++ ) {
				CK_SLOT_INFO slotInfo;
				if( p11->slotInfo((uint_t)i, slotInfo) )
					COUT << i << ": " << slotInfo.slotDescription << std::endl;
			}

			COUT << _Tr("Select slot: ");
			scanf("%d", &index);

			bool statusChanged = false;
			JQ_DEBUG(_Tr("Please, remove/insert token for a while..."));
			int loop=5;

			while(loop) {
				p11->waitForSlot(index, &statusChanged, true);
				if( statusChanged ) {
					if( p11->isTokenPresent(index) ) {
						JQ_TRACE(_Tr("Status changed: token present"));
						jq::Pkcs11Session* session = p11->openSession(index);

						JQ_TEST_INLINE_IF(session)
							char pass[128];

							COUT << _Tr("Enter user PIN: ");
							scanf("%127s", pass);
							pass[127] = '\0';

							JQ_TEST_INLINE2(session->login(pass), _Tr("Logged in is ok"));

							session->logout();
							p11->closeSession(session);
							JQ_TEST_INLINE2(session==NULL, _Tr("session closed and is NULL'ed"));
						JQ_TEST_ENDIF
					} else {
						JQ_TRACE(_Tr("Status changed: token absent"));
					}
					loop--;
				}
			}

		JQ_TEST_ENDIF

		jq::Pkcs11::close(p11);

	JQ_TEST_ENDIF

JQ_TEST_END

/*JQ_TEST_DEF(pkcs11_login)

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

			char pass[128];

			COUT << _Tr("Enter user PIN: ");
			scanf("%127s", pass);
			pass[127] = '\0';

			JQ_TEST_INLINE2(session->login(pass), _Tr("Logged in is ok"));
			session->logout();

			p11->closeSession(session);
			JQ_TEST_INLINE2(session==NULL, _Tr("session closed and is NULL'ed"));


			JQ_DEBUG(_Tr("Entering into loop while status will not be changed..."));
			JQ_DEBUG(_Tr("Remove or insert the token"));
			bool statusChanged = false;
			while(!statusChanged) {
				p11->waitForSlot(index, &statusChanged, true);
			}

			JQ_DEBUG(_Tr("Please, remove/insert token for a while..."));
			int loop=5;
			while(loop) {
				p11->waitForSlot(index, &statusChanged, true);
				if( statusChanged ) {
					if( p11->isTokenPresent(index) )
						JQ_TRACE(_Tr("Status changed: token present"));
					else
						JQ_TRACE(_Tr("Status changed: token absent"));
					loop--;
				}
			}

		JQ_TEST_ENDIF

		jq::Pkcs11::close(p11);

	JQ_TEST_ENDIF

JQ_TEST_END*/

