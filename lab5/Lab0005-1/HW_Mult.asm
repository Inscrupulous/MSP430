;-------------------------------------------------------------------------------
; File: HW_Mult.asm (CPE 325 Lab 5)
; Function: Uses Hardware multiplier to multiply numbers
; Input: base and base^n-1
; Output: base^n
; Author: Tyler Phillips
; Date: 09/22/20
;-------------------------------------------------------------------------------

			.cdecls C,LIST,"msp430.h" ; Include device header file

			.def hw_mult
			.text

hw_mult:	mov	10(SP), &MPY 		;move SP+10 to R5
			mov 2(SP),	&OP2		;move SP+2 to R6
			nop						;3 clock cycles
			nop
			nop
			mov RESLO, R7

			ret
			.end
