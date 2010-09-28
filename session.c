#include "application.h"
#include "session.h"
#include "datalink.h"

LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT paintstruct;
	TEXTMETRIC tm;
	SIZE size;
    PWDATA pWData;

    //static HANDLE hCom;
    //static enum STATE state; 

	switch (Message) {
        //case WM_CREATE:
        //    state = COMMAND;
        //    break;

		case WM_COMMAND:
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            //if (state != COMMAND) {
            if (pWData->state != COMMAND) {
                MessageBox (hwnd, TEXT("Need to be in COMMAND to change settings."), NULL, MB_ICONERROR);
                break;
            }

			switch (LOWORD (wParam)) {
          		case IDM_COM1:
                    //hCom = ConnectComm(hwnd, TEXT("COM1"));
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM1"));
					break;
				
				case IDM_COM2:
                    //hCom = ConnectComm(hwnd, TEXT("COM2"));
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM2"));
					break;

                case IDM_COM3:
                    //hCom = ConnectComm(hwnd, TEXT("COM3"));
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM3"));
					break;

                case IDM_COM4:
                    //hCom = ConnectComm(hwnd, TEXT("COM4"));
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM4"));
					break;

                case IDM_COM5:
                    //hCom = ConnectComm(hwnd, TEXT("COM5"));
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM5"));
					break;

                case IDM_COM6:
                    //hCom = ConnectComm(hwnd, TEXT("COM6"));
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM6"));
					break;

                case IDM_CONNECT:
                    pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
                    //state = CONNECT;
                    pWData->state = CONNECT;
                    //if (!StartReadThread(hCom, pWData)) {
                    if (!StartReadThread(pWData->hCom, pWData)) {
                        //state = COMMAND;
                        pWData->state = COMMAND;
                        MessageBox (hwnd, TEXT("ERROR: Couldn't start thread."), NULL, MB_ICONERROR);
                        return 0;
                    }

                    break;
			}
			break;

        case WM_CHAR:
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            //if (state != CONNECT) {
            if (pWData->state != CONNECT) {
                break;
            }

            if (wParam == VK_ESCAPE) {
                EndThread(pWData);
            }

            //Transmit(hCom, wParam);
            Transmit(pWData->hCom, wParam);
            break;

		case WM_PAINT:		// Process a repaint message
			hdc = BeginPaint (hwnd, &paintstruct); // Acquire DC

			EndPaint(hwnd, &paintstruct);
		break;

		case WM_DESTROY:	// Terminate program
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (pWData->bReading) {
                EndThread(pWData);
            }
      		PostQuitMessage(0);
		break;
		
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

HANDLE ConnectComm(HWND hwnd, LPWSTR lpFileName) {
    HANDLE hCom;
    DWORD err;
    BOOL success;
    COMMCONFIG cc;

    hCom = OpenPort(lpFileName);

    cc.dwSize = sizeof(COMMCONFIG);
    GetCommConfig(hCom, &cc, &cc.dwSize);
    CommConfigDialog(lpFileName, hwnd, &cc);

    success = SetCommState(hCom, &cc.dcb);
	if (!success) {
	    err = GetLastError();
		return NULL;
	}

    return hCom;
}

BOOL StartReadThread(HANDLE hCom, PWDATA pWData) {

/* create the thread for reading bytes */
    pWData->bReading = TRUE;
	pWData->hThread = CreateThread(
        NULL, /*def security */ 
        0, /* def stack size */ 
        (LPTHREAD_START_ROUTINE) ReadThread, 
        //hCom, /* param to pass to thread */
        pWData,
        0, 
        &pWData->threadId
    );
	if (pWData->hThread == INVALID_HANDLE_VALUE) {
        return FALSE;
	} 
	/* end if (error creating read thread) */

    return TRUE;
}

//BOOL ReadThread(HANDLE hCom) {
BOOL ReadThread(PWDATA pWData) {
    char* readBuff = (char*) malloc(sizeof(char[1]));
    *readBuff = NULL;

    while (pWData->state == CONNECT && pWData->bReading) {
        Sleep(10);
        if (!Recieve(pWData->hCom, readBuff)) {
            return FALSE;
        }
        if (*readBuff == NULL) {
            continue;
        }
        
        printChar(readBuff, pWData);
        *readBuff = NULL;
    }

    return TRUE;
}

void EndThread(PWDATA pWData) {
    /* kill the read thread */
    pWData->state = COMMAND;
    pWData->bReading = FALSE;
    /* in case the thread is not running, resume it now */
    ResumeThread(pWData->hThread);
    
    /* wait for thread to die...  */
    while (GetExitCodeThread(pWData->hThread, &pWData->threadId)) {
        if (pWData->threadId == STILL_ACTIVE) {
            continue;
		} else {
            break;
        }
    }
    /* end while (no error reading thread exit code) */	
    CloseHandle (pWData->hThread);
}
