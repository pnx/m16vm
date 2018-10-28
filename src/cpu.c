/* cpu.c
 *
 *   Copyright (C) 2012,2014-2015   Henrik Hautakoski <henrik.hautakoski@gmail.com>
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
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "cpu.h"
#include "mm.h"
#include "syscall.h"
#include "instr.h"

// Registers r0, r15
int16_t reg[CPU_NUM_REGS] = { 0 };

/* Program */
unsigned char *instr_mem = NULL;
unsigned long instr_cnt = 0;

/* Program counter */
uint16_t pc = 0;

/* CPU flags */
#define CPU_FLAGS_HALT (1<<0)
unsigned char cpu_flags = CPU_FLAGS_HALT;

#define debug(...) fprintf(stderr, __VA_ARGS__)

static void execute(struct instr *instr) {

	switch(instr->opcode) {
	case OP_NOOP :
		debug("noop\n");
		/* Do nothing */
		break;
	case OP_ADD :
		debug("add\tr%i r%i r%i\n", instr->r.rs, instr->r.r0, instr->r.r1);
		reg[instr->r.rs] = reg[instr->r.r0] + reg[instr->r.r1];
		break;
	case OP_MOVL :
		debug("movl\tr%i #%i\n", instr->i.rs, instr->i.imm);
		reg[instr->r.rs] = (reg[instr->r.rs] & 0xFF00) | instr->i.imm;
		break;
	case OP_MOVH :
		debug("movh\tr%i #%i\n", instr->i.rs, instr->i.imm);
		reg[instr->r.rs] = (reg[instr->r.rs] & 0x00FF) | (((uint16_t) instr->i.imm) << 8);
		break;
	case OP_LW :
		debug("lw\tr%i r%i #%i\n", instr->r.rs, instr->r.r0, instr->r.r1);
		reg[instr->r.rs] = mm_lw(reg[instr->r.r0] + instr->r.r1);
		break;
	case OP_SW :
		debug("sw\tr%i r%i #%i\n", instr->r.rs, instr->r.r0, instr->r.r1);
		mm_sw(reg[instr->r.rs] + instr->r.r1, reg[instr->r.r0]);
		break;
	case OP_JMP :
		debug("jmp\t#%i\n", instr->j.addr);
		cpu_set_pc(instr->j.addr);
		break;
	case OP_JR :
		debug("jr\tr%i(#%i)\n", instr->i.rs, instr->i.imm);
		cpu_set_pc(reg[instr->r.rs] + instr->i.imm);
		break;
	case OP_BEQ :
		debug("beq\tr%i r%i #%i\n", instr->ri.rs, instr->ri.r0, instr->ri.offset);
		// Compare rs, r0
		if (reg[instr->ri.rs] == reg[instr->ri.r0])
			cpu_set_pc(pc + instr->ri.offset);
		break;
	case OP_INT :
		debug("int %i(#%i)\n", instr->r.rs, instr->i.imm);
		vm_syscall(instr->i.rs, instr->i.imm, reg);
		break;
	default :
		fprintf(stderr, "Invalid instruction (%.2X)\n", instr->opcode);
		cpu_flags |= CPU_FLAGS_HALT;
		break;
	}
}

void print_memory() {

	int i = 0;

	printf("\n");

	for(i = 0; i < 32; i++) {
		printf("%.2X ", memory[i]);
	}
	printf("\n");
}

void print_reg() {

	int i = 0;

	printf("\n");

	for(i = 0; i < 16; i++) {
		printf("r%i = %i\n", i, reg[i]);
	}
	printf("\n");
}

static unsigned char* instr_fetch() {

	if (pc + 1 >= instr_cnt >> 1)
		cpu_flags |= CPU_FLAGS_HALT;

	return instr_mem + (pc++ << 1);
}

int cpu_instr_load(void *ptr, unsigned len) {

	if (len % 2) {
		fprintf(stderr, "Error: Instruction length must be a multiple of 2\n");
		return -1;
	}

	instr_mem = ptr;
	instr_cnt = len;
	return len;
}

void cpu_instr_unload() {

	instr_mem = NULL;
	instr_cnt = 0;
}

void cpu_set_pc(uint16_t addr) {

	if (addr > instr_cnt / 2) {
		fprintf(stderr, "Runtime error: Invalid instruction address %ui\n", addr);
		cpu_flags |= CPU_FLAGS_HALT;
		return;
	}
	pc = addr;
}

void cpu_run() {

	mm_init();

	cpu_flags &= ~CPU_FLAGS_HALT;

	while(!(cpu_flags & CPU_FLAGS_HALT)) {
		struct instr instr;
		unsigned char* next = instr_fetch();

		// decode instruction.
		instr_decode(next, &instr);

		// Execute it.
		execute(&instr);
	}

	print_reg();
	print_memory();

	mm_exit();
}
