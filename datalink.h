#include <Windows.h>
#include <stdio.h>

HANDLE OpenPort(LPCWSTR);
int Transmit(HANDLE, TCHAR);
TCHAR Recieve(HANDLE);