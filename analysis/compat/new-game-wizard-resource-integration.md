# NewGameWizard resource and geometry checkpoint

This checkpoint closes E2 for `NewGameWizard`: the three dialog resources
used by the original wizard are represented as a typed contract and checked
against the recovered NE resource layout.

The contract covers resources `0x186`, `0x187`, and `0x188`. Each is 261 by
210 dialog units with an 8-point font. It checks all 45 controls across the
three pages: control IDs, exact rectangles, resource-text lengths, and
resource-text checksums. The page counts are 27, 5, and 13 controls.

The strict host C89 probe reports 366/366 checks. The fresh VC++ 1.52c/Win98
run in `runs/vmctl-vc152nwres/` reports compile/link success and
`NEW_GAME_WIZARD_RESOURCE_E2_CHECKS=366 FAILURES=0 RESULT=PASS`; the post-run
window inventory showed no Program Error dialog. The source package and
CRLF wrappers are preserved under
`compat/vc152/new-game-wizard-resource/`.

Together with the packed-player E1 packet and the shared creation E3 packet,
this left no numbered Step-5 evidence units for `NewGameWizard`; the procedure
has since been promoted to 6/6. The actual `GenerateWorld` implementation
remains a separate procedure and is not implicitly claimed by this resource
acceptance packet.
