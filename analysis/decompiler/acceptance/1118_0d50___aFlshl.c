/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

long __cdecl16far __aFlshl(long value,uchar count)

{
  uint uVar1;
  
  for (uVar1 = (uint)count; uVar1 != 0; uVar1 = uVar1 - 1) {
    value = CONCAT22((int)(((ulong)value >> 0x10) << 1 | (ulong)((int)value < 0)),(int)value << 1);
  }
  return value;
}

