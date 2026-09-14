@echo off
if exist DONE.TXT goto used
if "%1"=="" goto usage
set PATH=%1\BIN;%PATH%
set INCLUDE=%1\INCLUDE
set CL=
echo Compiling original-interface byte-match experiment...
echo FLAGS=/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc > BUILD.LOG
%1\BIN\CL /c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc PROBE.C >> BUILD.LOG
if errorlevel 1 goto failed
if not exist PROBE.OBJ goto failed
echo COMPILE=PASS > DONE.TXT
echo Compilation passed. Return PROBE.OBJ, PROBE.COD and BUILD.LOG.
goto end
:failed
echo COMPILE=FAIL > DONE.TXT
echo Compilation failed. Preserve diagnostics and this folder.
goto end
:used
echo Existing results found. Use a fresh package copy.
goto end
:usage
echo Usage: BUILD C:\MSVC
:end
