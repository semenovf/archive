/*
 * SmartCardContext.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: wladt
 */


#include <jq/smartcard.hpp>

#ifdef JQ_WIN32
#include <jq/logger.hpp>
#include <winsvc.h>

JQ_NS_BEGIN

static void _jq_emitSmartCardError(DWORD rv, const String& msg)
{
	jq_emitError(String().sprintf(_Tr("%s: %s (0x%0lX)"))(msg)(jq_getSysErrorText(rv))(rv)());
}

/**
 * Checks if the service is available
 *
 * @param data as for WinSCard this parameter specifies scard service name
 * @return @c true if the service is available, @c false if otherwise
 */
bool SmartCardContext::serviceAvailable(const void* data) // [static]
{
	if( !data ) {
		jq_emitError(_Tr("service name must be specified"));
		return false;
	}

	LPCTSTR svcName = static_cast<LPCTSTR>(data);

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT); // SC_MANAGER_ALL_ACCESS - for Admin's account only
	if( !hSCManager ) {
		_jq_emitSmartCardError(GetLastError(), _T("open service control manager failed"));
		return false;
	}

	SC_HANDLE hSvc = OpenService( hSCManager, svcName, SERVICE_QUERY_STATUS /*SERVICE_ALL_ACCESS*/ );
	if( !hSvc ) {
		_jq_emitSmartCardError(GetLastError(),
			jq::String().sprintf(_T("open service '%s' failed"))(svcName)() );
		CloseServiceHandle(hSCManager);
		return false;
	}


	// Check the status in case the service is not stopped.
	DWORD bytesNeeded;
	SERVICE_STATUS_PROCESS svcStatus;
    if( !QueryServiceStatusEx(
			hSvc,
			SC_STATUS_PROCESS_INFO,
			(LPBYTE) &svcStatus,
			sizeof(SERVICE_STATUS_PROCESS),
			&bytesNeeded) ) {

		_jq_emitSmartCardError(GetLastError(), _T("query service status failed"));
        CloseServiceHandle(hSvc);
        CloseServiceHandle(hSCManager);
        return false;
    }

	// Save the tick count and initial ceckpoint.

    DWORD startTickCount = GetTickCount();
    DWORD oldCheckPoint = svcStatus.dwCheckPoint;

	// wait for service start complete
	while( svcStatus.dwCurrentState == SERVICE_START_PENDING ) {
		// Do not wait longer than the wait hint. A good interval is
        // one-tenth of the wait hint but not less than 1 second
        // and not more than 10 seconds.
		DWORD waitTime = svcStatus.dwWaitHint / 10;
		if( waitTime < 1000 )
            waitTime = 1000;
        else if ( waitTime > 10000 )
            waitTime = 10000;

        Sleep( waitTime );

        // Check the status until the service is no longer stop pending.

        if (!QueryServiceStatusEx(
                hSvc,
				SC_STATUS_PROCESS_INFO,
				(LPBYTE) &svcStatus,
                sizeof(SERVICE_STATUS_PROCESS),
				&bytesNeeded ) ) {

        	_jq_emitSmartCardError(GetLastError(), _T("query service status failed"));
			CloseServiceHandle(hSvc);
			CloseServiceHandle(hSCManager);
            return false;
        }

        if ( svcStatus.dwCheckPoint > oldCheckPoint ) {
            // Continue to wait and check.
            startTickCount = GetTickCount();
            oldCheckPoint = svcStatus.dwCheckPoint;
        } else {
            if( GetTickCount()-startTickCount > svcStatus.dwWaitHint ) {
            	jq_emitError( jq::String().sprintf(_T("timeout waiting for service '%s' to start"))(svcName)());
                CloseServiceHandle(hSvc);
                CloseServiceHandle(hSCManager);
                return false;
            }
        }
	}

	if( svcStatus.dwCurrentState != SERVICE_RUNNING ) {
		return false;
	}
	return true;
}

JQ_NS_END

#endif // JQ_WIN32
