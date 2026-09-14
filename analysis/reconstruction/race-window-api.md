# Race component Windows-call contract

`StarsWindowCall` is a host adapter record, not a Win16 stack layout. `window`,
`a`, `b` are unsigned 16-bit storage; `value` is 32 bits; `p` and `q` are host
pointers. Coordinates and signed results must be interpreted as signed Win16
words where the corresponding Windows API requires them. Results use the
original low word or DWORD; a negative word result can be returned in its low
16-bit representation. Unused numeric fields are zero and unused pointers null.

The table lists each operation used by the component. `h` means `window` and
`v` means `value`. Point/rectangle members are little-endian 16-bit coordinates.
Buffers are borrowed for the duration of a call; the adapter must not retain
stack pointers after returning.

| Operation | Input/output mapping |
|---|---|
| GetFocus | No arguments; return focus HWND. |
| MessageBox | h=owner, a=type, p=text, q=caption; return button result. |
| GetDlgItem | h=dialog, a=control ID; return HWND. |
| GetDlgItemText | h=dialog, a=ID, b=capacity including NUL, p=output; return character count. |
| SetDlgItemText | h=dialog, a=ID, p=NUL-terminated text. |
| SetWindowText | h=window, p=NUL-terminated text. |
| SendDlgItemMessage | h=dialog, a=ID, b=message, v=LPARAM, p=pointer to a U16 WPARAM. |
| SendMessage | h=control, a=message, b=WPARAM, v=numeric LPARAM. For combo-box add-string (0x403), p is the text and v is zero. |
| CheckRadioButton | h=dialog, a=first ID, b=last ID, v=selected ID. |
| CheckDlgButton | h=dialog, a=ID, b=check state. |
| IsDlgButtonChecked | h=dialog, a=ID; return state. |
| EnableWindow | h=control, a=boolean. |
| SetFocus | h=window. |
| EndDialog | h=dialog, a=signed result. |
| WinHelp | h=owner, a=command, v=topic/data, p=help filename. |
| GetWindowRect / GetClientRect | h=window, p=output RECT. |
| ScreenToClient | h=window, p=in/out POINT. |
| GetSystemMetrics | a=metric index; return signed word. |
| SetWindowPos | h=window, a=insert-after HWND, b=flags, p=four words x/y/cx/cy. |
| InvalidateRect | h=window, a=erase, p=RECT or null. |
| GetDC / ReleaseDC | Get: h=window, return HDC. Release: h=window, a=HDC. |
| BeginPaint / EndPaint | h=window, p=32-byte Win16 PAINTSTRUCT; Begin returns HDC. |
| SelectObject | h=HDC, a=object; return previous object. |
| SelectPalette | h=HDC, a=palette, b=force-background; return previous palette. |
| RealizePalette | h=HDC. |
| SetBkMode | h=HDC, a=mode; return previous mode. |
| SetBkColor / SetTextColor | h=HDC, v=COLORREF; return previous color. |
| GetTextExtent | h=HDC, a=byte count, p=text; return width in low word, height in high word. |
| GetTextMetrics | h=HDC, p=31-byte packed Win16 TEXTMETRIC output. |
| TextOut | h=HDC, a=x, b=y, v=count, p=text. |
| DrawText | h=HDC, a=count, b=flags, p=text, q=in/out RECT. |
| ExtTextOut | h=HDC, a=x, b=y, v=(options<<16)|count, p=text, q=RECT; spacing array is null. |
| FillRect / FrameRect | h=HDC, a=brush, p=RECT. |
| PatBlt | h=HDC, a=x, b=y, v=ROP, p=two words width/height. |
| LocalAlloc | a=flags, b=byte count; return a near offset into the runtime's DGROUP. The flag 0x40 requires zero initialization. |
| LocalFree | h=near allocation offset. |
| CreateFontIndirect | p=50-byte packed Win16 LOGFONT; return HFONT. |
| DeleteObject | h=object. |
| GlobalLock / GlobalUnlock | h=global handle. Lock returns a packed selector:offset DWORD resolved by the platform memory callback. |
| StretchDIBits | h=HDC, a=byte displacement of bits from the BITMAPINFO pointer, b=usage, v=ROP; p=eight words x/y/w/h/source-x/source-y/source-w/source-h; q=BITMAPINFO. |
| GetCurrentTime | No arguments; return DWORD milliseconds for button tracking. |
| PeekMessage | h=filter window, a=min message, b=max message, v=remove flag, p=18-byte Win16 MSG output; return boolean. |
| GetCursorPos | p=POINT output. |
| SetCapture / SetCursor | h=window or cursor respectively; return previous handle. |
| ReleaseCapture | No arguments. |
| MulDiv | a=signed multiplicand, b=signed multiplier, v=low-word signed divisor; return signed word with Windows rounding/overflow behavior. |

Pure rectangle operations and string length are implemented directly in the C
component. The original-instruction adapter executes their equivalent memory
operations without adding a Windows event to the trace. All other operations
are compared in order between independent argument decoders.

The independent decoder in `tools/race_ui_machine.py` reads original Pascal
stack arguments. `tests/test_race_ui.py` decodes the host record into the same
deterministic service model. This shared model supplies OS answers only; it does
not decide game control flow, score, file bytes or rendering arguments. It uses
synthetic metrics and dialog-unit scaling, not actual Windows font rendering.

The separate `dialog` platform callback receives a recovered StarsDialogProc.
A native adapter must bridge Win16 FAR PASCAL callbacks to that signature,
with the correct per-instance runtime context and original resource IDs. The
`save_choice` callback similarly maps StarsSaveChoice to OPENFILENAME. File,
clock, resource-memory and nonlocal-recovery services are described in
[race-component.md](race-component.md).
