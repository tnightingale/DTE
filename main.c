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
 --	
 ----------------------------------------------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow) {
	HWND hwnd;
	MSG Msg;
	WNDCLASSEX Wcl;
    PWDATA pWData;

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

	pWData = (PWDATA) malloc(sizeof(PWDATA));
    pWData->state = COMMAND;
	
    pWData->pOutput = (POUTPUT) malloc(sizeof(OUTPUT));
	pWData->pOutput->out = (TCHAR*) malloc(8 * sizeof(TCHAR));
    pWData->pOutput->size = 8;
    pWData->pOutput->pos = 0;

    pWData->wnSize.cx = 0;
    pWData->wnSize.cy = 0;

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG) pWData);

    ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

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
                pollPort(hwnd, pWData->hCom, pWData->pOutput);
            }
        }
	}

	return Msg.wParam;
}