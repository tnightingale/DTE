#ifndef SESSION_H /* Convention is file name with '.' replaced by underscores. */
#define SESSION_H

#include <Windows.h>

typedef struct _WDATA {
    //DWORD threadId;
    //BOOL bReading;
    //HANDLE hThread;
    HANDLE hCom;
    enum STATE state;
    HWND hwnd;
    //int textX;
    //int textY;
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
