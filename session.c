#include "application.h"
#include "session.h"
#include "datalink.h"

LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT paintstruct;
	//SIZE size;
    PWDATA pWData;

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
                    //if (!StartReadThread(pWData->hCom, pWData)) {
                        //state = COMMAND;
                    //    pWData->state = COMMAND;
                    //    MessageBox (hwnd, TEXT("ERROR: Couldn't start thread."), NULL, MB_ICONERROR);
                    //    return 0;
                    //}

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
                //EndThread(pWData);
                pWData->state = COMMAND;
            }

            //Transmit(hCom, wParam);
            Transmit(pWData->hCom, wParam);
            if (!outputAddChar((TCHAR) wParam, &(pWData->output))) {
                MessageBox (pWData->hwnd, TEXT("MASSIVE ERROR."), NULL, MB_ICONERROR);
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

    CommTimeouts.ReadIntervalTimeout = 1000;
    CommTimeouts.ReadTotalTimeoutMultiplier = 2;
    CommTimeouts.ReadTotalTimeoutConstant = 1000;

    CommTimeouts.WriteTotalTimeoutMultiplier = 2;
    CommTimeouts.WriteTotalTimeoutConstant = 10;

    SetCommTimeouts(hCom, &CommTimeouts);


    return hCom;
}

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

BOOL outputAddChar(TCHAR c, POUTPUT pOutput) {

    if (pOutput->pos == pOutput->size -1) {
        pOutput->size = pOutput->size * 2;

        pOutput->out = (TCHAR*) realloc(pOutput->out, pOutput->size * sizeof(TCHAR));
        if (pOutput->out == NULL) {
            return FALSE;
        }
    }

    pOutput->out[pOutput->pos++] = c;

    return TRUE;
}