/*------------------------------------------------------------------------------------------------------------------ 
 --	SOURCE FILE: datalink.c - Contains functions that directly interact with the serial port.
 -- 
 --	PROGRAM: dte
 -- 
 -- Functions:  
 --             HANDLE OpenPort(LPCWSTR)
 --             int Transmit(HANDLE, TCHAR)
 --             BOOL Recieve(HANDLE, TCHAR*)
 --
 -- DATE:       September 29th 2010
 --
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	NOTES:      Functions to create, transmit and recieve raw data on the port.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/

#include "datalink.h"

/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: OpenPort 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  OpenPort(LPCWSTR lpFileName)
 --                 LPCWSTR lpFileName: The name of the port to open.
 -- 
 --	RETURNS:    HANDLE: Handle to open port.
 -- 
 --	NOTES:      Opens desired port, returns invalid handle if failed.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/
HANDLE OpenPort(LPCWSTR lpFileName) {
	HANDLE hCom;
	DWORD err;

	// Create handle
	hCom = CreateFile (
		lpFileName,							// LPCSTR lpFileName
		GENERIC_READ | GENERIC_WRITE,		// DWORD dwDesiredAccess
		0,									// DWORD dwShareMode
		NULL,								// LPSECURITY_ATTRIBUTES lpSecurityAttributes
		OPEN_EXISTING,						// DWORD dwCreationDistribution 
		FILE_ATTRIBUTE_NORMAL,				// DWORD dwFlagsAndAttributes
		NULL								// HANDLE hTemplateFile
	);
	
    if (hCom == INVALID_HANDLE_VALUE) {
		return hCom;
	}

	// Setting extra port settings. (NOTE: What do these do?)
	//inQueue = outQueue = 1024;
	//if(!SetupComm (hCom, inQueue, outQueue)) {
	//	return hCom;
	//}

	return hCom;
}


/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: Transmit 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  Transmit(HANDLE hCom, TCHAR c)
 --                 HANDLE hCom: The handle to write to.
 --                 TCHAR c: The character to transmit.
 -- 
 --	RETURNS:    int: 1 if error, 0 on success.
 -- 
 --	NOTES:      Writes char to port.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/
int Transmit(HANDLE hCom, TCHAR c) {
    DWORD dwBytesToWrite = 1;
    DWORD dwBytesWritten = 0;
    DWORD err;

	// Write
	if (!WriteFile(hCom, &c, dwBytesToWrite, &dwBytesWritten, NULL)) {
		err = GetLastError();
		return 1;
	}

	return 0;
}


/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: Receive 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  Recieve(HANDLE hCom, TCHAR* readBuff)
 --                 HANDLE hCom: The handle to write to.
 --                 TCHAR*: A buffer to contain recieved char.
 -- 
 --	RETURNS:    FALSE on error, TRUE on success.
 -- 
 --	NOTES:     Reads from open port and writes chars to buffer.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/
TCHAR Recieve(HANDLE hCom) {
    DWORD dwBytesToRead = 1;
    DWORD dwBytesRead = 0;
    TCHAR readBuff = '\0';
    DWORD err;
    
    if(!ReadFile(hCom, &readBuff, dwBytesToRead, &dwBytesRead, NULL)) {
        err = GetLastError();
        return NULL;
    }


    return (TCHAR) readBuff;
}
