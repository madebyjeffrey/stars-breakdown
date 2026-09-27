# ICompReport

`ICompReport` is the shared qsort comparator for the planet, fleet, and
battle report views.  The original body is large because it resolves several
different 16-bit far-pointer record layouts and calls the name, production,
survival, battle, and mineral services before comparing the selected column.

The reconstruction makes that boundary explicit.  A report-specific adapter
normalizes one row into signed values, optional text, validity, and a stable
secondary value; `stars_i_comp_report` then applies the common ordering rules:
missing rows sort before present rows, text comparisons are normalized to a
sign, descending columns invert the result, and a previous sort column is used
as the one-level tie break.  No packed executable bytes or assembly are used
as the implementation.

The source probe covers numeric ascending/descending order, previous-column
ties, text order, and missing-row order.  The report-specific record adapters
remain the integration boundary for the original planet/fleet/battle data.
