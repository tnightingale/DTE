#include "datalink.h"

HANDLE OpenPort(LPCWSTR lpFileName) {
	HANDLE hCom;
	DWORD err;
	DWORD inQueue;
	DWORD outQueue;

	// Create handle
	hCom = CreateFile (
		lpFileName,								// LPCSTR lpFileName
		GENERIC_READ | GENERIC_WRITE,		// DWORD dwDesiredAccess
		0,									// DWORD dwShareMode
		NULL,								// LPSECURITY_ATTRIBUTES lpSecurityAttributes
		OPEN_EXISTING,						// DWORD dwCreationDistribution 
		0,									// DWORD dwFlagsAndAttributes
		NULL								// HANDLE hTemplateFile
	);
	
    if (hCom == INVALID_HANDLE_VALUE) {
		return hCom;
	}

	// Setting extra port settings. (NOTE: What do these do?)
	inQueue = outQueue = 1024;
	if(!SetupComm (hCom, inQueue, outQueue)) {
		return hCom;
	}

	return hCom;
}

int Transmit(HANDLE hCom, TCHAR c) {
    DWORD dwBytesToWrite = 1;
    DWORD dwBytesWritten = 0;
    DWORD err;

	// Write
	if (!WriteFile (hCom, &c, dwBytesToWrite, &dwBytesWritten, NULL)) {
		err = GetLastError();
		return 1;
	}

	return 0;
}

BOOL Recieve(HANDLE hCom, TCHAR* readBuff) {
    DWORD dwBytesToRead = 1;
    DWORD dwBytesRead = 0;
        
    if(!ReadFile(hCom, readBuff, dwBytesToRead, &dwBytesRead, NULL)) {
        return FALSE;
    }

    return TRUE;
}
