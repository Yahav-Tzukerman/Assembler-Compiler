    .entry WRONG
    MAIN: add r3, LIST
    LOOP: prn #48
    lea STR, WRONG
    WRONG: inc r6
    mov *r6, K
    mov K, XXX
    sub r1, r4
    cmp r3, #-6
    dec LABEL2

    bne r3
    dec K
    jmp *r7
    END: stop
    STR: .string "abcd"
    LIST: .data 6, -9
    .data -100
    K: .data 31
    ENTRY: .entry K
    .entry LIST
    .extern XXX