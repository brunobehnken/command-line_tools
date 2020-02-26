#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>

extern int alphasort();

int flag[3] = {0,0,0}; // option flags
int f_opt = 0; // option flag

void getOut() {
	printf("Please type ./ls [-afi] [<dir>]\n");
	exit(EXIT_FAILURE);
}

void parseUserOptions(int argc, char * const  argv[]){
  int c;
  while ((c = getopt(argc,argv,"afi")) != -1) {
		f_opt++; // turns flag on
		switch (c) {
			case 'a': flag[0]++; break;
      case 'f': flag[1]++; break;
			case 'i': flag[2]++; break;
			case 63: getOut(); // invalid option code
		}
	}
}

void printDefaultoption(char* name){
  struct stat sb;
  lstat(name, &sb);

  printf("%s\n", name);
}

void execLS(int argc,  char* const argv[]) {
  struct dirent **myfile;
  int n;
  int aux=0;

  int i;
  int erro;
  char *name;

  parseUserOptions(argc,argv);
  // if there is any option
	if (f_opt) {
    // set i to use argv[i]
		for (i = 1; (i < argc) && (argv[i][0] == '-'); i++);
    // no input file
    if (argc == i)
				name = ".";
    // have input file(s)
    else {
        if ((name = realpath(argv[i],name)) == NULL) {
          int erro = errno;
          printf("Error realpath: %s",strerror(erro));
          exit(EXIT_FAILURE);
        }
    }
  }
  // there is no option
  else {
    // no input file
    if (argc == 1)
      name = ".";
    // have input file(s)
    if (argc > 1)
      name = realpath(argv[1],name);
  }

  if (!flag[1])
    n = scandir(name, &myfile, NULL, alphasort);
  else
    n = scandir(name, &myfile, NULL, NULL);
  if(n<0) {
    int erro = errno;
    printf("Error scandir: %s",strerror(erro));
    exit(EXIT_FAILURE);
  }
  else {
    while(n--){
      //if -a is disabled
      if (!flag[0]) {
        //ignore hidden files
        if (myfile[aux]->d_name[0]!='.'){
          //if -i is enabled, show d_ino before other options
          if (flag[2]) {
            printf("%8ld ", (long)myfile[aux]->d_ino);
            printDefaultoption(myfile[aux]->d_name);
          }
          //if -i is disabled
          else printDefaultoption(myfile[aux]->d_name);
        }
        aux++; continue;
      }
      //if -a is enabled
      else {
        //if -i is enabled, show d_ino before other options
        if (flag[2]) {
          printf("%8ld ", (long)myfile[aux]->d_ino);
          printDefaultoption(myfile[aux]->d_name);
        }
        //if -i is disabled
        else printDefaultoption(myfile[aux]->d_name);
      }
      aux++; continue;
    }
  }
  //printf("\n");
}

int main(int argc, char * const argv[]) {

  execLS(argc, argv);
  return 0;
}
