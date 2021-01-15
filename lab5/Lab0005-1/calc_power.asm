;-------------------------------------------------------------------------------
; File: calc_power.asm (CPE 325 Lab 5)
; Function: Uses both SW_Mult and HW_Mult to return the two arrays
; Input: SW_Mult and HW_mult
; Output: base^n
; Author: Tyler Phillips
; Date: 09/22/20
;-------------------------------------------------------------------------------
			.cdecls C,LIST,"msp430.h" ; Include device header file

			.def calc_power
			.ref hw_mult
			.ref sw_mult

			.text
calc_power:	mov.w	2(SP), R5		; 2+SP to R5
			mov.w	6(SP), 0(R5)	; b^1 = b
			mov.w	#4,	R6			; counter that will check if array is full

hwmult:		push 	0(R5)			; pass parameter to subroutine
			call	#hw_mult		; call hw_mult (SP +2)
			add.w	#2, R5			; point to next location ;20 clock cycles
			mov.w	R7, 0(R5)		; store power in array
			pop		R7				; used to set stack to same as started
			dec		R6				; R6-1
			jnz		hwmult			; if still space in the array go to hwmult

			mov.w	#4, R6
			mov.w	4(SP), R5		; put in R5 the first address of the array
			mov.w	6(SP), 0(R5)	; b^1 = b

swmult:		push	0(R5)		    ; pass parameter to subroutine
			call	#sw_mult	    ; SP + 2
			add.w	#2, R5		    ; point to next location ; 103 clock cycles
			mov.w	R9, 0(R5)	    ; store power in array
			pop		R9			    ; used to set stack as when started
			dec		R6			    ; R6-1
			jnz		swmult		    ; if still space in the array go to swmult


			ret

			.end
