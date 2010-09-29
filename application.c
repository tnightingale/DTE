#include "application.h"
#include "session.h"

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

void setMenu(HMENU menu, UINT uEnable) {
	EnableMenuItem(menu, IDM_CONNECT, uEnable);
	EnableMenuItem(menu, IDM_COM1, uEnable);
	EnableMenuItem(menu, IDM_COM2, uEnable);
	EnableMenuItem(menu, IDM_COM3, uEnable);
	EnableMenuItem(menu, IDM_COM4, uEnable);
	EnableMenuItem(menu, IDM_COM5, uEnable);
	EnableMenuItem(menu, IDM_COM6, uEnable);
}
