/* Native OS services for the previously verified race component. */
#include "APP.H"
#include <commdlg.h>
#include <toolhelp.h>
#include <dos.h>
#include <io.h>
#include <stdlib.h>
#include <time.h>

static StarsDialogProc dialog_proc;
static int check_mode, check_page, check_fail, check_paints;
static char check_file[180], recent_race[144];
static char chosen_file[256];
static unsigned race_saves, race_loads;
static int race_ready;
static StarsI16 summary_points;
static int use_fallback,open_workshop;
int settings_fallback(void) {return use_fallback;}
int settings_workshop(void) {return open_workshop;}

static StarsI16 native_open(void *ctx,const StarsU8 *name,StarsU16 mode,StarsU16 *error)
{ OFSTRUCT of; HFILE h; (void)ctx; memset(&of,0,sizeof(of)); of.cBytes=sizeof(of);
  h=OpenFile((LPCSTR)name,&of,mode); *error=of.nErrCode; return h; }
static StarsI16 native_close(void *ctx,StarsI16 h)
{ (void)ctx; return _lclose(h); }
static StarsU16 native_read(void *ctx,StarsI16 h,StarsU8 *p,StarsU16 n)
{ (void)ctx; return _lread(h,p,n); }
static StarsU16 native_write(void *ctx,StarsI16 h,const StarsU8 *p,StarsU16 n)
{ (void)ctx; return _lwrite(h,(LPCSTR)p,n); }
static StarsU32 native_ticks(void *ctx)
{ (void)ctx; return GetTickCount(); }
static StarsU32 native_seek(void *ctx,StarsI16 h,StarsU32 n,StarsI16 origin)
{ (void)ctx; return _llseek(h,(LONG)n,origin); }
static void native_date(void *ctx,StarsU8 *p)
{ (void)ctx; _strdate((char *)p); }
static void native_time(void *ctx,StarsU8 *p)
{ (void)ctx; _strtime((char *)p); }
static StarsU32 native_timer(void *ctx)
{ TIMERINFO t; (void)ctx; memset(&t,0,sizeof(t)); t.dwSize=sizeof(t);
  if (!TimerCount(&t)) return GetTickCount(); return t.dwmsSinceStart; }
static void native_abort(void *ctx,StarsU16 token,StarsI16 value)
{ (void)ctx;(void)token;(void)value;log_line("RECOVERY=FAIL");FatalAppExit(0,"Unhandled race file error"); }

/* Each modal invocation retains its own callback, including password dialogs
 * nested during wizard completion. MakeProcInstance supplies Win16 DS. */
