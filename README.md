
# m16vm - 16 bit processor virtual machine  

This is a virtual machine for a RISC-processor designed for educational purposes.

The design is taken from the real world and tries to mimic existing RISC-architectures.
So it should in theory be possible to construct actual hardware based on the instruction set. Maybe not build a transistor-chip (it's hard, unless you know someone working at Intel/Amd or something) but by putting together a bunch of gate-chips on a breadboard.

The instruction set is by no means designed to be "fast" or "optimal" but focus
more on being easy to understand and modify/play with.

## Specification

The cpu has 16 general purpose registers, r0 - r15. Each register is 16-bit.

Memory addresses are aligned by 2 bytes (16 bit) and the machine has
a total of `65536` bytes of memory (per default, can be changed at compile time).


## instruction set overview

There are 16 different instructions.

| opcode | Name | Type | Description                             |
| ------ | -----| ---- | --------------------------------------- |
| 0000   | noop | -    | No operation                            |
| 0001   | add  | R    | Addition                                |
| 0010   | movl | I    | move value to register (lowest 8-bits)  |
| 0011   | movh | I    | move value to register (highest 8-bits) |
| 0100   | ld   | R    | load word                               |
| 0101   | sw   | R    | store word                              |
| 0110   | beq  | RI   | Branch on equal                         |
| 0111   | jmp  | J    | Jump                                    |
| 1000   | jr   | J    | Jump (register)                         |
| 1001   | -    | -    | Reserved                                |
| 1010   | -    | -    | Reserved                                |
| 1011   | -    | -    | Reserved                                |
| 1100   | -    | -    | Reserved                                |
| 1101   | -    | -    | Reserved                                |
| 1110   | -    | -    | Reserved                                |
| 1111   | wr   | I    | I/O Write                               |

NOTE: Subtraction can be implemented via `add` and negative register values, so no
special opcode is needed.

## Instruction set format

Register operation (R):

	opcode (4) | rs (4)  | r0 (4) | r1 (4)

	r0,r1      - Operand registers.
	rs         - save register

Register operation  offset (RI):

	opcode (4) | rs (4)  | r0 (4) | offset (signed 4)

	r0 	- Operand registers.
	offset  - constant offset from r0 value.
	rs   	- save register

Constant operation format (I):

	opcode (4) | reg (4) | data (signed 8)

	reg - Register
	data - Constant data to insert into reg.

Jump format (J):

	opcode (4) | addr (signed 12)

## Example programs.

in the `programs` directory, there is some example programs to run.
