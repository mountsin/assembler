MAIN: mov LENGTH, r1
lea STR[%LENGTH], r4
LOOP: jmp END
prn [K]STR[r3]
sub #1, r1
inc r0
mov r3,STR[%K]
bne LOOP
END: stop
STR: .string “abcdef”
LENGTH: .data 6
K: .data 2