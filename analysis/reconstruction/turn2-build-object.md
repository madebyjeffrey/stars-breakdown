# FBuildObject

`FBuildObject` is the turn-phase dispatcher for production actions selected
against a planet or fleet.  The reconstruction in
`reconstructed/src/f_build_object.c` separates the original packed-record
updates into five readable policies: ship and starbase construction,
installation limits and notifications, terraforming, packet creation/update,
and special planetary actions.

The public input uses a typed planet view.  Allocation, fleet storage, packet
list ownership, and message-list ownership remain callbacks because those are
shared game structures in the original executable.  This keeps the source
portable while preserving the observable decisions: quantity clamping,
starbase/design gating, installation message singular/plural selection,
terraform bounds, packet capacity/warp failures, packet strength scaling, and
special-action dispatch.

Validation covered installation quantity limiting, terraforming, ship and
starbase dispatch, and special-action dispatch in the host source probe.  The
same probe compiled and linked with VC++ 1.52c in StarDev and passed natively.
