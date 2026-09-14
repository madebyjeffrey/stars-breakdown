/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

/* WARNING: Removing unreachable block (ram,0x10001bda) */
/* WARNING: Restarted to delay deadcode elimination for space: ram */
/* CodeView 213:00000338; NE 1:124c; ["c:\\user\\jeffl\\2.7j\\init.c"]
   Procedure type agrees with ordered BP-relative parameter records.
   Locals and lexical lifetimes: analysis/debug/symbols.json */

void __cdecl16far ReadIniSettings(void)

{
  _itemaction_at_0 *p_Var1;
  uint *puVar2;
  char cVar3;
  _itemaction _Var4;
  schar *psVar5;
  schar *sz;
  short sVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined2 unaff_SS;
  long value;
  schar asStack_48 [16];
  char *pcStack_38;
  uint uStack_36;
  schar asStack_34 [16];
  _wn _Stack_24;
  schar sStack_1a;
  char cStack_19;
  schar asStack_18 [14];
  int iStack_a;
  uint uStack_8;
  uint uStack_6;
  
  ini.overlap_10.fStartupFile_view = ini.overlap_10.fStartupFile_view & 0xffe3;
  CchGetString(0x90,asStack_48);
  CchGetString(0x91,asStack_34);
  GetIniWinRc(asStack_48,asStack_34,0x94,&((_ini *)&ini)->wnFrame);
  GetIniWinRc(asStack_48,asStack_34,0xb2,&_Stack_24);
  if (_Stack_24.rc.left != -0x8000) {
    vrptFleet.ptDlg.x = _Stack_24.rc.left;
    vrptFleet.ptDlg.y = _Stack_24.rc.top;
    vrptFleet.ptSize.x = _Stack_24.rc.right - _Stack_24.rc.left;
    vrptFleet.ptSize.y = _Stack_24.rc.bottom - _Stack_24.rc.top;
  }
  GetIniWinRc(asStack_48,asStack_34,0xb3,&_Stack_24);
  if (_Stack_24.rc.left != -0x8000) {
    vrptEFleet.ptDlg.x = _Stack_24.rc.left;
    vrptEFleet.ptDlg.y = _Stack_24.rc.top;
    vrptEFleet.ptSize.x = _Stack_24.rc.right - _Stack_24.rc.left;
    vrptEFleet.ptSize.y = _Stack_24.rc.bottom - _Stack_24.rc.top;
  }
  GetIniWinRc(asStack_48,asStack_34,0xb4,&_Stack_24);
  if (_Stack_24.rc.left != -0x8000) {
    vrptBattle.ptDlg.x = _Stack_24.rc.left;
    vrptBattle.ptDlg.y = _Stack_24.rc.top;
    vrptBattle.ptSize.x = _Stack_24.rc.right - _Stack_24.rc.left;
    vrptBattle.ptSize.y = _Stack_24.rc.bottom - _Stack_24.rc.top;
  }
  GetIniWinRc(asStack_48,asStack_34,0xb1,&_Stack_24);
  if (_Stack_24.rc.left != -0x8000) {
    vrptPlanet.ptDlg.x = _Stack_24.rc.left;
    vrptPlanet.ptDlg.y = _Stack_24.rc.top;
    vrptPlanet.ptSize.x = _Stack_24.rc.right - _Stack_24.rc.left;
    vrptPlanet.ptSize.y = _Stack_24.rc.bottom - _Stack_24.rc.top;
  }
  CchGetString(0xad,&sStack_1a);
  uStack_8 = GETPRIVATEPROFILEINT
                       ((schar *)CONCAT22(unaff_SS,asStack_48),
                        (schar *)CONCAT22(unaff_SS,&sStack_1a),0,
                        (schar *)CONCAT22(unaff_SS,asStack_34));
  if ((uStack_8 == 0) &&
     ((vcScreenColors < 5 || ((uint)gd.overlap_0.grBits_view.grBits._0_2_ >> 0xe == 0)))) {
    sVar6 = 0x10;
    sz = PszFormatIds(0x2c9,(short *)0x0);
    AlertSz(sz,sVar6);
  }
  CchGetString(0xa3,&sStack_1a);
  iWindowLayout =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),1,
                  (schar *)CONCAT22(unaff_SS,asStack_34));
  uVar7 = iWindowLayout;
  if (0x7fff < (uint)iWindowLayout) {
    uVar7 = 0;
  }
  if ((int)uVar7 < 3) {
    if (0x7fff < (uint)iWindowLayout) {
      iWindowLayout = 0;
    }
  }
  else {
    iWindowLayout = 2;
  }
  CchGetString(0xa5,&sStack_1a);
  vfs.dxPlanWant =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
                  0x18c,(schar *)CONCAT22(unaff_SS,asStack_34));
  iVar8 = vfs.dxPlanWant;
  if (vfs.dxPlanWant < 0xb) {
    iVar8 = 10;
  }
  if (iVar8 < 2000) {
    if (vfs.dxPlanWant < 0xb) {
      vfs.dxPlanWant = 10;
    }
  }
  else {
    vfs.dxPlanWant = 2000;
  }
  CchGetString(0xa6,&sStack_1a);
  vfs.dyMsgWant =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0x6e
                  ,(schar *)CONCAT22(unaff_SS,asStack_34));
  iVar8 = vfs.dyMsgWant;
  if (vfs.dyMsgWant < 0xb) {
    iVar8 = 10;
  }
  if (iVar8 < 2000) {
    if (vfs.dyMsgWant < 0xb) {
      vfs.dyMsgWant = 10;
    }
  }
  else {
    vfs.dyMsgWant = 2000;
  }
  CchGetString(0xa7,&sStack_1a);
  vfs.dyMinWant =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0xc0
                  ,(schar *)CONCAT22(unaff_SS,asStack_34));
  iVar8 = vfs.dyMinWant;
  if (vfs.dyMinWant < 0xb) {
    iVar8 = 10;
  }
  if (iVar8 < 2000) {
    if (vfs.dyMinWant < 0xb) {
      vfs.dyMinWant = 10;
    }
  }
  else {
    vfs.dyMinWant = 2000;
  }
  CchGetString(0xa8,&sStack_1a);
  vfs.dx2PlanWant =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
                  0x18c,(schar *)CONCAT22(unaff_SS,asStack_34));
  iVar8 = vfs.dx2PlanWant;
  if (vfs.dx2PlanWant < 0xb) {
    iVar8 = 10;
  }
  if (iVar8 < 2000) {
    if (vfs.dx2PlanWant < 0xb) {
      vfs.dx2PlanWant = 10;
    }
  }
  else {
    vfs.dx2PlanWant = 2000;
  }
  CchGetString(0xa9,&sStack_1a);
  vfs.dy2MsgWant =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0x6e
                  ,(schar *)CONCAT22(unaff_SS,asStack_34));
  iVar8 = vfs.dy2MsgWant;
  if (vfs.dy2MsgWant < 0xb) {
    iVar8 = 10;
  }
  if (iVar8 < 2000) {
    if (vfs.dy2MsgWant < 0xb) {
      vfs.dy2MsgWant = 10;
    }
  }
  else {
    vfs.dy2MsgWant = 2000;
  }
  CchGetString(0xaa,&sStack_1a);
  vfs.dy2MinWant =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0xc0
                  ,(schar *)CONCAT22(unaff_SS,asStack_34));
  iVar8 = vfs.dy2MinWant;
  if (vfs.dy2MinWant < 0xb) {
    iVar8 = 10;
  }
  if (iVar8 < 2000) {
    if (vfs.dy2MinWant < 0xb) {
      vfs.dy2MinWant = 10;
    }
  }
  else {
    vfs.dy2MinWant = 2000;
  }
  CchGetString(0xab,&sStack_1a);
  uStack_8 = GETPRIVATEPROFILEINT
                       ((schar *)CONCAT22(unaff_SS,asStack_48),
                        (schar *)CONCAT22(unaff_SS,&sStack_1a),1,
                        (schar *)CONCAT22(unaff_SS,asStack_34));
  gd.overlap_0.grBits_view.grBits._2_2_ =
       gd.overlap_0.grBits_view.grBits._2_2_ & 0x7fff | (uint)(uStack_8 != 0) << 0xf;
  CchGetString(0xa4,&sStack_1a);
  sVar6 = GETPRIVATEPROFILESTRING
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
                     (schar *)0x1120073a,szWork,0x28,(schar *)CONCAT22(unaff_SS,asStack_34));
  if (sVar6 == 0x1c) {
    FSerialAndEnvFromSz((long *)&vSerialNumber,(uchar *)vrgbMachineConfig,(schar *)szWork);
  }
  else {
    vSerialNumber._0_2_ = 0;
    vSerialNumber._2_2_ = 0;
  }
  CchGetString(0x96,&sStack_1a);
  GETPRIVATEPROFILESTRING
            ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
             (schar *)0x1120073c,szWork,0x14,(schar *)CONCAT22(unaff_SS,asStack_34));
  ReadIniTileSettings((schar *)szWork,(_tile *)(_tile *)&rgtilePlanet,6);
  CchGetString(0x95,&sStack_1a);
  GETPRIVATEPROFILESTRING
            ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
             (schar *)0x1120073e,szWork,0x14,(schar *)CONCAT22(unaff_SS,asStack_34));
  ReadIniTileSettings((schar *)szWork,(_tile *)(_tile *)&rgtileShip,7);
  CchGetString(0x98,&sStack_1a);
  sVar6 = GETPRIVATEPROFILESTRING
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
                     (schar *)0x11200740,szWork,0x14,(schar *)CONCAT22(unaff_SS,asStack_34));
  if (sVar6 < 3) {
    ini.overlap_10.fStartupFile_view = ini.overlap_10.fStartupFile_view & 0xfe1f;
    goto LAB_1000_1995;
  }
  if (szWork[0] == 'E') {
    ini.overlap_10 = (_ini_at_10)((uint)ini.overlap_10 & 0xfe1f | 0x80);
  }
  else if (szWork[0] == 'N') {
LAB_1000_18db:
    ini.overlap_10.fStartupFile_view = ini.overlap_10.fStartupFile_view & 0xfe1f;
  }
  else if (szWork[0] == 'P') {
    ini.overlap_10 = (_ini_at_10)((uint)ini.overlap_10 & 0xfe1f | 0x20);
  }
  else {
    if (szWork[0] != 'S') goto LAB_1000_18db;
    ini.overlap_10 = (_ini_at_10)((uint)ini.overlap_10 & 0xfe1f | 0x40);
  }
  if ((szWork[1] < 'B') || ('Q' < szWork[1])) {
    ini.overlap_10.fStartupFile_view = ini.overlap_10.fStartupFile_view & 0xfe1f;
  }
  else {
    ini.idPlayer = szWork[1] + -0x42;
  }
  if (((uint)ini.overlap_10 >> 5 & 0xf) != 0) {
    ini.iObjSel = _atoi((schar *)szWork + 2);
  }
