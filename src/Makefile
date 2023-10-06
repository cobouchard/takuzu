CFLAGS = -stc=c11 -Wall -Wextra -g -O2
CPPFLAGS = -l../include -DDEBUG
lDFLAGS = -lm

all: main

main: main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean: 
	@rm -f *.o main

help:
	@echo "Usage: 	takuzu[-a|-o FILE|-v|-h] FILE... \n \t \t takuzu -g[SIZE] [-u|-o FILE|-v|-h]"
	@echo "Solve or generate takuzu grids of size: 4, 8, 16, 32, 64"
	@echo "-a, --all \t \t search for all possible solutions"
	@echo "-g[N], --generate[N] \t \t generate a grid of size NxN (default:8)"
	@echo "-o FILE, --output FILE \t \t write output to FILE"
	@echo "-u, --unique \t \t generate a grid with unique solution"
	@echo "-v, --verbose \t \t verbose output"
	@echo "-h, --help \t \t display this help and exit"


.PHONY: all clean help
