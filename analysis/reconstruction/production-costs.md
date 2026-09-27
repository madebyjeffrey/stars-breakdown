# Production-cost reconstruction

`GetProductionCosts` reads the packed production item as three independent
fields: the item kind, the production-class selector, and the requested item
quantity. The reconstruction keeps those fields named and uses callbacks for
the original race tables, part lookup, technology-adjusted part costs, and
hull tables.

The special production rows preserve the original mineral/resource lanes,
race ability gates, technology reductions, true-part lookup paths, and the
global quantity multiplier. Hull rows retain hidden-hull rejection, ordinary
hull costs, special-design half-cost rounding, and the starbase replacement
adjustment. The latter compares old and new component identities and
technology levels, applying the original replacement formulas before the
special-design reduction.

Host C89 checks cover resource, true-part technology scaling, ordinary hull,
hidden-hull rejection, special-design rounding, and quantity scaling cases.
The VC++ 1.52c Win98 probe compiled warning-free, linked, and executed with
zero failures in the StarDev guest.
