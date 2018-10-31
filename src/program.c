
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "program.h"

int program_loadfromfile(struct program *prog, const char *filename) {

	struct stat st;
	ssize_t rc = -1;
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Could not open file %s: %s\n",
			filename, strerror(errno));
		return -1;
	}

	if (fstat(fd, &st) < 0)
		goto close_fd;

	prog->len = st.st_size;
	prog->instr = malloc(prog->len);
	if (prog->instr == NULL)
		goto close_fd;

	rc = read(fd, prog->instr, prog->len);
	if (rc < 0)
		goto free_mem;
	return rc;
free_mem: free(prog->instr);
close_fd: close(fd);
	return rc;
}

void program_free(struct program *prog) {

	free(prog->instr);
	prog->len = 0;
}
