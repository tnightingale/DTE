#ifndef SESSION_H /* Convention is file name with '.' replaced by underscores. */
#define SESSION_H

/* INCLUDES: ------------------------------------------------------------------*/
#include <Windows.h>
#include <tchar.h>

/* MACROS: ----------------------------------------------------------------------
--      BUFFER(x, y)
--          Returns pointer at buffer location specified by line x, column y.
-------------------------------------------------------------------------------*/
#define BUFFER(x, y) *(pBuffer + y * pCursor->cxBuffer + x)

/* STRUCTS: ---------------------------------------------------------------------
-- _CURSOR, CURSOR, *PCURSOR
--     int xCaret:          The horizontal grid position of the cursor.
--     int yCaret:          The vertical grid position of the cursor.
--     int cxBuffer:        The width of the display area (buffer) in chars.
--     int cyBuffer:        The height of the display area (buffer) in chars.
--     int cxChar:          The width of a character in pixels.
--     int cyChar:          The height of a character in pixels.
-- 
-- _OUTPUT, OUTPUT, *POUTPUT
--     TCHAR* out:          Pointer to the output buffer. This stores every 
--                          character received by the program. Used for repainting 
--                          the display area. This buffer dynamically resizes when 
--                          necessary.
--     int pos:             A pointer to the "cursor" position in the output buffer. 
--     int size:            The size of the output buffer. Used to detirmine 
--                          available remaining space and when to resize.
--
-- _WDATA, WDATA, *PWDATA
--     HANDLE hCom:          A handle to the serial port.      
--     enum STATE state:     Tracks the current state of the program.
--     POUTPUT pOutput:      Pointer to the program's output object.
--     CURSOR cursor:        The program's cursor object.
-------------------------------------------------------------------------------*/
typedef struct _CURSOR {
    int xCaret;
    int yCaret;
    int cxBuffer;
    int cyBuffer;
    int cxChar;
    int cyChar;
} CURSOR, *PCURSOR;

typedef struct _OUTPUT {
    TCHAR* out;
    int pos;
    int size;
} OUTPUT, *POUTPUT;

typedef struct _WDATA {
    HANDLE hCom;
    enum STATE state;
    POUTPUT pOutput;
    CURSOR cursor;
} WDATA, *PWDATA;

/* ENUMERATORS: -----------------------------------------------------------------
--      STATE
--          COMMAND: Command mode is the default mode, Comm ports are closed and 
--                   program is waiting for user input.
--          CONNECT: Connect mode is enabled when a Comm port is open and the 
--                   program is actively listening for keystrokes and polling 
--                   the serial port.
-------------------------------------------------------------------------------*/
enum STATE {
    COMMAND,
    CONNECT
};

/* PROTOTYPES: ----------------------------------------------------------------*/   
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
HANDLE ConnectComm(HWND, LPCWSTR);
void pollPort(HWND, PWDATA);
BOOL outputAddChar(TCHAR, POUTPUT);

#endif
