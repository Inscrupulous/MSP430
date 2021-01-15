;-------------------------------------------------------------------------------
; File: SW_Mult.asm (CPE 325 Lab 5)
; Function: Uses Software multiplier to multiply numbers
; Input: base and base^n-1
; Output: base^n
; Author: Tyler Phillips
; Date: 09/22/20
;-------------------------------------------------------------------------------

			.cdecls C,LIST,"msp430.h" ; Include device header file

			.def sw_mult
			.text

sw_mult:	mov.w	#8, R8 		;set counter to 8 bits
			mov.w	10(SP), R10	; R10 holds A
			clr		R9			; R9 holds C
			mov.w	2(SP), R7	; R7 holds B

loop:		mov		R7, R11
			rrc		R11			; rotate right once to get the LSB to carry bit
			jc 		ladd

shift:		clrc				;clear carry
			rlc		R10			;perform left rotate
			clrc				;clear carry
			rrc		R7			;perform right rotate
			dec		R8			;R8-1
			jnz		loop		;if != 0 jump to loop
			ret

ladd:		add.w 	R10, R9		;add A and C
			jmp		shift
			nop


			.end

