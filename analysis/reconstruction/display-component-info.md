# DisplayComponentInfo reconstruction

`DisplayComponentInfo` (`001c:2ac6-65ad`, 15,079 bytes) is a component-report
renderer from `research.c`.  The original body combines a packed `_part`
record, component-specific `_armor`/`_hul` records, resource strings, and
Win16 GDI calls.  The reconstruction in
`reconstructed/src/display_component_info.c` separates those concerns into a
typed `StarsDisplayComponentPart` and `StarsDisplayComponentOps` service
boundary.

The source preserves the report phases: frame and empty-component handling,
title and component image, mineral and mass rows, development-cost formatting,
and category-specific detail panels for armor, engines, shields, scanners, and
the remaining weapon/special families.  The callbacks retain ownership of
handles, resource tables, text metrics, and graph drawing, which avoids
reintroducing the raw segmented pointers and magic stack offsets from the
decompiler output.

Validation:

- Host C89 compilation with warnings treated as errors passed.
- The source probe exercised the populated engine path, cost formatting,
  frame callbacks, and category detail dispatch.
- The same source and probe compiled and linked under VC++ 1.52c in the
  StarDev Win98 guest.  The native report is
  `runs/vmctl-vc152ex/dcomponent/DCOMPRES.TXT` and the build status is
  `runs/vmctl-vc152ex/dcomponent/DCOMPSTA3.TXT`.

This is a semantic C reconstruction, not a claim that the callback adapter
itself reproduces the original executable's instruction bytes.  Exact-byte
fixtures remain tracked separately from reviewed source.
