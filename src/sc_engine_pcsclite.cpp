/*
 * sc_engine_pcsclite.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: wladt
 */

#include <jq/smartcard.hpp>

#ifdef PCSCD

#include <sys/stat.h>
#include <jq/logger.hpp>
#include <PCSC/reader.h>


#ifndef PCSCLITE_CSOCK_NAME
	PCSCLITE_CSOCK_NAME "<PCSCLITE_daemon_not_found>"
#endif


JQ_NS_BEGIN

/**
 * Checks if the service is available
 *
 * @param data as for PCSC lite daemon this parameter specifies non-standard path to daemon socket
 * @return @c true if the service is available, @c false if otherwise
 */
bool SmartCardContext::serviceAvailable(const void* data) // [static]
{
	struct stat statBuffer;
	//static char socketName[sizeof(struct sockaddr_un)];
	static std::string socketName;

	if( socketName.length() == 0 ) {
		/* socket name not yet initialized */
		char *socketNameEnv;

		if( data ) {
			socketName = static_cast<const char*>(data);
		} else {
			socketNameEnv = getenv("PCSCLITE_CSOCK_NAME");
			if (socketNameEnv) {
				socketName = socketNameEnv;
			} else {
				socketName = PCSCLITE_CSOCK_NAME;
			}
		}
	}

	int rv = stat(socketName.c_str(), &statBuffer);

	if (rv != 0) {
		return false;
	}

	return true;
}

JQ_NS_END

#endif // PCSCD
