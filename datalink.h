#include <Windows.h>
#include <stdio.h>

HANDLE OpenPort(LPCWSTR);
int Transmit(HANDLE, TCHAR);
BOOL Recieve(HANDLE, TCHAR*);