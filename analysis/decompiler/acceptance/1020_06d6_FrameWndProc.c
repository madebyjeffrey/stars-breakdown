/* RAW GHIDRA OUTPUT: not reviewed or buildable source. */

/* CodeView 221:000000c4; NE 5:6d6; ["c:\\user\\jeffl\\2.7j\\mdi.c"]
   Registers SI and DI hold hwnd and wParam after entry loads from BP+14 and BP+10. msg and lParam
   remain recorded at BP+12 and BP+6. The recovered signature agrees with the procedure type.
   Locals and lexical lifetimes: analysis/debug/symbols.json */

long __stdcall16far FrameWndProc(ushort hwnd,ushort msg,ushort wParam,long lParam)

{
  tagPOINT pt;
  tagPOINT pt_00;
  tagPOINT dpt;
  tagPOINT dpt_00;
  _ini_at_10 _Var1;
  uint uVar2;
  int iVar3;
  schar *psVar4;
  short sVar5;
  ushort uVar6;
  undefined2 unaff_SS;
  long lVar7;
  tagPOINT tVar8;
  char *pcVar9;
  schar asStack_62 [52];
  ushort uStack_2e;
  int iStack_2c;
  short asStack_2a [2];
  tagPOINT tStack_26;
  short sStack_22;
  short sStack_20;
  ushort uStack_1e;
  short sStack_1c;
  short sStack_1a;
  undefined1 auStack_18 [4];
  tagPOINT tStack_14;
  ushort uStack_10;
  uint uStack_e;
  ushort uStack_c;
  ushort uStack_a;
  int iStack_8;
  ushort uStack_6;
  
  if (msg == 1) {
    uStack_6 = GETDC(hwnd);
    FCreateFonts(uStack_6);
    GETTEXTMETRICS(uStack_6,(short *)CONCAT22(unaff_SS,asStack_2a));
    dySysFont = asStack_2a[0];
    dySBar = (dyArial8 + 0xc) * 2;
    RELEASEDC(hwnd,uStack_6);
    InitTiles();
    EnsureTileSize((uint)(iWindowLayout == 2));
    return 0;
  }
  if (msg == 2) {
    if (uTimerId != 0) {
      KILLTIMER(0,uTimerId);
    }
    WriteIniSettings();
    uTimerId = 0;
    if (((uint)gd.overlap_0.grBits_view.grBits._0_2_ >> 3 & 1) != 0) {
      FMarkFile(2,-1,1,0);
    }
    DestroyCurGame();
    if (((uint)gd.overlap_0.grBits_view.grBits._2_2_ >> 6 & 1) == 0) {
      POSTQUITMESSAGE(vretExitValue);
      return 0;
    }
    EXITWINDOWS((long)vretExitValue,0);
    return 0;
  }
  if (msg == 5) {
    if ((wParam == 2) || (wParam == 0)) {
      vfs.dx = (int)lParam;
      lVar7 = __aFulshr(lParam,'\x10');
      vfs.dy = (short)lVar7;
      RefitFrameChildren();
      return 0;
    }
LAB_1020_1d5a:
    lVar7 = DEFWINDOWPROC(hwnd,msg,wParam,lParam);
    return lVar7;
  }
  if (msg == 6) {
    return 0;
  }
  if (msg == 0xf) {
    sVar5 = ISICONIC(hwnd);
    if (sVar5 == 0) {
      uStack_6 = BEGINPAINT(hwnd,(short *)CONCAT22(unaff_SS,asStack_2a));
      uStack_2e = SELECTOBJECT(uStack_6,hbrButtonShadow);
      if ((iWindowLayout == 0) || ((iWindowLayout != 1 && (iWindowLayout != 2)))) {
        PATBLT(uStack_6,vfs.xTop + 5,0,2,vfs.dy,0xf00021);
        PATBLT(uStack_6,0,vfs.y1 + 5,vfs.xTop + 2,2,0xf00021);
        PATBLT(uStack_6,0,vfs.y2 + 5,vfs.xTop + 2,2,0xf00021);
        SELECTOBJECT(uStack_6,hbrButtonHilite);
        PATBLT(uStack_6,vfs.xTop + 1,0,1,vfs.y1 + 2,0xf00021);
        PATBLT(uStack_6,0,vfs.y1 + 1,vfs.xTop + 1,1,0xf00021);
        PATBLT(uStack_6,vfs.xTop + 1,vfs.y1 + 6,1,(vfs.y2 - vfs.y1) + -4,0xf00021);
        PATBLT(uStack_6,0,vfs.y2 + 1,vfs.xTop + 1,1,0xf00021);
        PATBLT(uStack_6,vfs.xTop + 1,vfs.y2 + 6,1,(vfs.dy - vfs.y2) + -6,0xf00021);
      }
      else {
        if ((int)gd.overlap_0.grBits_view.grBits._2_2_ < 0) {
          iStack_2c = 0x24;
        }
        else {
          iStack_2c = 0;
        }
        PATBLT(uStack_6,vfs.xTop + 5,iStack_2c,2,(vfs.y2 + 2) - iStack_2c,0xf00021);
        PATBLT(uStack_6,0,vfs.y1 + 5,vfs.xTop + 2,2,0xf00021);
        PATBLT(uStack_6,vfs.xTop + 5,vfs.y2 + 5,(vfs.dx - vfs.xTop) + -5,2,0xf00021);
        PATBLT(uStack_6,vfs.xTop + 5,vfs.y2 + 6,2,(vfs.dy - vfs.y2) + -5,0xf00021);
        SELECTOBJECT(uStack_6,hbrButtonHilite);
        PATBLT(uStack_6,vfs.xTop + 1,iStack_2c,1,(vfs.y1 + 2) - iStack_2c,0xf00021);
        PATBLT(uStack_6,0,vfs.y1 + 1,vfs.xTop + 1,1,0xf00021);
        PATBLT(uStack_6,vfs.xTop + 1,vfs.y1 + 6,1,(vfs.dy - vfs.y1) + -6,0xf00021);
        PATBLT(uStack_6,vfs.xTop + 6,vfs.y2 + 1,(vfs.dx - vfs.xTop) + -6,1,0xf00021);
      }
      SELECTOBJECT(uStack_6,uStack_2e);
      ENDPAINT(hwnd,(short *)CONCAT22(unaff_SS,asStack_2a));
      return 0;
    }
    uStack_6 = BEGINPAINT(hwnd,(short *)CONCAT22(unaff_SS,asStack_2a));
    if (((idPlayer != -1) || ((uVar6 = hiconHost, (int)game.lid == 0 && (game.lid._2_2_ == 0)))) &&
       (uVar6 = hiconStars, uTimerId != 0)) {
      uVar6 = hiconWait;
    }
    DRAWICON(uStack_6,2,2,uVar6);
    ENDPAINT(hwnd,(short *)CONCAT22(unaff_SS,asStack_2a));
    return 0;
  }
  if (msg == 0x10) {
    DESTROYWINDOW(hwnd);
    return 0;
  }
  if (msg == 0x14) {
    sVar5 = ISICONIC(hwnd);
    if (sVar5 != 0) {
      GETCLIENTRECT(hwnd,(short *)CONCAT22(unaff_SS,&tStack_14.y));
      FILLRECT(wParam,(short *)CONCAT22(unaff_SS,&tStack_14.y),hbrDesktop);
      return 0;
    }
    GETCLIENTRECT(hwnd,(short *)CONCAT22(unaff_SS,&tStack_14.y));
    if (hwndScanner != 0) {
      GETCLIENTRECT(hwndScanner,(short *)CONCAT22(unaff_SS,&sStack_1a));
      MAPWINDOWPOINTS(hwndScanner,hwnd,(short *)CONCAT22(unaff_SS,&sStack_1a),2);
      EXCLUDECLIPRECT(wParam,sStack_1a,auStack_18._0_2_,auStack_18._2_2_,tStack_14.x);
    }
    FILLRECT(wParam,(short *)CONCAT22(unaff_SS,&tStack_14.y),hbrButtonFace);
    return 1;
  }
  if ((msg == 0x15) || (msg == 0x1a)) {
    FGetSystemColors();
    return 0;
  }
  if (msg == 0x20) {
    uStack_c = 0;
    sVar5 = ISICONIC(hwnd);
    if (sVar5 == 0) {
      GETCURSORPOS((ushort *)CONCAT22(unaff_SS,&uStack_10));
      SCREENTOCLIENT(hwndFrame,(ushort *)CONCAT22(unaff_SS,&uStack_10));
      GETCLIENTRECT(hwnd,(undefined1 *)CONCAT22(unaff_SS,auStack_18));
      sVar5 = PTINRECT((undefined1 *)CONCAT22(unaff_SS,auStack_18),CONCAT22(uStack_e,uStack_10));
      if (sVar5 != 0) {
        tVar8.y = uStack_e;
        tVar8.x = uStack_10;
        uStack_c = HcrsFromFrameWindowPt(tVar8,(short *)0x0);
        if (uStack_c != 0) {
          SETCURSOR(uStack_c);
          return 1;
        }
        uStack_c = 0;
      }
    }
    goto LAB_1020_1d5a;
  }
  if (msg == 0x24) {
    *(undefined2 *)((int)lParam + 0xc) = 0x208;
    *(undefined2 *)((int)lParam + 0xe) = 0x17c;
    return 0;
  }
  if (msg == 0x37) {
    uVar6 = hiconHost;
    if ((idPlayer != -1) && (uVar6 = hiconStars, uTimerId != 0)) {
      uVar6 = hiconWait;
    }
    return (ulong)uVar6;
  }
  if (msg == 0x102) {
    if ((hwndScanner != 0) && ((wParam == 0x2d || (wParam == 0x2b)))) {
      SENDMESSAGE(hwndScanner,0x102,wParam,lParam);
      return 0;
    }
    if ((hwndMessage != 0) && (((wParam == 0x2d || (wParam == 0x2b)) || (wParam == 0xd)))) {
      SENDMESSAGE(hwndMessage,0x102,wParam,lParam);
      return 0;
    }
    if ((hwndPlanet != 0) && ((wParam == 0x66 || (wParam == 0x46)))) {
      SENDMESSAGE(hwndPlanet,0x102,wParam,lParam);
      return 0;
    }
    if ((hwndPlanet != 0) && ((sel.grobj == 1 && ((wParam == 0x71 || (wParam == 0x51)))))) {
      ChangeProduction(0);
      return 0;
    }
    if ((sel.grobj & 3U) == 0) {
      return 0;
    }
    uStack_c = 0;
    uStack_e = 0;
    if (wParam == 0x6e) {
      uStack_c = 1;
    }
    else if ((wParam == 0x4e) || (wParam == 0x50)) {
      if (sel.grobj == 1) {
        uStack_e = IdFindAdjStarbase(sel.pl.id,(uint)(wParam == 0x4e));
      }
      else if (wParam == 0x4e) {
        uStack_c = 1;
      }
      else {
        uStack_c = -1;
      }
    }
    else if (wParam == 0x70) {
      uStack_c = -1;
    }
    else if (((wParam == 0x72) || (wParam == 0x52)) && (sel.grobj == 2)) {
      ShipCommandProc(hwndPlanet,0,(ulong)rghwndBtn[6]);
    }
    if ((uStack_c == 0) && (uStack_e == 0)) {
      return 0;
    }
    if (sel.grobj != 2) {
      SelectAdjPlanet(uStack_c,uStack_e);
      return 0;
    }
    SelectAdjFleet(uStack_c,uStack_e);
    return 0;
  }
  if (msg == 0x111) {
    CommandHandler(hwnd,wParam);
    return 0;
  }
  if (msg == 0x112) {
    if (((wParam & 0xfff0) == 0xf030) || ((wParam & 0xfff0) == 0xf120)) {
      uStack_c = idPlayer;
      uStack_10 = uTimerId;
      if (uTimerId != 0) {
        KILLTIMER(0,uTimerId);
        uTimerId = 0;
        CreateChildWindows();
      }
      if ((idPlayer == -1) || (sVar5 = FNewTurnAvail(idPlayer), sVar5 == 0)) {
        if (uStack_10 != 0) {
          if (uTimerType == 0xd) {
            POSTMESSAGE(hwnd,0x465,0,0);
          }
          else {
            if (uTimerType == 0xe) {
              pcVar9 = (char *)s_M6102__MATH___floating_point_err_1120_2022 + 1;
              psVar4 = PszFormatIds(0x18e,(short *)0x0);
              uStack_e = AlertSz(psVar4,(short)pcVar9);
              if ((uStack_e == 6) && (sVar5 = FMarkFile(1,idPlayer,2,0), sVar5 == 0)) {
                sVar5 = 0x10;
                psVar4 = PszFormatIds(399,(short *)0x0);
                AlertSz(psVar4,sVar5);
                _WSPRINTF((tagPOINT *)CONCAT22(unaff_SS,&tStack_14),(schar *)CONCAT22(0x1120,MPCTD),
                          idPlayer + 1);
                DestroyCurGame();
                sVar5 = FLoadGame((schar *)szBase,(schar *)&tStack_14);
                if (sVar5 == 0) {
                  sVar5 = 0x10;
                  psVar4 = PszFormatIds(0x1b,(short *)0x0);
                  AlertSz(psVar4,sVar5);
                }
                else {
                  CreateChildWindows();
                }
              }
              else if (uStack_e == 2) {
                POSTMESSAGE(hwndFrame,0x111,0x6a,0);
                return 1;
              }
            }
            SENDMESSAGE(hwndFrame,0x111,0xfa1,0);
            if ((1000 < sel.pt.x) && (1000 < sel.pt.y)) {
              pt.y = sel.pt.y;
              pt.x = sel.pt.x;
              CtrPointScan(pt,1);
            }
          }
        }
      }
      else {
        if (uStack_10 == 0) {
          pcVar9 = (char *)s_M6102__MATH___floating_point_err_1120_2022 + 1;
          psVar4 = PszFormatIds(0x2c5,(short *)0x0);
          uStack_e = AlertSz(psVar4,(short)pcVar9);
        }
        else {
          sVar5 = 0x40;
          psVar4 = PszFormatIds(0x2c6,(short *)0x0);
          AlertSz(psVar4,sVar5);
          uStack_e = 6;
        }
        if (uStack_e == 6) {
          _WSPRINTF((tagPOINT *)CONCAT22(unaff_SS,&tStack_14),(schar *)CONCAT22(0x1120,MPCTD),
                    idPlayer + 1);
          DestroyCurGame();
          sVar5 = FLoadGame((schar *)szBase,(schar *)&tStack_14);
          if (sVar5 == 0) {
            sVar5 = 0x10;
            psVar4 = PszFormatIds(0x1b,(short *)0x0);
            AlertSz(psVar4,sVar5);
          }
          else {
            CreateChildWindows();
          }
        }
        else if (uStack_e == 2) {
          if (uStack_10 == 0) {
            POSTMESSAGE(hwndFrame,0x112,0xf020,0);
          }
          else {
            POSTMESSAGE(hwndFrame,0x111,0x6a,0);
          }
          return 1;
        }
        SENDMESSAGE(hwndFrame,0x111,0xfa1,0);
      }
    }
    goto LAB_1020_1d5a;
  }
  if (msg == 0x116) {
    InitializeMenu(wParam);
    return 0;
  }
  if (msg == 0x121) {
    if ((((uint)gd.overlap_0.grBits_view.grBits._0_2_ >> 0xb & 1) != 0) &&
       (((uint)tutor.overlap_0 >> 2 & 1) != 0)) {
      AdvanceTutor();
    }
    return 0;
  }
  if (msg == 0x201) {
    tStack_14.x = (int)lParam;
    lVar7 = __aFulshr(lParam,'\x10');
    tStack_14.y = (short)lVar7;
    pt_00.y = tStack_14.y;
    pt_00.x = tStack_14.x;
    uVar6 = HcrsFromFrameWindowPt(pt_00,(short *)&uStack_10);
    if (uVar6 == 0) {
      return 0;
    }
    uStack_6 = GETDC(hwnd);
    uStack_1e = SELECTOBJECT(uStack_6,hbr50Screen);
    sStack_1a = 0;
    sStack_1c = 0;
    InvertPaneBorder(uStack_6,uStack_10,(tagPOINT)0x0,(tagPOINT *)0x0);
    sStack_22 = tStack_14.x;
    sStack_20 = tStack_14.y;
    SETCAPTURE(hwnd);
    uStack_e = tStack_14.x;
    uStack_c = tStack_14.y;
    auStack_18._0_2_ = 0;
    auStack_18._2_2_ = 0;
    while( true ) {
      sVar5 = FGetMouseMove(&tStack_14);
      if (sVar5 == 0) break;
      if ((tStack_14.x != uStack_e) || (tStack_14.y != uStack_c)) {
        sStack_1c = tStack_14.x - sStack_22;
        sStack_1a = tStack_14.y - sStack_20;
        tStack_26.x = tStack_14.x - uStack_e;
        tStack_26.y = tStack_14.y - uStack_c;
        dpt.y = sStack_1a;
        dpt.x = sStack_1c;
        tVar8 = InvertPaneBorder(uStack_6,uStack_10,dpt,&tStack_26);
        uStack_e = tStack_14.x;
        uStack_c = tStack_14.y;
        auStack_18 = (undefined1  [4])tVar8;
      }
    }
    dpt_00.y = sStack_1a;
    dpt_00.x = sStack_1c;
    InvertPaneBorder(uStack_6,uStack_10,dpt_00,(tagPOINT *)0x0);
    RELEASECAPTURE();
    SELECTOBJECT(uStack_6,uStack_1e);
    RELEASEDC(hwnd,uStack_6);
    if ((tagPOINT)auStack_18 == (tagPOINT)0x0) {
      return 0;
    }
    if ((uStack_10 & 1) != 0) {
      if (iWindowLayout == 0) {
        vfs.dxPlanWant = vfs.xTop + auStack_18._0_2_;
      }
      else {
        vfs.dx2PlanWant = vfs.xTop + auStack_18._0_2_;
      }
    }
    if ((uStack_10 & 2) != 0) {
      if (iWindowLayout == 0) {
        vfs.dyMsgWant = ((vfs.y2 - vfs.y1) + -8) - auStack_18._2_2_;
      }
      else {
        vfs.dy2MsgWant = ((vfs.dy - vfs.y1) + -8) - auStack_18._2_2_;
      }
    }
    if ((uStack_10 & 4) != 0) {
      if (iWindowLayout == 0) {
        vfs.dyMsgWant = (vfs.y2 - vfs.y1) + -8 + auStack_18._2_2_;
        vfs.dyMinWant = ((vfs.dy - vfs.y2) + -8) - auStack_18._2_2_;
      }
      else {
        vfs.dy2MinWant = ((vfs.dy - vfs.y2) + -8) - auStack_18._2_2_;
      }
    }
    INVALIDATERECT(hwnd,(void *)0x0,1);
    RefitFrameChildren();
    return 0;
  }
  if (msg == 0x30f) {
LAB_1020_074c:
    if (hwndTitle != 0) {
      lVar7 = SENDMESSAGE(hwndTitle,msg,wParam,lParam);
      return lVar7;
    }
    uStack_6 = GETDC(hwnd);
    uStack_a = SELECTPALETTE(uStack_6,vhpal,0);
    iStack_8 = REALIZEPALETTE(uStack_6);
    SELECTPALETTE(uStack_6,uStack_a,0);
    RELEASEDC(hwnd,uStack_6);
    if (iStack_8 != 0) {
      INVALIDATERECT(hwnd,(void *)0x0,1);
      return 1;
    }
    return 0;
  }
  if (msg == 0x311) {
    if (wParam == hwnd) {
      return 0;
    }
    goto LAB_1020_074c;
  }
  if (msg != 0x464) {
    if (msg == 0x465) {
      BringUpHostDlg();
      return 1;
    }
    if (msg == 0x466) {
      ShowTutor(0);
      game.fDirty = 0;
      DestroyCurGame();
      uStack_c = fFileErrSilent;
      fFileErrSilent = 1;
      gd.overlap_0.grBits_view.grBits._2_2_ = gd.overlap_0.grBits_view.grBits._2_2_ & 0xfffd | 2;
      sVar5 = FLoadGame((schar *)szBase,(schar *)0x3c8);
      if (sVar5 != 0) {
        gd.overlap_0.grBits_view.grBits._2_2_ = gd.overlap_0.grBits_view.grBits._2_2_ & 0xfffd;
        fFileErrSilent = uStack_c;
        idPlayer = 0;
        if (wParam == 0x9ca) {
          gd.overlap_0.grBits_view.grBits._0_2_ = gd.overlap_0.grBits_view.grBits._0_2_ & 0xfffd | 2
          ;
          _WSPRINTF(szWork,(schar *)0x112003cb,(schar *)szBase,0x1120);
          sVar5 = FLoadLogFile((schar *)szWork);
          if (sVar5 != 0) {
            FRunLogFile();
          }
          gd.overlap_0.grBits_view.grBits._0_2_ = gd.overlap_0.grBits_view.grBits._0_2_ & 0xfffd;
        }
        CreateChildWindows();
        SENDMESSAGE(hwndFrame,0x111,0xfa1,0);
        if (wParam == 0x9ca) {
          SENDMESSAGE(hwndMessage,0x100,0x23,0);
        }
        tutor.idt = 0;
        uStack_e = (uint)(wParam == 0x9ca);
        tutor.overlap_0 = (_tutor_at_0)((uint)tutor.overlap_0 & 0xfdf7 | uStack_e << 9);
        AdvanceTutor();
        return 0;
      }
      fFileErrSilent = uStack_c;
      gd.overlap_0.grBits_view.grBits._2_2_ = gd.overlap_0.grBits_view.grBits._2_2_ & 0xfffd;
      return 0;
    }
    goto LAB_1020_1d5a;
  }
  idPlayer = -1;
  if (((uint)ini.overlap_10 >> 1 & 1) != 0) {
    _Var1.fStartupFile_view = ini.overlap_10.fStartupFile_view & 0xfffd;
    if (((uint)ini.overlap_10 >> 0xe & 1) != 0) {
      fFileErrSilent = 1;
      ini.overlap_10 = _Var1;
      ClearFile(7);
      sVar5 = FLoadGame((schar *)szBase,(schar *)0x38e);
      if (sVar5 != 0) {
        VerifyTurns();
        DestroyCurGame();
        EnsureAis();
        _WSPRINTF((schar *)CONCAT22(unaff_SS,asStack_62),s___s__Year___d_1120_0392,0x90,0x1120,
                  game.turn + 0x960);
        OutputSz(7,(schar *)CONCAT22(unaff_SS,asStack_62));
        for (iStack_8 = 0; iStack_8 < game.cPlayer; iStack_8 = iStack_8 + 1) {
          if (((short *)rgOut)[iStack_8] + 1 < 4) {
            uStack_c = _WSPRINTF((schar *)CONCAT22(unaff_SS,asStack_62),(schar *)0x112003ac,
                                 iStack_8 + 1);
          }
          else {
            uStack_c = _WSPRINTF((schar *)CONCAT22(unaff_SS,asStack_62),s_Error___d__1120_03a0,
                                 iStack_8 + 1);
          }
          if (((uint)gd.overlap_4.grBits2_view.grBits2._2_2_ >> 6 & 1) == 0) {
            psVar4 = PszPlayerName(iStack_8,1,1,1,0,(_player *)0x0);
            sVar5 = _WSPRINTF((schar *)CONCAT22(unaff_SS,asStack_62 + uStack_c),(schar *)0x112003b1,
                              psVar4,0x1120);
            uStack_c = uStack_c + sVar5;
          }
          psVar4 = PszGetCompressedString(((short *)rgOut)[iStack_8] + 0x2cc);
          _strcat(asStack_62,psVar4);
          if (((uint)((_player_at_84_fDead *)&rgplr[0].overlap_84.fDead_view)[iStack_8 * 0x60] >> 4
              & 1) != 0) {
            _strcat(asStack_62,0x3b7);
          }
          OutputSz(7,(schar *)CONCAT22(unaff_SS,asStack_62));
        }
      }
LAB_1020_0af1:
      if (((uint)gd.overlap_0.grBits_view.grBits._2_2_ >> 6 & 1) == 0) {
        POSTQUITMESSAGE(vretExitValue);
      }
      else {
        EXITWINDOWS((long)vretExitValue,0);
      }
      return 0;
    }
    if (((uint)ini.overlap_10 >> 10 & 1) != 0) {
      if (((int)vSerialNumber != 0) || (ini.overlap_10 = _Var1, vSerialNumber._2_2_ != 0)) {
        ini.overlap_10 = _Var1;
        GenNewGameFromFile((schar *)szBase);
      }
      goto LAB_1020_0af1;
    }
    uVar2 = (uint)ini.overlap_10 >> 3;
    ini.overlap_10 = _Var1;
    if ((uVar2 & 1) != 0) {
      while( true ) {
        while (((((uint)ini.overlap_10 >> 2 & 1) == 0 && (((uint)ini.overlap_10 >> 4 & 1) == 0)) ||
               (sVar5 = CTurnsOutSafe(), sVar5 == 0))) {
          EnsureAis();
          FGenerateTurn();
          if ((((uint)ini.overlap_10 >> 9 & 1) != 0) && ((schar *)lpchBatch < (schar *)lpchBatchMac)
             ) goto LAB_1020_0a76;
          if (ini.cTurnGen == 0) goto LAB_1020_0af1;
          ini.cTurnGen = ini.cTurnGen + -1;
        }
        if (((uint)ini.overlap_10 >> 4 & 1) == 0) goto LAB_1020_0b67;
        if ((((uint)ini.overlap_10 >> 9 & 1) == 0) || ((schar *)lpchBatchMac <= (schar *)lpchBatch))
        break;
LAB_1020_0a76:
        DestroyCurGame();
        tStack_14.y = (short)szBase;
        while( true ) {
          if ((*lpchBatch == '\n') ||
             (((schar *)lpchBatch == (schar *)lpchBatchMac &&
              (lpchBatch._2_2_ == lpchBatchMac._2_2_)))) break;
          *(schar *)tStack_14.y = *lpchBatch;
          lpchBatch = (schar *)CONCAT22(lpchBatch._2_2_,(schar *)lpchBatch + 1);
          tStack_14.y = tStack_14.y + 1;
        }
        lpchBatch = (schar *)CONCAT22(lpchBatch._2_2_,(schar *)lpchBatch + 1);
        *(schar *)(tStack_14.y + -1) = '\0';
        ini.overlap_10 = (_ini_at_10)((uint)ini.overlap_10 & 0xfffe | 1);
      }
      goto LAB_1020_0af1;
    }
LAB_1020_0b67:
    CommandHandler(hwnd,0xed9);
    if (((uint)ini.overlap_10 >> 4 & 1) != 0) goto LAB_1020_0af1;
    if (((uint)ini.overlap_10 >> 3 & 1) != 0) goto LAB_1020_0d67;
    if (((int)game.lid != 0) || (game.lid._2_2_ != 0)) {
      if ((idPlayer != -1) &&
         (((((uint)ini.overlap_10 >> 0xc & 1) != 0 || (((uint)ini.overlap_10 >> 0xb & 1) != 0)) ||
          (((uint)ini.overlap_10 >> 0xd & 1) != 0)))) {
        if (((uint)ini.overlap_10 >> 0xd & 1) != 0) {
          POSTMESSAGE(hwndFrame,0x111,0x55,0);
        }
        if (((uint)ini.overlap_10 >> 0xc & 1) != 0) {
          POSTMESSAGE(hwndFrame,0x111,0x54,0);
        }
        if (((uint)ini.overlap_10 >> 0xb & 1) != 0) {
          POSTMESSAGE(hwndFrame,0x111,0x53,0);
        }
        goto LAB_1020_0af1;
      }
      SHOWWINDOW(hwndFrame,5);
      InitializeMenu(0);
      POSTMESSAGE(hwndFrame,0x111,0xfa1,0);
      if (((uint)ini.overlap_10 >> 2 & 1) != 0) {
        ini.overlap_10.fStartupFile_view = ini.overlap_10.fStartupFile_view & 0xfffb;
        CommandHandler(hwnd,0x6a);
      }
      goto LAB_1020_0d67;
    }
  }
  if (hwndTitle == 0) {
    uStack_10 = GETSYSTEMMETRICS(0);
    uStack_e = GETSYSTEMMETRICS(1);
    hwndTitle = CREATEWINDOW(szTitle,(schar *)0x112003c1,-0x70000000,0,0,uStack_10,uStack_e,
                             hwndFrame,0,hInst,(void *)0x0);
    fFreeingTitle = 0;
  }
  ini.overlap_10.fStartupFile_view = ini.overlap_10.fStartupFile_view & 0xfffe;
  DestroyCurGame();
LAB_1020_0d67:
  if ((((int)vSerialNumber != 0) || (vSerialNumber._2_2_ != 0)) &&
     (iVar3 = _memcmp((uchar *)vrgbMachineConfig,(uchar_0_ *)&vrgbEnvCur,0xb), iVar3 == 0)) {
    return 0;
  }
  if (((int)vSerialNumber == 0) && (vSerialNumber._2_2_ == 0)) {
    szWork[200] = '\0';
  }
  else {
    szWork[200] = '\x01';
  }
  join_0x00000008_0x00000000_ = (void *)MAKEPROCINSTANCE(MsgDlg,hInst);
  uVar6 = hwndTitle;
  if (hwndTitle == 0) {
    uVar6 = hwndFrame;
  }
  auStack_18._0_2_ = DIALOGBOX(hInst,(schar *)0x56,uVar6,join_0x00000008_0x00000000_);
  FREEPROCINSTANCE(stack0xffea);
  if (auStack_18._0_2_ == 0) {
    vSerialNumber._0_2_ = 0;
    vSerialNumber._2_2_ = 0;
    _memcpy((uchar *)vrgbMachineConfig,(uchar_0_ *)&vrgbEnvCur,0xb);
    POSTQUITMESSAGE(vretExitValue);
  }
  else {
    sVar5 = FValidSerialNo((schar *)szWork,(long *)&sStack_1c);
    if (sVar5 == 0) {
      if (((int)vSerialNumber == 0) && (vSerialNumber._2_2_ == 0)) {
        _memcpy((uchar *)vrgbMachineConfig,(uchar_0_ *)&vrgbEnvCur,0xb);
        POSTQUITMESSAGE(vretExitValue);
      }
    }
    else {
      vSerialNumber._0_2_ = sStack_1c;
      vSerialNumber._2_2_ = sStack_1a;
      _memcpy((uchar *)vrgbMachineConfig,(uchar_0_ *)&vrgbEnvCur,0xb);
    }
  }
  WriteIniSettings();
  return 0;
}

