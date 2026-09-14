# RETURN3 race build review

The returned folder is `ftp-share/RETURN3/Racevc1`. Every one of the 104
manifest-listed package inputs is unchanged, as is `MANIFEST.JSN`. All 23
expected object files are present and nonempty. `BLDSTAT.TXT` reports
`COMPILE=PASS`, `LINK=FAIL`. No runtime report was returned, so no native
acceptance test is credited. Hashes and captured diagnostics are retained in
[the evidence snapshot](racevc-return3.json); the returned folder is preserved.

The segmented linker identifies itself as **5.60.339, Dec 5 1994**. It accepts
the response-file continuations, all object names, the large-model Windows CRT
and Windows import library, and the module-definition file. The sole reported
linker error is:

```
RUNNER.OBJ(runner.c) : error L2029: '_chdir' : unresolved external
```

The launcher called C `chdir`, producing decorated symbol `_chdir`. Its sibling
`_chdrive` already resolved as `__chdrive`, visible in the map. Revision 2 calls
the Microsoft CRT entry `_chdir` directly, avoiding reliance on an old-name
alias library under `/NOD`. Microsoft's [CRT documentation](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/chdir)
identifies `chdir` as an alias of `_chdir`; this modern documentation supports
the naming distinction, while RETURN3 supplies the actual old-linker failure.
The revised call's native link remains to be checked in the VM.

`BUILD.LOG` also contains C4018 warnings at FORMAT.C:125 and :126, comparing a
signed fleet key with an unsigned ID. These comparisons can behave differently
when `int` is 16 bits. The original LpflFromId instructions use signed JL/JG at
NE 8:2123/2137. Revision 2 converts the already masked, at-most-8191 ID to `int`
before comparing. This preserves the original signed behavior and removes the
cause of both warnings. Valid addresses with signed boundary keys 8000/FFFF
are checked through the original formatter and recovered C. The race-only
catalog does not exercise this fleet lookup, so its 812 host checks alone would
not have detected the issue.

The allowlisted return contained no separate `OUTPUT.TXT`; the diagnostic
assessment uses BUILD.LOG, LINK.LOG, BLDSTAT.TXT and RACEVC.MAP. It does not
assume an unreturned console capture was clean. Package input verification and
object hashes do not imply that an executable successfully linked or ran.

The replacement is `ftp-share/RACEVC2` / `RACEVC2.ZIP`. Use a fresh VM folder,
run `BUILD C:\MSVC`, then `RACEVC.EXE` if linking passes, and return the whole
folder. The scenario data, original expectations, libraries, memory model and
expected native total of 816 checks are unchanged. Revision 1 and RETURN3 are
retained for comparison.

Local validation of revision 2 passed all 11 focused tests: the five package
checks (including 88 scenarios / 812 assertions) and all six original-instruction
formatter tests. A Windows-launcher declaration check passes the corrected call
and rejects the former alias; it does not claim to run the native SDK or linker.
The clean replacement's manifest and ZIP were verified, and every scenario,
expected file and static binary input is unchanged from revision 1.
