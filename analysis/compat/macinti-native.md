# Macinti AI-turn native probe

Reviewed 2026-09-17 in the running `StarDev` 86Box / Windows 98 guest.

The fresh package in `compat/vc152/macinti/` compiled the current Macinti
AI-turn orchestration slice with Microsoft Visual C++ 1.52c in the medium
memory model. The package used a slim native header containing the exact
adapter declarations and the source logic from
`reconstructed/src/do_macinti_ai_turn.c`; the source differs only in header
name, comments, formatting, and DOS line endings.

## Native result

| Check | Result |
|---|---|
| Compile | PASS |
| Link | PASS |
| Executable | Win16 NE executable produced and run |
| Probe | `MACINTI_NATIVE_CHECKS=29 FAILURES=0 RESULT=PASS` |
| Compiler/linker | VC++ 1.52c / linker 5.60.339 |

The probe covers the recovered prefix's starter handling, merge-mask order,
turn-dependent design budgets, late-turn split masks, potency values, and
callback sequencing. Host C89 and native results agree.

Returned artifacts are preserved in `runs/vmctl-vc152macinti/`, including the
status file, compile/link logs, executable, map, object files, and native
report. The executable SHA-256 is
`943be0aefdf801955eb3c153b89696e2aa8655a9e2652c31be13d537a4e870c7`.

## Scope limit

This does not complete or promote `DoMacintiAiTurn`. The later planet-
production policy, fleet retargeting/order mutation, colonizer and transport
routing, and full AI-table integration remain partial. It closes the native
compile/link/probe uncertainty for the recovered orchestration slice only.

The next native check should exercise the typed planet/fleet callbacks and
their state mutations in the same package, followed by completion of the
remaining Macinti production and fleet-routing body before any ledger
promotion is considered.

## Typed follow-up

The typed follow-up was run in a separate fresh guest directory,
`C:\\MACINTI2`, using the same native source and a conditional typed probe.
It compiled and linked with VC++ 1.52c and reported:

```text
MACINTI_TYPED_CHECKS=43 FAILURES=0 RESULT=PASS
```

The added checks covered two planet records, including mineral scoring,
ownership and production-candidate selection, plus two fleet records, including
owner filtering, ship-class totals, and order-repair recommendation. The
returned artifacts are preserved in `runs/vmctl-vc152macinti-typed/`; the
typed executable SHA-256 is
`1db4cc50b19182bf55665004e14f74ec1a2b6aa395cb10de454c38626cb2140a`.

## Typed production-gate follow-up

A second fresh typed directory, `C:\MACINTI3`, compiled the same native slice
after adding the bounded next-hull production gate. It checks the raw
queued-cost-plus-hull-cost subtraction, accepts exact-zero resources, rejects a
negative remainder, and suppresses the production-selection callback for the
rejected batch. The returned report was:

```text
MACINTI_TYPED_CHECKS=45 FAILURES=0 RESULT=PASS
```

The compile/link logs, map, executable, objects, and report are preserved in
`runs/vmctl-vc152macinti-typed-batch/`; the executable SHA-256 is
`2ccc3339deb7da8c30b5c044a86cd1149b1e3f5733e3217bc8e9a9471ed0e357`.

## Fleet-action follow-up

A third fresh typed directory, `C:\MACINTI5`, compiled the fleet plan/apply
boundary. It checks friendly-fleet filtering and dispatches an explicit attack
action through the native apply callback. The returned report was:

```text
MACINTI_TYPED_CHECKS=52 FAILURES=0 RESULT=PASS
```

The compile/link logs, map, executable, objects, and report are preserved in
`runs/vmctl-vc152macinti-fleet-actions/`; the executable SHA-256 is
`fbe9ed3b17a21fdfb1fa6da2af1c2d63c24ec1cdf31fcc48d5667aebd184b702`.

## Action-binding follow-up

A fourth fresh typed directory, `C:\MACINTI6`, compiled the named
action-specific callback bindings, including the direct attack binding with the
generic apply path retained as fallback. The returned report was:

```text
MACINTI_TYPED_CHECKS=52 FAILURES=0 RESULT=PASS
```

The compile/link logs, map, executable, objects, and report are preserved in
`runs/vmctl-vc152macinti-action-bindings/`; the executable SHA-256 is
`83fdbbfce4123b1f3f2b1fd1b873483c2489863e5b40eef14e9126d325f02179`.

## Final-phase follow-up

A fifth fresh typed directory, `C:\MACINTI7`, compiled the ordered
transport-routing, fleet-finalization, basic-task, and production-queue
callbacks after the main fleet scan. The returned report was:

```text
MACINTI_TYPED_CHECKS=52 FAILURES=0 RESULT=PASS
```

The compile/link logs, map, executable, objects, and report are preserved in
`runs/vmctl-vc152macinti-final-phases/`; the executable SHA-256 is
`91017b5d2fa6de516ec32527ab96c65e944a164235852f6cfa2af5b7dc873cd4`.

## Raw-view follow-up

A sixth fresh typed directory, `C:\MACINTI8`, compiled the optional raw-record
and coordinate views carried by the typed planet and fleet records. The
existing typed callbacks continued to pass under VC++ 1.52c:

```text
MACINTI_TYPED_CHECKS=52 FAILURES=0 RESULT=PASS
```

The compile/link logs, map, executable, objects, and report are preserved in
`runs/vmctl-vc152macinti-raw-views/`; the executable SHA-256 is
`11b786d467e124df0055e4d764458cfffbd29538b20cbe6e6b28a9fb47415ec1`.

Native validation is now complete for the currently recovered Macinti slice.
The next work is source completion: reconstruct the remaining production,
fleet-routing, colonizer/transport, and order-mutation phases, then extend the
native probe to compare those mutations against original expectations.
