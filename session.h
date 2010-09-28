#include <Windows.h>

typedef struct _WDATA {
    DWORD threadId;
    BOOL bReading;
    HANDLE hThread;
    HANDLE hCom;
    enum STATE state;
} WDATA, *PWDATA;

enum STATE {
    COMMAND,
    CONNECT
};

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
HANDLE ConnectComm(HWND hwnd, LPCWSTR lpFileName);
BOOL StartReadThread(HANDLE hCom, PWDATA pWData);
BOOL ReadThread(PWDATA);
void EndThread(PWDATA);
