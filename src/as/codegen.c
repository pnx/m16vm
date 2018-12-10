/* codegen.c
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
#include "codegen.h"

void codegen_emit(struct ast_instr* instr, symtab_t* symbols, uint8_t* out) {

	out[0] = instr->opcode << 4;

	if (instr->n_operands < 1)
		// No operands, return
		return;

	// J-Type
	if (instr->operands[0].type == DATATYPE_STRING) {
		uint16_t addr;

		if (symtab_get(symbols, instr->operands[0].s, &addr) >= 0) {
			out[0] |= (addr >> 8);
			out[1]  =  addr;
		}
	}
	// R/RI or I
	else if (instr->operands[0].type == DATATYPE_REGISTER) {

		out[0] |= instr->operands[0].r & 0xF;

		// I
		if (instr->operands[1].type == DATATYPE_NUMBER) {

			out[1] = instr->operands[1].n;
		}
		// R/RI-Type
		else if (instr->operands[1].type == DATATYPE_REGISTER &&
			(instr->operands[2].type == DATATYPE_REGISTER || instr->operands[2].type == DATATYPE_NUMBER)) {

			out[1] = (instr->operands[1].r << 4) | (instr->operands[2].r & 0xF);
		}
	}
}
