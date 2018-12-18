; NOTE: This is just to check the syntax.
; No logic behind any of the instructions (will prob crash)
movl 	$0, 3
movl 	$5, 6500
; Comment
noop    ; Comment
start0: add  	$3, $0, $1
sw	$2, $3, $2
ld	$5, $0, $3
beq	$0, $1, 1
beq	$0, $1, -25
beq	$0, $1, -4000
jr	$5, 0
int 	$0,    1
jmp	5
