/*------------------------------------------------------------------------------------------------------------------ 
 --	SOURCE FILE: session.c - Handles all messages via WndProc().
 -- 
 --	PROGRAM: dte
 -- 
 -- Functions:  
 --             LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
 --             HANDLE ConnectComm(HWND hwnd, LPCWSTR lpFileName);
 --             void pollPort(PWDATA pWData);
 --             BOOL outputAddChar(TCHAR c, POUTPUT pOutput);
 --
 -- DATE:       September 29th 2010
 --
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	NOTES:      All messages sent to programs window are handled in this file. Also has wrapper function to connect, 
 --             configure & initialize comm port.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/

#include "application.h"
#include "session.h"
#include "datalink.h"

/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: WndProc 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
 --                 HWND hwnd
 --                     A handle to the window.
 --                 UINT Message
 --                     The message.
 --                 WPARAM wParam
 --                     Additional message information. The contents of this parameter depend on the value of the 
 --                     uMsg parameter.
 --                 LPARAM lParam
 --                     Additional message information. The contents of this parameter depend on the value of the 
 --                     uMsg parameter.
 -- 
 --	RETURNS:    LRESULT - The return value is the result of the message processing and depends on the message sent.
 -- 
 --	NOTES:      Win32 api main message handling function. All messages come through here and action is taken.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/

LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT paintstruct;
    PWDATA pWData;
	HMENU menu;

	switch (Message) {

        case WM_SIZE:
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            pWData->wnSize.cx = LOWORD(lParam);
            pWData->wnSize.cy = HIWORD(lParam);
        
			hdc = GetDC(hwnd);
            printOut(pWData, hdc);
			ReleaseDC(hwnd, hdc);
            break;

		case WM_COMMAND:
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (pWData->state != COMMAND) {
                MessageBox (hwnd, TEXT("Need to be in COMMAND to change settings."), NULL, MB_ICONERROR);
                break;
            }

			switch (LOWORD (wParam)) {
          		case IDM_COM1:
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM1"));
					break;
				
				case IDM_COM2:
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM2"));
					break;

                case IDM_COM3:
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM3"));
					break;

                case IDM_COM4:
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM4"));
					break;

                case IDM_COM5:
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM5"));
					break;

                case IDM_COM6:
                    pWData->hCom = ConnectComm(hwnd, TEXT("COM6"));
					break;

                case IDM_CONNECT:
                    pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
					if (pWData->hCom == INVALID_HANDLE_VALUE) {
						MessageBox(pWData->hwnd, TEXT("You need to set a port before you can connect."), NULL, MB_ICONERROR);
						break;
					}
						
					pWData->state = CONNECT;
					
					menu = GetMenu(hwnd);
					setMenu(menu, MF_GRAYED);
                    break;
			}
			break;

        case WM_CHAR:
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            
            if (pWData->state != CONNECT) {
                break;
            }

            if (wParam == VK_ESCAPE) {
                pWData->state = COMMAND;
				CloseHandle(pWData->hCom);
				menu = GetMenu(hwnd);
				setMenu(menu, MF_ENABLED);
				break;
            }

            Transmit(pWData->hCom, wParam);
            if (!outputAddChar((TCHAR) wParam, &(pWData->output))) {
                MessageBox(pWData->hwnd, TEXT("MASSIVE ERROR."), NULL, MB_ICONERROR);
                CloseHandle(pWData->hCom);
                PostQuitMessage(0);
            }
            hdc = GetDC(hwnd);
            printOut(pWData, hdc);
            ReleaseDC(hwnd, hdc);
            break;

		case WM_PAINT:		// Process a repaint message
			pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            hdc = BeginPaint (hwnd, &paintstruct); // Acquire DC            
           
            printOut(pWData, hdc);
			
            EndPaint(hwnd, &paintstruct);
		break;

		case WM_DESTROY:	// Terminate program
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (pWData->state == CONNECT) {
                CloseHandle(pWData->hCom);
            }
      		PostQuitMessage(0);
		    break;
		
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}

	return 0;
}


/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: ConnectComm 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  ConnectComm(HWND hwnd, LPCWSTR lpFileName)
 --                 HWND hwnd: The window.
 --                 LPCWSTR lpFileName: The name of the Comm port.
 -- 
 --	RETURNS:    HANDLE: Handle to newly opened and configured Comm port.
 -- 
 --	NOTES:      Opens the desired Comm port via OpenPort() and then sets timeout & other protocol config.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/

HANDLE ConnectComm(HWND hwnd, LPCWSTR lpFileName) {
    HANDLE hCom;
    DWORD err;
    BOOL success;
    COMMCONFIG cc;
    COMMTIMEOUTS CommTimeouts;

    hCom = OpenPort(lpFileName);

    cc.dwSize = sizeof(COMMCONFIG);
    GetCommConfig(hCom, &cc, &cc.dwSize);
    CommConfigDialog(lpFileName, hwnd, &cc);

    success = SetCommState(hCom, &cc.dcb);
	if (!success) {
	    err = GetLastError();
		return NULL;
	}

    if(!GetCommTimeouts(hCom, &CommTimeouts)) {
    }

    CommTimeouts.ReadIntervalTimeout = 100;
    CommTimeouts.ReadTotalTimeoutMultiplier = 2;
    CommTimeouts.ReadTotalTimeoutConstant = 1000;

    CommTimeouts.WriteTotalTimeoutMultiplier = 2;
    CommTimeouts.WriteTotalTimeoutConstant = 1;

    SetCommTimeouts(hCom, &CommTimeouts);


    return hCom;
}


/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: pollPort 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  pollPort(PWDATA pWData)
 --                 PWDATA pWData: The window's stored data object.
 -- 
 --	RETURNS:    void.
 -- 
 --	NOTES:      Checks the open port for any new data, if received and not NULl, sends data to output string and calls 
 --             a printOut to render it on screen.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/

void pollPort(PWDATA pWData) {
    HDC hdc;
    TCHAR readBuff = '\0';

    if (!Recieve(pWData->hCom, &readBuff)) {
        return;
    }

    if (readBuff == (TCHAR) NULL) {
        return;
    }

    if (!outputAddChar(readBuff, &(pWData->output))) {
        MessageBox (pWData->hwnd, TEXT("MASSIVE ERROR."), NULL, MB_ICONERROR);
        CloseHandle(pWData->hCom);
        PostQuitMessage(0);
    }

    hdc = GetDC(pWData->hwnd);
    printOut(pWData, hdc);
    ReleaseDC(pWData->hwnd, hdc);
}


/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: outputAddChar 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  outputAddChar(TCHAR c, POUTPUT pOutput)
 --                 TCHAR c: A char to add to the output string.
 --                 POUTPUT pOutput: The programs output string (dynamically resizing struct).
 -- 
 --	RETURNS:    BOOL.
 --                 Returns true if char is added to string sucessfully.
 --                 Returns false if if failed (probably due to failed memory allocation when resizing the string).   
 -- 
 --	NOTES:      Tries to add a char to the output string, if string is full, the string is doubled in size and then 
 --             char is added.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/

BOOL outputAddChar(TCHAR c, POUTPUT pOutput) {
	DWORD err;
	TCHAR* tmp;

    if (pOutput->pos == pOutput->size -1) {
        pOutput->size = pOutput->size * 2;

		tmp = (TCHAR*) HeapReAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, pOutput->out, sizeof(TCHAR) * pOutput->size);
		if (tmp == NULL) {
			err = GetLastError();
			return FALSE;
		}
		FillMemory(tmp + pOutput->pos, pOutput->size - pOutput->pos, ' ');
		pOutput->out = tmp;

        if (pOutput->out == NULL) {
            return FALSE;
        }
    }

    pOutput->out[pOutput->pos++] = c;

    return TRUE;
}