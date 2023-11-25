/* symtab.h
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
#ifndef ASM_SYMTAB_H
#define ASM_SYMTAB_H

#include <stdint.h>

#define SYMTAB_LABEL_MAXLEN 32

typedef struct symtab symtab_t;

symtab_t* symtab_init();

void symtab_free(symtab_t *table);

int symtab_set(symtab_t *table, const char *label, uint16_t loc);

int symtab_get(symtab_t *table, const char *label, uint16_t *loc);

#endif /* ASM_SYMTAB_H */
