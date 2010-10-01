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
void printOut(PWDATA pWData, HDC hdc) {
    TEXTMETRIC tm; 
    HFONT hFont;

    UINT count = 0;
    UINT cxChar = 0;
    UINT cyChar = 0;
    UINT numCharsLine = 0;
	UINT numLines = 0;
    UINT lineH = 0;

    GetTextMetrics(hdc, &tm);

    hFont = (HFONT) GetStockObject(ANSI_FIXED_FONT);
    SelectObject(hdc, hFont);

	numCharsLine = pWData->wnSize.cx / tm.tmAveCharWidth;
	if (numCharsLine == 0) numCharsLine = 1;
	numLines = pWData->output.pos / numCharsLine;

	cyChar = tm.tmHeight + tm.tmExternalLeading;

    //TextOut(hdc, 0, 0, pWData->output.out, pWData->output.pos);
	for (count = 0; count < numLines + 1; count++) {
		TextOut(hdc, 0, count * cyChar, pWData->output.out + count * numCharsLine, pWData->output.pos % numCharsLine);
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
