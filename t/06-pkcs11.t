/*
 * 06-pkcs11.t
 *
 *  Created on: Sep 2, 2011
 *      Author: wladt
 */



JQ_TEST_DEF(pkcs11)
	char path[128];

	COUT << _Tr("Enter pkcs11 module path: ");
	scanf("%127s", path);
	path[127] = '\0';
	JQ_DEBUGF(_Tr("PKCS11 module path is %s"), path);

	jq::Pkcs11* p11 = jq::Pkcs11::open(path);

	JQ_TEST_INLINE_IF(p11)

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

		jq::Pkcs11::close(p11);

	JQ_TEST_ENDIF

JQ_TEST_END
