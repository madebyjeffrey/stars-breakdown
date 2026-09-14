/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

long __stdcall16far __aFlmul(long left,long right)

{
  if (right._2_2_ == 0 && left._2_2_ == 0) {
    return (left & 0xffffU) * (right & 0xffffU);
  }
  return right * left;
}

