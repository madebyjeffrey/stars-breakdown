1040:1a7c  55  PUSH BP
; c:\user\jeffl\2.7j\utilgen.c:189 | 
1040:1a7d  8bec  MOV BP,SP
1040:1a7f  81ec0200  SUB SP,0x2
1040:1a83  56  PUSH SI
1040:1a84  57  PUSH DI
1040:1a85  8b4606  MOV AX,word ptr [BP + 0x6]
; c:\user\jeffl\2.7j\utilgen.c:190 | 
1040:1a88  8b5608  MOV DX,word ptr [BP + 0x8]
1040:1a8b  a37222  MOV [0x2272],AX
1040:1a8e  89167422  MOV word ptr [0x2274],DX
1040:1a92  8b460a  MOV AX,word ptr [BP + 0xa]
; c:\user\jeffl\2.7j\utilgen.c:191 | 
1040:1a95  8b560c  MOV DX,word ptr [BP + 0xc]
1040:1a98  a37622  MOV [0x2276],AX
1040:1a9b  89167822  MOV word ptr [0x2278],DX
1040:1a9f  5f  POP DI
; c:\user\jeffl\2.7j\utilgen.c:192 | 
1040:1aa0  5e  POP SI
1040:1aa1  8be5  MOV SP,BP
1040:1aa3  5d  POP BP
1040:1aa4  cb  RETF
