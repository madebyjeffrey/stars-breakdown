/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl16near __trandisp2(void)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  uint uVar4;
  byte bVar5;
  byte bVar6;
  char cVar7;
  char cVar8;
  uint extraout_DX;
  int in_DX;
  undefined2 *puVar9;
  int unaff_BP;
  undefined2 unaff_SS;
  bool bVar10;
  undefined2 in_FPUControlWord;
  longdouble lVar11;
  longdouble in_ST0;
  longdouble lVar12;
  longdouble in_ST1;
  
  *(undefined2 *)(unaff_BP + -6) = in_FPUControlWord;
  *(undefined2 *)(unaff_BP + -8) = CONCAT11(0x13,*(byte *)(unaff_BP + -6) | 0x38);
  lVar12 = (longdouble)0;
  *(int *)(unaff_BP + -0x10) = in_DX;
  *(uint *)(unaff_BP + -10) =
       (uint)NAN(in_ST0) << 8 | (uint)(in_ST0 < lVar12) << 9 | (uint)(in_ST0 != lVar12) << 10 |
       (uint)(in_ST0 == lVar12) << 0xe;
  *(undefined1 *)(unaff_BP + -0x11) = 0;
  cVar7 = *(char *)(unaff_BP + -9);
  lVar12 = (longdouble)0;
  *(uint *)(unaff_BP + -10) =
       (uint)NAN(in_ST1) << 8 | (uint)(in_ST1 < lVar12) << 9 | (uint)(in_ST1 != lVar12) << 10 |
       (uint)(in_ST1 == lVar12) << 0xe;
  cVar8 = (char)(*(char *)(unaff_BP + -9) << 1) >> 1;
  bVar5 = cVar8 << 1;
  cVar7 = (char)(cVar7 << 1) >> 1;
  bVar1 = cVar7 << 1;
  bVar6 = bVar1 & 4;
  bVar2 = bVar5 & 4;
  puVar9 = (undefined2 *)
           (in_DX + (char)(*(byte *)(ulong)(((bVar1 | cVar7 < '\0') & 0xf) + 0x1abd) |
                          *(char *)(ulong)((bVar5 & 0xf | cVar8 < '\0') + 0x1abd) << 2) + 0x10);
  bVar10 = puVar9 == (undefined2 *)0x0;
                    /* WARNING: Switch is manually overridden */
  switch((undefined4)*puVar9) {
  case 0x11181698:
    if (bVar6 == 0) {
      return;
    }
    goto LAB_1118_16aa;
  case 0x111816a0:
    __rtifprojnpop();
    if (bVar10) {
      *(undefined1 *)(unaff_BP + -0x11) = 3;
      return;
    }
LAB_1118_16aa:
    __rtindfnpop();
    return;
  case 0x111816ae:
    if (bVar2 != 0) {
      DAT_1120_1900 = (uint)((unkuint10)in_ST0 >> 0x40);
      uVar3 = DAT_1120_1900 & 0x7fff;
      uVar4 = uVar3 + 0xc001;
      _DAT_1120_18f8 = in_ST0;
      if ((uVar3 < 0x3fff) || (0x3f < uVar4)) goto LAB_1118_174d;
      uVar4 = 0x3f - uVar4;
      uVar3 = CONCAT11(1,(char)uVar4) & 0xff07;
      uVar4 = uVar4 >> 3;
      bVar5 = ((char)(uVar3 >> 8) << (sbyte)uVar3) - 1U & ((undefined *)&DAT_1120_18f8)[uVar4];
      for (; uVar4 != 0; uVar4 = uVar4 - 1) {
        bVar5 = bVar5 | *(byte *)(uVar4 + 0x18f7);
      }
      if (bVar5 != 0) goto LAB_1118_174d;
      in_ST1 = ABS(in_ST1);
    }
    FUN_1118_178f();
    lVar12 = in_ST1;
    goto LAB_1118_16c2;
  case 0x111816ba:
    lVar12 = (longdouble)1.4426950408889634 * in_ST0;
    in_ST0 = in_ST1;
LAB_1118_16c2:
    __ffexpm1();
    lVar12 = (longdouble)1 + lVar12;
    if ((*(byte *)(unaff_BP + -9) & 1) != 0) {
      lVar12 = (longdouble)1 / lVar12;
    }
    if ((extraout_DX & 0x40) == 0) {
      fscale(lVar12,in_ST0);
    }
    __rttospop();
    return;
  case 0x1118173c:
    if (__cpower != '\x01') {
      __rtonepop();
      return;
    }
    goto LAB_1118_174d;
  case 0x11181746:
    *(undefined1 *)(unaff_BP + -0x11) = 2;
    goto LAB_1118_174d;
  case 0x1118175a:
    goto code_r0x1118175b;
  case 0x11181780:
    goto code_r0x11181789;
  case 0x11181785:
code_r0x11181789:
    if (bVar6 == 0) {
      log2(in_ST0);
      return;
    }
LAB_1118_174d:
    __rtindfpop();
    return;
  case 0x11181793:
    __rtonepop();
    return;
  case 0x11181796:
    __rtifprojpop();
  case 0x11181799:
    if (bVar6 == 0) {
LAB_1118_17a0:
      __rtzeronpop();
      return;
    }
    if (__cpower != '\x01') {
code_r0x1118175b:
      *(undefined1 *)(unaff_BP + -0x11) = 2;
      return;
    }
    goto LAB_1118_174d;
  case 0x111817bb:
    bVar10 = bVar2 == 0;
    if (!bVar10) {
LAB_1118_17a6:
      __rtindfnpop();
      return;
    }
    __rtifprojpop();
    if (bVar10) goto LAB_1118_17cf;
    goto LAB_1118_17a0;
  case 0x111817d4:
    __rtifprojnpop();
    if (bVar10) goto LAB_1118_17cf;
    goto LAB_1118_17a0;
  case 0x111817db:
    __rtifprojnpop();
    if (!bVar10) goto LAB_1118_17a6;
LAB_1118_17cf:
    *(undefined1 *)(unaff_BP + -0x11) = 3;
    return;
  case 0x1118182c:
    if (__80x87 < 3) {
      _Soft_fFCOS();
      return;
    }
    FUN_1118_18ff();
    fcos(in_ST0);
    return;
  case 0x1118183c:
    if (__80x87 < 3) {
      _Soft_fFSIN();
      return;
    }
    FUN_1118_18ff();
    fsin(in_ST0);
    return;
  case 0x1118184e:
    if (2 < __80x87) {
      FUN_1118_18ff();
      FUN_1118_1890();
      return;
    }
    _Soft_fFTAN();
    return;
  case 0x11181863:
    FUN_1118_18ff();
    FUN_1118_1890();
    return;
  case 0x11181884:
    *(undefined1 *)(unaff_BP + -0x11) = 5;
    __rtindfnpop();
    return;
  case 0x1118192a:
    FUN_1118_1986();
    lVar12 = in_ST0;
    break;
  case 0x11181931:
    FUN_1118_1986();
    lVar12 = in_ST1;
    in_ST1 = in_ST0;
    break;
  case 0x11181939:
    lVar12 = (longdouble)1;
    in_ST1 = ABS(in_ST0);
    break;
  case 0x11181945:
    lVar12 = ABS(in_ST0);
    in_ST1 = ABS(in_ST1);
    break;
  case 0x111819ba:
    return;
  case 0x111819c3:
    if (bVar6 != 0) {
      return;
    }
    __rtzeronpop();
    return;
  case 0x111819d4:
    if ((*(byte *)(unaff_BP + -5) & 0x10) != 0) goto LAB_1118_19ea;
  case 0x111819da:
    __rtindfnpop();
    return;
  case 0x111819e0:
    goto LAB_1118_19ea;
  case 0x111819e7:
    __rtifprojnpop();
LAB_1118_19ea:
    __rtpiby2();
    __rtchsifneg();
    return;
  case 0x11182485:
    __rttosnpopde();
  case 0x1118248e:
    return;
  case 0x1118248f:
    __rttosnpopde();
    __rtnospop();
    return;
  case 0x11182497:
    return;
  case 0x111824a1:
    return;
  case 0x111824a8:
    __rttosnpopde();
    return;
  case 0x111824b6:
  case 0x111824b9:
    if (*(char *)(unaff_BP + -0x11) < '\x01') {
code_r0x111824c7:
      *(undefined1 *)(unaff_BP + -0x11) = 1;
    }
    return;
  case 0x111824c7:
    goto code_r0x111824c7;
  }
  *(uint *)(unaff_BP + -10) =
       (uint)(lVar12 < in_ST1) << 8 | (uint)(NAN(lVar12) || NAN(in_ST1)) << 10 |
       (uint)(lVar12 == in_ST1) << 0xe;
  lVar11 = lVar12;
  if ((*(byte *)(unaff_BP + -9) & 1) != 0) {
    lVar11 = in_ST1;
    in_ST1 = lVar12;
  }
  fpatan(in_ST1,lVar11);
  return;
}

