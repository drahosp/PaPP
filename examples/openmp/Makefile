#
# Makefile for OpenMP demo programs
#
# "make program" to make one program
# "make" or "make all" to make all executables
# "make clean" to remove executables
#

CC		= gcc
CFLAGS	= -O3 -Wall -fopenmp
LFLAGS	= -lm

ALL =   omp_basic \
	omp_critical \
	omp_for \
	omp_get_env_info \
	omp_hello \
	omp_matrixmult \
	omp_num_threads \
	omp_pi \
	omp_reduction \
	omp_sections \
	omp_shared \
	omp_threadprivate

all:  $(ALL)

%: %.c 
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

clean:
	-rm $(ALL)
