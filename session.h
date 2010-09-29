#ifndef SESSION_H /* Convention is file name with '.' replaced by underscores. */
#define SESSION_H

#include <Windows.h>
#include <tchar.h>

typedef struct _OUTPUT {
    TCHAR* out;
    int pos;
    int size;
} OUTPUT, *POUTPUT;

typedef struct _WDATA {
    SIZE wnSize;
    HANDLE hCom;
    enum STATE state;
    HWND hwnd;
    OUTPUT output;
	HANDLE hHeap;
} WDATA, *PWDATA;

enum STATE {
    COMMAND,
    CONNECT
};

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
HANDLE ConnectComm(HWND hwnd, LPCWSTR lpFileName);
BOOL StartReadThread(HANDLE hCom, PWDATA pWData);
//BOOL ReadThread(PWDATA);
//void EndThread(PWDATA);
void pollPort(PWDATA pWData);
BOOL outputAddChar(TCHAR c, POUTPUT pOutput);

#endif
