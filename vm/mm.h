/* mm.h
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
#ifndef MM_H
#define MM_H

#include <stdint.h>

#ifndef MEM_SIZE
/* Set a default memory size. (2^8) */
#define MEM_SIZE 256
#endif

extern uint8_t *mm_base_addr;

void mm_init();

void mm_exit();

/* Save a word to memory. */
void mm_sw(uint16_t addr, int16_t value);

/* Load a word from memory. */
int16_t mm_lw(uint16_t addr);

#endif /* MM_H */
