/* Native Win16 application adapter. Bootstrap, unsupported-command notice and
 * logging are new integration code, not a claimed reconstruction of WinMain,
 * FCreateStuff, FrameWndProc or WriteIniSettings. */
#include <windows.h>
#include <dos.h>
#include <io.h>
#include <stdlib.h>
#include <direct.h>
#include "APP.H"
#include "runtime_internal.h"

StarsRuntime runtime;
StarsPlatform platform;
static StarsMemory memory;
static StarsHeapApi heap_api;
static HGLOBAL data_handle, strings_handle;
static HLOCAL local_handles[8];
HWND frame_window, title_window;
HINSTANCE app_instance;
static HACCEL accelerators;
char directory[144], ini_file[180];
static char filename[180];
static FILE *log_file;
static int smoke, paint_count, smoke_tick, adapter_failed;
static unsigned reads;
static unsigned long read_bytes;

void log_line(const char *text)
{ if (log_file) { fputs(text,log_file);fputs("\n",log_file);fclose(log_file);
    strcpy(filename,directory);strcat(filename,"APP.LOG");log_file=fopen(filename,"at"); } }

static char *path(const char *name)
{ strcpy(filename,directory); strcat(filename,name); return filename; }

static StarsU8 *load_image(const char *name, HGLOBAL *handle)
{
    FILE *file;
    StarsU8 *data;
    *handle = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,65536UL);
    if (!*handle) return NULL;
    data = (StarsU8 *)GlobalLock(*handle);
    if (!data || FP_OFF(data)) return NULL;
    file = fopen(path(name),"rb");
    if (!file) return NULL;
    if (fread(data,1,32768U,file)!=32768U || fread(data+32768U,1,32768U,file)!=32768U) {
        fclose(file); return NULL;
    }
    fclose(file); return data;
}

static const StarsU8 *resolve(void *context, StarsU16 selector, StarsU16 offset)
{
    (void)context;
    return selector == 0x3000U ? runtime.data + offset : (const StarsU8 *)MK_FP(selector,offset);
}
static StarsU8 *heap_resolve(void *context, StarsU16 selector)
{ return (StarsU8 *)resolve(context,selector,0); }
static StarsU16 heap_alloc(void *context, StarsU16 flags, StarsU32 size)
{ (void)context; return (StarsU16)GlobalAlloc(flags,size); }
static StarsU16 heap_realloc(void *context, StarsU16 handle, StarsU32 size, StarsU16 flags)
{ (void)context; return (StarsU16)GlobalReAlloc((HGLOBAL)handle,size,flags); }
static StarsFar heap_lock(void *context, StarsU16 handle)
{ (void)context; return (StarsFar)GlobalLock((HGLOBAL)handle); }
static StarsU16 heap_unlock(void *context, StarsU16 handle)
{ (void)context; return (StarsU16)GlobalUnlock((HGLOBAL)handle); }
static StarsU16 heap_free(void *context, StarsU16 handle)
{ (void)context; return (StarsU16)GlobalFree((HGLOBAL)handle); }
static StarsI16 file_access(void *context, const StarsU8 *name, StarsI16 mode)
{ (void)context; return (StarsI16)_access((const char *)name,mode); }

