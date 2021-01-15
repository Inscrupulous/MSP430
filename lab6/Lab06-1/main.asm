;-------------------------------------------------------------------------------
; File: main.asm (CPE 325 Lab 6)
; Function: References to the SW1_ISR and SW2_ISR to use interrupts to perform a specific task as each switch is pushed
; Input: Users choice of switch combination
; Output: If SW1 is hit, LED1 (RED) Blinks 3 times at 1 hz and toggles LED2. If SW2 is hit, LED 2 is toggled on then off and continuous.
; Author: Tyler Phillips
; Date: 10/02/20
;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.

            .def	SW1_ISR

            .def	SW2_ISR
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.
			.ref	SW1_ISR

			.ref	SW2_ISR
;-------------------------------------------------------------------------------
RESET:       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT:     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
Setup:
			bis.b	#001h, &P1DIR		; Set P1.0 to output
										; direction (0000_0001)
			bis.b	#080h, &P4DIR		; turn P4.7 ON

			bic.b	#001h, &P1OUT		; Set P1OUT to output
										; direction (0000_0001)
			bic.b	#080h, &P4OUT		; set P4.7 as output

			bic.b	#002h, &P1DIR		; SET P1.1 as input for S2
            bis.b	#002h, &P1REN		; Enable Pull-Up resister at P1.1
            bis.b	#002h, &P1OUT		; required for proper IO set up

            bic.b	#002h, &P2DIR		; SET P2.1 as input for SW1
            bis.b	#002h, &P2REN		; Enable Pull-Up resister at P2.1
			bis.b	#002h, &P2OUT		; required for proper IO set up

            bis.w	#GIE, SR			; Enable Global Interrupts
            bis.b	#002h, &P1IE		; Enable Port 1 interrupt from bit 1
			bis.b	#002h, &P1IES		; Set interrupt to call from hi to low
			bis.b	#002h, &P2IE		; Enable Port 2 interrupt from bit 1
			bis.b	#002h, &P2IES		; Set interrupt to call from hi to low
			bic.b	#002h, &P1IFG		; Clear interrupt flag
			bic.b	#002h, &P2IFG		; Clear interrupt flag

Start:		cmp.w	#1, R5
			jne		RLED
			clr.w	R5
			xor.b	#001h, &P1OUT

RLED:		cmp.w	#1, R6
			jne 	InfLoop
			clr.w	R6
			mov.w	#6, R5


Cycle:		mov.w	#0xFFFF, R7

Delay:		nop
			nop
			nop
			nop
			nop
			dec		R7
			jnz		Delay
			xor.b	#001h, &P1OUT
			dec		R5
			jnz 	Cycle
			bit.b	#001h, &P1OUT
			xor.b	#080h, &P4OUT
			jz		InfLoop
			bis.b	#080h, &P4OUT

InfLoop:
			jmp		Start					; Loop here until interrupt

;----------------------------------------------------------------------------
; P1_0 (Red) / P2_1 (SW1) interrupt service routine (ISR)
;----------------------------------------------------------------------------
SW1_ISR:
			bic.b	#002h, &P2IFG		; Clear interrupt flag
			bit.b	#00000010b, &P2IN	; Check if SW1 is pressed; (0000_0010 on P2IN)
			jnz		LExit_				; If not zero, SW is not pressed; loop and check again

         	mov.b	#2000, R7		; Set to (2000 * 10 cc )
SWD20ms_:	dec.w	R7              ; Decrement R7
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			jnz 	SWD20ms_		 ; Delay over?
			bit.b	#002h, &P2IN     ; Verify SW1 is still pressed
			jnz		LExit_			 ; If not, wait for S1 press
			mov.b   #1, R6			 ; increment counter
LExit_:		reti					 ; Return from interrupt


;----------------------------------------------------------------------------
; P4_7 (Green) / P1_1 (SW2) interrupt service routine (ISR)
;----------------------------------------------------------------------------
SW2_ISR:
			bic.b	#002h, &P1IFG		; Clear interrupt flag
			bit.b	#00000010b, &P1IN	; Check if S2 is pressed; (0000_0010 on P1IN)
			jnz		LExit				; If not zero, SW is not pressed; loop and check again
			xor.b	#080h, &P4OUT

            mov.b	#2000, R7		; Set to (2000 * 10 cc )
SWD20ms:	dec.w	R7              ; Decrement R15
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			jnz 	SWD20ms			; Delay over?
			bit.b	#002h, &P1IN    ; Verify S2 is still pressed
			jnz		LExit			 ; If not, wait for S2 press
			mov.b 	#1, R7			;increment counter
LExit:		reti					 ; Return from interrupt
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"        ; MSP430 RESET Vector
            .short  RESET
 			.sect   ".int47"        ; PORT2_VECTOR,
            .short  SW2_ISR
            .sect   ".int42"        ; PORT1_VECTOR,
            .short  SW1_ISR
            .end