BOOL CALLBACK RaceProc(HWND h,UINT m,WPARAM w,LPARAM l)
{
    BOOL result;
    StarsDialogProc proc;
    DWORD began=GetTickCount();
    char timing[80];
    /* Parent windows can repaint while a nested password dialog is open.
     * Bind each HWND to its own recovered procedure, not the current modal
     * invocation. None of these original dialogs uses DWL_USER. */
    if(m==WM_INITDIALOG) SetWindowLong(h,DWL_USER,(LONG)dialog_proc);
    proc=(StarsDialogProc)GetWindowLong(h,DWL_USER);
    if(!proc) return FALSE;
    if (check_mode && m==WM_TIMER && w==7) {
        KillTimer(h,7);
        if (!check_paints) check_fail=1;
        if (check_mode==2) { PostMessage(h,WM_COMMAND,2,0);return TRUE; }
        if (check_page==1) {
            SetDlgItemText(h,0x10c,"Start Test");
            SetDlgItemText(h,0x81b,"Start Tests");
        }
        PostMessage(h,WM_COMMAND,check_page==6 ? 1072 : 1071,0);
        return TRUE;
    }
    result=proc(&runtime,(StarsU16)h,m,w,(StarsU32)l);
    if (m==WM_INITDIALOG) {
        log_line("DIALOG_OPEN=PASS");
        if (check_mode) { ++check_page;check_paints=0;
            if(check_page>6 || !SetTimer(h,7,750,NULL)) {check_fail=1;PostMessage(h,WM_COMMAND,2,0);} }
    }
    if (m==WM_PAINT) {++check_paints;sprintf(timing,"DIALOG_PAINT_MS=%lu",GetTickCount()-began);log_line(timing);}
    if (m==WM_CLOSE) { SendMessage(h,WM_COMMAND,2,0);return TRUE; }
    return result;
}
static StarsI16 native_dialog(void *ctx,StarsRuntime *r,StarsU16 resource,StarsU16 owner,StarsDialogProc proc)
{
    StarsDialogProc old=dialog_proc;
    FARPROC thunk;
    int result;
    (void)ctx;(void)r;
    dialog_proc=proc;
    thunk=MakeProcInstance((FARPROC)RaceProc,app_instance);
    if(!thunk) {log_line("DIALOG_THUNK=FAIL");dialog_proc=old;return 0;}
    result=DialogBox(app_instance,MAKEINTRESOURCE(resource),(HWND)owner,(DLGPROC)thunk);
    FreeProcInstance(thunk);dialog_proc=old;
    if(result==-1) {log_line("DIALOG_CREATE=FAIL");return 0;}
    return result;
}
static StarsI16 native_save(void *ctx,StarsSaveChoice *c)
{
    OPENFILENAME of;
    BOOL ok;
    (void)ctx;
    if(check_mode) {
        if(_access(check_file,0)==0) {check_fail=1;return 0;}
        strcpy((char *)c->file,check_file);
        c->file_offset=(WORD)(strrchr(check_file,'\\')+1-check_file);
        strcpy(chosen_file,check_file);return 1;
    }
    memset(&of,0,sizeof(of));of.lStructSize=sizeof(of);
    of.hwndOwner=(HWND)c->owner;of.hInstance=app_instance;
    of.lpstrFilter=(LPCSTR)c->filter;of.nFilterIndex=c->filter_index;
    of.lpstrFile=(LPSTR)c->file;of.nMaxFile=c->file_capacity;
    of.lpstrFileTitle=(LPSTR)c->title;of.nMaxFileTitle=c->title_capacity;
    of.lpstrInitialDir=(LPCSTR)c->directory;of.lpstrDefExt=(LPCSTR)c->extension;
    of.Flags=c->flags;
    ok=GetSaveFileName(&of);
    if(ok) {c->file_offset=of.nFileOffset;strcpy(chosen_file,(char *)c->file);}
    else if(CommDlgExtendedError()) log_line("SAVE_DIALOG=FAIL");
    return ok;
}
void native_platform(void)
{
    platform.open=native_open;platform.close=native_close;
    platform.read=native_read;platform.write=native_write;
    platform.ticks=native_ticks;platform.seek=native_seek;
    platform.date=native_date;platform.time=native_time;
    platform.timer_count=native_timer;platform.dialog=native_dialog;
    platform.save_choice=native_save;platform.abort=native_abort;
}

StarsU32 native_window(const StarsWindowCall *c,int *handled)
{
    const char *n=c->operation;
    HWND h=(HWND)c->window; HDC dc=(HDC)c->window;
    UINT a=c->a,b=c->b; StarsU32 v=c->value;
    StarsU16 *g=(StarsU16 *)c->p;
    StarsProfileString *profile=(StarsProfileString *)c->q;
    *handled=1;
#define OP(s) (!strcmp(n,s))
    if(OP("GetWindowPlacement")) return GetWindowPlacement(h,(WINDOWPLACEMENT FAR *)c->p);
    if(OP("IsZoomed")) return IsZoomed(h);
    if(OP("GetPrivateProfileInt")) return GetPrivateProfileInt((LPCSTR)profile->section,(LPCSTR)profile->key,(short)a,(LPCSTR)profile->file);
    if(OP("WritePrivateProfileString")) return WritePrivateProfileString((LPCSTR)profile->section,(LPCSTR)profile->key,(LPCSTR)c->p,(LPCSTR)profile->file);
    if(OP("GetDlgItem")) return (StarsU16)GetDlgItem(h,a);
    if(OP("GetFocus")) return (StarsU16)GetFocus();
    if(OP("SetFocus")) return (StarsU16)SetFocus(h);
    if(OP("MessageBox")) return MessageBox(h,(LPCSTR)c->p,(LPCSTR)c->q,a);
    if(OP("SetDlgItemText")) {SetDlgItemText(h,a,(LPCSTR)c->p);return 1;}
    if(OP("GetDlgItemText")) return GetDlgItemText(h,a,(LPSTR)c->p,b);
    if(OP("SetWindowText")) {SetWindowText(h,(LPCSTR)c->p);return 1;}
    if(OP("CheckRadioButton")) {CheckRadioButton(h,a,b,(UINT)v);return 1;}
    if(OP("CheckDlgButton")) {CheckDlgButton(h,a,b);return 1;}
    if(OP("IsDlgButtonChecked")) return IsDlgButtonChecked(h,a);
    if(OP("SendDlgItemMessage")) return SendDlgItemMessage(h,a,b,*g,(LPARAM)v);
    if(OP("ScreenToClient")) {ScreenToClient(h,(POINT FAR *)c->p);return 1;}
    if(OP("SetWindowPos")) return SetWindowPos(h,(HWND)a,(short)g[0],(short)g[1],(short)g[2],(short)g[3],b);
    if(OP("DrawText")) return DrawText(dc,(LPCSTR)c->p,a,(RECT FAR *)c->q,b);
    if(OP("FrameRect")) return FrameRect(dc,(RECT FAR *)c->p,(HBRUSH)a);
    if(OP("GetCurrentTime")) return GetCurrentTime();
    if(OP("PeekMessage")) return PeekMessage((MSG FAR *)c->p,h,a,b,(UINT)v);
    if(OP("GetCursorPos")) {GetCursorPos((POINT FAR *)c->p);return 1;}
    if(OP("SetCapture")) return (StarsU16)SetCapture(h);
    if(OP("ReleaseCapture")) {ReleaseCapture();return 1;}
    if(OP("EndDialog")) {EndDialog(h,(short)a);return 1;}
    if(OP("WinHelp")) return WinHelp(h,(LPCSTR)c->p,a,v);
    if(OP("LoadBitmap")) return (StarsU16)LoadBitmap((HINSTANCE)h,(LPCSTR)c->p);
    if(OP("LoadIcon")) return (StarsU16)LoadIcon((HINSTANCE)h,(LPCSTR)c->p);
    if(OP("CreatePen")) return (StarsU16)CreatePen(a,(short)b,v);
    if(OP("CreateRectRgn")) return (StarsU16)CreateRectRgn((short)g[0],(short)g[1],(short)g[2],(short)g[3]);
    if(OP("CreatePatternBrush")) return (StarsU16)CreatePatternBrush((HBITMAP)h);
    *handled=0;return 0;
#undef OP
}

