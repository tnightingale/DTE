/* INCLUDES -------------------------------------------------------------------*/
#include <Windows.h>
#include <stdio.h>
#include "session.h"

/* MACROS: ----------------------------------------------------------------------
--      IDM_CONNECT
--      IDM_COM1
--      IDM_COM2
--      IDM_COM3
--      IDM_COM4
--      IDM_COM5
--      IDM_COM6
--          Menu item identifiers.
--      
--      CARETWIDTH
--          Define the width of the caret.    
-------------------------------------------------------------------------------*/
#define IDM_CONNECT		100
#define IDM_COM1		109
#define IDM_COM2		110
#define IDM_COM3		111
#define IDM_COM4		112
#define IDM_COM5		113
#define IDM_COM6		114

#define CARETWIDTH 2

/* PROTOTYPES: --------------------------------------------------------------------*/   
void printOut(HWND, PCURSOR, POUTPUT, HDC);
void printChar(HWND, PCURSOR, POUTPUT, HDC);
void setMenu(HMENU, UINT);
