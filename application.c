/*---------------------------------------------------------------------------------------------------------------------
 --	SOURCE FILE: application.c - Handles visual presentation functionality.
 -- 
 --	PROGRAM: dte
 -- 
 -- Functions:  
 --             void printOut(HWND, PCURSOR, POUTPUT, HDC);
 --             void printChar(HWND, PCURSOR, POUTPUT, HDC);
 --             void setMenu(HMENU, UINT);
 --
 -- DATE:       September 29th 2010
 --
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	NOTES:      Handles rendering of program visual interface to display. Functions create menus and print data stored 
 --             in the output buffer to display area. 
 --             Formatting, position and other visual aspects are handled in this layer.	
 ----------------------------------------------------------------------------------------------------------------------*/

#include "application.h"
#include "session.h"

/*--------------------------------------------------------------------------------------------------------------------- 
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
 --	INTERFACE:  void printOut(HWND, PCURSOR, POUTPUT, HDC)
 --                 HWND hwnd: The program's window handle.
 --                 PCURSOR pCursor: Pointer to the program's cursor object.
 --                 POUTPUT pOutput: Point to the programs output file.
 --                 HDC hdc: Hardware device controller.
 -- 
 --	RETURNS:    void.
 -- 
 --	NOTES:      Prints data stored in the data string (pWData->output.out) to display area. Handles formatting, 
 --             newlines, etc. Math could do with a bit of work.
 --	
 ----------------------------------------------------------------------------------------------------------------------*/
void printOut(HWND hwnd, PCURSOR pCursor, POUTPUT pOutput, HDC hdc) {
    UINT x, y;           // Cursor column & line.
    int pBufferSize;
    TCHAR* pBuffer;      // Char rendering buffer.

    pBufferSize = pCursor->cxBuffer * pCursor->cyBuffer * sizeof(TCHAR);
    pBuffer = (TCHAR*) malloc(pBufferSize);

    for (x = 0; x < pBufferSize / sizeof(TCHAR); x++) {
        if (x < pOutput->pos) {
            pBuffer[x] = (pOutput->out)[x];
        } else {
            pBuffer[x] = ' ';
        }
    }

    for (y = 0; y < pCursor->cyBuffer ; y++) {
        TextOut (hdc, 0, y * pCursor->cyChar, &BUFFER(0, y), pCursor->cxBuffer);
    }
    
}

/*--------------------------------------------------------------------------------------------------------------------- 
 --	FUNCTION: printChar 
 -- 
 --	DATE: October 3rd, 2010 
 -- 
 --	REVISIONS:  (Date and Description)
 -- 
 --	DESIGNER:   Tom Nightingale
 -- 
 --	PROGRAMMER: Tom Nightingale
 -- 
 --	INTERFACE:  void printChar(HWND hwnd, PCURSOR pCursor, POUTPUT pOutput, HDC hdc)
 --                 HWND hwnd: The program's window handle.
 --                 PCURSOR pCursor: Pointer to the program's cursor object.
 --                 POUTPUT pOutput: Point to the programs output file.
 --                 HDC hdc: Hardware device controller.
 -- 
 --	RETURNS:    void.
 -- 
 --	NOTES:      Prints a single char to the window's display area, handling window width & newline etc...
 --	
 ----------------------------------------------------------------------------------------------------------------------*/
void printChar(HWND hwnd, PCURSOR pCursor, POUTPUT pOutput, HDC hdc) {
    int x, y;
    TCHAR index;

    x = pCursor->xCaret * pCursor->cxChar;
    y = pCursor->yCaret * pCursor->cyChar;
    index = *(pOutput->out + (pOutput->pos - 1));

    TextOut(hdc, x, y, &index, 1);
}

/*--------------------------------------------------------------------------------------------------------------------- 
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