static StarsU32 window_api(void *context, const StarsWindowCall *c)
{
    const char *op = c->operation;
    HWND h = (HWND)c->window;
    HDC dc = (HDC)c->window;
    StarsU16 a = c->a, b = c->b;
    int i,handled;
    StarsU32 extra;
    StarsCreateWindow *cw;
    StarsProfileString *profile;
    StarsI16 *g;
    const void FAR *bits;
    (void)context;
#define OP(n) (!strcmp(op,n))
    if (OP("GetSysColor")) return GetSysColor(a);
    if (OP("GetSystemMetrics")) return GetSystemMetrics(a);
    if (OP("GetDeviceCaps")) return GetDeviceCaps(dc,a);
    if (OP("GetDC")) return (StarsU16)GetDC(h);
    if (OP("ReleaseDC")) return ReleaseDC(h,(HDC)a);
    if (OP("CreateSolidBrush")) return (StarsU16)CreateSolidBrush(c->value);
    if (OP("DeleteObject")) return c->window ? DeleteObject((HGDIOBJ)c->window) : 0;
    if (OP("SelectObject")) return (StarsU16)SelectObject(dc,(HGDIOBJ)a);
    if (OP("SetTextColor")) return SetTextColor(dc,c->value);
    if (OP("SetBkColor")) return SetBkColor(dc,c->value);
    if (OP("SetBkMode")) return SetBkMode(dc,a);
    if (OP("GetTextExtent")) return GetTextExtent(dc,(LPCSTR)c->p,a);
    if (OP("GetTextMetrics")) return GetTextMetrics(dc,(TEXTMETRIC FAR *)c->p);
    if (OP("TextOut")) return TextOut(dc,(short)a,(short)b,(LPCSTR)c->p,(UINT)c->value);
    if (OP("ExtTextOut")) return ExtTextOut(dc,(short)a,(short)b,(UINT)(c->value>>16),
        (RECT FAR *)c->q,(LPCSTR)c->p,(UINT)c->value,NULL);
    if (OP("CreateFontIndirect")) return (StarsU16)CreateFontIndirect((LOGFONT FAR *)c->p);
    if (OP("MulDiv")) return MulDiv((short)a,(short)b,(short)c->value);
    /* Original near pointers are offsets in the emulated DGROUP image.
     * Reserve eight 2 KiB slots there, backed by real LocalAlloc lifetimes.
     * The original currently requests only fixed, zeroed LOGFONT/LOGPALETTE
     * storage. USER/GDI read the corresponding virtual image bytes. */
    if (OP("LocalAlloc")) {
        if (b>2048U || a!=LMEM_ZEROINIT) goto unsupported;
        for (i=0;i<8;++i) if (!local_handles[i]) {
            local_handles[i]=LocalAlloc(a,b);
            if (!local_handles[i]) goto unsupported;
            memset(runtime.data+0xa000U+i*2048U,0,b);
            return 0xa000U+i*2048U;
        }
        goto unsupported;
    }
    if (OP("LocalFree")) {
        i=(c->window-0xa000U)/2048U;
        if (i<0 || i>=8 || !local_handles[i]) goto unsupported;
        LocalFree(local_handles[i]);local_handles[i]=NULL;return 0;
    }
    if (OP("FindResource")) return (StarsU16)FindResource((HINSTANCE)c->window,MAKEINTRESOURCE(a),MAKEINTRESOURCE(b));
    if (OP("AllocResource")) return (StarsU16)AllocResource((HINSTANCE)c->window,(HRSRC)a,c->value);
    if (OP("AccessResource")) return AccessResource((HINSTANCE)c->window,(HRSRC)a);
    if (OP("SizeofResource")) return SizeofResource((HINSTANCE)c->window,(HRSRC)a);
    if (OP("LockResource")) return (StarsU32)LockResource((HGLOBAL)c->window);
    if (OP("GlobalLock")) return (StarsU32)GlobalLock((HGLOBAL)c->window);
    if (OP("GlobalUnlock")) return c->window ? GlobalUnlock((HGLOBAL)c->window) : 0;
    if (OP("FreeResource")) return c->window ? FreeResource((HGLOBAL)c->window) : 0;
    if (OP("_lread")) {
        i=_lread((HFILE)c->window,(void huge *)c->value,a);
        ++reads;read_bytes+=(unsigned)i;return (unsigned)i;
    }
    if (OP("_lclose")) return _lclose((HFILE)c->window);
    if (OP("CreatePalette")) return (StarsU16)CreatePalette((LOGPALETTE FAR *)c->p);
    if (OP("SelectPalette")) return (StarsU16)SelectPalette(dc,(HPALETTE)a,b);
    if (OP("RealizePalette")) return RealizePalette(dc);
    if (OP("StretchDIBits")) {
        g=(StarsI16 *)c->p;
        bits=MK_FP(FP_SEG(c->q),(WORD)(FP_OFF(c->q)+a));
        return StretchDIBits(dc,g[0],g[1],g[2],g[3],g[4],g[5],g[6],g[7],bits,(BITMAPINFO FAR *)c->q,b,c->value);
    }
    if (OP("PatBlt")) { g=(StarsI16 *)c->p;return PatBlt(dc,(short)a,(short)b,g[0],g[1],c->value); }
    if (OP("FillRect")) return FillRect(dc,(RECT FAR *)c->p,(HBRUSH)a);
    if (OP("GetClientRect")) { GetClientRect(h,(RECT FAR *)c->p);return 1; }
    if (OP("GetWindowRect")) { GetWindowRect(h,(RECT FAR *)c->p);return 1; }
    if (OP("InflateRect")) { InflateRect((RECT FAR *)c->p,(short)a,(short)b);return 1; }
    if (OP("BeginPaint")) return (StarsU16)BeginPaint(h,(PAINTSTRUCT FAR *)c->p);
    if (OP("EndPaint")) { EndPaint(h,(PAINTSTRUCT FAR *)c->p);return 1; }
    if (OP("IsIconic")) return IsIconic(h);
    if (OP("DrawIcon")) return DrawIcon(dc,(short)a,(short)b,(HICON)c->value);
    if (OP("CreateWindow")) {
        cw=(StarsCreateWindow *)c->p;
        return (StarsU16)CreateWindow((LPCSTR)cw->class_name,(LPCSTR)cw->text,c->value,
            (short)cw->geometry.left,(short)cw->geometry.top,(short)cw->geometry.right,(short)cw->geometry.bottom,
            h,(HMENU)cw->menu,(HINSTANCE)cw->instance,NULL);
    }
    if (OP("EnableWindow")) return EnableWindow(h,a);
    if (OP("SendMessage")) return SendMessage(h,a,b,c->p ? (LPARAM)c->p : c->value);
    if (OP("InvalidateRect")) { InvalidateRect(h,(RECT FAR *)c->p,a);return 1; }
    if (OP("GetPrivateProfileString")) {
        profile=(StarsProfileString *)c->q;
        return GetPrivateProfileString((LPCSTR)profile->section,(LPCSTR)profile->key,
            (LPCSTR)profile->fallback,(LPSTR)c->p,a,(LPCSTR)profile->file);
    }
    if (OP("LoadCursor")) return (StarsU16)LoadCursor((HINSTANCE)c->window,(LPCSTR)c->value);
    if (OP("SetCursor")) return (StarsU16)SetCursor((HCURSOR)c->window);
    if (OP("DestroyCursor")) return c->window ? DestroyCursor((HCURSOR)c->window) : 0;
    if (OP("DestroyIcon")) return c->window ? DestroyIcon((HICON)c->window) : 0;
    if (OP("FreeProcInstance")) { if(c->value) FreeProcInstance((FARPROC)c->value);return 0; }
    extra=native_window(c,&handled);if(handled) return extra;
unsupported:
    adapter_failed=1;log_line(op);
    MessageBox(title_window,"Unexpected Windows adapter operation. See APP.LOG.","Startup milestone",MB_OK|MB_ICONSTOP);
    FatalAppExit(0,"Startup adapter failed");
    return 0;
#undef OP
}

