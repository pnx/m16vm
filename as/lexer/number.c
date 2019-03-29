/* lexer/number.c
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
#include "grammar.h"
#include "number.h"

int lexer_read_num_hex(FILE *fp, int *out) {

	int c, val = 0;

	while((c = fgetc(fp)) != EOF) {
		char n = 0;
		if (lexer_is_num(c)) {
			n = c - '0';
		}
		else if (  (c >= 'a' && c <= 'f')
			|| (c >= 'A' && c <= 'F')) {
			n = (c % 0x20) + 9;
		}
		else {
			ungetc(c, fp);
			break;
		}

		val = (val * 16) + n;
		if (val > 0xFF)
			goto overflow;
	}
	*out = val;
	return 0;

overflow:
	*out = 0xFF;
	return -1;
}

int lexer_read_num_dec(FILE *fp, int neg, int *out) {

	int c, val = 0;

	while((c = fgetc(fp)) != EOF) {
		if (!lexer_is_num(c)) {
			ungetc(c, fp);
			break;
		}
		val = (val * 10) + (c - '0');

		// Cool trick here.
		// because the range is -128 (0x80) to +127 (0x7F)
		// We can do 0x80 - 1 if it is NOT a negative number.
		if (val > (0x80 - !neg))
			goto overflow;
	}

	*out = neg ? -1 * val : val;
	return 0;

overflow:
	*out = neg ? -1 * 0x80 : 0x7F;
	return -1;
}
