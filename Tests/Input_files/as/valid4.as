.entry LOOP
.extern X
.extern Y
START:      mov X, Y
            cmp @r2, @r5
            prn -6
            sub Y, @r1
            .data 0, 7, -7
            .string "Assembler Test"
LOOP:       jmp L1
L1:         bne END
            inc TOTAL
TOTAL:      .data 9
            .entry END
END:        stop
