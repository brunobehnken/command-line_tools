CC=gcc

main: cat.c cd.c ls.c pwd.c
	$(CC) -o cat cat.c
	$(CC) -o cd cd.c
	$(CC) -o ls ls.c 
	$(CC) -o pwd pwd.c
