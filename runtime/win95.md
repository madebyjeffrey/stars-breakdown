# Image-based Windows runtime

Launch with `runtime/start-win95.command`. It supplies an absolute configuration
path and suppresses the DOSBox working-directory prompt. The configuration is
`runtime/win95.conf`; the filename is retained even though its window title says
Windows 98. That title is not an independent Windows-version check.

## Drives after the next full DOSBox restart

| Device | Image | Expected Windows letter |
| --- | --- | --- |
| Primary IDE master | `runs/step4/win95.img` | C: |
| Primary IDE slave | `runs/step4/transfer.img` | D: |
| Secondary IDE master | User's MSVC 1.52c ISO, named in the configuration | E: |

Windows assigns drive letters itself. The added hard disk normally shifts the
compiler CD from D: to E:. Finish the current MSVC installation before closing
and restarting DOSBox. Check My Computer afterward; if the installer later asks
for its CD, use the actual CD drive letter.

The main image has one active FAT32 partition and a valid MBR. This review did
not mount, alter or copy files into that running image. The old folder-backed
installation is not used by this configuration. The live screen showed the
MSVC program group, but a compiler invocation and new-drive boot test remain
pending completion of the user's installation.

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
`runs/win95-validation`. A restart and in-guest drive/compiler test are still
required after the user finishes installation. No Win9x game/color acceptance
is claimed by this configuration review.

Drive attachment and image boot behavior follow the
[DOSBox-X Windows 98 guide](https://dosbox-x.com/wiki/Guide:Installing-Windows-98).
