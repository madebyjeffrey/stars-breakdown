/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

long __stdcall16far __aFldiv(long left,long right)

{
  ulong uVar1;
  long lVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  bool bVar9;
  char cVar10;
  
  cVar10 = left < 0;
  if ((bool)cVar10) {
    left = CONCAT22(-(uint)((uint)left != 0) - left._2_2_,-(uint)left);
  }
  uVar3 = right._2_2_;
  if (right < 0) {
    cVar10 = cVar10 + '\x01';
    uVar3 = -(uint)((uint)right != 0) - right._2_2_;
    right = CONCAT22(uVar3,-(uint)right);
  }
  uVar5 = (uint)left;
  uVar8 = (uint)right;
  uVar6 = left._2_2_;
  if (uVar3 == 0) {
    uVar3 = left._2_2_ / (uint)right;
    iVar4 = (int)(((ulong)left._2_2_ % (right & 0xffffU) << 0x10 | left & 0xffffU) /
                 (right & 0xffffU));
  }
  else {
    do {
      uVar7 = uVar3 & 1;
      uVar3 = uVar3 >> 1;
      uVar8 = (uint)(CONCAT12(uVar7 != 0,uVar8) >> 1);
      uVar7 = uVar6 >> 1;
      uVar5 = (uint)(CONCAT12((uVar6 & 1) != 0,uVar5) >> 1);
      uVar6 = uVar7;
    } while (uVar3 != 0);
    uVar1 = CONCAT22(uVar7,uVar5) / (ulong)uVar8;
    iVar4 = (int)uVar1;
    lVar2 = (right & 0xffffU) * (uVar1 & 0xffff);
    uVar3 = (uint)((ulong)lVar2 >> 0x10);
    uVar8 = uVar3 + iVar4 * right._2_2_;
    if (((CARRY2(uVar3,iVar4 * right._2_2_)) || (left._2_2_ < uVar8)) ||
       ((left._2_2_ <= uVar8 && ((uint)left < (uint)lVar2)))) {
      iVar4 = iVar4 + -1;
    }
    uVar3 = 0;
  }
  if (cVar10 == '\x01') {
    bVar9 = iVar4 != 0;
    iVar4 = -iVar4;
    uVar3 = -(uint)bVar9 - uVar3;
  }
  return CONCAT22(uVar3,iVar4);
}