int race_resources(void)
{
    /* FCreateStuff's race resources; these handles are owned by FreeStuff. */
    static const StarsU16 ids[3]={0x85,0x50,0x4f};
    static const StarsU16 offsets[3]={0x5476,0x2606,0x2604};
    int i;StarsU16 h;
    for(i=0;i<3;++i) {
        h=stars_load_big_resource(&runtime,ids[i]);
        stars_put_word(runtime.data,offsets[i],h);
        if(!h) {log_line("RACE_RESOURCES=FAIL");return 0;}
    }
    stars_put_word(runtime.data,0x5314,stars_palette_from_dib(&runtime,stars_word(runtime.data,0x5476)));
    if(!stars_word(runtime.data,0x5314)) return 0;
    log_line("RACE_RESOURCES=PASS");return 1;
}

void workshop_show(void)
{ ShowWindow(title_window,SW_HIDE);ShowWindow(frame_window,SW_SHOWNORMAL);SetActiveWindow(frame_window);UpdateWindow(frame_window); }
void workshop_paint(HWND h)
{
    PAINTSTRUCT ps;RECT rc;char text[320];
    BeginPaint(h,&ps);GetClientRect(h,&rc);InflateRect(&rc,-18,-18);
    strcpy(text,"Create a race, change its traits, and save it as a Stars! race file.\r\n\r\nUse the Race menu to create or open a race.");
    if(race_ready) sprintf(text,"Current race: %s\r\nAdvantage points: %d\r\n\r\nUse Open Race or Recent Race to edit a saved race.\r\nCancel closes the wizard without saving.",runtime.data+0x501a,(int)summary_points);
    SetBkMode(ps.hdc,TRANSPARENT);DrawText(ps.hdc,text,-1,&rc,DT_LEFT|DT_WORDBREAK);EndPaint(h,&ps);
}
static void remember_save(void)
{
    if(strlen(chosen_file)<sizeof(recent_race)) strcpy(recent_race,chosen_file);
    ++race_saves;log_line("RACE_SAVE=PASS");settings_write();
}
void race_command(HWND owner,unsigned cmd)
{
    OPENFILENAME of; char file[256]; int result;
    StarsU16 old_owner=stars_word(runtime.data,0x352);
    stars_put_word(runtime.data,0x352,(StarsU16)owner);
    stars_put_word(runtime.data,0x48aa,(StarsU16)owner);
    if(cmd==CMD_NEW_RACE) {
        /* New session isolation is integration behavior; stars_new_race itself
         * preserves the original caller's filename/password buffers. */
        runtime.data[0x1308]=runtime.data[0x1318]=0;
        result=stars_new_race(&runtime,(StarsU16)owner);race_ready=1;
        if(result) remember_save();else log_line("RACE_CANCEL=PASS");
    } else {
        memset(&of,0,sizeof(of));file[0]=0;
        if(cmd==CMD_RECENT_RACE) strcpy(file,recent_race);
        else {
            of.lStructSize=sizeof(of);of.hwndOwner=owner;of.lpstrFile=file;of.nMaxFile=sizeof(file);
            of.lpstrFilter="Stars! Race Files\0*.R*\0All Files\0*.*\0\0";of.nFilterIndex=1;of.lpstrInitialDir=directory;
            of.Flags=OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;of.lpstrDefExt="r1";
            if(!GetOpenFileName(&of)) goto done;
        }
        if(!file[0]) {MessageBox(owner,"No recent race has been saved or opened yet.","Open Race",MB_OK);goto done;}
        strcpy((char *)runtime.data+0xc100,file);
        result=stars_was_race_file(&runtime,0xc100,1);
        if(result!=1) {log_line("RACE_LOAD=REJECTED");MessageBox(owner,"The race file could not be opened.","Open Race",MB_OK|MB_ICONEXCLAMATION);goto done;}
        ++race_loads;race_ready=1;log_line("RACE_LOAD=PASS");
        if(strlen(file)<sizeof(recent_race)) strcpy(recent_race,file);
        if(stars_race_wizard(&runtime,(StarsU16)owner,0,0)) remember_save();else log_line("RACE_CANCEL=PASS");
    }
done:
    if(race_ready) summary_points=stars_race_advantage_points(runtime.data,0x4f9a);
    stars_put_word(runtime.data,0x352,old_owner);InvalidateRect(frame_window,NULL,TRUE);settings_write();
}

