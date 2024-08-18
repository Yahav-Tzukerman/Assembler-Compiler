; Error Test

.data 10, -20, 30

.string "Error Test"

START:  mov r8, r1 ; Invalid register
        add r2, #INVALID ; Invalid immediate value
        sub r3, MISSING_LABEL ; Missing label
        jmp MISSING_LABEL
        invalid_op r4, r5 ; Invalid operation

END:    stop

