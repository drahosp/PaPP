#
# Makefile for Philosophers demo programs
#
# "make program" to make one program
# "make" or "make all" to make all executables
# "make clean" to remove executables
#

CC		= gcc
CFLAGS	= -O3 -Wall
LFLAGS	= -lm -lpthread

ALL	= dphil_leftie \
	dphil_locktable \
	dphil_footman

all:  $(ALL)

%: %.c 
	$(CC) -o $@ dphil_common.c $(CFLAGS) $< $(LFLAGS)

clean:
	-rm $(ALL)
