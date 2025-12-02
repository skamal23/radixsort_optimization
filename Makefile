CC = gcc
CXX = g++
# -O3: Max optimization
# -march=native: Use AVX2 instructions available on EPYC 7402P
# -fopenmp: Enable multi-threading
CFLAGS = -O3 -march=native -fopenmp -Wall -Wextra
CXXFLAGS = -O3 -march=native -fopenmp
LDFLAGS = -lm

all: reference sorting

reference: reference_sort.cpp
	$(CXX) $(CXXFLAGS) -o reference reference_sort.cpp

sorting: driver.c sorting_final.c
	$(CC) $(CFLAGS) -o sorting driver.c sorting_final.c $(LDFLAGS)

clean:
	rm -f reference sorting
