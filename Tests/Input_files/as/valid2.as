MAIN:       mov @r4, DATA
            cmp @r5, @r6
            bne LABEL1
            .string "Test String"
            .data 8, -1, 3, 5
LABEL1:     sub @r3, VALUE
            jsr FUNC
            .entry FUNC
            jmp EXIT
DATA:       .data 100
VALUE:      .data 1
FUNC:       rts
EXIT:       stop
.extern VALUE
