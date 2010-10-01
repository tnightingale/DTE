#ifndef SESSION_H /* Convention is file name with '.' replaced by underscores. */
#define SESSION_H

#include <Windows.h>
#include <tchar.h>

#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

typedef struct _OUTPUT {
    TCHAR* out;
    int pos;
    int size;
} OUTPUT, *POUTPUT;

typedef struct _WDATA {
    SIZE wnSize;
    HANDLE hCom;
    enum STATE state;
    POUTPUT pOutput;
} WDATA, *PWDATA;

enum STATE {
    COMMAND,
    CONNECT
};

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
HANDLE ConnectComm(HWND hwnd, LPCWSTR lpFileName);
void pollPort(HWND hwnd, HANDLE hCom, POUTPUT pOutput);
BOOL outputAddChar(TCHAR, POUTPUT pOutput);

#endif
