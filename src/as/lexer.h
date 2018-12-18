/* lexer.h
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
#ifndef ASM_LEXER_H
#define ASM_LEXER_H

#include <stdlib.h>
#include <stdint.h>

/**
 * All token types.
 */
enum token_type {
	TOKEN_EOI = -1,
	TOKEN_EOL,		// Newline
	TOKEN_OPCODE_NOOP,
	TOKEN_OPCODE_ADD,
	TOKEN_OPCODE_MOVL,
	TOKEN_OPCODE_MOVH,
	TOKEN_OPCODE_LD,
	TOKEN_OPCODE_SW,
	TOKEN_OPCODE_BEQ,
	TOKEN_OPCODE_JMP,
	TOKEN_OPCODE_JR,
	TOKEN_OPCODE_INT,
	TOKEN_LABEL,
	TOKEN_LABEL_DECL,
	TOKEN_REG,
	TOKEN_NUMBER,
	TOKEN_ARG_SEP
};

/**
 * Token structure.
 *
 * Holds information about a single token.
 */
struct token {
	// Line number where the token was extracted from.
	uint16_t 	lineno;

	enum token_type	type;

	/*
	 * Token value, depending on type
	 * this can be a string or unsigned short
	 */
	union {
		int8_t n;
		char   s[32];
	} value;
};

/**
 * Lexer state
 */
struct lexer {
	uint16_t 	lineno;		// current line number
	FILE *		fp;		// File being lexed.
	struct token 	token;		// Current token
};

/**
 * Initialize the lexer with a file pointer to the file
 * that should be lexed.
 */
void lexer_init(struct lexer *lex, FILE *fp);

/**
 * Advance the lexer to the next token.
 */
int lexer_get_next(struct lexer *lex);

/**
 * For debugging, prints the token to standard output.
 */
void lexer_print_token(struct token *token);

#endif /* ASM_LEXER_H */