void settings_read(void)
{
    StarsPoint point;
    use_fallback=GetPrivateProfileInt("Reconstruction","Starfield",0,ini_file)!=0;
    open_workshop=GetPrivateProfileInt("Reconstruction","OpenWorkshop",0,ini_file)!=0;
    GetPrivateProfileString("Reconstruction","RecentRace","",recent_race,sizeof(recent_race),ini_file);
    point.x=(WORD)GetPrivateProfileInt("Reconstruction","RaceX",-1,ini_file);
    point.y=(WORD)GetPrivateProfileInt("Reconstruction","RaceY",-1,ini_file);
    stars_put_word(runtime.data,0x12ec,point.x);stars_put_word(runtime.data,0x12ee,point.y);
}
void settings_write(void)
{
    char value[20];
    WritePrivateProfileString("Reconstruction","Starfield",use_fallback ? "1":"0",ini_file);
    WritePrivateProfileString("Reconstruction","OpenWorkshop",open_workshop ? "1":"0",ini_file);
    WritePrivateProfileString("Reconstruction","RecentRace",recent_race,ini_file);
    sprintf(value,"%d",(short)stars_word(runtime.data,0x12ec));WritePrivateProfileString("Reconstruction","RaceX",value,ini_file);
    sprintf(value,"%d",(short)stars_word(runtime.data,0x12ee));WritePrivateProfileString("Reconstruction","RaceY",value,ini_file);
}

int native_selftest(void)
{
    StarsU8 saved[192];int result,i;
    char name[16];
    for(i=1;i<1000;++i) {sprintf(name,"CHECK%03d.R1",i);strcpy(check_file,directory);strcat(check_file,name);if(_access(check_file,0)!=0)break;}
    if(i==1000) return 0;
    SetWindowText(frame_window,"Stars! Race Workshop - Automatic check running");
    log_line("ACCEPTANCE=RUNNING");
    check_page=check_fail=0;check_mode=1;
    runtime.data[0x1308]=runtime.data[0x1318]=0;
    result=stars_new_race(&runtime,(StarsU16)frame_window);
    check_mode=0;
    if(!result || check_page!=6 || check_fail) {log_line("WIZARD_CHECK=FAIL");return 0;}
    memcpy(saved,runtime.data+0x4f9a,192);
    strcpy((char *)runtime.data+0xc100,check_file);
    memset(runtime.data+0x4f9a,0,192);
    result=stars_was_race_file(&runtime,0xc100,1);
    if(result!=1 || memcmp(saved,runtime.data+0x4f9a,192)) {log_line("ROUNDTRIP=FAIL");return 0;}
    log_line("WIZARD_CHECK=PASS");log_line("ROUNDTRIP=PASS");
    check_page=check_fail=0;check_mode=2;
    result=stars_race_wizard(&runtime,(StarsU16)frame_window,0,0);check_mode=0;
    if(result || check_page!=1 || check_fail) {log_line("CANCEL_CHECK=FAIL");return 0;}
    log_line("CANCEL_CHECK=PASS");log_line("ACCEPTANCE=PASS");
    SetWindowText(frame_window,"Stars! Race Workshop");race_ready=1;summary_points=stars_race_advantage_points(runtime.data,0x4f9a);return 1;
}

