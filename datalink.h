/* INCLUDES: ------------------------------------------------------------------*/
#include <Windows.h>
#include <stdio.h>

/* PROTOTYPES: ----------------------------------------------------------------*/
HANDLE OpenPort(LPCWSTR);
int Transmit(HANDLE, TCHAR);
TCHAR Recieve(HANDLE);