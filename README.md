m16vm - 16 bit processor virtual machine
========================================

This is a virtual machine for a RISC-processor designed for educational purposes.

The design is taken from the real world and tries to mimic existing RISC-architectures. So it should in theory be possible to construct actual hardware based on the instruction set. Maybe not build a transistor-chip (it's hard, unless you know someone working at Intel/Amd or something) but by putting together a bunch of gate-chips on a breadboard.

The instruction set is by no means designed to be "fast" or "optimal" but focus more on being easy to understand and modify/play with.

Compile
-------

The code is compiled via Makefile.

```sh
$ make
```

The code is compiled into 2 programs:

* `m16as` - Assembler - Compiles assembler code into binary form.
* `m16vm` - Virtual Machine, will run binary code from a file.


Example for first compile assembly code and then run it:

```sh
$ ./m16as examples/asm/hello_world.as > prog.bin
$ ./m16vm ./prog.bin
HELLO WORLD
```

Usage
-----

#### m16as

```
Usage: ./m16as <inputfile> [ <outputfile> ]
```

`m16as` is pretty streigth forward. if no `outputfile` is given, the output is written to `stdout`

#### m16vm

```
usage: ./m16vm [ --dmem | --dreg ] <file>
```

* `--dmem` - debug memory, after the program has executed the last instruction, the contents of the memory is printed to `stdout`. NOTE: only the first 32 bytes are printed to

* `--dreg` - debug registers, after the program has executed the last instruction, the contents of the registers is printed to `stdout`.

Specification
-------------

The cpu has 16 general purpose registers, r0 - r15. Each register is 16-bit.

Memory addresses are aligned by 2 bytes (16 bit) and the machine has a total of `65536` bytes of memory (per default, can be changed at compile time).

instruction set overview
------------------------

There are 16 different instructions.

| opcode | Name | Type | Description                             |
|--------|------|------|-----------------------------------------|
| 0000   | noop | \-   | No operation                            |
| 0001   | add  | R    | Addition                                |
| 0010   | movl | I    | move value to register (lowest 8-bits)  |
| 0011   | movh | I    | move value to register (highest 8-bits) |
| 0100   | ld   | RI   | load word                               |
| 0101   | sw   | RI   | store word                              |
| 0110   | beq  | RI   | Branch on equal                         |
| 0111   | jmp  | J    | Jump                                    |
| 1000   | jr   | I    | Jump (register)                         |
| 1001   | \-   | \-   | Reserved                                |
| 1010   | \-   | \-   | Reserved                                |
| 1011   | \-   | \-   | Reserved                                |
| 1100   | \-   | \-   | Reserved                                |
| 1101   | \-   | \-   | Reserved                                |
| 1110   | \-   | \-   | Reserved                                |
| 1111   | int  | I    | Interrupt                               |

NOTE: Subtraction can be implemented via `add` and negative register values, so no special opcode is needed.

Instruction set format
----------------------

Register operation (R):

```
opcode (4) | rs (4)  | r0 (4) | r1 (4)

r0,r1      - Operand registers.
rs         - save register
```

Register operation offset (RI):

```
opcode (4) | rs (4)  | r0 (4) | offset (signed 4)

r0  - Operand registers.
offset  - constant offset from r0 value.
rs      - save register
```

Constant operation format (I):

```
opcode (4) | reg (4) | data (signed 8)

reg - Register
data - Constant data to insert into reg.
```

Jump format (J):

```
opcode (4) | addr (12)
```

Interrupts
----------

There can be a total of `16` Interrupts and the instruction uses the `I-format`.

The interrupt number is stored in `reg` (note that for interrupts this is a constant and not a register).

The `data` field can be used as an argument to the interrupt.

### Interrupt functions overview

| Number | Name      | data   | Description                                              |
|-------:|-----------|--------|----------------------------------------------------------|
|     10 | I/O Write | format | Outputs a value to the screen, value are stored in `r15` |

### I/O Write - int 10

The formatting of the output can be controlled by the number in `data` as follows:

| Argument | Datatype                |
|----------|-------------------------|
| 0        | Integer (16 bit signed) |
| 1        | Integer (8 bit signed)  |
| 2        | Character               |

Example programs.
-----------------

in the `programs` directory, there is some example programs to run.

Author
======

Henrik Hautakoski - [henrik.hautakoski@gmail.com](mailto:henrik.hautakoski@gmail.com)