/* New settings dialog exposes the original font keys and package preferences.
 * Fonts are re-created on next launch; live USER controls retain valid handles. */
LRESULT CALLBACK SettingsProc(HWND h,UINT m,WPARAM w,LPARAM l)
{
    char text[80],key[32],section[32];int i,n;
    static const char *labels[4]={"Regular font:","Bold font:","Italic font:","Bold italic:"};
    if(m==WM_CREATE) {
        for(i=0;i<4;++i) {
            CreateWindow("STATIC",labels[i],WS_CHILD|WS_VISIBLE,16,18+i*36,90,20,h,NULL,app_instance,NULL);
            CreateWindow("EDIT",(LPCSTR)runtime.data+0x6a6+i*32,WS_CHILD|WS_VISIBLE|WS_BORDER|WS_TABSTOP|ES_AUTOHSCROLL,
                110,16+i*36,210,24,h,(HMENU)(700+i),app_instance,NULL);
            SendDlgItemMessage(h,700+i,EM_LIMITTEXT,31,0);
        }
        CreateWindow("BUTTON","Use the &starfield title",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX,16,170,310,22,h,(HMENU)704,app_instance,NULL);
        CreateWindow("BUTTON","Open directly in Race &Workshop",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX,16,198,310,22,h,(HMENU)705,app_instance,NULL);
        CheckDlgButton(h,704,use_fallback);CheckDlgButton(h,705,open_workshop);
        CreateWindow("STATIC","Changes apply on the next launch.",WS_CHILD|WS_VISIBLE,16,234,315,20,h,NULL,app_instance,NULL);
        CreateWindow("BUTTON","&Save",WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_DEFPUSHBUTTON,140,270,84,28,h,(HMENU)IDOK,app_instance,NULL);
        CreateWindow("BUTTON","Cancel",WS_CHILD|WS_VISIBLE|WS_TABSTOP,235,270,84,28,h,(HMENU)IDCANCEL,app_instance,NULL);
        return 0;
    }
    if(m==WM_COMMAND && (w==IDOK || w==IDCANCEL)) {
        if(w==IDOK) {
            for(i=0;i<4;++i) {
                n=GetDlgItemText(h,700+i,text,sizeof(text));
                if(n<5 || n>31) {MessageBox(h,"Font names must contain 5 to 31 characters, matching the original settings reader.","Font settings",MB_OK);SetFocus(GetDlgItem(h,700+i));return 0;}
            }
            strcpy(section,(char *)runtime.data+stars_game_string(&runtime,0xc4));
            for(i=0;i<4;++i) {
                GetDlgItemText(h,700+i,text,sizeof(text));strcpy(key,(char *)runtime.data+stars_game_string(&runtime,(StarsI16)(0xc5+i)));
                WritePrivateProfileString(section,key,text,ini_file);
            }
            use_fallback=IsDlgButtonChecked(h,704)!=0;open_workshop=IsDlgButtonChecked(h,705)!=0;
            settings_write();log_line("SETTINGS_SAVE=PASS");
        }
        DestroyWindow(h);return 0;
    }
    if(m==WM_CLOSE) {DestroyWindow(h);return 0;}
    return DefWindowProc(h,m,w,l);
}
void settings_show(HWND owner)
{
    HWND h;MSG msg;
    h=CreateWindow("StarsRecSettings","Settings",WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_DLGFRAME,
        GetSystemMetrics(SM_CXSCREEN)/2-174,GetSystemMetrics(SM_CYSCREEN)/2-172,348,344,owner,NULL,app_instance,NULL);
    if(!h) {log_line("SETTINGS_DIALOG=FAIL");return;}
    EnableWindow(owner,FALSE);ShowWindow(h,SW_SHOW);SetFocus(GetDlgItem(h,700));
    while(IsWindow(h) && GetMessage(&msg,NULL,0,0)) {
        if(!IsDialogMessage(h,&msg)) {TranslateMessage(&msg);DispatchMessage(&msg);}
    }
    EnableWindow(owner,TRUE);SetActiveWindow(owner);
}
