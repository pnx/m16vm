/* instr_encode.c
 *
 *   Copyright (C) 2018   Henrik Hautakoski <henrik@fiktivkod.org>
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
#include <stdio.h>
#include <string.h>
#include "instr_encode.h"

void instr_encode(struct instr* instr, uint16_t *out) {

	uint8_t *p = (uint8_t *) out;

	*out = 0;
	*p = instr->opcode << 4;

	if (instr->opcode == OP_NOOP) {
		// Do nothing.
	} else if (instr->opcode == OP_JMP) {
		*p |= (instr->j.addr >> 8);
		*(p+1) = instr->j.addr;
	} else {
		*p |= instr->r.rs;

		// I-Type
		if (instr->opcode == OP_MOVL || instr->opcode == OP_MOVH
			|| instr->opcode == OP_JR || instr->opcode == OP_INT) {

			*(p+1) = instr->i.imm;
		}
		// R/RI-Type
		else {
			*(p+1) = (instr->r.r0 << 4) | instr->r.r1;
		}
	}
}
