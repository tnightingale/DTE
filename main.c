#include "main.h"
#include "session.h"

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
    pWData->bReading = FALSE;
    pWData->hThread = NULL;
    pWData->hwnd = hwnd;
            //pWData->textX = 0;
            //pWData->textY = 0;

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG) pWData);

    ShowWindow (hwnd, nCmdShow);
	UpdateWindow (hwnd);

	while (GetMessage(&Msg, NULL, 0, 0)) {
   		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

    free(pWData);

	return Msg.wParam;
}