#include <Windows.h>
#include <stdio.h>
#include "session.h"

#define IDM_CONNECT		100
#define IDM_HELP		108

#define IDM_COM1		109
#define IDM_COM2		110
#define IDM_COM3		111
#define IDM_COM4		112
#define IDM_COM5		113
#define IDM_COM6		114

void printOut(HWND hwnd, POUTPUT pOutput, HDC hdc);
void setMenu(HMENU, UINT);
