/* mm.c
 *
 *   Copyright (C) 2012,2014,2018-2019   Henrik Hautakoski <henrik.hautakoski@gmail.com>
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
#include <stdlib.h>
#include "mm.h"

#ifndef MEM_SIZE
/* Set a default memory size. (2^16) */
#define MEM_SIZE 65536
#endif

#define EXCEPTION_STRING "Runtime exception: Memory address '%u' is out of bounds.\n"

int16_t* base_addr = NULL;
uint8_t *memory = NULL;

#define __check_bounds(addr) 				\
	if ((addr) > MEM_SIZE) {			\
		fprintf(stderr, EXCEPTION_STRING, addr);\
		exit(1);				\
	}


void mm_init() {

	base_addr = malloc(MEM_SIZE);
	memory = (uint8_t*) base_addr;
}

void mm_exit() {

	if (base_addr) {
		free(base_addr);
		base_addr = NULL;
		memory = NULL;
	}
}

void mm_sw(uint16_t addr, int16_t value) {

	__check_bounds(addr);

	base_addr[addr] = value;
}

int16_t mm_lw(uint16_t addr) {

	__check_bounds(addr);

	return base_addr[addr];
}
