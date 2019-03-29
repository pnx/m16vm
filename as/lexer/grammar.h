/* lexer/grammar.h
 *
 * Macros for the grammar.
 *
 *   Copyright (C) 2018-2019   Henrik Hautakoski <henrik.hautakoski@gmail.com>
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
#ifndef ASM_LEXER_GRAMMAR_H
#define ASM_LEXER_GRAMMAR_H

// Numbers is defined as [0-9]
#define lexer_is_num(x) ((x) >= '0' && (x) <= '9')

// The first digit can however also contain '-'
#define lexer_is_num_start(x) (lexer_is_num(x) || (x) == '-' )

// First character in strings can be [a-z][A-Z] or '_'
#define lexer_is_string_start(x)			\
	(  ((x) >= 'a' && (x) <= 'z')   \
	|| ((x) >= 'A' && (x) <= 'Z')	\
	||  (x) == '_'			)

// All characters after can also include numbers or ':'
#define lexer_is_string(x) \
	(lexer_is_string_start(x) || lexer_is_num(x))

#define lexer_is_space(x) ((x) == ' ' || (x) == '\t' || (x) == '\r')

#endif /* ASM_LEXER_GRAMMAR_H */
