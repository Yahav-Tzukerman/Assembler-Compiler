; Complex Test

.data 1, 2, 3, 4, 5

.string "Complex Test"

START:  mov r3, #5
        add r3, r4
        sub r3, MY_LABEL
        not r3
        clr r5
        inc r6
        dec r7
        jmp MIDDLE
        bne END

EXTERN: .extern EXTERNAL_LABEL

MIDDLE: lea r1, END
        red r1
        prn r2
        jsr EXTERNAL_LABEL

END:    rts

