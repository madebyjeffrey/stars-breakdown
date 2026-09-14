/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

/* CodeView 207:00001424; NE 11:6004; ["c:\\user\\jeffl\\2.7j\\ship.c"]
   Procedure type agrees with ordered BP-relative parameter records.
   Locals and lexical lifetimes: analysis/debug/symbols.json */

long __cdecl16far GetFuelFree(_fleet *lpfl)

{
  long lVar1;
  
  lVar1 = LGetFleetStat(lpfl,1);
  return CONCAT22(((int)((ulong)lVar1 >> 0x10) - *(int *)((int)((_fleet *)lpfl)->rgwtMin + 0x12)) -
                  (uint)((uint)lVar1 < *(uint *)(((_fleet *)lpfl)->rgwtMin + 4)),
                  (uint)lVar1 - *(uint *)(((_fleet *)lpfl)->rgwtMin + 4));
}

