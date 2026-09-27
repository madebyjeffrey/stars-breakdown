# DrawBuildSelComp reconstruction

`DrawBuildSelComp` renders the selected ship component summary at NE
`001a:3ab2-42e6` (2,100 bytes). The reviewed source is
[`reconstructed/src/draw_build_sel_comp.c`](../../reconstructed/src/draw_build_sel_comp.c).

The C reconstruction preserves DC ownership and restoration, the build-area
rectangle and starbase vertical offset, the empty-selection and dialog-derived
selection branches, description-only components, full component headers,
three scaled mineral-cost rows, the non-starbase mass row, and all early-return
cleanup paths. Component tables, localized text construction, and GDI text
operations are typed callbacks rather than byte replay or assembly output.

Validation:

- host C89 probe: `BUILD_SEL_COMP_CHECKS=3 FAILURES=0 RESULT=PASS`;
- native VC1.52c compile/link: `runs/vmctl-vc152ex/build-sel-comp/BCSTA.TXT`;
- native Win98 execution: `runs/vmctl-vc152ex/build-sel-comp/BCR.TXT`, with
  `BUILD_SEL_COMP_CHECKS=3 FAILURES=0 RESULT=PASS`.
