/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

/* CodeView 211:00000ac0; NE 15:432a; ["c:\\user\\jeffl\\2.7j\\file.c"]
   Raw procedure type 0x1036 lists zero parameters; BP-relative parameter records and RETF 10
   establish a four-argument far Pascal callback. This is an evidence-backed effective signature,
   not a rewrite of raw metadata.
   Locals and lexical lifetimes: analysis/debug/symbols.json */

short __stdcall16far AskSaveDialog(ushort hwnd,ushort message,ushort wParam,long lParam)

{
  short arg1;
  
  if (message != 2) {
    if (message == 0x110) {
      return 1;
    }
    if (message == 0x111) {
      if (((wParam == 0x429) || (wParam == 0x42b)) || (wParam == 0x42a)) {
        if (wParam == 0x42b) {
          arg1 = 0;
        }
        else if (wParam == 0x42a) {
          arg1 = -1;
        }
        else {
          arg1 = 1;
        }
        ENDDIALOG(hwnd,arg1);
        return 1;
      }
      if (wParam == 0x76) {
        WINHELP(hwnd,(schar *)CONCAT22(0x1120,szHelpFile),1,0x442);
        return 1;
      }
    }
  }
  return 0;
}

