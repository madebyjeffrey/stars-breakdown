@echo off
rem Build a fresh copy from a DOS prompt inside Windows 98.
if "%1"=="" goto usage
if not exist %1\BIN\CL.EXE goto badroot
if not exist %1\BIN\LINK.EXE goto badroot
if not exist %1\INCLUDE\WINDOWS.H goto badroot
if not exist %1\LIB\LLIBCEW.LIB goto badroot
if not exist %1\LIB\LIBW.LIB goto badroot
if not exist RACEVC.RSP goto wrongdir
if exist BLDSTAT.TXT goto used
if exist RACEVC.EXE goto used
if exist RESULTS.TXT goto used
set PATH=%1\BIN;%PATH%
set INCLUDE=%1\INCLUDE
set LIB=%1\LIB
set CL=
set LINK=
>BLDSTAT.TXT echo STARS_RACEVC_BUILD=2
>>BLDSTAT.TXT echo ROOT=%1
echo Compiling the race component and automatic test program.
echo Keep any on-screen diagnostics in OUTPUT.TXT as well as these logs.
>BUILD.LOG echo COMMAND: CL @RACEVC.RSP
%1\BIN\CL @RACEVC.RSP >>BUILD.LOG
if errorlevel 1 goto compilefail
>>BLDSTAT.TXT echo COMPILE=PASS
echo Compile: PASS
echo Linking the Win16 program.
>LINK.LOG echo COMMAND: LINK @RACEVC.LNK
%1\BIN\LINK @RACEVC.LNK >>LINK.LOG
if errorlevel 1 goto linkfail
if not exist RACEVC.EXE goto linkfail
>>BLDSTAT.TXT echo LINK=PASS
echo Link: PASS
echo Run RACEVC.EXE here while Windows is running.
echo It runs all 88 scenarios automatically. No wizard clicking is needed.
echo Return this whole folder, including ACTUAL and RESULTS.TXT.
goto end
:compilefail
>>BLDSTAT.TXT echo COMPILE=FAIL
>>BLDSTAT.TXT echo LINK=SKIPPED
echo Compile: FAIL. Keep BUILD.LOG and all on-screen diagnostics.
echo Return the entire folder even if compilation failed.
goto end
:linkfail
>>BLDSTAT.TXT echo LINK=FAIL
echo Link: FAIL. Keep LINK.LOG and all on-screen diagnostics.
echo Return the entire folder even if linking failed.
goto end
:badroot
echo Cannot find the 16-bit compiler, Windows headers, or large-model libraries.
echo Supply the compiler root containing BIN, INCLUDE, and LIB.
echo Example: BUILD C:\MSVC
goto end
:wrongdir
echo Change into the copied RACEVC2 folder first.
goto end
:used
echo Preserve the results here. Use a fresh package copy for another attempt.
goto end
:usage
echo Usage: BUILD compiler-root
echo Example: BUILD C:\MSVC
:end
