/* instr.h
 *
 *   Copyright (C) 2012,2014   Henrik Hautakoski <henrik.hautakoski@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *   MA 02110-1301, USA.
 */
#ifndef INSTR_H
#define INSTR_H

#include <stdint.h>

/* Opcodes */
#define OP_NOOP 	0
#define OP_ADD		1
#define OP_MOVL 	2
#define OP_MOVH 	3
#define OP_LW		4
#define OP_SW   	5
#define OP_BEQ  	6
#define OP_JMP  	7
#define OP_JR   	8
#define OP_INT 		15

/* Register type */
struct instr_R {
	uint8_t rs;
	uint8_t r0;
	uint8_t r1;
};

struct instr_RI {
	uint8_t rs;
	uint8_t r0;
	int8_t 	offset;
};

struct instr_I {
	uint8_t  rs;
	int8_t   imm;
};

struct instr_J {
	uint16_t addr;
};

struct instr {
	uint8_t opcode;
	union {
		struct instr_R  r;
		struct instr_RI ri;
		struct instr_I  i;
		struct instr_J  j;
	};
};

void instr_decode(unsigned char *nibble, struct instr *instr);

#endif /* INSTR_H */
