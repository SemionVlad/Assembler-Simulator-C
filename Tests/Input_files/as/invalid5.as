STR: .string "" ; valid empty
STR2: .string "OK"
STR3: .string "Invalid escape\\q"
STR4: .string abcdef ; missing quotes

DATA1: .data 1,,2,,3
DATA2: .data -40000 ; out of range
DATA3: .data +

JMP: jmp
BNE: bne bne bne ; repeated

MIX: mov @r1, , @r2

END: stop
