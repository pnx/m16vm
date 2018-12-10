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
#include "codegen.h"
#include "ast.h"
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

// Same as match_type() but also generates a operand in the AST.
static int match_operand(struct lexer* lex, enum token_type type, struct ast *ast) {

	if (match_type(lex, type) < 0)
		return -1;

	if (type == TOKEN_REG) {
		ast_instr_operand(ast, DATATYPE_REGISTER, lex->token.value.n);
	} else if (type == TOKEN_NUMBER) {
		ast_instr_operand(ast, DATATYPE_NUMBER, lex->token.value.n);
	} else {
		ast_instr_operand(ast, DATATYPE_STRING, lex->token.value.s);
	}

	return 0;
}

/*
 * Helper macros for matching tokens.
 */
#define match_reg(pos, ast) \
	if (match_operand(lex, TOKEN_REG, ast) < 0) \
		return asm_error((lex)->lineno, "Expected number at argument %i", pos)

#define match_imm(pos, ast)  \
	if (match_operand(lex, TOKEN_NUMBER, ast) < 0) \
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
static int match_typeR(struct lexer* lex, struct ast *ast) {

	match_reg(1, ast); match_arg(1);
	match_reg(2, ast); match_arg(2);
	match_reg(3, ast);
	match_end;

	return 0;
}

// RI-Type (rs : u8, r0 : u8, offset : s8)
static int match_typeRI(struct lexer* lex, struct ast *ast) {

	match_reg(1, ast); match_arg(1);
	match_reg(2, ast); match_arg(2);
	match_imm(3, ast);
	match_end;

	return 1;
}

// I-Type (rs : u8, imm : s8)
static int match_typeI(struct lexer* lex, struct ast *ast) {

	match_reg(1, ast); match_arg(1);
	match_imm(2, ast);
	match_end;

	return 1;
}

// J-Type (addr : string)
static int match_typeJ(struct lexer* lex, struct ast *ast) {

	if (match_operand(lex, TOKEN_LABEL, ast) < 0)
		return asm_error(lex->lineno, "Expected label at argument 1");
	match_end;
	return 1;
}

// Match \n*<opcode>
static int match_label_decl(struct lexer* lex, struct ast *ast) {

	uint16_t location = ast->instr.size / sizeof(struct ast_instr);

	ast_location(ast, lex->token.value.s, location);

	return 1;
}

#define opcode_guard(op) \
	if (op_set == 0) { op_set = 1; ast_instr(ast, op); }

/*
 * Parse a single line.
 */
static int parse_line(struct lexer* lex, struct ast *ast) {

	int op_set = 0;

	if (lexer_get_next(lex) < 0)
		return -1;

	// Opcode should come first.
	switch(lex->token.type) {
	case TOKEN_EOI: return -1;
	case TOKEN_EOL: break;
	case TOKEN_OPCODE_NOOP : ast_instr(ast, OP_NOOP);
		match_end;
		break;
	// Type-R
	case TOKEN_OPCODE_ADD : opcode_guard(OP_ADD);
		return match_typeR(lex, ast);
	// Type-I
	case TOKEN_OPCODE_MOVL : opcode_guard(OP_MOVL);
	case TOKEN_OPCODE_MOVH : opcode_guard(OP_MOVH);
	case TOKEN_OPCODE_JR   : opcode_guard(OP_JR);
	case TOKEN_OPCODE_INT  : opcode_guard(OP_INT);
		return match_typeI(lex, ast);
	// Type-RI
	case TOKEN_OPCODE_LD  : opcode_guard(OP_LW);
	case TOKEN_OPCODE_SW  : opcode_guard(OP_SW);
	case TOKEN_OPCODE_BEQ : opcode_guard(OP_BEQ);
		return match_typeRI(lex, ast);
	// Type-J
	case TOKEN_OPCODE_JMP : opcode_guard(OP_JMP);
		return match_typeJ(lex, ast);
	case TOKEN_LABEL_DECL :
		return match_label_decl(lex, ast);
	default:
		return asm_error(lex->lineno, "Opcode or label expected");
	}

	return 0;
}

/*
 * Main parser function.
 */
int parse(FILE *source_fd, FILE *dest_fd) {

	int rc;
	struct lexer lex;
	struct ast ast;

	ast_init(&ast);
	lexer_init(&lex, source_fd);

	// Parse and build AST.
	do {
		rc = parse_line(&lex, &ast);
	} while(rc >= 0);

	// TODO: Second pass validation
	// make sure all referenced labels are actually defined.

	// Code generation
	for(int i = 0; i < ast.instr.size; i += sizeof(struct ast_instr)) {
		struct ast_instr *instr = ast.instr.base + i;
		uint8_t code[2] = { 0 };

		codegen_emit(instr, ast.symbols, &code);

		fwrite(&code, sizeof(code), 1, dest_fd);
	}

	// Cleanup
	ast_free(&ast);

	return 0;
}
