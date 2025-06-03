.entry COUNT
.extern EXT_SYM
START:      mov @r1, COUNT
            add @r2, @r3
            sub @r3, EXT_SYM
            bne LOOP
            .data 5, -7, 12
LOOP:       prn STR
            lea STR, @r6
            inc TOTAL
            jmp END
STR:        .string "hello, assembler"
COUNT:      .data 10, 20, 30
TOTAL:      .data 0
            .entry LOOP
            .extern END
END:        stop
