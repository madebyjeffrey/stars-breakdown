10e0:30d2  55  PUSH BP
; c:\user\jeffl\2.7j\race.c:1501 | 
10e0:30d3  8bec  MOV BP,SP
10e0:30d5  81ec0200  SUB SP,0x2
10e0:30d9  56  PUSH SI
10e0:30da  57  PUSH DI
10e0:30db  b83e00  MOV AX,0x3e
; c:\user\jeffl\2.7j\race.c:1504 | 
10e0:30de  8b5e06  MOV BX,word ptr [BP + 0x6]
10e0:30e1  03d8  ADD BX,AX
10e0:30e3  8b4608  MOV AX,word ptr [BP + 0x8]
10e0:30e6  03d8  ADD BX,AX
10e0:30e8  8a07  MOV AL,byte ptr [BX]
10e0:30ea  98  CBW
10e0:30eb  e90000  JMP 0x10e0:30ee
10e0:30ee  5f  POP DI
; c:\user\jeffl\2.7j\race.c:1505 | 
10e0:30ef  5e  POP SI
10e0:30f0  8be5  MOV SP,BP
10e0:30f2  5d  POP BP
10e0:30f3  cb  RETF
