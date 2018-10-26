
#include <stdio.h>

void forward(FILE *fd, char ch) {

 	for(;;) {
		char c = getc(fd);
		if (c == EOF)
			break;
		if (c == ch) {
			getc(fd);
			break;
		}
	}
}

void execute(FILE *fd) {

	unsigned char mem[2048] = { 0 }; /* Memory block */
	unsigned char *ptr = mem;
	long int jmpaddr = -1;

	while(!feof(fd)) {
		char ch = getc(fd);

		switch(ch) {
		case '<':
			ptr--;
			break;
		case '>':
			ptr++;
			break;
		case '+':
			(*ptr)++;
			break;
		case '-':
			(*ptr)--;
			break;
		case '.':
			putchar(*ptr);
			break;
		case '[':
			if (*ptr == 0) {
				forward(fd, ']');
				jmpaddr = -1;
			} else {
				jmpaddr = ftell(fd) - 1;
			}
			break;
		case ']':
			if (jmpaddr > 0) {
				fseek(fd, jmpaddr, SEEK_SET);
			}
			break;
		}
	}
}

int main(int argc, char **argv) {

	FILE *fd;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <inputfile>\n", argv[0]);
		return 1;
	}

	fd = fopen(argv[1], "r");
	if (!fd) {
		return 1;
	}

	execute(fd);

	fclose(fd);

	return 0;
}
