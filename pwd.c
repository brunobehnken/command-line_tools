#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

int main(int argc, char* const argv[]) {
	int erro;
	char* buf = (char*) malloc(PATH_MAX);

	if (!buf) {
		erro = errno;
		printf("Erro: %s\n",strerror(erro));
		exit(EXIT_FAILURE);
	}
	if (!getcwd(buf,PATH_MAX)) {
		erro = errno;
		printf("Erro: %s\n",strerror(erro));
		exit(EXIT_FAILURE);
	}
    printf("%s\n",buf);
    return 0;
}