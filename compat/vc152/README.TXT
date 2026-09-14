STARS VC++ 1.52c COMPATIBILITY PROBE - PACKAGE 1
==============================================

BUILD AND RUN IN YOUR WINDOWS 98 SE / 86BOX VM

1. Copy every file from this package into C:\STPROBE in the VM.
   Use binary FTP transfer mode for all files, including the ZIP if used.
   The ZIP contains the same files, with no enclosing directory.
   Keep the source files together and use a short path without spaces.

2. Open an MS-DOS Prompt while Windows 98 is running. Do not reboot to DOS.
   Enter:

       C:
       CD \STPROBE
       BUILD C:\MSVC15

   Replace C:\MSVC15 with your actual VC++ 1.52c installation directory.
   It must contain BIN\CL.EXE, BIN\LINK.EXE, INCLUDE\WINDOWS.H,
   LIB\MLIBCEW.LIB, and LIB\LIBW.LIB. Use the short DOS path if necessary.
   No IDE project setup is needed. BUILD sets PATH, INCLUDE and LIB in this
   prompt and clears CL/LINK option overrides. Close the prompt afterwards
   to discard those environment changes.

3. BUILD prints a final status for COMPILE, LINK and LAYOUT. If LINK=PASS,
   enter the following in the same prompt, even if LAYOUT=FAIL:

       PROBE.EXE

   Wait for its message box, then click OK. It writes RESULTS.TXT to the
   current folder before showing the message. A successful native run ends
   with RESULT=PASS and FAILURES=0. MANIFEST.TXT gives the expected count.

4. Copy the whole C:\STPROBE folder back into a separate folder such as
   ftp-share/RETURN1 on the host, using binary FTP transfer mode. Tell Codex
   the returned folder is ready. Preserve the original outgoing package.

   The useful generated files are:
       BLDSTAT.TXT   independent compile/link/header status
       BUILD.LOG     runtime probe compiler output and command
       LINK.LOG      linker output and command
       LAYOUT.LOG    full recovered-header compiler output and command
       RESULTS.TXT   runtime platform, ABI and original-game vector checks
       *.OBJ         compiler-generated object code and debug information
       PROBE.EXE     Win16 executable with CodeView information
       PROBE.MAP     linked symbol and segment map

   Compiler/linker banners are enabled. COMMAND.COM cannot redirect stderr
   with the newer 2>&1 syntax; some diagnostics/banners may only be visible
   on screen. If absent from the logs, copy the compiler/linker version lines
   and the FIRST error into a BANNERS.TXT file or your reply. Return failed
   builds too. Do not strip debug information or change flags to force a pass.

   Use a fresh package copy in another directory for each new attempt.
   BUILD refuses to overwrite a folder containing BLDSTAT.TXT.

WHAT THIS EXPERIMENT CHECKS

- An actual Win16 executable, using /AM (medium: far code and near data),
  /GA /GEs /Zp1 /Od /Zi /W3, MLIBCEW and LIBW. It records _MSC_VER,
  _M_I86 and _M_I86MM, scalar sizes and data/function pointer sizes.
- Explicit near/cdecl, far/cdecl and far/pascal calls, a pascal function
  pointer, mixed-width arguments, repeated stack cleanup and 32-bit returns.
- Eleven reconstructed utility functions: Randomize, Randomize2, Random,
  GetFileSeeds, SetFileSeeds, SetFileXorStream, the file XOR generator,
  DistanceSquared, CompareLong, ExpandRc and OffsetRc. Expected values were
  captured by executing the original game's x86 instructions on synthetic
  inputs, including arithmetic wrap, sign, seed and output-aliasing cases.
- A separate compile of all four generated headers, using real near/far
  pointer types and the recovered size/offset assertions. Additional named
  checkpoints cover the 56-byte planet, 124-byte fleet and 192-byte player.
  LAYOUT.OBJ is deliberately not linked into PROBE.EXE.

This is an integration experiment, not the complete game or a byte-identical
rebuild. The utility routines still use explicit state adapters. A pass does
not establish the original IDE, exact maintenance release, all compiler flags,
Windows 3.1 execution, all floating-point behavior or full-game compatibility.
The /Zp1 choice tests packed layouts; it is not a recovered original flag.
Native build/run results are pending until you perform this experiment.

PROVENANCE AND FILE FORMAT

MANIFEST.TXT records input/output hashes and the original executable identity.
No original executable, OS, compiler or runtime libraries are included.
UTILGEN.C retains the reviewed logic. Packaging changes only local include
filenames (including SEEDTAB.INC), filename case, and text line endings.
All outgoing names are DOS 8.3; text is ASCII with CRLF line endings.
VECTEST.C is generated from retained original-machine observations.

Build recipe reference: Microsoft KB Q88113, archived Microsoft instructions
for medium-model Windows builds. This probe is plain C and omits MFC.
https://jeffpar.github.io/kbarchive/kb/088/Q88113/
Link response-file syntax: Microsoft KB Q40429.
https://jeffpar.github.io/kbarchive/kb/040/Q40429/
