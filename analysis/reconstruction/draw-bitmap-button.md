# DrawBitmapButton reconstruction

`reconstructed/src/draw_bitmap_button.c` is a typed semantic reconstruction of bitmap-button drawing, preserving button geometry, state, bitmap selection, and drawing callbacks. The host probe links the shared `tb.c` geometry helper.

Validation: `tests/draw_bitmap_button_source_probe.c` plus `reconstructed/src/tb.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DBBSTA.TXT` and `runs/vmctl-vc152ew/DBBRES.TXT`.
