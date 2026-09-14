@echo off
if "%1"=="" goto usage
if not exist %1\BIN\CL.EXE goto usage
if not exist STARS.RES goto usage
set PATH=%1\BIN;%PATH%
set INCLUDE=%1\INCLUDE
set LIB=%1\LIB
set CL=
set LINK=
echo Compiling the reconstructed application...
echo BUILD=START1>BLDSTAT.TXT
%1\BIN\CL @START.RSP >BUILD.LOG
if errorlevel 1 goto failed
echo COMPILE=PASS>>BLDSTAT.TXT
echo Linking...
%1\BIN\LINK @START.LNK >LINK.LOG
if errorlevel 1 goto failed
echo LINK=PASS>>BLDSTAT.TXT
echo Binding the original resources...
%1\BIN\RC STARS.RES START.EXE >RC.LOG
if errorlevel 1 goto failed
echo RESOURCES=PASS>>BLDSTAT.TXT
echo Build passed. Run START.EXE while Windows is running.
goto end
:failed
echo BUILD=FAIL>>BLDSTAT.TXT
echo Build failed. Keep BUILD.LOG, LINK.LOG, RC.LOG and screen diagnostics.
goto end
:usage
echo Change to the package folder. Then run BUILD C:\MSVC
:end
