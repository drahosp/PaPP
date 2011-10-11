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
UNAME := $(shell uname -s)

ALL	= dphil_leftie \
	dphil_locktable \
	dphil_footman

all:  $(ALL)

%: %.c 
	$(CC) -o $@ dphil_common.c $(CFLAGS) $< $(LFLAGS)

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
