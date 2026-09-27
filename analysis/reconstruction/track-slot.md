# FTrackSlot reconstruction

`reconstructed/src/track_slot.c` is a typed C reconstruction of the slot
interaction routine at NE `001a:306a–3ab1`.  It preserves the original
decision order while moving USER/GDI tracking into explicit callbacks:

- spin-button hit testing cycles the hull bitmap and returns handled;
- grid hits distinguish empty slots from populated parts before selection;
- list-box parts retain the three original popup gates and message identifiers;
- populated left-clicks enter build-mode drag/drop only in mode 4;
- right-clicks select the part and dispatch inspection;
- misses and non-drag build modes return unhandled.

The host C89 probe covers spin, empty-slot selection, build-mode drag, the
empty-part popup, and right-click inspection.  The same source and probe were
compiled and linked by VC1.52c in Win98 and the native report passed.