LRESULT CALLBACK FrameProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
    int checked;
    switch(message) {
    case WM_COMMAND:
        if(wp==CMD_NEW_RACE || wp==CMD_OPEN_RACE || wp==CMD_RECENT_RACE) {race_command(hwnd,wp);return 0;}
        if(wp==CMD_TITLE) {ShowWindow(hwnd,SW_HIDE);ShowWindow(title_window,SW_SHOW);SetActiveWindow(title_window);return 0;}
        if(wp==CMD_EXIT) {SendMessage(title_window,WM_CLOSE,0,0);return 0;}
        if(wp==CMD_CHECK) {checked=native_selftest();if(!checked) adapter_failed=1;SetWindowText(hwnd,"Stars! Race Workshop");MessageBox(hwnd,checked ? "Wizard, save/reopen and cancellation checks passed. See APP.LOG." : "Acceptance check failed. Please return APP.LOG with the package.","Acceptance Check",MB_OK);InvalidateRect(hwnd,NULL,TRUE);return 0;}
        if(wp==CMD_SETTINGS) {settings_show(hwnd);return 0;}
        if(wp==CMD_ABOUT) {MessageBox(hwnd,"Stars! reconstruction: race workshop.\nCreate, edit, save and reopen races. Game generation and turn processing are not connected yet.","About this build",MB_OK);return 0;}
        break;
    case WM_PAINT: workshop_paint(hwnd);return 0;
    case WM_CLOSE: SendMessage(hwnd,WM_COMMAND,CMD_TITLE,0);return 0;
    }
    return DefWindowProc(hwnd,message,wp,lp);
}

