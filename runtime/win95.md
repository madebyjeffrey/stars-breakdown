# Image-based Windows runtime

Launch with `runtime/start-win95.command`. It supplies an absolute configuration
path and suppresses the DOSBox working-directory prompt. The configuration is
`runtime/win95.conf`. The guest reports Windows 95 version 4.00.1111; the
configuration title now matches it (effective on the next launch).

## Verified drives

| Device | Image | Verified Windows letter |
| --- | --- | --- |
| Primary IDE master | `runs/step4/win95.img` | C: |
| Primary IDE slave | `runs/step4/transfer.img` | D: |
| Secondary IDE master | User's MSVC 1.52c ISO, named in the configuration | E: |

The restarted guest recognizes the transfer disk as D: and the MSVC152C CD as
E:. The installed compiler is at C:\MSVC. Use E: if the installer asks for its CD.

The main image has one active FAT32 partition and a valid MBR. It was never
mounted on the host during validation. The old folder-backed installation is
not used. Through Windows, START2A was copied to C:\START2A and rebuilt using
C:\MSVC; compilation, linking and resource binding all passed.

## Mouse

Autolock remains enabled. Ctrl+F10 releases or recaptures the pointer; on a Mac
keyboard, Fn may also be needed to send F10. Main > Capture mouse is the menu
alternative. A standard guest Windows mouse driver uses relative movement, so
the `mouse emulation=integration` host setting alone does not establish seamless
guest mouse integration. No driver was installed or replaced during this review.
See the [DOSBox-X mouse guide](https://dosbox-x.com/wiki/Guide:Mouse-support-in-DOSBox%E2%80%90X).

## Persistent transfer disk

The approximately 64 MiB FAT16 transfer image contains:

- `START2A`: the frozen source, executable and regression package.
- `STARS`: only the allowlisted original STARS.EXE. User will provide serial number.
- `RETURN`: space for completed synthetic test folders.

In Windows, copy START2A to C:\START2A before running or building. Copy completed
test folders into the transfer disk's RETURN directory. This is a real disk
image, not a live shared Mac folder. Files remain in the image across restarts.

After shutting down Windows **and closing its DOSBox instance**, export a new
host snapshot from the project root:

```sh
python3 tools/export_win95_transfer.py runs/win95-return-01
```

Choose a new destination for each export. The helper preserves existing outputs,
refuses to open an image already held by another process, and uses `mcopy` from
Homebrew's mtools package. It copies the entire transfer disk; returned tests
appear under the destination's RETURN directory. Keep the image until the
returned data has been reviewed. Future packages can be staged with mtools while
the image is closed; host changes must never overlap the guest's disk access.

## Validation, 2026-09-14

DOSBox-X accepted the revised configuration with autoexec disabled, so that
check did not mount or boot the user's live image. The CD now has an explicit
IDE attachment. Obsolete host-DOS commands after BOOT were removed: the booted
OS uses its own startup files and environment. CPU, memory, graphics, sound and
mouse choices were retained.

The separate transfer image was created, closed, reopened and exported. All
173 files / 10,864,185 bytes matched the staged files exactly, including the
original executable's recorded hash and the frozen START2A package. The export
helper also rejected a deliberately open image. DOSBox's bundled XCOPY could
copy host files into the image but failed to enumerate image files on export;
the working export path uses mtools 4.0.49 instead.

The original configuration backup and local validation logs are under
`runs/win95-validation`. After the user restarted, the native START2A build and
`START.EXE /CHECK` acceptance passed: startup, page-4 pixel colors, wizard, race
save/load roundtrip, cancellation, cleanup and exit. Switches are case-sensitive.
A separate manual run showed the bitmap title and correctly placed blue, red
and green habitability ranges. The full 816-check regression was not rerun.

This validates Windows 95 with the configured S3 display path; it does not isolate
the precise cause of the old Windows 3.11/ET4000 drawing problem. Keyboard control
works with mouse capture active, but rapid injected text can drop characters;
check guest commands before relying on them.

Drive attachment and image boot behavior follow the
[DOSBox-X Windows 98 guide](https://dosbox-x.com/wiki/Guide:Installing-Windows-98).

Native build logs and acceptance evidence: [startup2a-win95.json](../analysis/compat/startup2a-win95.json).
The full returned build is under `runs/win95-acceptance/export/RETURN/WIN95A`.
The fresh acceptance log is `win9hk.log`; the package's older `CHECK.LOG` is
retained unchanged. `output.txt` is empty; BUILD.LOG, LINK.LOG, RC.LOG and
BLDSTAT.TXT contain the build diagnostics and passing status.
