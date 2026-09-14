# VC++ 1.52c function-byte experiment

This standalone compiler probe reproduces GetRaceStat, SetRaceStat and Random
instruction bytes with the original 16-bit interfaces. It does not replace the
reviewed host component or build a complete Stars! executable.

Copy PROBE.C and BUILD.BAT into a fresh guest folder. From DOS or a DOS prompt
inside Windows, run `BUILD C:\MSVC` (adjust the compiler directory if necessary).
Return PROBE.OBJ, PROBE.COD, BUILD.LOG and DONE.TXT. The source/commands are plain
ASCII. The object is the comparison input; the compiler listing alone contains
unresolved forward-reference placeholders and is not a final byte comparison.

The demonstrated flags are `/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc`.
The warning C4135 on Random's return is expected: the unsigned long remainder
is narrowed to the 16-bit int return, just as the original returns AX.

On the host, with the fixed original executable available:

```
python3 tools/compare_byte_match.py path/to/returned-folder --output runs/new-comparison.json
```

Use a new output path to preserve prior results. All three functions and the
32-byte race-stat tables must match. The comparison resolves actual OMF fixups
by explicit target identity into the original address layout. It does not mask
address fields or rewrite differing instructions. Random uses two explicitly
mapped global aliases, lSeed1/lSeed2, for original lRandSeed1/lRandSeed2.

The PLAYER declaration is only the accessed prefix with statistics at offset 62;
it is not offered as the complete original structure. The source preserves the
matching local declaration order and global aliases intentionally. Renaming
those globals changed the compiler's stack allocation in a control experiment.
The compiler's internal reason is not yet isolated.

Accepted results and limits: [experiment report](../../analysis/compat/byte-match.md).
Raw local trials: `runs/byte-match/`. No Windows 95 disk image needs to be mounted
for this compiler-only experiment; the existing folder-backed MSVC installation
can run in a separate DOSBox process.
