; Macro Test

; Define a simple macro to load a constant value into a register
macro LOAD_CONST reg, value
    mov reg, value
endmacr

; Define a macro to perform a sequence of operations
macro SEQUENCE reg1, reg2
    add reg1, reg2
    sub reg1, reg2
    not reg1
endmacr

.data 1, 2, 3, 4

.string "Macro Test"

; Use the LOAD_CONST macro to load values into registers
START:  LOAD_CONST r1, #10
        LOAD_CONST r2, #20

; Use the SEQUENCE macro to perform operations on registers
        SEQUENCE r1, r2

        jmp END

; Define another macro that uses a label
macro LOOP label, count
    mov r3, count
label: dec r3
       bne label
endmacr

; Use the LOOP macro
        LOOP LOOP_LABEL, #5

END:    stop

; External label definition
.extern EXTERN_LABEL

; Another macro definition that interacts with labels
macro CALL_EXTERN
    jsr EXTERN_LABEL
endmacr

; Use the CALL_EXTERN macro
        CALL_EXTERN

