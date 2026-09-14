/* DOS-only entry, original file 0200h..0262h. The Windows NE loader uses
 * a separate entry. The near CALL puts the inline message address on the
 * stack; DOS function 09h prints up to '$', then 4Ch exits with status 1.
 * GNU/LLVM assembler syntax; no compiler-library or original binary copied. */
.code16
.text
.globl stars_dos_entry
stars_dos_entry:
    callw dos_print_and_exit
    .ascii "This program requires Microsoft Windows.\r\n$"
    .fill 0x56 - (. - stars_dos_entry), 1, 0x20
dos_print_and_exit:
    popw %dx
    pushw %cs
    popw %ds
    movb $9, %ah
    int $0x21
    movw $0x4c01, %ax
    int $0x21
