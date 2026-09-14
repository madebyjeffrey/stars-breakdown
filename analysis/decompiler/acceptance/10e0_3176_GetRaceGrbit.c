/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

/* CodeView 178:00000c90; NE 29:3176; ["c:\\user\\jeffl\\2.7j\\race.c"]
   Procedure type agrees with ordered BP-relative parameter records.
   Locals and lexical lifetimes: analysis/debug/symbols.json */

short __cdecl16far GetRaceGrbit(_player *pplr,short ibit)

{
  short sVar1;
  long lVar2;
  
  lVar2 = __aFlshl(1,(uchar)ibit);
  if ((((uint)lVar2 & (uint)pplr->grbitAttr) == 0) &&
     (((uint)((ulong)lVar2 >> 0x10) & *(uint *)((int)&pplr->grbitAttr + 2)) == 0)) {
    sVar1 = 0;
  }
  else {
    sVar1 = 1;
  }
  return sVar1;
}

