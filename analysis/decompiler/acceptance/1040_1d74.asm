1040:1d74  55  PUSH BP
; c:\user\jeffl\2.7j\utilgen.c:266 | 
1040:1d75  8bec  MOV BP,SP
1040:1d77  81ec0200  SUB SP,0x2
1040:1d7b  56  PUSH SI
1040:1d7c  57  PUSH DI
1040:1d7d  8b5e06  MOV BX,word ptr [BP + 0x6]
; c:\user\jeffl\2.7j\utilgen.c:267 | 
1040:1d80  8b07  MOV AX,word ptr [BX]
1040:1d82  8b5702  MOV DX,word ptr [BX + 0x2]
1040:1d85  8b5e08  MOV BX,word ptr [BP + 0x8]
1040:1d88  2b07  SUB AX,word ptr [BX]
1040:1d8a  1b5702  SBB DX,word ptr [BX + 0x2]
1040:1d8d  e90000  JMP 0x1040:1d90
1040:1d90  5f  POP DI
; c:\user\jeffl\2.7j\utilgen.c:268 | 
1040:1d91  5e  POP SI
1040:1d92  8be5  MOV SP,BP
1040:1d94  5d  POP BP
1040:1d95  cb  RETF