LAB_1000_1995:
  CchGetString(0xcb,&sStack_1a);
  ini.iMsg = GETPRIVATEPROFILEINT
                       ((schar *)CONCAT22(unaff_SS,asStack_48),
                        (schar *)CONCAT22(unaff_SS,&sStack_1a),0,
                        (schar *)CONCAT22(unaff_SS,asStack_34));
  uStack_8 = ini.iMsg;
  CchGetString(0xac,&sStack_1a);
  sVar6 = GETPRIVATEPROFILESTRING
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
                     (schar *)0x11200742,szWork,10,(schar *)CONCAT22(unaff_SS,asStack_34));
  value = 0;
  for (uStack_8 = 0; ini.lid = value, (int)uStack_8 < sVar6; uStack_8 = uStack_8 + 1) {
    value = __aFlshl(value,'\x04');
    if ((((schar *)szWork)[uStack_8] < '0') || ('9' < ((schar *)szWork)[uStack_8])) {
      if (('`' < ((schar *)szWork)[uStack_8]) && (((schar *)szWork)[uStack_8] < 'g')) {
        value = value + (((schar *)szWork)[uStack_8] + -0x57);
      }
    }
    else {
      value = value + (((schar *)szWork)[uStack_8] + -0x30);
    }
  }
  CchGetString(0x97,&sStack_1a);
  uStack_36 = GETPRIVATEPROFILEINT
                        ((schar *)CONCAT22(unaff_SS,asStack_48),
                         (schar *)CONCAT22(unaff_SS,&sStack_1a),4,
                         (schar *)CONCAT22(unaff_SS,asStack_34));
  if ((uStack_36 != 0) && (uStack_36 < 10)) {
    iScanZoom = uStack_36 - 5;
  }
  CchGetString(0x9e,&sStack_1a);
  grbitScanShip =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0,
                  (schar *)CONCAT22(unaff_SS,asStack_34));
  uStack_36 = grbitScanShip;
  CchGetString(0x9f,&sStack_1a);
  grbitScanEShip =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0,
                  (schar *)CONCAT22(unaff_SS,asStack_34));
  uStack_36 = grbitScanEShip;
  CchGetString(0xa0,&sStack_1a);
  uStack_36 = GETPRIVATEPROFILEINT
                        ((schar *)CONCAT22(unaff_SS,asStack_48),
                         (schar *)CONCAT22(unaff_SS,&sStack_1a),0xf,
                         (schar *)CONCAT22(unaff_SS,asStack_34));
  grbitScanMines = uStack_36 & 0xf;
  CchGetString(0xa1,&sStack_1a);
  uStack_36 = GETPRIVATEPROFILEINT
                        ((schar *)CONCAT22(unaff_SS,asStack_48),
                         (schar *)CONCAT22(unaff_SS,&sStack_1a),100,
                         (schar *)CONCAT22(unaff_SS,asStack_34));
  if (100 < uStack_36) {
    uStack_36 = 100;
  }
  vpctRadarView = uStack_36;
  CchGetString(0x9d,&sStack_1a);
  uStack_36 = GETPRIVATEPROFILEINT
                        ((schar *)CONCAT22(unaff_SS,asStack_48),
                         (schar *)CONCAT22(unaff_SS,&sStack_1a),0xe0,
                         (schar *)CONCAT22(unaff_SS,asStack_34));
  grbitScan = uStack_36;
  if (5 < (uStack_36 & 0xc00f)) {
    grbitScan = 0;
    grbitScanShip = 0;
  }
  CchGetString(0xa2,&sStack_1a);
  cMinGrafMax = GETPRIVATEPROFILEINT
                          ((schar *)CONCAT22(unaff_SS,asStack_48),
                           (schar *)CONCAT22(unaff_SS,&sStack_1a),cMinGrafMax,
                           (schar *)CONCAT22(unaff_SS,asStack_34));
  if ((cMinGrafMax < 100) || (30000 < cMinGrafMax)) {
    cMinGrafMax = 5000;
  }
  CchGetString(0x99,asStack_48);
  CchGetString(0xcc,&sStack_1a);
  sVar6 = GETPRIVATEPROFILEINT
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0
                     ,(schar *)CONCAT22(unaff_SS,asStack_34));
  ini.overlap_10 = (_ini_at_10)((uint)ini.overlap_10 & 0x7fff | sVar6 << 0xf);
  CchGetString(0x9a,&sStack_1a);
  uVar7 = GETPRIVATEPROFILEINT
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0
                     ,(schar *)CONCAT22(unaff_SS,asStack_34));
  ini.overlap_10 = (_ini_at_10)((uint)ini.overlap_10 & 0xfffb | (uVar7 & 1) << 2);
  CchGetString(0x9b,&sStack_1a);
  sVar6 = GETPRIVATEPROFILESTRING
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
                     (schar *)0x11200744,szWork,0x100,(schar *)CONCAT22(unaff_SS,asStack_34));
  if (sVar6 < 4) {
    ini.overlap_10.fStartupFile_view = ini.overlap_10.fStartupFile_view & 0xfffe;
  }
  else {
    ini.overlap_10 = (_ini_at_10)((uint)ini.overlap_10 & 0xfffe | 1);
    _strcpy((schar *)szBase,(schar *)szWork);
  }
  if (((schar *)vrgszMRU == (schar *)0x0) &&
     (vrgszMRU = (schar *)CONCAT22(vrgszMRU._2_2_,(schar *)vrgszMRU), vrgszMRU._2_2_ == 0)) {
    vrgszMRU = LpAlloc(0x900,9);
  }
  iVar8 = _strlen(&sStack_1a);
  pcStack_38 = (char *)((int)&_Stack_24.overlap_8 + iVar8 + 1);
  for (uStack_8 = 0; (int)uStack_8 < 9; uStack_8 = uStack_8 + 1) {
    *pcStack_38 = (char)uStack_8 + '1';
    sVar6 = GETPRIVATEPROFILESTRING
                      ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a)
                       ,(schar *)0x11200746,
                       (schar *)CONCAT22(vrgszMRU._2_2_,(schar *)vrgszMRU + uStack_8 * 0x100),0x100,
                       (schar *)CONCAT22(unaff_SS,asStack_34));
    if (sVar6 < 4) {
      ((schar *)vrgszMRU)[uStack_8 * 0x100] = '\0';
    }
  }
  iStack_a = 0;
  uStack_8 = 0;
  psVar5 = vrgszMRU;
  while( true ) {
    vrgszMRU._2_2_ = (int)((ulong)psVar5 >> 0x10);
    vrgszMRU._0_2_ = (schar *)psVar5;
    if (8 < (int)uStack_8) break;
    if (((schar *)vrgszMRU)[uStack_8 * 0x100] != '\0') {
      if (uStack_8 != iStack_a) {
        __fstrcpy((schar *)vrgszMRU + iStack_a * 0x100,vrgszMRU._2_2_,
                  (schar *)vrgszMRU + uStack_8 * 0x100,vrgszMRU._2_2_);
        vrgszMRU = psVar5;
        ((schar *)vrgszMRU)[uStack_8 * 0x100] = '\0';
        psVar5 = vrgszMRU;
      }
      iStack_a = iStack_a + 1;
    }
    uStack_8 = uStack_8 + 1;
  }
  vrgszMRU = psVar5;
  CchGetString(0x9c,&sStack_1a);
  ini.turn = GETPRIVATEPROFILEINT
                       ((schar *)CONCAT22(unaff_SS,asStack_48),
                        (schar *)CONCAT22(unaff_SS,&sStack_1a),game.turn,
                        (schar *)CONCAT22(unaff_SS,asStack_34));
  CchGetString(0x92,asStack_48);
  CchGetString(0xae,&sStack_1a);
  GETPRIVATEPROFILESTRING
            ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
             (schar *)0x11200748,vszDefPass,0x10,(schar *)CONCAT22(unaff_SS,asStack_34));
  CchGetString(0xaf,&sStack_1a);
  uStack_36 = GETPRIVATEPROFILEINT
                        ((schar *)CONCAT22(unaff_SS,asStack_48),
                         (schar *)CONCAT22(unaff_SS,&sStack_1a),0,
                         (schar *)CONCAT22(unaff_SS,asStack_34));
  gd.overlap_0.grBits_view.grBits._2_2_ =
       gd.overlap_0.grBits_view.grBits._2_2_ & 0xfbff | (uint)(uStack_36 != 0) << 10;
  CchGetString(0xc9,&sStack_1a);
  uStack_36 = GETPRIVATEPROFILEINT
                        ((schar *)CONCAT22(unaff_SS,asStack_48),
                         (schar *)CONCAT22(unaff_SS,&sStack_1a),0,
                         (schar *)CONCAT22(unaff_SS,asStack_34));
  gd.overlap_4.grBits2_view.grBits2._2_2_ =
       gd.overlap_4.grBits2_view.grBits2._2_2_ & 0xffdf | (uint)(uStack_36 != 0) << 5;
  CchGetString(0xca,&sStack_1a);
  uStack_36 = GETPRIVATEPROFILEINT
                        ((schar *)CONCAT22(unaff_SS,asStack_48),
                         (schar *)CONCAT22(unaff_SS,&sStack_1a),0,
                         (schar *)CONCAT22(unaff_SS,asStack_34));
  gd.overlap_4.grBits2_view.grBits2._2_2_ =
       gd.overlap_4.grBits2_view.grBits2._2_2_ & 0xffbf | (uint)(uStack_36 != 0) << 6;
  CchGetString(0xb0,&sStack_1a);
  vcBackupDirs = GETPRIVATEPROFILEINT
                           ((schar *)CONCAT22(unaff_SS,asStack_48),
                            (schar *)CONCAT22(unaff_SS,&sStack_1a),1,
                            (schar *)CONCAT22(unaff_SS,asStack_34));
  if ((vcBackupDirs < 1) || (999 < vcBackupDirs)) {
    vcBackupDirs = 1;
  }
  CchGetString(0xb5,&sStack_1a);
  vrptPlanet.grbitVisible._0_2_ =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),-1,
                  (schar *)CONCAT22(unaff_SS,asStack_34));
  vrptPlanet.grbitVisible._2_2_ = 0;
  CchGetString(0xb6,&sStack_1a);
  uVar7 = GETPRIVATEPROFILEINT
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0
                     ,(schar *)CONCAT22(unaff_SS,asStack_34));
  vrptPlanet.fAscending = (short)((uVar7 & 0x100) != 0);
  vrptPlanet.icolSort = uVar7 & 0xff;
  uStack_8 = vrptPlanet.icolSort;
  CchGetString(0xb7,&sStack_1a);
  vrptFleet.grbitVisible._0_2_ =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),-1,
                  (schar *)CONCAT22(unaff_SS,asStack_34));
  vrptFleet.grbitVisible._2_2_ = 0;
  CchGetString(0xb8,&sStack_1a);
  uVar7 = GETPRIVATEPROFILEINT
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0
                     ,(schar *)CONCAT22(unaff_SS,asStack_34));
  vrptFleet.fAscending = (short)((uVar7 & 0x100) != 0);
  vrptFleet.icolSort = uVar7 & 0xff;
  uStack_8 = vrptFleet.icolSort;
  CchGetString(0xb9,&sStack_1a);
  vrptEFleet.grbitVisible._0_2_ =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),-1,
                  (schar *)CONCAT22(unaff_SS,asStack_34));
  vrptEFleet.grbitVisible._2_2_ = 0;
  CchGetString(0xba,&sStack_1a);
  uVar7 = GETPRIVATEPROFILEINT
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0
                     ,(schar *)CONCAT22(unaff_SS,asStack_34));
  vrptEFleet.fAscending = (short)((uVar7 & 0x100) != 0);
  vrptEFleet.icolSort = uVar7 & 0xff;
  uStack_8 = vrptEFleet.icolSort;
  CchGetString(0xbb,&sStack_1a);
  vrptBattle.grbitVisible._0_2_ =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),-1,
                  (schar *)CONCAT22(unaff_SS,asStack_34));
  vrptBattle.grbitVisible._2_2_ = 0;
  CchGetString(0xbc,&sStack_1a);
  uVar7 = GETPRIVATEPROFILEINT
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),0
                     ,(schar *)CONCAT22(unaff_SS,asStack_34));
  vrptBattle.fAscending = (short)((uVar7 & 0x100) != 0);
  vrptBattle.icolSort = uVar7 & 0xff;
  uStack_8 = vrptBattle.icolSort;
  CchGetString(0xbd,&sStack_1a);
  uVar7 = GETPRIVATEPROFILEINT
                    ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),7
                     ,(schar *)CONCAT22(unaff_SS,asStack_34));
  gd.overlap_4.grBits2_view.grBits2._2_2_ =
       gd.overlap_4.grBits2_view.grBits2._2_2_ & 0xfff0 | uVar7 & 0xf;
  CchGetString(0xba,&sStack_1a);
  if (7 < (gd.overlap_4.grBits2_view.grBits2._2_2_ & 0xf)) {
    gd.overlap_4.grBits2_view.grBits2._2_2_ = gd.overlap_4.grBits2_view.grBits2._2_2_ & 0xfff0 | 7;
  }
  CchGetString(0xc0,&sStack_1a);
  viSpeedVCR = GETPRIVATEPROFILEINT
                         ((schar *)CONCAT22(unaff_SS,asStack_48),
                          (schar *)CONCAT22(unaff_SS,&sStack_1a),1,
                          (schar *)CONCAT22(unaff_SS,asStack_34));
  __strdate((schar *)szWork);
  szWork[5] = '\0';
  szWork[2] = '\0';
  iVar8 = _atoi((schar *)szWork + 6);
  iVar9 = _atoi((schar *)szWork);
  iVar10 = _atoi((schar *)szWork + 3);
  uStack_6 = iVar10 + iVar9 * 0x1f + iVar8 * 0x174;
  CchGetString(0xbf,&sStack_1a);
  uDateInstalled =
       GETPRIVATEPROFILEINT
                 ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),-1,
                  (schar *)CONCAT22(unaff_SS,asStack_34));
  if (uStack_6 < uDateInstalled) {
    uDateInstalled = uStack_6;
  }
  gd.overlap_4.grBits2_view.grBits2._0_2_ =
       gd.overlap_4.grBits2_view.grBits2._0_2_ & 0xfffd |
       (uint)(uDateInstalled + 0x15 <= uStack_6) << 1;
  CchGetString(0xc4,asStack_48);
  for (uStack_8 = 0; (int)uStack_8 < 4; uStack_8 = uStack_8 + 1) {
    CchGetString(uStack_8 + 0xc5,&sStack_1a);
    GETPRIVATEPROFILESTRING
              ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
               (schar *)0x11200749,szWork,0x50,(schar *)CONCAT22(unaff_SS,asStack_34));
    iVar8 = _strlen((schar *)szWork);
    if ((4 < iVar8) && (iVar8 < 0x20)) {
      _strcpy(uStack_8 * 0x20 + 0x6a6,(schar *)szWork);
    }
  }
  CchGetString(0x93,asStack_48);
  _memset((_ziporder *)vrgZip,0,0x60);
  for (uStack_8 = 0; (int)uStack_8 < 4; uStack_8 = uStack_8 + 1) {
    _strcpy(&sStack_1a,asStack_48);
    iVar8 = _strlen(&sStack_1a);
    pcStack_38 = &sStack_1a + iVar8;
    *pcStack_38 = (char)uStack_8 + '1';
    (&sStack_1a)[iVar8 + 1] = '\0';
    GETPRIVATEPROFILESTRING
              ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
               (schar *)0x1120074a,szWork,0x50,(schar *)CONCAT22(unaff_SS,asStack_34));
    iVar8 = _strlen((schar *)szWork);
    if ((0x13 < iVar8) && (iVar8 < 0x21)) {
      iStack_a = 0;
      for (pcStack_38 = (char *)szWork;
          ((iStack_a < 0x14 && ('`' < *pcStack_38)) && (*pcStack_38 < 'q'));
          pcStack_38 = pcStack_38 + 1) {
        iStack_a = iStack_a + 1;
      }
      if (0x13 < iStack_a) {
        pcStack_38 = (char *)szWork;
        for (iStack_a = 0; iStack_a < 5; iStack_a = iStack_a + 1) {
          ((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0 =
               (_itemaction_at_0)
               ((uint)((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0 & 0xfff |
               (*pcStack_38 + -0x61) * 0x1000);
          ((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0 =
               (_itemaction_at_0)
               ((uint)((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0 & 0xf000 |
               (int)pcStack_38[1] - 0x61U & 0xfff);
          cVar3 = pcStack_38[2];
          _Var4.overlap_0 = ((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0;
          p_Var1 = &((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0;
          *p_Var1 = (_itemaction_at_0)(*p_Var1 & 0xf000);
          p_Var1 = &((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0;
          *p_Var1 = (_itemaction_at_0)
                    (*p_Var1 | ((cVar3 + -0x61) * 0x10 | (uint)_Var4.overlap_0) & 0xfff);
          cVar3 = pcStack_38[3];
          _Var4.overlap_0 = ((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0;
          p_Var1 = &((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0;
          *p_Var1 = (_itemaction_at_0)(*p_Var1 & 0xf000);
          p_Var1 = &((_ziporder *)vrgZip)[uStack_8].txp.rgia[iStack_a].overlap_0;
          *p_Var1 = (_itemaction_at_0)
                    (*p_Var1 | ((cVar3 + -0x61) * 0x100 | (uint)_Var4.overlap_0) & 0xfff);
          pcStack_38 = pcStack_38 + 4;
        }
        _strcpy(uStack_8 * 0x18 + 0x526e,pcStack_38);
        *(undefined1 *)((int)&vrgZip[0].fValid + uStack_8 * 0x18) = 1;
      }
    }
  }
  _memset((_zipprodq *)vrgZipProd,0,200);
  for (uStack_8 = 0; (int)uStack_8 < 5; uStack_8 = uStack_8 + 1) {
    _strcpy(&sStack_1a,asStack_48);
    iVar8 = _strlen(&sStack_1a);
    pcStack_38 = &sStack_1a + iVar8 + 1;
    (&sStack_1a)[iVar8] = 'P';
    *pcStack_38 = (char)uStack_8 + '1';
    (&sStack_1a)[iVar8 + 2] = '\0';
    GETPRIVATEPROFILESTRING
              ((schar *)CONCAT22(unaff_SS,asStack_48),(schar *)CONCAT22(unaff_SS,&sStack_1a),
               (schar *)0x1120074b,szWork,0x50,(schar *)CONCAT22(unaff_SS,asStack_34));
    iVar8 = _strlen((schar *)szWork);
    if ((2 < iVar8) && (iVar8 < 0x41)) {
      pcStack_38 = (char *)szWork;
      iVar8 = szWork[1] + -0x61;
      if ((-1 < iVar8) && (iVar8 < 0xd)) {
        iStack_a = 0;
        for (; ((iStack_a < iVar8 * 4 + 2 && ('`' < *pcStack_38)) && (*pcStack_38 < 'q'));
            pcStack_38 = pcStack_38 + 1) {
          iStack_a = iStack_a + 1;
        }
        if (iVar8 * 4 + 2 <= iStack_a) {
          uVar7 = _strlen(pcStack_38);
          if (uVar7 < 0xd) {
            _strcpy((_zipprodq *)vrgZipProd + uStack_8,pcStack_38);
            *(bool *)(uStack_8 * 0x28 + 0x2304) = szWork[0] != 'a';
            *(undefined1 *)((int)&vrgZipProd[0].fValid + uStack_8 * 0x28) = 1;
            *(undefined1 *)
             ((int)(_zipprodq_at_14_zpq1 *)&vrgZipProd[0].overlap_14.zpq1_view + uStack_8 * 0x28 + 1
             ) = (char)iVar8;
            pcStack_38 = (schar *)szWork + 2;
            for (iStack_a = 0; iStack_a < iVar8; iStack_a = iStack_a + 1) {
              *(int *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2) = *pcStack_38 + -0x61;
              puVar2 = (uint *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2);
              *puVar2 = *puVar2 | (pcStack_38[1] + -0x61) * 0x10;
              puVar2 = (uint *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2);
              *puVar2 = *puVar2 | (pcStack_38[2] + -0x61) * 0x100;
              puVar2 = (uint *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2);
              *puVar2 = *puVar2 | (pcStack_38[3] + -0x61) * 0x1000;
              pcStack_38 = pcStack_38 + 4;
              if (0x3fc < *(uint *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2) >> 6) {
                *(uint *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2) =
                     *(uint *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2) & 0x3f | 0x40;
              }
              if (6 < (*(uint *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2) & 0x3f)) {
                *(uint *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2) =
                     *(uint *)(uStack_8 * 0x28 + 0x2306 + iStack_a * 2) & 0xffc0;
              }
            }
          }
        }
      }
    }
  }
  CchGetString(0x230,((_zipprodq *)vrgZipProd)->szName);
  vrgZipProd[0].fValid = '\x01';
  return;
}

