
; mov h/l test.
; Using 2 instructions to store a 16-bit words is abit tricky to write code for

; Storing 32767 = (2^15) - 1 (highest value in 2's complement 16-bit).
; MSB (signed flag) = 0, rest 1.
; H [0111 1111] L [1111 1111]
movl	$0x0, -1
movh    $0x0, 127

; Storing -32768 = (2^15) (highest value in 2's complement 16-bit).
; MSB (signed flag) = 1, rest 0.
; H [1000 0000] L [0000 0000]
movl	$0x1, 0
movh    $0x1, -128
