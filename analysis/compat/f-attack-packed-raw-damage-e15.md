# FAttack packed raw-damage integration — E15

This host checkpoint extends the E14 raw stateful-damage probe through the
packed attacker-part boundary. The probe supplies the original four-byte
`_hs` slot (`grhst=0x0010`, item 7, count 2), resolves that slot through the
production lookup callback, and then runs the resolved complex-beam section
through raw capacity, beam-defense, distance/part-DP attenuation, and the
stateful `FDamageTok` callback.

The strict C89 host run reports
`F_ATTACK_RAW_DAMAGE_CHECKS=7 FAILURES=0 RESULT=PASS`. It checks packed slot
forwarding, caller-section preservation, the 39-point scaled beam, shield and
hull record order, casualty mutation, and the FAttack hit/battle-record
fields.

This is a stronger supporting boundary than E14, but it is not yet a named
Step-5 closure: the fresh VC++ 1.52c replay is pending the Win98 agent
reconnection, and the broader original damage differential/full battle replay
remain open. Before transfer, the packet's 41 DOS text files were normalized
to ASCII CRLF line endings; this is required by `COMMAND.COM` and the VC1.52c
response/linker files and removes a packet-format variable from the native
replay.
