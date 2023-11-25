/* error.c
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
#include <stdio.h>
#include <stdarg.h>
#include "error.h"

static inline void _write(FILE *fd, const char *prefix, const char *fmt, va_list list) {

	char buf[4096];
	vsnprintf(buf, sizeof(buf), fmt, list);
	fprintf(stderr, "%s: %s\n", prefix, buf);
}

void warn(const char *message, ...) {

	va_list vl;

	va_start(vl, message);
	_write(stderr, "Warning", message, vl);
	va_end(vl);
}

int error(const char *message, ...) {

	va_list vl;

	va_start(vl, message);
	_write(stderr, "Error", message, vl);
	va_end(vl);
	return -1;
}
