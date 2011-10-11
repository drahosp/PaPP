#
# Makefile for MPI demo programs
#
# "make program" to make one program
# "make" or "make all" to make all executables
# "make clean" to remove executables
#

CC		= gcc
CFLAGS	= -O3 -Wall
LFLAGS	= -lm -lmpi

ALL =   mpi_cpi \
	mpi_datatype \
	mpi_gather \
	mpi_hello \
	mpi_random_sum \
	mpi_readfile \
	mpi_rowcol \
	mpi_scatter \
	mpi_scatterv \
	mpi_send-buffered.c \
	mpi_send-nonblocking-wait \
	mpi_send-nonblocking-waitall \
	mpi_send-nonblocking-waitany \
	mpi_send-standard-large \
	mpi_send-synchronous \
	mpi_sendcol \
	mpi_writefile
#	mpi_mpegraph
#	mpi_wave
	

all:  $(ALL)

%: %.c 
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

clean:
	-rm $(ALL)
