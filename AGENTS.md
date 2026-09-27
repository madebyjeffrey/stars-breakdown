# Win98 / 86Box agent runbook

This is the operational guide for future agent requests that need to compile or
run reconstructed Stars! C code with Microsoft Visual C++ 1.52c inside the
Win98 guest.

## What is available

- The running 86Box machine is normally named `StarDev`.
- The guest compiler is installed at `C:\MSVC` and is the VC++ 1.52c toolchain.
- `ftp-share/VMCTL1/` contains the `STARSAGT` Win32 guest-agent source, its
  host controller, and the guest startup batch file. The currently tested
  compiled agent is installed in the guest under `C:\STARSAGT`.
- The agent connects from the guest to host address `10.0.2.2`, port `2122`.
  It transfers files directly; FTP is not required.
- The controller can transfer files, create directories, run and wait for
  processes, enumerate/focus/click windows, send keys, and capture screenshots.

The 86Box manager's preview may say `No screenshot`. That is not a blocker:
the agent's `windows` and `screenshot` commands are the reliable control and
observation path.

### Selecting the running guest instead of the manager

On this macOS setup, `86Box VM Manager` and the running `StarDev` guest are
separate 86Box processes. The manager lists the VM and shows a small preview;
the guest console shows the actual Win98 desktop. The computer-use
`getApp("86Box")` binding selected the **manager** even after the guest was
visible, so do not send guest keystrokes to that binding or infer guest state
from its preview. Bring the separate StarDev guest console to the foreground
using the macOS window switcher/Dock, then operate it through the connected
STARSAGT controller. With the listener running, confirm `info`, `ping`, and
`windows` show `STARSAGT 0.4.2`, `PONG`, and the Win98 desktop before issuing
guest commands. If the agent is not connected, start `C:\VMCTL1\STARTAGT.BAT`
inside the guest console. The manager's Hard reset and Force shutdown buttons
are not a way to select the guest window.

For original `STARS.EXE` experiments, stop after any timeout, abnormal exit,
or guest-agent stall. Preserve the partial output and `AGENT.LOG`, then
diagnose the input offline before another original run. Two task-5
deep-space candidates (`C:\SYDSP1` and `C:\SYDSP2`) left only partial
`.H2`/`.X2` files and disrupted the guest agent; neither is valid
original-behavior evidence. Do not repeat them as routine probes.

## Preconditions

1. Start or bring the `StarDev` 86Box machine to the Win98 desktop.
2. Ensure the guest agent is running. In the guest, use:

       C:
       CD \VMCTL1
       STARTAGT

   The agent window should identify itself as `STARSAGT`. The current tested
   agent version is `0.4.2`. If it is not running after a normal restart, start
   it again; avoid hard-resetting the VM while it is writing files.
3. From the project root, start the host listener in a persistent interactive
   terminal:

       python3 ftp-share/VMCTL1/HOSTCTL.PY listen --port 2122

   The expected connection banner is `AGENTINFO STARSAGT 0.4.2`. A sandbox may
   require host-network approval to bind port 2122. Request that permission
   only for this local listener. The listener is interactive; keep its terminal
   open while issuing controller commands.
4. Begin each session with:

       info
       ping
       windows

   `ping` should return `PONG`. `windows` should show the agent window and the
   Win98 desktop windows.

The local protocol self-test is useful when diagnosing the controller itself:

    python3 ftp-share/VMCTL1/HOSTTEST.PY

It uses a local socket pair only; it does not test the live guest connection.

## Agent bootstrap and maintenance

The remote controller cannot bootstrap a missing agent because the agent is the
network endpoint. If the guest has no `C:\STARSAGT\STARSAGT.EXE`, a person must
start the guest and build or restore it locally first. The agent itself is a
Win32 program built with Visual C++ 6.0, not with the VC1.52c compiler under
test. The full maintenance instructions are in
[`ftp-share/VMCTL1/README.TXT`](ftp-share/VMCTL1/README.TXT); in particular:

    C:
    CD \VMCTL1
    BUILD
    STARTAGT

The default VC6 setup path is `C:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat`.
The guest startup script uses the tested endpoint and token:
`STARSAGT.EXE 10.0.2.2 2122 stars98-preview`.

If the agent starts but cannot connect, inspect `C:\STARSAGT\AGENT.LOG` in the
guest. A connected session can retrieve it with:

    getlog runs/<run-id>/AGENT.LOG

The agent update flow (`update` / `activate`) is separate from testing the
reconstructed C code. Do not update the agent during a compiler experiment
unless the request specifically concerns the agent.

## Preparing a VC1.52c package

Run the package checks on the host first:

    python3 -m unittest tests.test_vc152_package -v

Generate a fresh package for every guest build. Never reuse a directory that
contains `BLDSTAT.TXT`, because `BUILD.BAT` deliberately refuses to overwrite
previous results:

    python3 tools/package_vc152.py --output ftp-share/VC152D

Use a new suffix for each attempt (`VC152E`, etc.) and preserve prior return
folders. The exporter produces flat DOS 8.3 filenames, ASCII text, and CRLF
line endings. Do not rename package files or add long filenames.

