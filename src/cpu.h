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

int cpu_instr_load(void *ptr, unsigned len);

void cpu_instr_unload();

void cpu_set_pc(uint16_t addr);

void cpu_run();

#endif /* CPU_H */
