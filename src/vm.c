/* vm.c
 *
 *   Copyright (C) 2012,2014   Henrik Hautakoski <henrik.hautakoski@gmail.com>
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
#include "program.h"
#include "mm.h"
#include "cpu.h"

#ifdef M16_DEBUG_MEM
void print_memory() {

	int i = 0;

	printf("\n");

	for(i = 0; i < 32; i++) {
		printf("%.2X ", memory[i]);
	}
	printf("\n");
}
#else
#define print_memory()
#endif /* ! M16_DEBUG_MEM */

#ifdef M16_DEBUG_REG
void print_regs(uint16_t *regs) {

	int i = 0;

	printf("\n");

	for(i = 0; i < CPU_NUM_REGS; i++) {
		printf("r%i = %i\n", i, regs[i]);
	}
	printf("\n");
}
#else
#define print_regs(regs)
#endif /* ! M16_DEBUG_REG */

void run(struct program *prog) {

	struct cpu_state state;
	int rc;

	mm_init();
	cpu_init(&state);

	cpu_instr_load(&state, prog->instr, prog->len);

	do {
		rc = cpu_tick(&state);
	} while(rc == 0);

	print_regs(state.reg);

	print_memory();

	mm_exit();
}

int main(int argc, char **argv) {

	struct program prog = { 0 };

	if (argc < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return 1;
	}

	if (program_loadfromfile(&prog, argv[1]) < 0)
		return 1;

	// Execute the program.
	run(&prog);

	program_free(&prog);

	return 0;
}
