; maman14 expample macro 1

MAIN:	add r3, LIST
LOOP:	prn #48
	macr yahav1
	add #6, r3 
	cmp r3, #-6 
	bne END 
	endmacr

	macr yahav2
	add #6, r3 
	cmp r3, #-6 
	bne END 
	endmacr
	lea STR, r6 
	inc r6
	mov *r6,K 
	sub r1, r4 
	yahav1
	yahav2
	dec K
	jmp LOOP
END:	stop
STR:	.string “abcd” 
LIST:   .data 6, -9
	.data -100 
K:	.data 31
