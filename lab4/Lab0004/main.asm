;-------------------------------------------------------------------------------
; File: main.asm (CPE 325 Lab 4 Question 1)
; Function: Counts the number of words and sentences in a given string
; Description: Program traverses an input array of characters
; Input: The input string is specified in myStr
; Output: The registers R5 and R7 output the value in hex of sentences and words respectively.
; Author: Tyler Phillips
; Date: 09/12/20
;-------------------------------------------------------------------------------
        .cdecls C,LIST,"msp430.h"       ; Include device header file

;-------------------------------------------------------------------------------
        .def    RESET                   ; Export program entry-point to
                                        ; make it known to linker.

myStr:  .cstring "Hello! Welcome to CPE325. It is a great day, is it not?"
        ; .string does add NULL at the end of the string;
        ;    '' ensures that a NULL follows the string.
        ; You can alternatively use .cstring "Hello! Welcome to CPE325. It is a great day, is it not?"
        ;    that adds a NULL character at the end of the string automatically.
;-------------------------------------------------------------------------------
		.data
sent_count:		.int	0
word_count:		.int	0
;-------------------------------------------------------------------------------
        .text                           ; Assemble into program memory.
        .retain                         ; Override ELF conditional linking
                                        ; and retain current section.
        .retainrefs                     ; And retain any sections that have
                                        ; references to current section.

;-------------------------------------------------------------------------------
RESET:  		mov.w   #__STACK_END,SP         ; Initialize stack pointer
		        mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer

;-------------------------------------------------------------------------------
; Main loop here sentence: "Hello! Welcome to CPE325. It is a great day, is it not?"
;-------------------------------------------------------------------------------
main:
        mov.w   #myStr, R4              ; load the starting address of the string into R4
        clr.b	R7						; register R7 will store the sentence count
        clr.b	R5                      ; register R5 will store the word count
        mov.b   #1, R5                  ; R5 = 1 because word count = number of ' '  + 1
gnext:  mov.b   @R4+, R6                ; get a new character
        cmp     #0,R6                   ; is it a null character
        jeq     lend                    ; if yes, go to the end
        cmp.b   #' ', R6                ; is it an ' ' character
        jne     dot                   	; if not, go to dot
        inc.w   R5                      ; if yes, increment word counter
        jmp     gnext					; next character

dot:	cmp		#'.', R6				; is it a '.'
		jne		exc						; if not go to exc
		inc.w	R7						; if yes increment sentence counter
		jmp 	gnext					; next character

exc:	cmp		#'!', R6				; is it a '!'
		jne		que					    ; if not go to que
		inc.w	R7						; if yes increment sentence counter
		jmp 	gnext					; next character

que:	cmp		#'?', R6				; is it a '!'
		jne		gnext					; if not next character
		inc.w	R7						; if yes increment sentence counter
		jmp 	gnext					; next character


lend:   mov.w   R5, &word_count		   	; write result in R7 and r5
		mov.w 	R7, &sent_count
        bis.w   #LPM4,SR                ; LPM4
        nop                             ; required only for Debugger


;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
        .global __STACK_END
        .sect   .stack

;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
         .sect   ".reset"               ; MSP430 RESET Vector
         .short  RESET
         .end
