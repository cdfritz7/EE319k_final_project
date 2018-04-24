; Print.s
; Student names: Connor Fritz an Tatiana Larina
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
quotient equ 0				;;;; BINDING
counter equ 4
nineNine equ 9999
itervar equ 4
remainder RN 5
spCounter RN 8

	
LCD_OutDec
	PUSH{LR, R4, R5, R6, R7, R8}
	
	SUBS SP, #8				;;;; ALLOCATION/INITIALIZATION
	MOV R4, #0
	STR R4, [SP, #quotient]
	STR R4, [SP, #counter]
	
	MOV remainder, #0;
	MOV spCounter, #counter;	//used to count number of pushes in order to access counter from stack
	MOV R1, #10
;;;; BEGIN ACCESS
again
	CMP R0, #0		;if there is nothing left to divide, it's time to output
	BEQ output
	LDR R4, [SP, #quotient]
	LDR R6, [SP, spCounter]
	UDIV R4, R0, R1	;divide number by 10 and put into quotient
	MUL R2, R4, R1
	SUB remainder, R0, R2 	;remainder is original number - quotient*10
	PUSH{remainder, R7}
	MOV R0, R4
	ADD R6, #1
	ADD spCounter, #8
	STR R6, [SP, spCounter]
	B again
	
output
	LDR R6, [SP, spCounter]
	CMP R6, #0			;sees if the original number was 0 (no iterations recorded)
	BEQ outzero
output2
	LDR R6, [SP, spCounter]
	CMP R6, #0			;iterivley pop digits off stack and display them
	BEQ done
	SUB R6, #1
	STR R6, [SP, spCounter]
	POP{R0, R2}
	ADD R0, #0x30		;convert to ASCII
;	PUSH{R3,R2}
	BL ST7735_OutChar
;	POP{R3,R2}
	SUB spCounter, #8
	B output2
	
outzero
	MOV R0, #0x30		;prints only zero if original number was 0
	BL ST7735_OutChar
;;;; END ACCESS
done
	ADD SP, #8					;;;; DEALLOCATION
	POP{LR, R4, R5, R6, R7, R8}

    BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	PUSH{LR, R4, R5, R6, R7, R8}
	
	SUB SP, #8
	MOV R4, #0
	STR R4, [SP, #quotient]
	STR R4, [SP, #counter]
	
	LDR R7,=nineNine
	CMP R0, R7			;sees if the original number was >9999
	BHI outStarFix
	
	MOV remainder, #0;
	MOV spCounter, #counter;	;used to count number of pushes in order to access counter from stack
	MOV R1, #10
	LDR R7, =itervar

againFix
	CMP R7, #0		;if there is nothing left to divide, it's time to output
	BEQ outputFix
 ; no check if want equal to zero because want leading zeroes to be present. Could optimize to not perform extra
 ; calculations if zero.
	LDR R4, [SP, #quotient]
	LDR R6, [SP, spCounter]
	UDIV R4, R0, R1	        ;divide number by 10 and put result into quotient
	MUL R2, R4, R1			
	SUB remainder, R0, R2 	;remainder is part of original number - quotient*10
	PUSH{remainder, R7}
	MOV R0, R4
	ADD R6, #1
	ADD spCounter, #8
	STR R6, [SP, spCounter]
	SUB R7, #1
	B againFix
	
outputFix
	MOV R7,#1   ; ensures period is placed correctly in number	
output2Fix
	LDR R6, [SP, spCounter]
	CMP R6, #0			;iterivley pop digits off stack and display them
	BEQ doneFix			; if no more digits to display, finish up
	SUB R6, #1			; counter for number of digits to display
	STR R6, [SP, spCounter]	; store new amount of digits left to allocated spot on stack
	POP{R0, R2}			; pop number at current SP into r0, number just below that (on the stack) into r2
	ADD R0, #0x30		;convert to ASCII
	BL ST7735_OutChar
	CMP R7, #0			;prints . in correct location; printed only once
	BEQ skip
	MOV R0, #0x2E		
	BL ST7735_OutChar
	MOV R7,#0			; makes sure it wont be printed again
skip
	SUB spCounter, #8
	B output2Fix
	
outStarFix
	MOV R0, #0x2A		;prints only * if original number was >9999
	BL ST7735_OutChar
	MOV R0, #0x2E		;prints .
	BL ST7735_OutChar
	MOV R0, #0x2A		;prints *
	BL ST7735_OutChar
	MOV R0, #0x2A		
	BL ST7735_OutChar
	MOV R0, #0x2A		
	BL ST7735_OutChar
	B doneFix

doneFix
	ADD SP, #8			; deallocate
	POP{LR, R4, R5, R6, R7, R8}
    BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
