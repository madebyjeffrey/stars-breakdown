/* Declaration subset for host checks, NOT the Win16 SDK or an ABI test. */
#ifndef RACEVC_WINDOWS_DECLARATIONS
#define RACEVC_WINDOWS_DECLARATIONS
typedef unsigned short HGLOBAL;
typedef void *HINSTANCE;
typedef void *HWND;
typedef void *HMENU;
typedef void *HBRUSH;
typedef void *HCURSOR;
typedef char *LPSTR;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned UINT;
typedef UINT WPARAM;
typedef LONG LPARAM;
typedef LONG LRESULT;
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef struct {
    unsigned style; WNDPROC lpfnWndProc; int cbClsExtra, cbWndExtra;
    HINSTANCE hInstance; void *hIcon; HCURSOR hCursor; HBRUSH hbrBackground;
    const char *lpszMenuName, *lpszClassName;
} WNDCLASS;
typedef struct { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; } MSG;
#define PASCAL
#define FAR
#define CALLBACK
#define GMEM_FIXED 0
#define GMEM_ZEROINIT 0x40
#define MB_OK 0
#define MB_ICONEXCLAMATION 0x30
#define MB_ICONINFORMATION 0x40
#define WM_CLOSE 0x10
#define WM_QUIT 0x12
#define WM_KEYDOWN 0x100
#define WM_COMMAND 0x111
#define IDCANCEL 2
#define IDC_ARROW ((const char *)32512)
#define COLOR_WINDOW 5
#define WS_OVERLAPPED 0L
#define WS_CAPTION 0xc00000L
#define WS_SYSMENU 0x80000L
#define WS_MINIMIZEBOX 0x20000L
#define WS_CHILD 0x40000000L
#define WS_VISIBLE 0x10000000L
#define SS_LEFT 0
#define BS_PUSHBUTTON 0
#define CW_USEDEFAULT (-32768)
#define SW_SHOWNORMAL 1
#define PM_REMOVE 1
#define VK_ESCAPE 27
HGLOBAL GlobalAlloc(unsigned, unsigned long);
void *GlobalLock(HGLOBAL);
int GlobalUnlock(HGLOBAL);
HGLOBAL GlobalFree(HGLOBAL);
unsigned GetModuleFileName(HINSTANCE, LPSTR, unsigned);
int MessageBox(HWND, const char *, const char *, unsigned);
DWORD GetTickCount(void);
LRESULT DefWindowProc(HWND, UINT, WPARAM, LPARAM);
int SetWindowText(HWND, const char *);
HCURSOR LoadCursor(HINSTANCE, const char *);
unsigned RegisterClass(const WNDCLASS *);
HWND CreateWindow(const char *, const char *, DWORD, int, int, int, int,
    HWND, HMENU, HINSTANCE, void *);
int DestroyWindow(HWND);
int ShowWindow(HWND, int);
int UpdateWindow(HWND);
HWND SetFocus(HWND);
int PeekMessage(MSG *, HWND, unsigned, unsigned, unsigned);
int TranslateMessage(const MSG *);
LONG DispatchMessage(const MSG *);
#endif