The package exercises eleven reconstructed utility functions using retained
original-x86 cases. It also checks the recovered storage headers and field
offsets. It is a focused native compatibility probe, not a complete rebuilt
game.

## Transferring the package

Create the matching directory in the guest through the controller:

    mkdir C:\VC152D

Then issue one `put` command per package file, for example:

    put ftp-share/VC152D/PROBE.C C:\VC152D\PROBE.C
    put ftp-share/VC152D/BUILD.BAT C:\VC152D\BUILD.BAT
    put ftp-share/VC152D/PROBE.RSP C:\VC152D\PROBE.RSP

Repeat this for every file in the generated package, including all headers,
response files, definitions, `VECTEST.C`, and `UTILGEN.C`. The controller's
`put` operation reports the byte count after each successful transfer.

## Important Win98 process and batch rules

The agent launches processes with its own inherited working directory. A bare
command such as:

    run C:\VC152D\BUILD.BAT C:\MSVC 300

can exit successfully without producing results in `C:\VC152D`, because the
batch file runs from the agent directory. Always run through a wrapper that
changes directory first:

    @echo off
    cd C:\VC152D
    call BUILD.BAT C:\MSVC

The wrapper must be a DOS CRLF file. Win98 `COMMAND.COM` can misparse an LF-only
wrapper; if a host-created wrapper is used, convert it to CRLF before `put`.
The same rule applies to wrappers used to launch the executables:

    @echo off
    cd C:\VC152D
    PROBE.EXE

and:

    @echo off
    cd C:\VC152D
    LAYOUT.EXE

The agent returns process status, not the child's console output. Use the build
logs and report files for evidence. The probe and layout programs display a
message box, so a successful run must also dismiss that box remotely.

## Build procedure

1. Transfer a CRLF wrapper to the guest, for example `C:\RUNVC.BAT`.
2. Run it:

       run command.com /c C:\RUNVC.BAT 300

   The controller returns `RUNSTART <PID>` immediately.
3. Wait for that PID:

       wait <PID> 360

   A normal build should finish with `PROC <PID> EXIT 0`.
4. Retrieve at least:

       get C:\VC152D\BLDSTAT.TXT runs/<run-id>/BLDSTAT.TXT
       get C:\VC152D\BUILD.LOG runs/<run-id>/BUILD.LOG
       get C:\VC152D\LINK.LOG runs/<run-id>/LINK.LOG
       get C:\VC152D\LAYOUT.LOG runs/<run-id>/LAYOUT.LOG
       get C:\VC152D\LAYLINK.LOG runs/<run-id>/LAYLINK.LOG

Expected build status:

    COMPILE=PASS
    LINK=PASS
    LAYOUT=PASS
    LAYLINK=PASS

Keep failed logs too. The first compiler or linker error is valuable evidence.

## Running the native probes

For each executable, use a CRLF wrapper that changes to `C:\VC152D`, then:

    run command.com /c C:\RUNPROBE.BAT 300
    windows

Find the window titled `Stars compatibility: UTILITY`, then focus and dismiss
it. The exact handle changes per run:

    focus <HWND>
    key 13
    wait <PID> 60
    get C:\VC152D\RESULTS.TXT runs/<run-id>/RESULTS.TXT

Repeat for `RUNLAYOUT.BAT`, looking for `Stars compatibility: LAYOUT`, and
retrieve:

    get C:\VC152D\LAYRES.TXT runs/<run-id>/LAYRES.TXT

The expected native reports are:

    RESULTS.TXT: CHECKS=563, FAILURES=0, RESULT=PASS
    LAYRES.TXT:  CHECKS=1491, FAILURES=0, RESULT=PASS

They should also say `MODE=WIN16_MEDIUM`, `_MSC_VER=800`, `sizeof(int)=2`,
`sizeof(long)=4`, and `sizeof(void*)=2`.

Optional visual evidence can be collected with:

    screenshot runs/<run-id>/guest-screen.bmp

## Evidence and cleanup

- Use a unique host directory such as `runs/vmctl-vc152d/` for every run.
- Preserve reports, build logs, linker maps, object files, executables, and the
  agent log before interpreting results. Do not overwrite an earlier return.
- A report is not a pass unless both the build status and the executable report
  show success. A process exit code alone is insufficient.
- The VM agent can be left running for follow-up work. If it is intentionally
  stopped, restart it with `C:\VMCTL1\STARTAGT.BAT` before the next request.
- Stop the host listener after the session; do not use `quit` unless stopping
  the guest agent is intended. Normal Windows shutdown is safer than 86Box
  Hard reset or Force shutdown while files are being written.

## Scope and limitations

This workflow is sufficient for native VC1.52c compilation, Win16 linking,
ABI checks, reconstructed utility probes, generated-layout checks, and focused
Win16 harnesses. It does not by itself establish whole-game compatibility,
original compiler flags, complete CRT/data compatibility, or faithful behavior
of every Win16 USER/GDI interaction. Larger or UI-heavy experiments should
retain their own explicit harness, expected outputs, and return checklist.

For the validated reference run, see the native reports in
`runs/vmctl-vc152d/` and the broader fixture description in
`compat/vc152/README.md`.
