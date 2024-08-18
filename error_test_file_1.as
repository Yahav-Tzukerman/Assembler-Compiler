; Assembler test file
    mov #-1

    .entry Yahav
    .extern LABEL2
    X: mov r3, LABEL
    LABEL: .string "Abcd
    .string "SHLOB"

    macr yahav1
    add #6, r3
    cmp r3, #-6
    inc r3
    endmacr

    .data 6, -9
    yahav1
    Yahav: .data +7
    LABEL3: mov Yahav ,LABEL2
    .data 8,9,10

shlibity shlobity
    yahav1: .string "SHLOB"

    rts
    stop

    dec X, X
    dec

    dec *r1, r1ss

    dec Yahav











