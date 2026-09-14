/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

/* WARNING: Removing unreachable block (ram,0x10085eaf) */
/* WARNING: Removing unreachable block (ram,0x10085fdb) */
/* WARNING: Removing unreachable block (ram,0x10085997) */
/* WARNING: Removing unreachable block (ram,0x10085f02) */
/* CodeView 188:00000214; NE 2:524e; ["c:\\user\\jeffl\\2.7j\\parts.c"]
   Procedure type agrees with ordered BP-relative parameter records.
   Locals and lexical lifetimes: analysis/debug/symbols.json */

short __cdecl16far FLookupPart(_part *ppart)

{
  ushort uVar1;
  _hs_at_2 _Var2;
  _armor *p_Var3;
  short sVar4;
  short sVar5;
  
  sVar4 = GetRaceStat((_player *)rgplr + idPlayer,0xe);
  uVar1 = (ppart->hs).grhst;
  _Var2 = (ppart->hs).overlap_2;
  if (uVar1 == 1) {
    if (0xf < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x4e;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      if ((((uint)_Var2 & 0xff) == 0) && (sVar4 != 0)) {
        return -1;
      }
      if ((((10 < ((uint)_Var2 & 0xff)) && (((uint)_Var2 & 0xff) < 0x10)) ||
          (((uint)_Var2 & 0xff) == 10)) &&
         (sVar4 = GetRaceGrbit((_player *)rgplr + idPlayer,7), sVar4 != 0)) {
        return -1;
      }
      if (((((uint)_Var2 & 0xff) == 0xf) || (((uint)_Var2 & 0xff) == 2)) &&
         (sVar4 = GetRaceGrbit((_player *)rgplr + idPlayer,0), sVar4 == 0)) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) == 7) &&
         (sVar4 = GetRaceGrbit((_player *)rgplr + idPlayer,7), sVar4 == 0)) {
        return -1;
      }
      sVar4 = FShouldPartBeHidden(ppart);
      if (sVar4 != 0) {
        return -1;
      }
    }
  }
  else if (uVar1 == 2) {
    if (0xf < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x38 + 0x768;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      if ((((((uint)_Var2 & 0xff) == 7) || (((uint)_Var2 & 0xff) == 8)) ||
          (((uint)_Var2 & 0xff) == 0xc)) &&
         (sVar5 = GetRaceGrbit((_player *)rgplr + idPlayer,10), sVar5 != 0)) {
        return -1;
      }
      if ((((((uint)_Var2 & 0xff) == 6) || (((uint)_Var2 & 0xff) == 5)) ||
          (((uint)_Var2 & 0xff) == 0xe)) && (sVar4 != 1)) {
        return -1;
      }
    }
  }
  else if (uVar1 == 4) {
    if (9 < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x36 + 0xae8;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if ((((uint)_Var2 & 0xff) == 4) && (sVar4 != 1)) {
      return -1;
    }
    if ((((uint)_Var2 & 0xff) == 3) && (sVar4 != 4)) {
      return -1;
    }
    sVar4 = FShouldPartBeHidden(ppart);
    if (sVar4 != 0) {
      return -1;
    }
  }
  else if (uVar1 == 8) {
    if (0xb < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x36 + 0x4e0;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if ((((uint)_Var2 & 0xff) == 7) && (sVar4 != 1)) {
      return -1;
    }
    if ((((uint)_Var2 & 0xff) == 6) && (sVar4 != 4)) {
      return -1;
    }
    sVar4 = FShouldPartBeHidden(ppart);
    if (sVar4 != 0) {
      return -1;
    }
  }
  else if (uVar1 == 0x10) {
    if (0x17 < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x3c + 0x2450;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if ((((uint)_Var2 & 0xff) == 2) && (sVar4 != 4)) {
      return -1;
    }
    if (((((uint)_Var2 & 0xff) == 0x10) || (((uint)_Var2 & 0xff) == 0xe)) && (sVar4 != 2)) {
      return -1;
    }
    sVar4 = FShouldPartBeHidden(ppart);
    if (sVar4 != 0) {
      return -1;
    }
  }
  else if (uVar1 == 0x20) {
    if (0xb < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(char **)&((&ppart->overlap_4)->pcom_view).pcom =
         (char *)s_M6108__MATH___floating_point_err_1120_2153 + 0x2d + ((uint)_Var2 & 0xff) * 0x3c;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if ((idPlayer != -1) && (sVar4 = FShouldPartBeHidden(ppart), sVar4 != 0)) {
      return -1;
    }
  }
  else if (uVar1 == 0x40) {
    if (0xe < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x3a + 0x1e1a;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      if (((9 < ((uint)_Var2 & 0xff)) && (((uint)_Var2 & 0xff) < 0xf)) && (sVar4 == 4)) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) == 9) && (sVar4 != 3)) {
        return -1;
      }
      sVar4 = FShouldPartBeHidden(ppart);
      if (sVar4 != 0) {
        return -1;
      }
    }
  }
  else if (uVar1 == 0x80) {
    if (7 < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x36 + 0x1508;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      if (((((((uint)_Var2 & 0xff) == 2) || (((uint)_Var2 & 0xff) == 3)) ||
           (((uint)_Var2 & 0xff) == 4)) ||
          ((((uint)_Var2 & 0xff) == 0 || (((uint)_Var2 & 0xff) == 5)))) &&
         (sVar5 = GetRaceGrbit((_player *)rgplr + idPlayer,9), sVar5 != 0)) {
        return -1;
      }
      if (((((uint)_Var2 & 0xff) == 0) || (((uint)_Var2 & 0xff) == 5)) &&
         (sVar5 = GetRaceGrbit((_player *)rgplr + idPlayer,2), sVar5 == 0)) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) == 7) && (sVar4 != 3)) {
        return -1;
      }
      sVar4 = FShouldPartBeHidden(ppart);
      if (sVar4 != 0) {
        return -1;
      }
    }
  }
  else if (uVar1 == 0x100) {
    if (9 < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom =
         (int)(tagPOINT *)&ptStickyRaceDlg + ((uint)_Var2 & 0xff) * 0x36;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      if ((((((uint)_Var2 & 0xff) == 0) || (((uint)_Var2 & 0xff) == 2)) ||
          ((((uint)_Var2 & 0xff) == 3 ||
           ((((((uint)_Var2 & 0xff) == 4 || (((uint)_Var2 & 0xff) == 5)) ||
             (((uint)_Var2 & 0xff) == 6)) ||
            ((((uint)_Var2 & 0xff) == 8 || (((uint)_Var2 & 0xff) == 9)))))))) && (sVar4 != 5)) {
        return -1;
      }
      if (((((uint)_Var2 & 0xff) == 7) && (sVar4 != 5)) && (sVar4 != 4)) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) == 1) && (sVar4 == 2)) {
        return -1;
      }
    }
  }
  else if (uVar1 == 0x200) {
    if (0xf < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(schar **)&((&ppart->overlap_4)->pcom_view).pcom =
         (schar *)rgbCur + 0x1f2 + ((uint)_Var2 & 0xff) * 0x38;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      if ((((uint)_Var2 & 0xff) < 7) || (0xf < ((uint)_Var2 & 0xff))) {
        if (((uint)_Var2 & 0xff) < 7) {
          if ((sVar4 != 7) && ((((uint)_Var2 & 0xff) == 1 || (3 < ((uint)_Var2 & 0xff))))) {
            return -1;
          }
          if (sVar4 == 0) {
            return -1;
          }
        }
      }
      else if (((((uint)_Var2 & 0xff) != 9) && (((uint)_Var2 & 0xff) != 0xc)) && (sVar4 != 6)) {
        return -1;
      }
    }
  }
  else if (uVar1 == 0x400) {
    if (4 < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(schar **)&((&ppart->overlap_4)->pcom_view).pcom =
         (schar *)szMsgTitle + 0x26 + ((uint)_Var2 & 0xff) * 0x8f;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      if (((((uint)_Var2 & 0xff) == 1) || (((uint)_Var2 & 0xff) == 3)) &&
         (sVar5 = GetRaceGrbit((_player *)rgplr + idPlayer,3), sVar5 == 0)) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) == 4) && (sVar4 != 8)) {
        return -1;
      }
    }
  }
  else if (uVar1 == 0x800) {
    if (0x10 < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x36 + 0xd04;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      sVar5 = FShouldPartBeHidden(ppart);
      if (sVar5 != 0) {
        return -1;
      }
      switch((uint)_Var2 & 0xff) {
      case 0:
      case 3:
        if (sVar4 != 1) {
          return -1;
        }
        break;
      case 8:
      case 0xb:
      case 0xf:
        if (sVar4 != 4) {
          return -1;
        }
        break;
      case 0xd:
        if (sVar4 != 0) {
          return -1;
        }
        break;
      case 0xe:
        if (sVar4 != 5) {
          return -1;
        }
        break;
      case 0x10:
        if (sVar4 != 7) {
          return -1;
        }
      }
    }
  }
  else if (uVar1 == 0x1000) {
    if (10 < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x36 + 0x109a;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      sVar5 = FShouldPartBeHidden(ppart);
      if (sVar5 != 0) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) == 0) && (sVar4 == 8)) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) == 1) && (sVar4 != 8)) {
        return -1;
      }
    }
  }
  else if (uVar1 == 0x2000) {
    if (0x13 < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x36 + 0x19e2;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (((idPlayer != -1) && (((uint)_Var2 & 0xff) < 8)) &&
       (sVar4 = GetRaceGrbit((_player *)rgplr + idPlayer,1), sVar4 == 0)) {
      return -1;
    }
  }
  else if (uVar1 == 0x4000) {
    if (0x1f < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(uint *)&((&ppart->overlap_4)->pcom_view).pcom = ((uint)_Var2 & 0xff) * 0x8f + 0x29f0;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      if (((((uint)_Var2 & 0xff) == 0xe) || (((uint)_Var2 & 0xff) == 0x1f)) && (sVar4 != 0)) {
        return -1;
      }
      if (((((uint)_Var2 & 0xff) == 0x19) || (((uint)_Var2 & 0xff) == 3)) && (sVar4 != 4)) {
        return -1;
      }
      if ((((((uint)_Var2 & 0xff) == 0x16) || (((uint)_Var2 & 0xff) == 0x17)) ||
          ((((uint)_Var2 & 0xff) == 0x14 || (((uint)_Var2 & 0xff) == 0x18)))) &&
         (sVar5 = GetRaceGrbit((_player *)rgplr + idPlayer,9), sVar5 != 0)) {
        return -1;
      }
      if ((((((uint)_Var2 & 0xff) == 0x14) || (((uint)_Var2 & 0xff) == 0x16)) ||
          (((uint)_Var2 & 0xff) == 0x18)) &&
         (sVar5 = GetRaceGrbit((_player *)rgplr + idPlayer,2), sVar5 == 0)) {
        return -1;
      }
      if (((((uint)_Var2 & 0xff) == 10) || (((uint)_Var2 & 0xff) == 8)) && (sVar4 != 2)) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) == 0xc) && (sVar4 != 1)) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) == 0x12) && (sVar4 != 1)) {
        return -1;
      }
      if (((((uint)_Var2 & 0xff) == 0x1b) || (((uint)_Var2 & 0xff) == 0x1c)) && (sVar4 != 5)) {
        return -1;
      }
      sVar4 = FShouldPartBeHidden(ppart);
      if (sVar4 != 0) {
        return -1;
      }
    }
  }
  else {
    if (uVar1 != 0x8000) {
      return 0;
    }
    if (0xe < ((uint)_Var2 & 0xff)) {
      return 0;
    }
    *(ushort **)&((&ppart->overlap_4)->pcom_view).pcom =
         (ushort *)&hwndZipOrderDlg + ((uint)_Var2 & 0xff) * 0x1b;
    *(undefined2 *)((int)&ppart->overlap_4 + 2) = 0x10e0;
    if (idPlayer != -1) {
      if (((((uint)_Var2 & 0xff) < 9) &&
          (p_Var3 = ((&ppart->overlap_4)->parmor_view).parmor, ((_armor *)p_Var3)->dp < 0)) &&
         (sVar5 = GetRaceGrbit((_player *)rgplr + idPlayer,10), sVar5 != 0)) {
        return -1;
      }
      if ((((uint)_Var2 & 0xff) < 9) && (sVar4 == 8)) {
        return -1;
      }
      if (((8 < ((uint)_Var2 & 0xff)) && (((uint)_Var2 & 0xff) < 0xe)) && (sVar4 == 8)) {
        return -1;
      }
      if (((10 < ((uint)_Var2 & 0xff)) && (((uint)_Var2 & 0xff) < 0xe)) && (sVar4 == 2)) {
        return -1;
      }
      sVar4 = FShouldPartBeHidden(ppart);
      if (sVar4 != 0) {
        return -1;
      }
    }
  }
  sVar4 = TechStatus((schar *)CONCAT22(*(undefined2 *)((int)&ppart->overlap_4 + 2),
                                       (schar *)(*(int *)&((&ppart->overlap_4)->pcom_view).pcom + 2)
                                      ));
  return sVar4;
}

