START: lea 123, @r1
prn @r9 ; invalid register
.string "Missing label"
.data 100, 200, three
.extern EXT1
.entry START
.entry EXT1 ; extern marked as entry

L: add @r1, @r2, @r3 ; too many operands
START: lea 123             @r1                      invalid register
prn @r9
.string "Missing label"
.data 100                 200                      three
.extern EXT1
.entry START
.entry EXT1                                         extern marked as entry
