/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

/* WARNING: Removing unreachable block (ram,0x111820e5) */

undefined8 * __cdecl16far __ctrandisp1(double param_1,undefined8 param_2)

{
  int iVar1;
  char cVar2;
  undefined8 *puVar3;
  uint uVar4;
  longdouble lVar5;
  byte bStack_13;
  int iStack_12;
  
  lVar5 = (longdouble)param_1;
  __trandisp1();
  DAT_1120_1aa4 = '\0';
  puVar3 = (undefined8 *)(uint)bStack_13;
  if ((((char)bStack_13 < '\x01') || (bStack_13 == 6)) && (__fac = (double)lVar5, bStack_13 != 6))
  goto LAB_1118_20c9;
  uVar4 = (uint)(char)bStack_13;
  DAT_1120_1a72 = (int *)(iStack_12 + 1);
  DAT_1120_1aa3 = '\0';
  if (((*DAT_1120_1a72 == 0x6f6c) && (*(char *)(iStack_12 + 3) == 'g')) && (bStack_13 == 2)) {
    DAT_1120_1aa3 = '\x01';
  }
  DAT_1120_1a74 = param_1;
  if (*(char *)(iStack_12 + 0xd) != '\x01') {
    DAT_1120_1a7c = param_2;
  }
                    /* WARNING: Switch is manually overridden */
  switch(*(undefined1 *)((int)DAT_1120_1a72 + uVar4 + 5)) {
  case 2:
    lVar5 = (longdouble)0;
    break;
  case 4:
    if (lVar5 < (longdouble)0) goto code_r0x11182163;
    lVar5 = (longdouble)__HUGE;
    break;
  case 6:
code_r0x11182163:
    lVar5 = -(longdouble)__HUGE;
  }
  DAT_1120_1a84 = (double)lVar5;
  DAT_1120_1a70 = uVar4;
  puVar3 = (undefined8 *)__matherr((undefined2 *)&DAT_1120_1a70);
  if (puVar3 == (undefined8 *)0x0) {
    cVar2 = '!';
    if ((DAT_1120_1aa3 != '\0') || (2 < DAT_1120_1a70)) {
      cVar2 = '\"';
    }
    puVar3 = (undefined8 *)(int)cVar2;
    iVar1 = *(int *)(uVar4 * 2 + 0x1a92);
    _errno = puVar3;
    if (iVar1 != 0) {
      __wrt2err();
      __wrt2err();
      puVar3 = (undefined8 *)__NMSG_WRITE(iVar1);
    }
  }
  lVar5 = (longdouble)DAT_1120_1a84;
  __fac = DAT_1120_1a84;
LAB_1118_20c9:
  if (DAT_1120_1aa4 == '\0') {
    __fac = (double)lVar5;
    puVar3 = (undefined8 *)&__fac;
  }
  return puVar3;
}

