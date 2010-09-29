#include "application.h"
#include "session.h"

void printOut(PWDATA pWData, HDC hdc) {
    TEXTMETRIC tm; 
    HFONT hFont;

    int textX = 0; 
    int textY = 0;

    GetTextMetrics(hdc, &tm);

    hFont = (HFONT) GetStockObject(ANSI_FIXED_FONT);
    SelectObject(hdc, hFont);

    TextOut(hdc, 0, 0, pWData->output.out, pWData->output.pos);
    
    // Print text.
    //TextOut(hdc, textX, textY, readBuff, 1);

    // Move cursor.
    //textX = textX + 15;
    //textY = textY + 10;
}