LRESULT CALLBACK TitleProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
    switch(message) {
    case WM_CREATE:
        stars_title_create(&runtime,(StarsU16)hwnd);
        log_line("TITLE_CREATE=PASS");return 0;
    case WM_PAINT:
        stars_title_paint(&runtime,(StarsU16)hwnd,0);
        ++paint_count;
        if(paint_count==1) log_line("FIRST_PAINT=PASS");
        return 0;
    case WM_QUERYNEWPALETTE:
    case WM_PALETTECHANGED:
        return stars_title_palette(&runtime,(StarsU16)hwnd,message,wp);
    case WM_SYSCHAR:
        if(wp=='w' || wp=='W') {workshop_show();return 0;}
        break;
    case WM_KEYDOWN:
        if(wp==VK_F6) {workshop_show();return 0;}
        break;
    case WM_COMMAND:
        if(wp==CMD_RACES) {workshop_show();return 0;}
        if(wp>=250 && wp<=253) wp-=250;
        if(wp==3) { DestroyWindow(hwnd);return 0; }
        if(wp<3) {
            MessageBox(hwnd,"This build recreates the starting screen. Game creation and loading are not connected yet.",
                       "Reconstruction milestone 2",MB_OK|MB_ICONINFORMATION);
            return 0;
        }
        break;
    case WM_TIMER:
        if(wp==2) {KillTimer(hwnd,2);workshop_show();if(!native_selftest()) adapter_failed=1;SendMessage(title_window,WM_CLOSE,0,0);return 0;}
        if(smoke && wp==1) {
            ++smoke_tick;
            if(smoke_tick==1) InvalidateRect(hwnd,NULL,TRUE);
            else if(smoke_tick==2) {
                log_line(paint_count>=2 ? "REPAINT=PASS" : "REPAINT=FAIL");
                KillTimer(hwnd,1);DestroyWindow(hwnd);
            }
            return 0;
        }
        break;
    case WM_CLOSE: DestroyWindow(hwnd);return 0;
    case WM_DESTROY:
        stars_title_release(&runtime);title_window=NULL;log_line("TITLE_RELEASE=PASS");
        PostQuitMessage(0);return 0;
    }
    return DefWindowProc(hwnd,message,wp,lp);
}

