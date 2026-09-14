@echo off
rem Run in the copied package folder from a DOS prompt INSIDE Windows 98.
rem Supply the compiler installation root, e.g. BUILD C:\MSVC
if "%1"=="" goto usage
if not exist %1\BIN\CL.EXE goto badroot
if not exist %1\BIN\LINK.EXE goto badroot
if not exist %1\INCLUDE\WINDOWS.H goto badroot
if not exist %1\LIB\MLIBCEW.LIB goto badroot
if not exist %1\LIB\LIBW.LIB goto badroot
if not exist PROBE.RSP goto wrongdir
if exist BLDSTAT.TXT goto used
set VCP_COMP=SKIPPED
set VCP_LINK=SKIPPED
set VCP_LAY=SKIPPED
set VCP_LL=SKIPPED
set PATH=%1\BIN;%PATH%
set INCLUDE=%1\INCLUDE
set LIB=%1\LIB
rem Keep pre-existing compiler/linker option variables out of this experiment.
set CL=
set LINK=
>BLDSTAT.TXT echo STARS_VC152_BUILD=2
>>BLDSTAT.TXT echo ROOT=%1
echo Building the runtime probe. Diagnostics may also appear on screen.
>BUILD.LOG echo COMMAND: %1\BIN\CL @PROBE.RSP
%1\BIN\CL @PROBE.RSP >>BUILD.LOG
if errorlevel 1 goto compilefail
if not exist PROBE.OBJ goto compilefail
if not exist ABI.OBJ goto compilefail
if not exist VECTEST.OBJ goto compilefail
if not exist UTILGEN.OBJ goto compilefail
>>BLDSTAT.TXT echo COMPILE=PASS
set VCP_COMP=PASS
echo Linking the Win16 executable.
>LINK.LOG echo COMMAND: %1\BIN\LINK @PROBE.LNK
%1\BIN\LINK @PROBE.LNK >>LINK.LOG
if errorlevel 1 goto linkfail
if not exist PROBE.EXE goto linkfail
>>BLDSTAT.TXT echo LINK=PASS
set VCP_LINK=PASS
goto layout
:compilefail
set VCP_COMP=FAIL
>>BLDSTAT.TXT echo COMPILE=FAIL
>>BLDSTAT.TXT echo LINK=SKIPPED
echo Compilation failed. Keep the first on-screen error and BUILD.LOG.
type BUILD.LOG
goto layout
:linkfail
set VCP_LINK=FAIL
>>BLDSTAT.TXT echo LINK=FAIL
echo Linking failed. Keep the first on-screen error and LINK.LOG.
type LINK.LOG
:layout
echo Building the separate recovered-layout test.
>LAYOUT.LOG echo COMMAND: %1\BIN\CL @LAYOUT.RSP
%1\BIN\CL @LAYOUT.RSP >>LAYOUT.LOG
if errorlevel 1 goto layoutfail
if not exist LAYOUT.OBJ goto layoutfail
>>LAYOUT.LOG echo COMMAND: %1\BIN\CL @LAYMAIN.RSP
%1\BIN\CL @LAYMAIN.RSP >>LAYOUT.LOG
if errorlevel 1 goto layoutfail
if not exist LAYMAIN.OBJ goto layoutfail
>>BLDSTAT.TXT echo LAYOUT=PASS
set VCP_LAY=PASS
>LAYLINK.LOG echo COMMAND: %1\BIN\LINK @LAYOUT.LNK
%1\BIN\LINK @LAYOUT.LNK >>LAYLINK.LOG
if errorlevel 1 goto laylinkfail
if not exist LAYOUT.EXE goto laylinkfail
>>BLDSTAT.TXT echo LAYLINK=PASS
set VCP_LL=PASS
goto done
:layoutfail
set VCP_LAY=FAIL
>>BLDSTAT.TXT echo LAYOUT=FAIL
>>BLDSTAT.TXT echo LAYLINK=SKIPPED
echo Header checking failed. Keep the first on-screen error and LAYOUT.LOG.
type LAYOUT.LOG
goto done
:laylinkfail
set VCP_LL=FAIL
>>BLDSTAT.TXT echo LAYLINK=FAIL
echo Layout linking failed. Keep the first on-screen error and LAYLINK.LOG.
type LAYLINK.LOG
:done
echo.
rem Print directly: TYPE displayed spaced/truncated text in the user's VM.
echo Utility compile: %VCP_COMP%
echo Utility link:    %VCP_LINK%
echo Layout compile:  %VCP_LAY%
echo Layout link:     %VCP_LL%
echo.
if "%VCP_LINK%"=="PASS" echo Ready: run PROBE.EXE here while Windows is running.
if "%VCP_LL%"=="PASS" echo Ready: run LAYOUT.EXE here. It writes LAYRES.TXT.
echo Return this whole folder, including the generated files.
echo Use a fresh copy of the package for another build attempt.
goto end
:used
echo This folder already contains build results. Preserve it for return.
echo Build again from a fresh package copy in a different folder.
goto end
:badroot
echo Cannot find the 16-bit compiler, Windows headers, or libraries at %1.
echo Supply the directory containing BIN, INCLUDE, and LIB, without spaces.
echo Example: BUILD C:\MSVC
goto end
:wrongdir
echo Change into the copied package folder before running BUILD.
goto end
:usage
echo Usage: BUILD compiler-directory
echo Example: BUILD C:\MSVC
:end
set VCP_COMP=
set VCP_LINK=
set VCP_LAY=
set VCP_LL=
