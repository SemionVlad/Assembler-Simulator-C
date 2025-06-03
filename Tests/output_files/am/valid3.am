.extern EXT
.entry MAIN
MAIN:       lea EXT, @r2
            prn -9
            add @r1, EXT
            sub COUNT, @r3
            .data -1, 2, 3
            .string "Another string"
            cmp @r4, COUNT
COUNT:      .data 4, 5, 6
            bne LOOP
LOOP:       inc MAIN
            dec COUNT
            jmp MAIN
END:        stop
