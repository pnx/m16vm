/* parser.c
 *
 *   Copyright (C) 2012   Henrik Hautakoski <henrik@fiktivkod.org>
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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "instr_encode.h"
#include "lexer.h"
#include "parser.h"

/*
 * Helper functions/macros for defining parser rules.
 */

// match the next token.
// returns 0 if the token was of the correct type. -1 otherwise
static int match_type(struct lexer* lex, enum token_type type) {

	lexer_get_next(lex);
	return lex->token.type == type ? 0 : -1;
}

// Same as match_type() but extracts a number from the token.
static int match_type_num(struct lexer* lex, enum token_type type,
			void* out, size_t size) {

	if (match_type(lex, type) < 0)
		return -1;

	memcpy(out, &lex->token.value.n, size);
	return 0;
}

/*
 * Helper macros for matching tokens.
 */
#define match_reg(pos, out) \
	if (match_type_num(lex, TOKEN_REG, out, sizeof(uint8_t))) \
		return asm_error((lex)->lineno, "Expected number at argument %i", pos)

#define match_imm(pos, out)  \
	if (match_type_num(lex, TOKEN_NUMBER, out, sizeof(int16_t)) < 0) \
		return asm_error((lex)->lineno, "Expected number at argument %i", pos)

#define match_arg(pos) \
	if (match_type(lex, TOKEN_ARG_SEP) < 0) \
		return asm_error((lex)->lineno, "Expected separator after argument %i", pos)

#define match_end \
	if (match_type(lex, TOKEN_EOL) < 0) \
		return asm_error(lex->lineno, "Expected newline")

/*
 * Functions for matching complete instructions.
 */

// R-Type (rs : u8, r0 : u8, r1 : u8)
static int match_typeR(struct instr_R *instr, struct lexer* lex) {

	match_reg(1, &instr->rs); match_arg(1);
	match_reg(2, &instr->r0); match_arg(2);
	match_reg(3, &instr->r1);
	match_end;

	return 0;
}

// RI-Type (rs : u8, r0 : u8, offset : s8)
static int match_typeRI(struct instr_RI *instr, struct lexer* lex) {

	match_reg(1, &instr->rs); match_arg(1);
	match_reg(2, &instr->r0); match_arg(2);
	match_imm(3, &instr->offset);
	match_end;

	return 1;
}

// I-Type (rs : u8, imm : s8)
static int match_typeI(struct instr_I *instr, struct lexer* lex) {

	match_reg(1, &instr->rs); match_arg(1);
	match_imm(2, &instr->imm);
	match_end;

	return 1;
}

// J-Type (rs : u8, addr : u16)
static int match_typeJ(struct instr_J *instr, struct lexer* lex) {

	match_imm(1, &instr->addr);
	match_end;
	return 1;
}

#define opcode_guard(op, v) \
	if (op == OP_NONE) op = v

/*
 * Parse a single line.
 */
static int parse_line(struct lexer* lex, struct instr *instr) {

	instr->opcode = OP_NONE;

	if (lexer_get_next(lex) < 0)
		return -1;

	// Opcode should come first.
	switch(lex->token.type) {
	case TOKEN_EOI: return -1;
	case TOKEN_EOL: break;
	case TOKEN_OPCODE_NOOP : instr->opcode = OP_NOOP;
		match_end;
		break;
	// Type-R
	case TOKEN_OPCODE_ADD : opcode_guard(instr->opcode, OP_ADD);
		return match_typeR(&instr->r, lex);
	// Type-I
	case TOKEN_OPCODE_MOVL : opcode_guard(instr->opcode, OP_MOVL);
	case TOKEN_OPCODE_MOVH : opcode_guard(instr->opcode, OP_MOVH);
	case TOKEN_OPCODE_JR   : opcode_guard(instr->opcode, OP_JR);
	case TOKEN_OPCODE_INT  : opcode_guard(instr->opcode, OP_INT);
		return match_typeI(&instr->i, lex);
	// Type-RI
	case TOKEN_OPCODE_LD  : opcode_guard(instr->opcode, OP_LW);
	case TOKEN_OPCODE_SW  : opcode_guard(instr->opcode, OP_SW);
	case TOKEN_OPCODE_BEQ : opcode_guard(instr->opcode, OP_BEQ);
		return match_typeRI(&instr->ri, lex);
	// Type-J
	case TOKEN_OPCODE_JMP : opcode_guard(instr->opcode, OP_JMP);
		return match_typeJ(&instr->j, lex);
	case TOKEN_LABEL_DECL :
		asm_warn(lex->lineno, "labels are not supported yet. ignoring.");
		break;
	default:
		return asm_error(lex->lineno, "Opcode or label expected");
	}

	return 0;
}

static int gencode(FILE *fd, struct instr *instructions, int len) {

	uint16_t buf; // 2-bytes (16-bit) per instruction.

	for(int i = 0; i < len; i++) {
		instr_encode(instructions + i, &buf);

		fwrite(&buf, sizeof(buf), 1, fd);
	}
}

/*
 * Main parser function.
 */
int parse(FILE *source_fd, FILE *dest_fd) {

	int rc;
	struct lexer lex;
	struct instr instr[256];
	int n = 0;

	lexer_init(&lex, source_fd);

	do {
		rc = parse_line(&lex, instr + n);

		if (instr[n].opcode != OP_NONE) {
			n++;
			if (n >= 256)
				// TODO: Dynamic allocs :)
				return asm_error(-1, "Oops, parser ran out of memory.");
		}
	} while(rc >= 0);

	gencode(dest_fd, instr, n);

	return 0;
}
