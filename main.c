/*------------------------------------------------------------------------------------------------------------------ 
 --	SOURCE FILE: main.c - Contains WinMain, initializes the program.
 -- 
 --	PROGRAM: dte
 -- 
 -- Functions:  
 --             int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
 --
 -- DATE:       September 29th 2010
 --
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	NOTES:      Contains win32 main program initializing function WinMain(). Initiates message loop and handles 
 --             calling dispatch on the messages.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/

#include "main.h"
#include "session.h"


/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: WinMain 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
 --                 HINSTANCE hInst
 --                 HINSTANCE hprevInstance
 --                 LPSTR lspszCmdParam
 --                 int nCmdShow
 -- 
 --	RETURNS:    WINAPI.
 -- 
 --	NOTES: 
 --             The message loop is handled with PeekMessage() as opposed to GetMessage() to allow non-interferring 
 --             polling on the COMM port when there are no messages in the queue.
 --
 ----------------------------------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow) {
	HWND hwnd;
	MSG Msg;
	WNDCLASSEX Wcl;
    PWDATA pWData;
    int i;

	TCHAR Name[] = TEXT("Dumb Terminial Emulator");

	Wcl.cbSize = sizeof (WNDCLASSEX);
	Wcl.style = 0;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW); // cursor style
	
	Wcl.lpfnWndProc = WndProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = Name;
	
	Wcl.lpszMenuName = TEXT("DTEMENU"); // The menu Class
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = sizeof(PWDATA); 
	
	if (!RegisterClassEx(&Wcl)) {
		return 0;
	}

	hwnd = CreateWindow (
        Name, 
        Name, 
        WS_OVERLAPPEDWINDOW, 
        10, 
        10, 
        600, 
        400, 
        NULL, 
        NULL, 
        hInst, 
        NULL
    );

    // Create window storage struct.
	pWData = (PWDATA) malloc(sizeof(PWDATA));
    
    // Set default window storage values.
    pWData->state = COMMAND;                // Program state.
    pWData->hCom = INVALID_HANDLE_VALUE;    // Comm port handle.
	
    // Prepare output storage struct.
    pWData->pOutput = (POUTPUT) malloc(sizeof(OUTPUT));
    pWData->pOutput->size = OUTPUTBUFFSIZE;
    pWData->pOutput->pos = 0;
    pWData->pOutput->out = (TCHAR*) malloc(pWData->pOutput->size * sizeof(TCHAR));

    // Initially white-out the output buffer.
    for (i = 0; i < pWData->pOutput->size; i++) {
        pWData->pOutput->out[i] = ' ';
    }

    // Set up the cursor struct with default values.
    pWData->cursor.xCaret = 0;
    pWData->cursor.yCaret = 0;
    pWData->cursor.cxBuffer = 0;
    pWData->cursor.cyBuffer = 0;
    pWData->cursor.cxChar = 0;
    pWData->cursor.cyChar = 0;

    // Set window struct to the window.
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) pWData);

    ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

    // Message handling uses PeekMessage() instead of GetMessage.
    // Essentially this will handle all messages when they arrive
    // on the queue. But whenever the queue is empty, poll the 
    // serial port.
    // This provides us with a very clean method of constantly polling the 
    // open serial port with out jamming the program.
	while (TRUE) {
        if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
            if (Msg.message == WM_QUIT) {
                return Msg.wParam;
            }
            TranslateMessage(&Msg);
		    DispatchMessage(&Msg);
        }
        else {
            if (pWData->state == CONNECT) {
                // If no messages, poll the open serial port.
                pollPort(hwnd, pWData);
            }
        }
	}

	return Msg.wParam;
}