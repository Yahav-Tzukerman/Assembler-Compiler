; Simple Test

.data 5, -10, 20

.string "Hello, World!"

START:  mov r1, r2
        add r3, r4
        sub r5, r6
        cmp r7, r0
        jmp END

EXTERN: .extern MY_LABEL

MY_LABEL: lea r1, START

END:    stop
