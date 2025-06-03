.entry TEXT
.extern EXT_LABEL
MAIN:       lea TEXT, @r0
            mov @r1, @r2
            bne LABEL
            .data 1, 2, 3
TEXT:       .string "Final Test"
LABEL:      prn -4
            add EXT_LABEL, @r3
            sub @r6, @r7
            jmp MAIN
            .entry MAIN
            .entry LABEL
            .extern FINAL
FINAL:      stop
