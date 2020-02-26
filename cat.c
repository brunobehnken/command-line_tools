/* trabalho 4: programa cat */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

FILE *arq;
int flag[4] = {0}; // option flags

void printOpt(char *buf) {
	static int n = 1;
	static int flag_s = 0;
	int i,len;
	char *num;
	char *old;

	if (flag[3]) { // s option
		if (buf[0] == 10) { // buf == '\n'
			if (flag_s) {
				n++;
				return;
			} else
				flag_s++;
		} else {
			if (flag_s)
				flag_s--;
		}
	}
	if (flag[2]) { // n option
		num = malloc(10);
		len = sprintf(num,"%d ",n);
		n++;
		if (!(num = realloc(num,strlen(buf) + len))) {
			printf("Realloc error\n");
			exit(EXIT_FAILURE);
		}
		strcat(num,buf);
		old = buf;
		buf = num;
		// free(old);
	}
	if (flag[0] || flag[1]) { // E option or T option
		len = strlen(buf);
		for (i = 0; i < len; i++) {
			if (buf[i] == 10 && flag[0]) { // '\n'
				printf("$");
				// printf("%c",buf[i]);
			}
			if (buf[i] == 9 && flag[1]) { // 'tab'
				printf("^I");
			} else {
				printf("%c",buf[i]);
			}
		}
	} else 
		printf("%s",buf);
}

void readFile(char const *arg, int f_opt) {
	char *file_buf, *buf;
	int erro;

	file_buf = (char *) malloc(PATH_MAX);
	if (!realpath(arg,file_buf)) {
		erro = errno;
		printf("Error: %s\n",strerror(erro));
		exit(EXIT_FAILURE);
	}
	arq = fopen(file_buf,"r");
	if (arq == NULL ) {
		erro = errno;
		printf("Error: %s\n",strerror(erro));
		exit(EXIT_FAILURE);
	}
	while (!feof(arq)) {
		buf = malloc(1024);
		if (!buf) {
			printf("Malloc error");
			exit(EXIT_FAILURE);
		}
		if (fgets(buf,1024,arq))
			if (f_opt) {
				printOpt(buf);
			} else {
				printf("%s",buf);
			}
		free(buf);
	}
}

void sendHelp() {
	printf("Program cat.\nDescription: Concatenates the input file(s) and print on the standard output. Default input is keyboard.\nUsage: ./cat [-ETns] [<file> ...]\nOptions:\n   -E: Display $ at end of each line\n   -T: Display TAB characters as ^I\n   -n: number all output lines\n   -s: supress repeated empty output lines.\n");
	exit(EXIT_FAILURE);
}

void getOut() {
	printf("Please type ./cat [-ETns] [<file> ...]\nor type ./cat -h for help.\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char * const argv[]) {
	int c, i;
	int f_opt = 0; // option flag
	char buf[2];

	while ((c = getopt(argc,argv,"ETnsh")) != -1) {
		f_opt++; // turns flag on
		switch (c) {
			case 'E': flag[0]++; break;
			case 'T': flag[1]++; break;
			case 'n': flag[2]++; break;
			case 's': flag[3]++; break;
			case 'h': sendHelp();
			case 63: getOut(); // invalid option code
		}
	}
	if (f_opt) { // if there is any option
		for (i = 1; (i < argc) && (argv[i][0] == '-'); i++); // set i to use argv[i]
		if (argc == i) { // no input file
			while (fgets(buf,2,stdin))
				printOpt(buf);
		} else { // have input file(s)
			for (; i < argc; i++)
				readFile(argv[i],f_opt);
		}
	} else { // there is no option
		if (argc == 1) {
			while (fgets(buf,2,stdin))
				printf("%s",buf);
		}
		if (argc > 1) {
			for (i = 1; i < argc ; i++)	{
				readFile(argv[i],f_opt);
			}
		}
	}
	return 0;
}