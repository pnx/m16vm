/* asm_error.c
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
#include <stdio.h>
#include <stdarg.h>
#include "error.h"
#include "asm_error.h"

void asm_warn(int lineno, const char *message, ...) {

	fprintf(stderr, "Line %i: ", lineno);
	warn(message);
}

int asm_error(int lineno, const char *message, ...) {

	fprintf(stderr, "Line %i: ", lineno);
	return error(message);
}
