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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "cpu.h"

// Load program from file
unsigned long load_program(int fd, unsigned char **buf) {

	struct stat st;
	ssize_t rc;

	if (fstat(fd, &st) < 0)
		return -1;

	*buf = malloc(st.st_size);
	if (*buf == NULL)
		return -1;

	rc = read(fd, *buf, st.st_size);
	if (rc < 0)
		free(*buf);
	return rc;
}

int main(int argc, char **argv) {

	int fd;
	unsigned char *instr_ptr;
	unsigned instr_len;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return 1;
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Could not open file %s: %s\n",
			argv[1], strerror(errno));
		return 1;
	}

	instr_len = load_program(fd, &instr_ptr);
	close(fd);

	if (instr_len < 0)
		return 1;

	cpu_instr_load(instr_ptr, instr_len);

	cpu_run();

	if (instr_ptr)
		free(instr_ptr);

	return 0;
}
