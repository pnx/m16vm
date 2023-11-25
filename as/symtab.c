/* symtab.c
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
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// TODO: Right now, the symtab is implemented as a simple dynamic growing array.
// This is fine for now, but for lookup speed. A hashtable is more efficient.

// How much memory that should be allocated each time.
// Note: this should be defined as number of `struct symbol` objects, not bytes.
#define BLOCK_SIZE 32

// Helper macro to calculate number of bytes the table need.
#define __M_SIZE(t) \
	((t)->size * sizeof(struct symbol))

struct symbol {
	// The label associated with the location.
	char label[SYMTAB_LABEL_MAXLEN];

	// Location (program adress)
	uint16_t loc;
};

// Symbol table structure.
struct symtab {
	// Array of symbols.
	struct symbol *data;

	// Number of symbols in the array.
	size_t num;

	// Total number of symbols the array can hold.
	size_t size;
};

symtab_t* symtab_init() {

	struct symtab *table = malloc(sizeof(struct symtab));

	table->num = 0;
	table->size = BLOCK_SIZE;
	table->data = malloc(__M_SIZE(table));

	return table;
}

void symtab_free(symtab_t *table) {

	if (!table)
		return;

	if (table->data)
		free(table->data);

	memset(table, 0, sizeof(struct symtab));
	free(table);
}

int symtab_set(symtab_t *table, const char *label, uint16_t loc) {

	// Check if it exist first.
	int index = symtab_get(table, label, NULL);

	// Entry did not exists. Insert
	if (index < 0) {
		struct symbol *sym;

		// We have one more.
		table->num += 1;

		// Make sure we resize the memory.
		if (table->num > table->size) {
			table->size += BLOCK_SIZE;
			table->data = realloc(table->data, __M_SIZE(table));
		}

		// insert the symbol at the end.
		sym = table->data + (table->num - 1);
		strncpy(sym->label, label, SYMTAB_LABEL_MAXLEN);
		sym->loc = loc;

		return 0;
	}

	// Could not insert. return as error.
	return -1;
}

int symtab_get(symtab_t *table, const char *label, uint16_t *loc) {

	// Linear search here for simplicity.
	for(size_t i = 0; i < table->num; i++) {
		struct symbol *sym = table->data + i;

		if (!strncmp(sym->label, label, SYMTAB_LABEL_MAXLEN)) {

			if (loc) *loc = sym->loc;
			return i;
		}
	}
	return -1;
}
