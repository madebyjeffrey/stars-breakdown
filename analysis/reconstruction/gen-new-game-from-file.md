# GenNewGameFromFile

`GenNewGameFromFile` parses the text game-definition format used by the new
game wizard.  `reconstructed/src/gen_new_game_from_file.c` keeps the parser
and its validation rules in ordinary C: leading-space handling, bounded game
settings, seven option flags, player/race records, eight victory-condition
records, lowercase `.xy` base-name removal, and the final name/bitmap repair
passes are explicit.

The typed callback boundary represents race-file loading, computer-race
selection, advantage-point recovery, localized player names, error reporting,
and `GenerateWorld`.  Those operations own global records outside the parser;
the parser passes the decoded state to them rather than replaying the original
machine bytes.

The source probe checks a valid two-player file, map/settings decoding, option
bits, race and computer-player callbacks, victory encoding, `.xy` stripping,
advantage recovery, and world-generation dispatch.  The same probe passed
after compilation and linking with VC++ 1.52c in the Win98 StarDev guest.
