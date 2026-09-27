# DrawBuildSelHull reconstruction

`reconstructed/src/draw_build_sel_hull.c` is a typed C reconstruction of the
3,771-byte hull-summary renderer at NE `001a:451e–53d9`.  It retains the
original presentation decisions while exposing drawing and DC ownership
through callbacks:

- mode 3 exits without touching the window;
- a null DC is acquired, the background region is filled, and the DC is
  restored/released on every owned path;
- normal hull mode emits mineral costs, weight, armor/shield, power, cloak and
  jammer, speed, and scanner rows with the starbase discounts and race gate;
- VCR mode emits remaining shield after damage;
- the optional popup-distance row is emitted only when the popup state is
  active and has a nonzero maximum/value.

The host C89 probe covers DC lifecycle, normal rows, starbase cost discount,
the mode-3 early return, and popup output.  The same package compiled and
linked with VC1.52c in Win98 and its native probe passed.
