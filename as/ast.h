/* ast.h
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
#ifndef ASM_AST_H
#define ASM_AST_H

#include <vector.h>
#include "symtab.h"

// All datatypes in the language.
enum ast_datatype {
	DATATYPE_NONE = 0,   // Not an actual type.
	DATATYPE_REGISTER,   // 4 bit unsigned.
	DATATYPE_NUMBER,     // 8 bit signed.
	DATATYPE_STRING
};

// Struct to represent a single operand to a instruction.
struct ast_instr_operand {
	enum ast_datatype type;
	union {
		uint8_t     r;
		int8_t      n;
		uint16_t    addr;
		const char *s;
	};
};

// Struct to represent a single instruction.
struct ast_instr {
	uint8_t opcode;
	uint8_t n_operands;
	struct ast_instr_operand operands[3];
};

// Structure to represent a program.
struct ast {

	// List of instructions.
	struct vector instr;

	// List for storing label pointers (so we can free)
	struct vector labels;

	// Symbol table with label - location mapping.
	symtab_t *symbols;
};

void ast_init(struct ast* ast);

void ast_free(struct ast* ast);

// Add an instruction
void ast_instr(struct ast* ast, uint8_t opcode);

// Add an operand to the latest instruction.
//void ast_instr_operand(struct ast* ast, const struct ast_instr_operand* op);
void ast_instr_operand(struct ast* ast, enum ast_datatype type, void* value);

void ast_location(struct ast* ast, const char *label, uint16_t loc);

// For debugging :)
void ast_print(struct ast* ast);

#endif /* ASM_AST_H */
