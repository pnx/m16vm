/* as.c
 *
 *   Copyright (C) 2012   Henrik Hautakoski <henrik@fiktivkod.org>
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
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int usage(char *program) {

	fprintf(stderr, "Usage: %s <string>\n", program);
	return -1;
}

int main(int argc, char **argv) {

	FILE *fd;
	struct lexer lex;

	if (argc < 2)
		return usage(argv[0]);

	fd = fopen(argv[1], "r");
	if (fd == NULL)
		return -1;

	lexer_init(&lex, fd);

	do {
		if (lexer_get_next(&lex) < 0)
			break;

		lexer_print_token(&lex.token);
	} while(lex.token.type != TOKEN_EOI);


	fclose(fd);
	return 0;
}
