/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

/* File 0x256: POP DX takes CALL's return address (message at file 0x203). DS=CS; INT 21h/AH=09
   prints the $-terminated message; INT 21h/AX=4c01 exits with status 1. Register/interrupt
   pseudocode is raw; these semantics come from the original instructions. */

void __cdecl16near dos_print_and_exit(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  return;
}

