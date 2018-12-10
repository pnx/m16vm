
; -- Store "Hello World" string in memory.
movl 	$1, 72 ; H
sw	$0, $1, 0
movl 	$1, 69 ; E
sw	$0, $1, 1
movl 	$1, 76 ; L
sw	$0, $1, 2
sw	$0, $1, 3
movl 	$1, 79 ; O
sw	$0, $1, 4
movl 	$1, 32 ; Space
sw	$0, $1, 5
movl 	$1, 87 ; W
sw	$0, $1, 6
movl 	$1, 79 ; O
sw	$0, $1, 7
movl 	$1, 82 ; R
sw	$0, $1, 8
movl 	$1, 76 ; L
sw	$0, $1, 9
movl 	$1, 68 ; D
sw	$0, $1, 10

; -- setup print loop.
movl 	$1, 1		; Load 1 in R1 (used for increment the counter)
_start:
ld	$15, $0, 0	; Load memory address stored in R0 into R15
int     $10, 2		; Print character
add 	$0, $0, $1	; Add 1 (R1) to counter (R0)
beq	$15, $2, 1	; Branch to "noop" (skipping next instruction) if R15 = 0 (R2 holds 0)
jmp     _start		; jump back to "_start" label
noop
