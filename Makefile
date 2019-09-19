CC=gcc
CFLAGS= -Wall

SimpleShell: simple-shell.o simple-execute.o
		$(CC) $(CFLAGS) -o SimpleShell simple-shell.o simple-execute.o

simple-shell.o: simple-shell.c
		$(CC) $(CFLAGS) -c -o simple-shell.o simple-shell.c

simple-execute.o: simple-execute.c
		$(CC) $(CFLAGS) -c -o simple-execute.o simple-execute.c

clean:
	rm *.o SimpleShell
