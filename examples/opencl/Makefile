#
# Makefile for OpenCL demo programs
#
# "make program" to make one program
# "make" or "make all" to make all executables
# "make clean" to remove executables
#

CC		= gcc
CFLAGS	= -O3 -Wall -fopenmp
UNAME := $(shell uname -s)

ALL =   cl_vector

all:  $(ALL)

%: %.c 
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

# Windows
LFLAGS	= -lm -lOpenCL
clean:
	-del *.exe

# Linux
ifeq ($(UNAME),Linux)
LFLAGS = -lm -lCL
clean:
	-rm $(ALL)
endif

# OSX
ifeq ($(UNAME),Darwin)
LFLAGS = -lm -framework OpenCL
clean:
	-rm $(ALL)
endif
