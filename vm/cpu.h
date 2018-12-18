/* cpu.h
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
#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "instr.h"

#define CPU_NUM_REGS 16

#define CPU_FLAGS_HALT (1<<0)

struct cpu_state {
	// Registers r0, r15
	int16_t reg[CPU_NUM_REGS];

	// Program counter
	uint16_t pc;

	// flags
	unsigned char flags;

	// Instruction
	unsigned char *instr_mem;
	unsigned long instr_cnt;
};

void cpu_init(struct cpu_state *state);

int cpu_instr_load(struct cpu_state *state, void *ptr, unsigned len);

void cpu_instr_unload(struct cpu_state *state);

void cpu_set_pc(struct cpu_state *state, uint16_t addr);

int cpu_tick(struct cpu_state *state);

#endif /* CPU_H */
