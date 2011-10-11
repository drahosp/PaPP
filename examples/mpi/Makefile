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
UNAME := $(shell uname -s)

ALL =   mpi_cpi \
	mpi_datatype \
	mpi_hello \
	mpi_random_sum \
	mpi_readfile \
	mpi_writefile \
	mpi_rowcol \
	mpi_scatter \
	mpi_scatterv \
	mpi_send-nonblocking-wait \
	mpi_send-nonblocking-waitall \
	mpi_send-nonblocking-waitany \
	mpi_send-standard \
	mpi_send-standard-large \
	mpi_send-synchronous \
	mpi_sendcol
#	mpi_gather
#	mpi_mpegraph
#	mpi_wave

all:  $(ALL)

%: %.c 
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

# Windows
clean:
	-del *.exe

# Linux
ifeq ($(UNAME),Linux)
clean:
	-rm $(ALL)
endif

# OSX
ifeq ($(UNAME),Darwin)
clean:
	-rm $(ALL)
endif
