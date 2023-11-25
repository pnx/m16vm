/* vector.h
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
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>

struct vector  {
	size_t blk_sz;
	size_t alloc;
	size_t size;
	void  *base;
};

#define VECTOR_INIT(block_size) { block_size, 0, 0, NULL }

void vector_init(struct vector* v, size_t block_size);

void vector_destory(struct vector* v);

void vector_append(struct vector* v, const void *ptr, size_t n);

#endif /* VECTOR_H */
