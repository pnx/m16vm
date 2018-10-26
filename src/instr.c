/* instr.c
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
#include "instr.h"

void instr_decode(unsigned char *instr, struct instr *out) {

	out->opcode 	= *instr >> 4;

	if (out->opcode == OP_NOOP)
		return;

	// J-Type
	if (out->opcode == OP_JMP) {

		out->j.addr = ((*instr & 0xF) << 8) + *(instr + 1);

		// if MSB (bit 12) is set
		// perform 2s complement by setting bit 13-16 to 1
		/*
		if (out->j.addr & 0x0800)
			out->j.addr |= 0xF000; */
	} else {
		out->r.rs = *instr & 0xF;

		// I-Type
		if (out->opcode == OP_MOVL || out->opcode == OP_MOVH || out->opcode == OP_SYSC_WR) {
			out->i.imm = *(instr + 1);
		}
		// R/RI-Type
		else {
			out->r.r0 = *(instr + 1) >> 4;
			out->r.r1 = *(instr + 1) & 0xF;
		}
	}
}
