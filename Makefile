CC = gcc
CFLAGS = -O3 -Wall -Wextra -fopenmp -march=znver2

all: baseline optimized
baseline: baseline.c helpers.c helpers.h
	$(CC) $(CFLAGS) -o baseline baseline.c helpers.c
optimized: optimized.c helpers.c helpers.h
	$(CC) $(CFLAGS) -o optimized optimized.c helpers.c

clean:
	rm -f baseline optimized
