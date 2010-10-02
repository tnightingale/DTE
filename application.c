/*------------------------------------------------------------------------------------------------------------------ 
 --	SOURCE FILE: application.c - Handles visual presentation functionality.
 -- 
 --	PROGRAM: dte
 -- 
 -- Functions:  
 --             void printOut(PWDATA pWData, HDC hdc)
 --             void setMenu(HMENU menu, UINT uEnable)
 --
 -- DATE:       September 29th 2010
 --
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	NOTES:      Prints data stored in the data string (pWData->output.out) to display area. Handles formatting, 
 --             newlines, etc. Math could do with a bit of work.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/

#include "application.h"
#include "session.h"

/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: printOut 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  printOut(PWDATA pWData, HDC hdc)
 --                 PWDATA pWData: Program's window data storage struct containing the buffer string (output) and it's
 --                                metadata.
 --                 HDC hdc: Hardware device controller.
 -- 
 --	RETURNS:    void.
 -- 
 --	NOTES:      Prints data stored in the data string (pWData->output.out) to display area. Handles formatting, 
 --             newlines, etc. Math could do with a bit of work.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/
void printOut(HWND hwnd, POUTPUT pOutput, HDC hdc) {
    RECT rec;
    TEXTMETRIC tm; 
    HFONT hFont;
    int cxClient, cyClient; // Window width and height.
    int cxChar, cyChar;     // Char width and height.
    int cxBuffer, cyBuffer; // Window width and height in Chars.
    UINT x, y;           // Cursor column & line.
    int pBufferSize;
    TCHAR* pBuffer;         // Char rendering buffer.

    hFont = (HFONT) GetStockObject(ANSI_FIXED_FONT);
    SelectObject(hdc, hFont);

    GetClientRect(hwnd, &rec);
    GetTextMetrics(hdc, &tm);

    cxClient = rec.right;
    cyClient = rec.bottom;
    cxChar = tm.tmMaxCharWidth;
    cyChar = tm.tmHeight;

    cxBuffer = max(1, cxClient / cxChar);
    cyBuffer = max(1, cyClient / cyChar);
    pBufferSize = cxBuffer * cyBuffer * sizeof(TCHAR);

    pBuffer = (TCHAR*) malloc(pBufferSize);

    for (x = 0; x < pBufferSize / sizeof(TCHAR); x++) {
        if (x < pOutput->pos) {
            pBuffer[x] = (pOutput->out)[x];
        } else {
            pBuffer[x] = ' ';
        }
    }

    for (y = 0, x = 0 ; y < cyBuffer ; y++) {
        TextOut (hdc, 0, y * cyChar, & BUFFER(x,y), cxBuffer);
    }
    
}

/*------------------------------------------------------------------------------------------------------------------ 
 --	FUNCTION: setMenu 
 -- 
 --	DATE: September 29th, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  setMenu(HMENU menu, UINT uEnable
 --                 HMENU menu: A win32 menu struct returned by GetMenu().
 --                 UINIT uEnable: Option to be passed to EnableMenuItem. The setting to apply to the menu.
 -- 
 --	RETURNS:    void.
 -- 
 --	NOTES:      Sets all of the menu items to the value of uEnable.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/
void setMenu(HMENU menu, UINT uEnable) {
	EnableMenuItem(menu, IDM_CONNECT, uEnable);
	EnableMenuItem(menu, IDM_COM1, uEnable);
	EnableMenuItem(menu, IDM_COM2, uEnable);
	EnableMenuItem(menu, IDM_COM3, uEnable);
	EnableMenuItem(menu, IDM_COM4, uEnable);
	EnableMenuItem(menu, IDM_COM5, uEnable);
	EnableMenuItem(menu, IDM_COM6, uEnable);
}
