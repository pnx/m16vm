/* vector.c
 *
 *   Copyright (C) 2018   Henrik Hautakoski <henrik@fiktivkod.org>
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
#include <string.h>
#include <stdlib.h>
#include "vector.h"

static void ensure_size(struct vector* v, size_t n) {

	size_t old_alloc = v->alloc;
	while(v->alloc < v->size + n)
		v->alloc += v->blk_sz;

	if (old_alloc != v->alloc)
		v->base = realloc(v->base, v->alloc);
}

void vector_init(struct vector* v, size_t block_size) {

	v->blk_sz = block_size > 0 ? block_size : VECTOR_BLOCK_SIZE;
	v->alloc  = 0;
	v->size   = 0;
	v->base   = NULL;
}

void vector_destory(struct vector *v) {

	if (v->base)
		free(v->base);
}

void vector_append(struct vector *v, const void *ptr, size_t n) {

	ensure_size(v, n);

	memcpy(v->base + v->size, ptr, n);
	v->size += n;
}
