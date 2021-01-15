;-------------------------------------------------------------------------------
; File: main.asm (CPE 325 Lab 5)
; Function: Uses the stack to push the address of a certain array and calls calc_power subroutine
; Input: num, hw_array, sw_array
; Output: none
; Author: Tyler Phillips
; Date: 09/22/20
;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have

            .data
num:		.int	2
hw_arr:		.usect ".bss", 5*2
sw_arr:		.usect ".bss", 5*2
                                            ; references to current section.
			.ref	calc_power
;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:
		push	num			;put number in R4
		push	#sw_arr		;push address of first array into the stack
		push	#hw_arr		;push address of second array into the stack
		call	#calc_power	;call subroutine

		jmp 	$			;infinite loop
		nop					;get results after one clock cycle
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
