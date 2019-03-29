/* lexer/number.h
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
#ifndef ASM_LEXER_NUMBER_H
#define ASM_LEXER_NUMBER_H

#include <stdio.h>

int lexer_read_num_hex(FILE *fp, int *out);

int lexer_read_num_dec(FILE *fp, int neg, int *out);

#endif /* ASM_LEXER_NUMBER_H */
