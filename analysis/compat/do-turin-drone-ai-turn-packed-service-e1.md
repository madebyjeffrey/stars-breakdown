# DoTurinDroneAiTurn E1 packed-record and potency-service bridge

The original support decision uses the 56-byte `_planet`, 124-byte `_fleet`,
the variable-length four-byte `PLPROD` header/items, and
`FPotentRobWarFleet(fleet,2)`. Original `1088:31BC` sums ship slots 2–5
once and slots 6–7 twice, then compares the 16-bit result with
`vrgAiArmadaPotency[0]` for potency 2. The Turin source now has a default
instruction-derived implementation of that decision when no external
potent-fleet callback is supplied.

Production-source decoders now read the recovered planet id/owner/visibility/
mineral fields, fleet id/owner/planet/order count/signed 16-bit ship slots,
and queued-production items. The queue scan requires a complete record and
recognizes category 2 with a design index below 16, matching original raw
lines 233–251. It does not infer production resources, queue costs, or
desirability from nonexistent packed offsets; those remain service results.

The strict-C89 probe checks record-size rejection, exact fields including
ship count 256, queue category/design and truncation boundaries, and an
end-to-end Turin turn from decoded planet/fleet data through the default
potency decision, full four-argument support queue, and
`FinishProduction(changed)`. Host and fresh VC++ 1.52c/Win98 `C:\TURPK1`
both report `TURIN_PACKED_SERVICE_CHECKS=17 FAILURES=0 RESULT=PASS`.
Native compile/link and process exit pass; the post-run window list has no
Program Error. Package: `compat/vc152/turin-packed-service-e1/`; build
logs, map, object, executable, report, and agent log:
`runs/vmctl-turpk1/`. The full host suite passes 539 tests (153 expected
skips), 2,241 layout assertions, and 846 declarations.

This is supporting E1 evidence, not a live original-turn differential.
Actual VM memory table enumeration and service-call side effects, the
preceding growth/resource services, all production-table design inputs,
and original output comparison remain. E2–E4 and promotion are separate.
