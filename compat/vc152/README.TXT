STARS VC++ 1.52c COMPATIBILITY PROBE - PACKAGE 2A
==============================================

RETURN2 PASSED BOTH NATIVE TESTS - NO REPEAT BUILD NEEDED

RETURN2 passed 563 utility and 1,491 layout checks with zero failures.
Package 2A changes only BUILD.BAT's status display and documentation.
The native C code, headers, compiler/linker inputs and test cases are unchanged.
The completed early compatibility check does not need another build/return.

The returned logs are ASCII/CRLF, without UTF-16 or embedded NUL bytes.
The user's DOS window displayed the TYPE summary with extra spaces. BUILD
now prints four status lines directly and shows a run instruction only when
that executable linked successfully. BLDSTAT.TXT is still retained as evidence.
This display workaround can be observed during a future ordinary build.

FOR FUTURE BUILDS IN YOUR WINDOWS 98 SE / 86BOX VM

1. Copy every file into a FRESH directory, for example C:\STPROB2.
   Use binary FTP mode. The ZIP contains the same files, without an enclosing
   directory. Keep filenames together in a short DOS path without spaces.
   Preserve the previous build/return folder.

2. Open an MS-DOS Prompt while Windows 98 is running. Enter:

       C:
       CD \STPROB2
       BUILD C:\MSVC

   C:\MSVC is the installation root recorded by RETURN1. If you moved the
   compiler, substitute the directory containing BIN, INCLUDE and LIB.
   BUILD sets PATH, INCLUDE and LIB in this prompt and clears CL/LINK option
   overrides. Close the prompt afterwards to discard environment changes.

3. When LINK=PASS, run:

       PROBE.EXE

   Wait for the message box and click OK. RESULTS.TXT should report:
       KIND=UTILITY
       MODE=WIN16_MEDIUM
       CHECKS=563
       FAILURES=0
       RESULT=PASS

4. When LAYLINK=PASS, also run:

       LAYOUT.EXE

   Wait for its separate message box and click OK. LAYRES.TXT should report:
       KIND=LAYOUT
       MODE=WIN16_MEDIUM
       CHECKS=1491
       FAILURES=0
       RESULT=PASS

   LAYOUT=PASS in the build status means compilation passed. The offsets
   are not validated until LAYOUT.EXE runs and LAYRES.TXT reports success.
   Each program writes its report in the current folder before its message.

5. Copy the whole C:\STPROB2 directory back to ftp-share/RETURN2 on the host
   using binary FTP mode. Tell Codex the returned folder is ready.
   Keep failed builds too; partial success is useful evidence.

FILES TO RETURN

   BLDSTAT.TXT          independent utility/layout compile and link statuses
   BUILD.LOG, LINK.LOG utility compiler/linker commands and output
   LAYOUT.LOG          layout compiler commands and output
   LAYLINK.LOG         layout linker commands and output
   RESULTS.TXT         utility runtime report
   LAYRES.TXT          recovered-layout runtime report
   *.OBJ, *.EXE, *.MAP  code, debug records, and linker maps
   MSVC.PDB            compiler debug database, if generated

Compiler/linker banners are enabled. Some output may only appear on screen
because COMMAND.COM lacks the newer 2>&1 stderr-redirection syntax. If an
error or version line is missing from logs, copy it into your reply or a
BANNERS.TXT file. The FIRST error is particularly useful.

BUILD refuses to overwrite a folder containing BLDSTAT.TXT. The "already
contains build results" message in RETURN1/OUTPUT.TXT was this safeguard,
not a compiler failure. To collect a new console transcript, start from a
fresh package copy; rerunning BUILD in a used folder only records the guard.

WHAT IS TESTED

PROBE.EXE uses eleven reconstructed utility functions with 113 retained
original-x86 cases, plus native integer/pointer widths, near/cdecl and
far/cdecl calls, far/pascal calls and callbacks, and 32-bit returns.

LAYOUT.EXE compiles all recovered type/function/global/constant headers.
The generated types retain 772 type-size assertions plus primitive checks.
The complete SOFFSETS.INC checklist supplies all 1,469 field-offset checks
at runtime, alongside platform and representative record-size checks.
A failed layout.offset[index] maps to the zero-based checklist entry.

All offset expressions use the compiler's actual offsetof implementation;
expected offsets remain the values recovered from the game's debug types.
STARS_RUNTIME_OFFSETS defers offset checks only. It does not change storage
views, sizes, pointer kinds or member locations.

This is a compiler integration experiment, not a full game rebuild. The
utility functions still use explicit state adapters. Matching 8.00c debug
records and runtime helpers support this toolchain's compatibility but do
not prove the exact original IDE, maintenance release, or all compiler flags.

BUILD AND PROVENANCE

The build uses /AM /GA /GEs /Zp1 /Od /Zi /W3, MLIBCEW and LIBW. The layout
runner also uses /DLAYOUT_PROBE. /AM selects far code and near data; /Zp1
is an experimental packing choice, not a proven original compiler flag.

MANIFEST.TXT records hashes, original executable identity and check counts.
All outgoing names are DOS 8.3, with ASCII/CRLF text. Only quoted include
filenames and line endings change in the copied reconstructed utility code.
No original executable, compiler, OS or runtime libraries are distributed.

References: archived Microsoft KB Q88113 (medium Windows build recipe) and
Q40429 (five-field linker response files).
https://jeffpar.github.io/kbarchive/kb/088/Q88113/
https://jeffpar.github.io/kbarchive/kb/040/Q40429/
