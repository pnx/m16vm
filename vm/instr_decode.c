/* instr_decode.c
 *
 *   Copyright (C) 2012,2014-2015,2018-2019,2023   Henrik Hautakoski <henrik.hautakoski@gmail.com>
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
#include "instr_decode.h"

void instr_decode(unsigned char *instr, struct instr *out) {

	out->opcode 	= *instr >> 4;

	if (out->opcode == OP_NOOP)
		return;

	// J-Type
	if (out->opcode == OP_JMP) {
		out->j.addr = ((*instr & 0xF) << 8) + *(instr + 1);
	} else {
		out->r.rs = *instr & 0xF;

		// I-Type
		if (out->opcode == OP_MOVL || out->opcode == OP_MOVH || out->opcode == OP_INT) {
			out->i.imm = *(instr + 1);
		}
		// R/RI-Type
		else {
			out->r.r0 = *(instr + 1) >> 4;
			out->r.r1 = *(instr + 1) & 0xF;
		}
	}
}
