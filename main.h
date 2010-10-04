/* INCLUDES: ------------------------------------------------------------------*/
#include <windows.h>

/* MACROS: ----------------------------------------------------------------------
--      BUFFER(x, y)
--          Returns pointer at buffer location specified by line x, column y.    
-------------------------------------------------------------------------------*/
#define OUTPUTBUFFSIZE 128

/* PROTOTYPES: ----------------------------------------------------------------*/   
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);