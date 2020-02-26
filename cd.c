#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[]) {
	char *path;
	char *new_path;
	int save_errno;

	if (argc > 2) {
		printf("Program CD.\nDescription: change current directory.\nUsage: cd [<dir>]\n");
		exit(EXIT_FAILURE);
	}
	path = (char *) malloc(PATH_MAX);
	new_path = (char *) malloc(PATH_MAX);
	if (!path || !new_path) {
		printf("malloc error\n");
		exit(EXIT_FAILURE);
	}
	if (getcwd(path, PATH_MAX) == NULL) {
		save_errno = errno;
		printf("Error: %s\n",strerror(save_errno));
		exit(EXIT_FAILURE);
	}
	if (argc == 1) {
		printf("%s\n",path);
		return 0;
	}
	if (realpath(argv[1],new_path) == NULL) {
		save_errno = errno;
		printf("Error: %s\n",strerror(save_errno));
		exit(EXIT_FAILURE);
	}
	if (chdir(new_path)) {
		save_errno = errno;
		printf("Error: %s\n",strerror(save_errno));
		exit(EXIT_FAILURE);
	}
	if (getcwd(path, PATH_MAX) == NULL) {
		save_errno = errno;
		printf("Error: %s\n",strerror(save_errno));
		exit(EXIT_FAILURE);
	}
	printf("%s\n",path);
	return 0;
}