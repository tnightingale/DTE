#ifndef SESSION_H /* Convention is file name with '.' replaced by underscores. */
#define SESSION_H

#include <Windows.h>
#include <tchar.h>

#define BUFFER(x, y) *(pBuffer + y * pCursor->cxBuffer + x)

typedef struct _CURSOR {
    int xCaret;
    int yCaret;
    int cxBuffer;
    int cyBuffer;
    int cxChar;
    int cyChar;
} CURSOR, *PCURSOR;

typedef struct _OUTPUT {
    TCHAR* out;
    int pos;
    int size;
} OUTPUT, *POUTPUT;

typedef struct _WDATA {
    HANDLE hCom;
    enum STATE state;
    POUTPUT pOutput;
    CURSOR cursor;
} WDATA, *PWDATA;

enum STATE {
    COMMAND,
    CONNECT
};

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
HANDLE ConnectComm(HWND, LPCWSTR);
void pollPort(HWND, PWDATA);
BOOL outputAddChar(TCHAR, POUTPUT);

#endif
