; Label Test

.data 5, 10, 15

.string "Label Test"

START:  mov r2, r3
        add r3, r4
        sub r4, LABEL1
        jmp LABEL2

LABEL1: lea r5, LABEL3
        prn r5

.extern EXTERN_LABEL

LABEL2: jsr EXTERN_LABEL
        bne LABEL3

LABEL3: clr r6
        stop

