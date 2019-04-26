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

// Get a WORD aligned (16-bit) address.
// Casts the 8-bit pointer (mm_base_addr) to 16-bit (WORD) and add offset.
#define addr_align_word(offset) \
	(((uint16_t*) mm_base_addr) + (offset))

#define EXCEPTION_STRING "Runtime exception: Memory address '%u' is out of bounds.\n"

uint8_t *mm_base_addr = NULL;

#define __check_bounds(addr) 				\
	if ((addr) + 1 > MEM_SIZE) {			\
		fprintf(stderr, EXCEPTION_STRING, addr);\
		exit(1);				\
	}

void mm_init() {

	mm_base_addr = (uint8_t*) malloc(MEM_SIZE);
}

void mm_exit() {

	if (mm_base_addr) {
		free(mm_base_addr);
		mm_base_addr = NULL;
	}
}

void mm_sw(uint16_t addr, int16_t value) {

	__check_bounds(addr);

	*addr_align_word(addr) = value;
}

int16_t mm_lw(uint16_t addr) {

	__check_bounds(addr);

	return *addr_align_word(addr);
}
