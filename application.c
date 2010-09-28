#include "application.h"
#include "session.h"

void printChar(char* readBuff, PWDATA pWData) {
    HDC hdc = GetDC(pWData->hwnd);
    LPCWSTR cstr;
    WCHAR str[6];

    static int textX = 0; 
    static int textY = 0;
    
    // This is gross.
    MultiByteToWideChar(0, 0, readBuff, 5, str, 6);
    cstr = str;

    // Print text.
    TextOut(hdc, textX, textY, cstr, 1);

    // Move cursor.
    textX = textX + 15;
    //textY = textY + 10;

    ReleaseDC(pWData->hwnd, hdc);
}