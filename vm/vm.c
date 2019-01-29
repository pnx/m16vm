/* vm.c
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
#include <string.h>
#include "program.h"
#include "mm.h"
#include "cpu.h"
#include "debug.h"

static unsigned debug_mem = 0;
static unsigned debug_reg = 0;

void run(struct program *prog) {

	struct cpu_state state;
	int rc;

	mm_init();
	cpu_init(&state);

	cpu_instr_load(&state, prog->instr, prog->len);

	do {
		rc = cpu_tick(&state);
	} while(rc == 0);

	if (debug_reg)
		debug_print_regs(state.reg);

	if (debug_mem)
		debug_print_memory(memory);

	mm_exit();
}

int main(int argc, char **argv) {

	struct program prog = { 0 };
	const char *filename;
	int i;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return 1;
	}

	// Parse options.
	for(i = 1; i < argc; i++) {

		if (argv[i][0] == '-') {
			if (!strcmp(argv[i], "-dmem")) {
				debug_mem = 1;
			} else if (!strcmp(argv[i], "-dreg")) {
				debug_reg = 1;
			} else {
				fprintf(stderr, "Invalid option '%s'\n", argv[i]);
			}
		} else {
			filename = argv[i];
			break;
		}
	}

	if (program_loadfromfile(&prog, filename) < 0)
		return 1;

	// Execute the program.
	run(&prog);

	program_free(&prog);

	return 0;
}
