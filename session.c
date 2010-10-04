/*------------------------------------------------------------------------------------------------------------------ 
 --	SOURCE FILE: session.c - Handles all messages via WndProc().
 -- 
 --	PROGRAM: dte
 -- 
 -- Functions:  
 --             LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
 --             HANDLE ConnectComm(HWND, LPCWSTR);
 --             void pollPort(HWND, PWDATA);
 --             BOOL outputAddChar(TCHAR, POUTPUT);
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
    TEXTMETRIC tm;
    HFONT hFont;
    int i;

	switch (Message) {

        case WM_SETFOCUS:
            hdc = GetDC(hwnd);
            hFont = (HFONT) GetStockObject(ANSI_FIXED_FONT);
            SelectObject(hdc, hFont);
            GetTextMetrics(hdc, &tm);
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            
            pWData->cursor.cxChar = tm.tmMaxCharWidth;
            pWData->cursor.cyChar = tm.tmHeight;

            if (pWData->state == CONNECT) {
                // Create a solid black caret. 
                CreateCaret(hwnd, NULL, 2, pWData->cursor.cyChar); 
                SetCaretPos(pWData->cursor.xCaret * pWData->cursor.cxChar, pWData->cursor.yCaret * pWData->cursor.cyChar);
                // Display the caret. 
                ShowCaret(hwnd);
            }
            break;

        case WM_KILLFOCUS: 
            // The window is losing the keyboard focus, so destroy the caret. 
            DestroyCaret(); 
            break;

        case WM_SIZE:
            hdc = GetDC(hwnd);
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);

            pWData->cursor.cxBuffer = max(1, LOWORD(lParam) / pWData->cursor.cxChar);
            pWData->cursor.cyBuffer = max(1, HIWORD(lParam) / pWData->cursor.cyChar);
            
            pWData->cursor.xCaret = pWData->pOutput->pos % pWData->cursor.cxBuffer;
            pWData->cursor.yCaret = max(0, pWData->pOutput->pos / pWData->cursor.cxBuffer);
			SetCaretPos(pWData->cursor.xCaret * pWData->cursor.cxChar, pWData->cursor.yCaret * pWData->cursor.cyChar);

            InvalidateRect(hwnd, NULL, FALSE);
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
					// Only connect when a valid Comm port is open.
                    if (pWData->hCom == INVALID_HANDLE_VALUE) {
						MessageBox(hwnd, TEXT("You need to set a port before you can connect."), NULL, MB_ICONERROR);
						break;
					}
						
					pWData->state = CONNECT;
					
                    // Disable menu items.
					menu = GetMenu(hwnd);
					setMenu(menu, MF_GRAYED);

                    // Enable caret.
                    CreateCaret(hwnd, NULL, 2, pWData->cursor.cyChar);
                    SetCaretPos(pWData->cursor.xCaret * pWData->cursor.cxChar, pWData->cursor.yCaret * pWData->cursor.cyChar);
                    ShowCaret(hwnd);
                    break;
			}
			break;

        case WM_KEYDOWN:
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            switch (wParam) {
                case VK_ESCAPE:
                    // If in CONNECT mode, ESC will disconnect Comm port.
                    if (pWData->state == CONNECT) {
                        pWData->state = COMMAND;
				        CloseHandle(pWData->hCom);
                        pWData->hCom = INVALID_HANDLE_VALUE;
				        menu = GetMenu(hwnd);
				        setMenu(menu, MF_ENABLED);
                        InvalidateRect(hwnd, NULL, FALSE);
                        DestroyCaret();
                    }
				break;

                //case VK_DELETE:
                //    
                //    break;
            }
            break;
        
        case WM_CHAR:
            pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            
            // Only repond to keystrokes when in CONNECT mode.
            if (pWData->state != CONNECT) {
                break;
            }

            // LOWORD(lParam) contains the number of keystrokes received (in the case of 
            // a key being held down).
            for (i = 0; i < (int) LOWORD(lParam); i++) {
                switch (wParam) {
                    case '\b': // backspace
                        SendMessage (hwnd, WM_KEYDOWN, VK_DELETE, 1) ;
                    
                        // ^ 
                        // |
                        //FALL THROUGH;
                        // |
                        // v

                    default:
                        HideCaret(hwnd);

                        // Send Char out to Comm port.
                        Transmit(pWData->hCom, wParam);

                        // Add Char to Outpu buffer.
                        if (!outputAddChar((TCHAR) wParam, pWData->pOutput)) {
                            MessageBox(hwnd, TEXT("MASSIVE ERROR."), NULL, MB_ICONERROR);
                            CloseHandle(pWData->hCom);
                            PostQuitMessage(0);
                        }

                        // Prepare for printing.
                        hdc = GetDC(hwnd);
                        hFont = (HFONT) GetStockObject(ANSI_FIXED_FONT);
                        SelectObject(hdc, hFont);
                        
                        // Print character.
                        printChar(hwnd, &pWData->cursor, pWData->pOutput, hdc);
                        ReleaseDC(hwnd, hdc);
                        
                        // Increment caret position.
                        pWData->cursor.xCaret = ++pWData->cursor.xCaret % pWData->cursor.cxBuffer;
                        pWData->cursor.yCaret = max(0, pWData->pOutput->pos / pWData->cursor.cxBuffer);
                        SetCaretPos(pWData->cursor.xCaret * pWData->cursor.cxChar, pWData->cursor.yCaret * pWData->cursor.cyChar);
                        ShowCaret(hwnd);
                }
            }
            break;

		case WM_PAINT:		
            // Process a repaint message
			pWData = (PWDATA) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            hdc = BeginPaint(hwnd, &paintstruct); // Acquire DC            
            
            // Prepare for painting output.
            hFont = (HFONT) GetStockObject(ANSI_FIXED_FONT);
            SelectObject(hdc, hFont);  
            printOut(hwnd, &pWData->cursor, pWData->pOutput, hdc);

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
    if (hCom == INVALID_HANDLE_VALUE) {
	    MessageBox(hwnd, TEXT("Could not connect to port."), NULL, MB_ICONERROR);
		return INVALID_HANDLE_VALUE;
	}

    // Prompt for Comm port configuration.
    cc.dwSize = sizeof(COMMCONFIG);
    GetCommConfig(hCom, &cc, &cc.dwSize);
    CommConfigDialog(lpFileName, hwnd, &cc);

    // Save Comm port configuration. 
    success = SetCommState(hCom, &cc.dcb);
	if (!success) {
	    err = GetLastError();
		return INVALID_HANDLE_VALUE;
	}

    // Prepare Comm port timeouts.
    if(!GetCommTimeouts(hCom, &CommTimeouts)) {
        return INVALID_HANDLE_VALUE;
    }

    // Set some reasonable timeouts to help balance Comm port usage for reading/writing.
    CommTimeouts.ReadIntervalTimeout = 1;
    CommTimeouts.ReadTotalTimeoutMultiplier = 1;
    CommTimeouts.ReadTotalTimeoutConstant = 1;

    CommTimeouts.WriteTotalTimeoutMultiplier = 100;
    CommTimeouts.WriteTotalTimeoutConstant = 100;

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
 --	INTERFACE:  pollPort(HWND hwnd, PWDATA pWData)
 --                 HWND hwnd: The program's window handle.
 --                 PWDATA pWData: The window's stored data object.
 -- 
 --	RETURNS:    void.
 -- 
 --	NOTES:      Checks the open port for any new data, if received and not NULL, sends data to output string and calls 
 --             a printOut to render it on screen.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/

void pollPort(HWND hwnd, PWDATA pWData) {
    HDC hdc;
    HFONT hFont;
    TCHAR readBuff;

    if ((readBuff = Recieve(pWData->hCom)) == NULL) {
        return;
    }

    if (readBuff == '\0') {
        return;
    }

    if (!outputAddChar(readBuff, pWData->pOutput)) {
        MessageBox (hwnd, TEXT("MASSIVE ERROR."), NULL, MB_ICONERROR);
        CloseHandle(pWData->hCom);
        PostQuitMessage(0);
    }

    // Prepare to print character.
    hdc = GetDC(hwnd);
    hFont = (HFONT) GetStockObject(ANSI_FIXED_FONT);
    SelectObject(hdc, hFont);

    // Print character.
    printChar(hwnd, &pWData->cursor, pWData->pOutput, hdc);
    ReleaseDC(hwnd, hdc);

    // Repostion caret.
    pWData->cursor.xCaret = ++pWData->cursor.xCaret % pWData->cursor.cxBuffer;
    pWData->cursor.yCaret = max(0, pWData->pOutput->pos / pWData->cursor.cxBuffer);
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
    int i;

    if (pOutput->pos == pOutput->size - 1) {
        pOutput->size = 2 * pOutput->size;
        pOutput->out = (TCHAR*) realloc(pOutput->out, pOutput->size * sizeof(TCHAR));

        if (!pOutput->out) {
            return FALSE;
        }
    }

    for (i = pOutput->pos + 1; i < pOutput->size; i++) {
        pOutput->out[i] = ' ';
    }

    pOutput->out[pOutput->pos] = c;
    pOutput->pos++;

    return TRUE;
}