int PASCAL WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command, int show)
{
    WNDCLASS wc;
    MSG msg;
    StarsU8 *data,*strings;
    StarsFar sample;
    StarsU16 dc;
    int i, result=1;
    HMENU menu,race_menu,app_menu;
    (void)show;app_instance=instance;
    GetModuleFileName(instance,directory,sizeof(directory));
    for(i=strlen(directory);i>0 && directory[i-1]!='\\' && directory[i-1]!=':';--i) ;
    directory[i]=0;
    if(directory[1]==':') _chdrive((int)(directory[0]&31));
    strcpy(filename,directory);
    if(i>3 && filename[i-1]=='\\') filename[i-1]=0;
    if(_chdir(filename)) {MessageBox(NULL,"Cannot use the package directory.","Stars!",MB_OK);return 1;}
    log_file=fopen(path("APP.LOG"),"wt");log_line("STARTUP_MILESTONE=2");
    smoke=strstr(command,"/SMOKE")!=NULL;
    data=load_image("INDATA.BIN",&data_handle);
    strings=load_image("STRINGS.BIN",&strings_handle);
    if(!data || !strings) {
        log_line("STATIC_DATA=FAIL");
        MessageBox(NULL,"Cannot load the static data files. Keep INDATA.BIN and STRINGS.BIN beside START.EXE.","Startup milestone",MB_OK);
        goto done;
    }
    log_line("STATIC_DATA=PASS");
    memset(&platform,0,sizeof(platform));platform.memory=resolve;platform.window=window_api;platform.access=file_access;
    native_platform();
    stars_runtime_init(&runtime,data,strings,&platform,NULL,0xe000U,0xff00U);
    heap_api.resolve=heap_resolve;heap_api.alloc=heap_alloc;heap_api.realloc=heap_realloc;
    heap_api.lock=heap_lock;heap_api.unlock=heap_unlock;heap_api.free=heap_free;
    memory.runtime=&runtime;memory.api=&heap_api;memory.context=NULL;
    stars_put_word(data,0x5310U,(StarsU16)instance);data[0x56a2U]=0;
    strcpy(ini_file,path("STARSREC.INI"));
    stars_read_ui_config(&runtime,(const StarsU8 *)ini_file);
    settings_read();
    log_line("CONFIGURATION=PASS");
    dc=(StarsU16)GetDC(NULL);stars_create_fonts(&runtime,dc);ReleaseDC(NULL,(HDC)dc);
    stars_system_colors(&runtime);stars_init_tiles(data);
    if(!race_resources()) goto cleanup;
    if(strstr(command,"/FALLBACK") || settings_fallback()) stars_put_word(data,0x5312U,4);
    log_line(stars_word(data,0x5312U)<8 ? "DRAWING=STARFIELD" : "DRAWING=BITMAP");
    sample=stars_alloc(&memory,128U,0);stars_free(&memory,sample,0);
    log_line("MEMORY_ALLOC_FREE=PASS");
    if(!previous) {
        memset(&wc,0,sizeof(wc));wc.lpfnWndProc=FrameProc;wc.hInstance=instance;
        wc.hCursor=LoadCursor(NULL,IDC_ARROW);wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);wc.lpszClassName="StarsRecFrame";
        if(!RegisterClass(&wc)) goto cleanup;
        wc.lpfnWndProc=TitleProc;wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.hIcon=LoadIcon(instance,"STARSICO");wc.lpszClassName="StarsRecTitle";
        if(!RegisterClass(&wc)) goto cleanup;
        wc.lpfnWndProc=SettingsProc;wc.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);wc.lpszClassName="StarsRecSettings";
        if(!RegisterClass(&wc)) goto cleanup;
    }
    stars_put_word(data,0x356U,(StarsU16)LoadIcon(instance,"STARSICO"));
    menu=CreateMenu();race_menu=CreatePopupMenu();app_menu=CreatePopupMenu();
    AppendMenu(race_menu,MF_STRING,CMD_NEW_RACE,"&New Race...");
    AppendMenu(race_menu,MF_STRING,CMD_OPEN_RACE,"&Open Race...");
    AppendMenu(race_menu,MF_STRING,CMD_RECENT_RACE,"&Recent Race...");
    AppendMenu(race_menu,MF_SEPARATOR,0,NULL);
    AppendMenu(race_menu,MF_STRING,CMD_TITLE,"&Starting Screen");
    AppendMenu(race_menu,MF_STRING,CMD_EXIT,"E&xit");
    AppendMenu(app_menu,MF_STRING,CMD_SETTINGS,"&Settings...");
    AppendMenu(app_menu,MF_STRING,CMD_CHECK,"Run Acceptance &Check");
    AppendMenu(app_menu,MF_STRING,CMD_ABOUT,"&About...");
    AppendMenu(menu,MF_POPUP,(UINT)race_menu,"&Race");AppendMenu(menu,MF_POPUP,(UINT)app_menu,"&Help");
    frame_window=CreateWindow("StarsRecFrame","Stars! Race Workshop",WS_OVERLAPPEDWINDOW,
        (short)stars_word(data,0x2290U),(short)stars_word(data,0x2292U),
        (short)stars_word(data,0x2294U),(short)stars_word(data,0x2296U),NULL,menu,instance,NULL);
    if(!frame_window) goto cleanup;
    accelerators=LoadAccelerators(instance,MAKEINTRESOURCE(0x438));
    title_window=CreateWindow("StarsRecTitle","Stars!",WS_POPUP|WS_VISIBLE,0,0,
        GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),frame_window,NULL,instance,NULL);
    if(!title_window) goto cleanup;
    CreateWindow("BUTTON","Race &Workshop...",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
        GetSystemMetrics(SM_CXSCREEN)/2-85,GetSystemMetrics(SM_CYSCREEN)-105,170,28,
        title_window,(HMENU)CMD_RACES,instance,NULL);
    if(strstr(command,"/CHECK") && !SetTimer(title_window,2,500,NULL)) {log_line("TIMER=FAIL");adapter_failed=1;goto cleanup;}
    if(smoke && !SetTimer(title_window,1,2500,NULL)) { log_line("TIMER=FAIL");DestroyWindow(title_window);goto cleanup; }
    UpdateWindow(title_window);
    if(settings_workshop() && !smoke) workshop_show();
    while(GetMessage(&msg,NULL,0,0)) {
        if(!TranslateAccelerator(title_window,accelerators,&msg)) { TranslateMessage(&msg);DispatchMessage(&msg); }
    }
    result=adapter_failed ? 1 : 0;
cleanup:
    if(data && frame_window) {stars_write_ui_config(&runtime,(const StarsU8 *)ini_file,(StarsU16)frame_window);settings_write();}
    if(title_window) DestroyWindow(title_window);
    if(frame_window) DestroyWindow(frame_window);
    stars_free_stuff(&memory);log_line("CLEANUP=PASS");
    if(log_file) { fprintf(log_file,"RESOURCE_READS=%u\nRESOURCE_BYTES=%lu\nPAINTS=%d\n",reads,read_bytes,paint_count);fflush(log_file); }
done:
    if(strings_handle) { GlobalUnlock(strings_handle);GlobalFree(strings_handle); }
    if(data_handle) { GlobalUnlock(data_handle);GlobalFree(data_handle); }
    log_line(result ? "EXIT=FAIL" : "EXIT=PASS");if(log_file) fclose(log_file);
    return result;
}
