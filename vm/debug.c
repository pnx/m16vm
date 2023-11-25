/* debug.c
 *
 *   Copyright (C) 2019   Henrik Hautakoski <henrik.hautakoski@gmail.com>
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
#include "cpu.h"
#include "debug.h"
#include "mm.h"

void debug_print_memory(uint8_t *mm) {

	int i = 0;

	printf("\n"
		"    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"
		"   ------------------------------------------------");

	for(i = 0; i < MEM_SIZE; i++) {
		if ((i % 16) == 0)
			printf("\n%.2X| ", i);

		printf("%.2X ", mm[i]);
	}
	printf("\n");
}

void debug_print_regs(int16_t *regs) {

	int i = 0;

	printf("\n");

	for(i = 0; i < CPU_NUM_REGS; i++) {
		printf("r%i = %i\n", i, regs[i]);
	}
	printf("\n");
}
