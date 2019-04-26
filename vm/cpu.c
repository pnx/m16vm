/* cpu.c
 *
 *   Copyright (C) 2012,2014-2015,2018-2019   Henrik Hautakoski <henrik.hautakoski@gmail.com>
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
#include "instr_decode.h"

#ifdef M16_DEBUG_INSTR
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...)
#endif /* M16_DEBUG_INSTR */

static void execute(struct cpu_state *state, struct instr *instr) {

	switch(instr->opcode) {
	case OP_NOOP :
		debug("noop\n");
		/* Do nothing */
		break;
	case OP_ADD :
		debug("add\tr%i r%i r%i\n", instr->r.rs, instr->r.r0, instr->r.r1);
		state->reg[instr->r.rs] = state->reg[instr->r.r0] + state->reg[instr->r.r1];
		break;
	case OP_MOVL :
		debug("movl\tr%i #%i\n", instr->i.rs, instr->i.imm);
		state->reg[instr->r.rs] = (state->reg[instr->r.rs] & 0xFF00) | instr->i.imm;
		break;
	case OP_MOVH :
		debug("movh\tr%i #%i\n", instr->i.rs, instr->i.imm);
		state->reg[instr->r.rs] = (state->reg[instr->r.rs] & 0x00FF) | (((uint16_t) instr->i.imm) << 8);
		break;
	case OP_LW :
		debug("lw\tr%i r%i #%i\n", instr->ri.rs, instr->ri.r0, instr->ri.offset);
		state->reg[instr->r.rs] = mm_lw(state->reg[instr->ri.r0] + instr->ri.offset);
		break;
	case OP_SW :
		debug("sw\tr%i r%i #%i\n", instr->ri.rs, instr->ri.r0, instr->ri.offset);
		mm_sw(state->reg[instr->ri.rs] + instr->ri.offset, state->reg[instr->ri.r0]);
		break;
	case OP_JMP :
		debug("jmp\t#%i\n", instr->j.addr);
		cpu_set_pc(state, instr->j.addr);
		break;
	case OP_JR :
		debug("jr\tr%i(#%i)\n", instr->i.rs, instr->i.imm);
		cpu_set_pc(state, state->reg[instr->r.rs] + instr->i.imm);
		break;
	case OP_BEQ :
		debug("beq\tr%i r%i #%i\n", instr->ri.rs, instr->ri.r0, instr->ri.offset);
		// Compare rs, r0
		if (state->reg[instr->ri.rs] == state->reg[instr->ri.r0])
			cpu_set_pc(state, state->pc + instr->ri.offset);
		break;
	case OP_INT :
		debug("int %i(#%i)\n", instr->r.rs, instr->i.imm);
		vm_syscall(instr->i.rs, instr->i.imm, state->reg);
		break;
	default :
		fprintf(stderr, "Invalid instruction (%.2X)\n", instr->opcode);
		state->pc = state->instr_cnt;
		break;
	}
}

void cpu_init(struct cpu_state *state)
{
	state->pc = 0;
	memset(state->reg, 0, sizeof(state->reg[0]) * CPU_NUM_REGS);

	state->instr_mem = NULL;
	state->instr_cnt = 0;
}

int cpu_instr_load(struct cpu_state *state, void *ptr, unsigned len) {

	if (len % 2) {
		fprintf(stderr, "Error: Instruction length must be a multiple of 2\n");
		return -1;
	}

	state->instr_mem = ptr;
	state->instr_cnt = len;
	return len;
}

void cpu_instr_unload(struct cpu_state *state) {

	state->instr_mem = NULL;
	state->instr_cnt = 0;
}

void cpu_set_pc(struct cpu_state *state, uint16_t addr) {

	if (addr > state->instr_cnt / 2)
		fprintf(stderr, "Runtime error: Invalid instruction address %ui\n", addr);
	state->pc = addr;
}

static unsigned char* instr_fetch(struct cpu_state *state) {

	return state->instr_mem + (state->pc++ << 1);
}

int cpu_tick(struct cpu_state *state) {

	struct instr instr;
	unsigned char* next;

	// Fetch next instruction
	next = instr_fetch(state);

	// We abort if we move past the last program address.
	if (state->pc > state->instr_cnt >> 1)
		return 1;

	// decode instruction.
	instr_decode(next, &instr);

	// Execute it.
	execute(state, &instr);

	return 0;
}
