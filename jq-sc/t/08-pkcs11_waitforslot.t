/*
 * 08-pkcs11_waitforslot.t
 *
 *  Created on: Sep 6, 2011
 *      Author: wladt
 */

JQ_TEST_DEF(pkcs11_waitforslot)
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

JQ_TEST_END

