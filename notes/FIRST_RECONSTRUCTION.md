# First reconstruction: GetRaceStat

This is a manual reconstruction from the supplied executable, not recovered original source text. The function name, parameter names, parameter types, member name and source location come from its CodeView records. The expression is reconstructed from machine instructions.

- Original location: `c:\user\jeffl\2.7j\race.c`, first mapped line 1501.
- NE address: `001d:30d2`; file offset: `0xcbfd2` (835538).
- Function length: 34 bytes.
- Debug return type: 16-bit `int`; parameters: near pointer to `_player`, 16-bit `int`.
- `_player.rgAttr` starts at byte 62 (`0x3e`), and is a 16-byte array of `char`.

```c
/* Semantic reconstruction for the original 16-bit memory model. */
int __far GetRaceStat(struct _player __near *pplr, int iStat)
{
    return (signed char)pplr->rgAttr[iStat];
}
```

The explicit cast documents the sign extension visible in the instructions. It does not claim that the original author wrote a cast. Far C calling convention comes from procedure type 0x11f8 (calling-convention value 1), the far return, and stack layout. The original typedef spelling is not asserted here.

## Instruction evidence

| Offset within function | Bytes | 16-bit instruction | Meaning |
|---:|---|---|---|
| 0x00 | `55` | `push bp` | Save caller frame |
| 0x01 | `8b ec` | `mov bp, sp` | Establish stack frame |
| 0x03 | `81 ec 02 00` | `sub sp, 2` | Reserve two stack bytes |
| 0x07 | `56` | `push si` | Save SI |
| 0x08 | `57` | `push di` | Save DI |
| 0x09 | `b8 3e 00` | `mov ax, 0x3e` | Offset of `rgAttr` |
| 0x0c | `8b 5e 06` | `mov bx, [bp+6]` | Load `pplr` |
| 0x0f | `03 d8` | `add bx, ax` | Address attribute array |
| 0x11 | `8b 46 08` | `mov ax, [bp+8]` | Load `iStat` |
| 0x14 | `03 d8` | `add bx, ax` | Index one-byte elements |
| 0x16 | `8a 07` | `mov al, [bx]` | Read attribute through DS |
| 0x18 | `98` | `cbw` | Sign-extend AL into AX |
| 0x19 | `e9 00 00` | `jmp next_instruction` | Redundant jump into epilogue |
| 0x1c | `5f` | `pop di` | Restore DI |
| 0x1d | `5e` | `pop si` | Restore SI |
| 0x1e | `8b e5` | `mov sp, bp` | Discard frame |
| 0x20 | `5d` | `pop bp` | Restore BP |
| 0x21 | `cb` | `retf` | Return across code segments |

## What this teaches

A near data pointer occupies two bytes and relies on the current data segment. A far function call stores both return offset and code segment, so after saving BP the first argument is at `[bp+6]`. Code and data therefore need different pointer assumptions even within this tiny function.

Race attributes occupy one byte each, then are promoted to signed 16-bit values when read. This makes negative attributes possible without paying two bytes per stored attribute. There is no bounds check in this function; the caller must supply a valid index.

The saved-but-unused SI/DI registers, unused stack reservation, and redundant jump suggest little optimization in this function. They do not prove a particular project-wide compiler switch.

This function has no relocation-bearing operands. Larger functions need NE relocation processing before interpreting far calls, imported APIs, or embedded addresses: the bytes on disk can contain loader fixups rather than final destinations.
